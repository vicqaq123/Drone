// ObstacleScannerComponent.cpp
#include "ObstacleScannerComponent.h"
#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"

UObstacleScannerComponent::UObstacleScannerComponent()
{
    PrimaryComponentTick.bCanEverTick = true;
}

void UObstacleScannerComponent::BeginPlay()
{
    Super::BeginPlay();
    
    // 自动查找GridMap组件
    if (!GridMap)
    {
        AActor* Owner = GetOwner();
        if (Owner)
        {
            GridMap = Owner->FindComponentByClass<UGridMapComponent>();
            if (!GridMap)
            {
                UE_LOG(LogTemp, Error, TEXT("ObstacleScanner: No GridMap component found on owner actor"));
            }
        }
    }
}

void UObstacleScannerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    // 累计时间
    TimeSinceLastScan += DeltaTime;
    
    // 达到扫描间隔时执行扫描
    if (TimeSinceLastScan >= ScanInterval)
    {
        TimeSinceLastScan = 0.0f;
        
        // 执行扫描
        if (bAutoScan)
        {
            AActor* Owner = GetOwner();
            if (Owner)
            {
                ScanArea(Owner->GetActorLocation(), ScanRadius, ScanHeight);
            }
        }
    }

    // // 每一帧都绘制所有障碍物
    // if (GridMap)
    // {
    //     DrawAllObstacles();
    // }
}

void UObstacleScannerComponent::SetGridMap(UGridMapComponent* InGridMap)
{
    GridMap = InGridMap;
}

void UObstacleScannerComponent::ScanArea(const FVector& Center, float Radius, float Height)
{
    if (!GridMap || !GetWorld())
    {
        UE_LOG(LogTemp, Error, TEXT("ObstacleScanner: GridMap or World is not valid"));
        return;
    }

    // 每次扫描前清空地图
    // GridMap->ClearObstacles();

    // 获取无人机前向方向
    AActor* Owner = GetOwner();
    if (!Owner)
        return;
    
    FRotator OwnerRotation = Owner->GetActorRotation();
    FVector ForwardVector = OwnerRotation.Vector();
    FVector RightVector = FVector::CrossProduct(FRotator(0, OwnerRotation.Yaw, 0).Vector(), FVector::UpVector);
    FVector UpVector = FVector::CrossProduct(ForwardVector, RightVector);

    // 计算扫描范围
    float StartDistance = 50.0f;  // 从无人机前方50厘米开始扫描
    float EndDistance = Radius;   // 扫描到最大半径
    
    // 计算水平扫描点数量（180度）
    int32 NumPoints = FMath::CeilToInt(PI * Radius / RaycastSpacing);
    float AngleStep = PI / NumPoints;  // 180度 = PI

    // 执行扫描
    for (int32 i = 0; i < NumPoints; i++)
    {
        float Angle = i * AngleStep - PI/2;  // 从-90度到90度
        FVector Direction = ForwardVector * FMath::Cos(Angle) + RightVector * FMath::Sin(Angle);
        
        // 计算扫描起点和终点，使用无人机当前高度
        FVector Start = Center + Direction * StartDistance;
        FVector End = Center + Direction * EndDistance;
        Start.Z = Center.Z;  // 使用无人机当前高度
        End.Z = Center.Z;    // 使用无人机当前高度
        
        // 执行射线检测
        PerformRaycast(Start, End);
    }

    // 可视化扫描结果
    if (bShowDebugVisualization)
    {
        VisualizeScanResults(Center, Radius, Height);
    }

    // 输出扫描完成信息
    // UE_LOG(LogTemp, Warning, TEXT("扫描完成"));
}

void UObstacleScannerComponent::PerformRaycast(const FVector& Start, const FVector& End)
{
    if (!GetWorld())
        return;
    FHitResult HitResult;
    FCollisionQueryParams QueryParams;
    QueryParams.AddIgnoredActor(GetOwner());
    QueryParams.bTraceComplex = true;
    QueryParams.bReturnPhysicalMaterial = true;

    bool bHit = GetWorld()->LineTraceSingleByChannel(
        HitResult,
        Start,
        End,
        ECC_Visibility,
        QueryParams
    );
    
    if (bHit)
    {
        // 检查是否是地面（z接近0）
        if (FMath::Abs(HitResult.Location.Z) < 1.0f)
        {
            // 是地面，直接更新网格
            UpdateGridMap(HitResult.Location, false);
            return;
        }

        UpdateGridMap(HitResult.Location, true);
        UE_LOG(LogTemp, Warning, TEXT("[ObstacleScanner] 检测到障碍物，位置: %s"), *HitResult.Location.ToString());

        // 调试可视化
        if (bShowDebugVisualization)
        {
            DrawDebugLine(
                GetWorld(),
                Start,
                HitResult.Location,
                FColor::Red,
                false,
                0.5f,
                0,
                1.0f
            );
            DrawDebugSphere(
                GetWorld(),
                HitResult.Location,
                10.0f,
                8,
                FColor::Red,
                false,
                0.5f,
                0,
                1.0f
            );
        }
    }
}


void UObstacleScannerComponent::UpdateGridMap(const FVector& HitLocation, bool bIsObstacle)
{
    if (!GridMap)
        return;

    // 将击中位置转换为网格坐标
    int32 GridX, GridY, GridZ;
    if (GridMap->WorldToGrid(HitLocation, GridX, GridY, GridZ))
    {
        if (bIsObstacle)
        {
            // 对障碍物进行膨胀处理
            float InflationRadius = 100.0f;  // 膨胀半径
            float CellSize = 5.0f;  // 使用固定的网格大小
            int32 InflationCells = FMath::CeilToInt(InflationRadius / CellSize);  // 转换为网格数量
            
            // 遍历膨胀范围内的所有网格点
            for (int32 dx = -InflationCells; dx <= InflationCells; dx++)
            {
                for (int32 dy = -InflationCells; dy <= InflationCells; dy++)
                {
                    // 计算到中心的距离
                    float Distance = FMath::Sqrt(static_cast<float>(dx * dx + dy * dy)) * CellSize;
                    if (Distance <= InflationRadius)
                    {
                        // 计算膨胀点的网格坐标
                        int32 InflatedGridX = GridX + dx;
                        int32 InflatedGridY = GridY + dy;
                        int32 InflatedGridZ = GridZ;

                        // 将网格坐标转换为世界坐标
                        FVector InflatedPoint = GridMap->GridToWorld(InflatedGridX, InflatedGridY, InflatedGridZ);
                        GridMap->MarkAsOccupied(InflatedPoint);
                    }
                }
            }
            
            // 垂直方向膨胀（只在必要时）
            if (FMath::Abs(HitLocation.Z) > 1.0f)
            {
                int32 VerticalCells = FMath::CeilToInt(InflationRadius * 0.5f / CellSize);
                for (int32 dz = -VerticalCells; dz <= VerticalCells; dz++)
                {
                    // 计算膨胀点的网格坐标
                    int32 InflatedGridX = GridX;
                    int32 InflatedGridY = GridY;
                    int32 InflatedGridZ = GridZ + dz;

                    // 将网格坐标转换为世界坐标
                    FVector InflatedPoint = GridMap->GridToWorld(InflatedGridX, InflatedGridY, InflatedGridZ);
                    GridMap->MarkAsOccupied(InflatedPoint);
                }
            }
        }
    }
}

float UObstacleScannerComponent::GetGroundHeight(const FVector& Location) const
{
    if (!GridMap)
        return 0.0f;

    // 从当前位置向下检测地面
    FVector Start = Location;
    FVector End = Start;
    End.Z -= GroundCheckDistance;

    FHitResult HitResult;
    FCollisionQueryParams QueryParams;
    QueryParams.AddIgnoredActor(GetOwner());

    if (GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Visibility, QueryParams))
    {
        return HitResult.Location.Z;
    }

    return 0.0f;
}

bool UObstacleScannerComponent::IsLocationOccupied(const FVector& Location) const
{
    if (!GridMap)
        return false;

    return GridMap->IsOccupied(Location);
}

void UObstacleScannerComponent::VisualizeScanResults(const FVector& Center, float Radius, float Height)
{
    if (!GetWorld())
        return;

    // 绘制扫描范围
    DrawDebugCylinder(
        GetWorld(),
        Center,
        Center + FVector(0, 0, Height),
        Radius,
        32,
        FColor::Yellow,
        false,
        0.5f,
        0,
        1.0f
    );

    // 绘制扫描中心点
    DrawDebugSphere(
        GetWorld(),
        Center,
        20.0f,
        12,
        FColor::Yellow,
        false,
        0.5f,
        0,
        2.0f
    );

    // 绘制前向方向
    AActor* Owner = GetOwner();
    if (Owner)
    {
        FVector Forward = Owner->GetActorForwardVector() * 100.0f;
        FVector End = Center + Forward;
        
        // 绘制主方向线
        DrawDebugLine(
            GetWorld(),
            Center,
            End,
            FColor::Green,
            false,
            0.5f,
            0,
            2.0f
        );

        // 绘制箭头
        float ArrowSize = 20.0f;
        FVector Right = Owner->GetActorRightVector() * ArrowSize;
        
        // 箭头左侧
        DrawDebugLine(
            GetWorld(),
            End,
            End - Forward * 0.3f + Right,
            FColor::Green,
            false,
            0.5f,
            0,
            2.0f
        );
        
        // 箭头右侧
        DrawDebugLine(
            GetWorld(),
            End,
            End - Forward * 0.3f - Right,
            FColor::Green,
            false,
            0.5f,
            0,
            2.0f
        );
    }
}

UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ObstacleScanner")
float ScanInterval = 0.5f; // 修改为5.0秒

// 添加新函数用于绘制所有障碍物
// void UObstacleScannerComponent::DrawAllObstacles()
// {
//     if (!GridMap || !GetWorld())
//         return;

//     // 获取所有被占用的网格点
//     TArray<FVector> OccupiedPoints;
//     GridMap->GetAllOccupiedPoints(OccupiedPoints);

//     // 绘制每个被占用的点
//     for (const FVector& Point : OccupiedPoints)
//     {
//         DrawDebugSphere(
//             GetWorld(),
//             Point,
//             10.0f,  // 球体半径
//             8,      // 球体分段数
//             FColor::Blue,  // 蓝色
//             false,  // 不持久化
//             -1.0f,  // 持续到下一帧
//             0,      // 深度优先级
//             1.0f    // 线条粗细
//         );
//     }
// }