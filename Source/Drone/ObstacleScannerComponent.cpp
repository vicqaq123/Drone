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

    // 输出扫描参数
    // UE_LOG(LogTemp, Warning, TEXT("扫描点数: %d"), NumPoints);

    // // 可视化扫描范围
    // if (bShowDebugVisualization)
    // {
    //     // 绘制扫描范围
    //     DrawDebugCylinder(
    //         GetWorld(),
    //         Center,
    //         Center + FVector(0, 0, Height),
    //         Radius,
    //         32,
    //         FColor::Yellow,
    //         false,
    //         0.5f,
    //         0,
    //         1.0f
    //     );

    //     // 绘制扫描中心点
    //     DrawDebugSphere(
    //         GetWorld(),
    //         Center,
    //         20.0f,
    //         12,
    //         FColor::Yellow,
    //         false,
    //         0.5f,
    //         0,
    //         2.0f
    //     );
    // }

    // 执行扫描
    for (int32 i = 0; i < NumPoints; i++)
    {
        float Angle = i * AngleStep - PI/2;  // 从-90度到90度
        FVector Direction = ForwardVector * FMath::Cos(Angle) + RightVector * FMath::Sin(Angle);
        
        // 计算扫描起点和终点
        FVector Start = Center + Direction * StartDistance;
        FVector End = Center + Direction * EndDistance;

        // 可视化扫描线
        // if (bShowDebugVisualization)
        // {
        //     DrawDebugLine(
        //         GetWorld(),
        //         Start,
        //         End,
        //         FColor::Blue,
        //         false,
        //         0.5f,
        //         0,
        //         0.5f
        //     );
        // }

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
    QueryParams.bTraceComplex = true;  // 使用复杂碰撞检测
    QueryParams.bReturnPhysicalMaterial = true;  // 返回物理材质信息

    // 执行射线检测
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
            // 是地面，不输出日志，只更新网格
            UpdateGridMap(HitResult.Location, false);
            return;
        }

        // // 输出障碍物信息
        // if (HitResult.GetActor())
        // {
        //     UE_LOG(LogTemp, Warning, TEXT("检测到障碍物: %s"), *HitResult.GetActor()->GetName());
        //     UE_LOG(LogTemp, Warning, TEXT("障碍物位置: %s"), *HitResult.Location.ToString());
        //     UE_LOG(LogTemp, Warning, TEXT("障碍物类型: %s"), *HitResult.GetActor()->GetClass()->GetName());
        // }

        // 更新网格地图
        UpdateGridMap(HitResult.Location, true);

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
    else
    {
        // 如果没有击中任何物体，标记为地面（不输出日志）
        UpdateGridMap(End, false);

        // 调试可视化
        if (bShowDebugVisualization)
        {
            DrawDebugLine(
                GetWorld(),
                Start,
                End,
                FColor::Green,
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
        // 更新网格状态
        if (bIsObstacle)
        {
            GridMap->MarkAsOccupied(HitLocation);
        }
        else
        {
            GridMap->MarkAsGround(HitLocation);
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