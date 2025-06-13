// ObstacleScannerComponent.cpp
#include "ObstacleScannerComponent.h"
#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "Async/ParallelFor.h"
#include "Misc/FileHelper.h"

UObstacleScannerComponent::UObstacleScannerComponent()
{
    PrimaryComponentTick.bCanEverTick = true;
    GridMap = nullptr;
    FileHandle = nullptr;
    PlatformFile = &FPlatformFileManager::Get().GetPlatformFile();
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

    // 记录开始时间
    StartTime = FPlatformTime::Seconds();
    
    // 初始化输出文件
    InitializeOutputFile();

    // 启动定时器，每0.1秒执行一次扫描
    if (bAutoScan)
    {
        GetWorld()->GetTimerManager().SetTimer(
            ScanTimerHandle,
            this,
            &UObstacleScannerComponent::OnScanTimer,
            ScanInterval,
            true  // 循环执行
        );
    }
}

void UObstacleScannerComponent::InitializeOutputFile()
{
    // 设置固定路径到D盘的data文件夹
    OutputFilePath = TEXT("D:/data/scan_results.txt");
    
    // 确保目录存在
    FString Directory = FPaths::GetPath(OutputFilePath);
    if (!PlatformFile->DirectoryExists(*Directory))
    {
        PlatformFile->CreateDirectoryTree(*Directory);
    }
    
    // 如果文件不存在，创建文件并写入文件头
    if (!PlatformFile->FileExists(*OutputFilePath))
    {
        FString Header = TEXT("TimeFromStart,X,Y,Z\n");  // 修改文件头
        if (FFileHelper::SaveStringToFile(Header, *OutputFilePath, FFileHelper::EEncodingOptions::ForceUTF8))
        {
            UE_LOG(LogTemp, Log, TEXT("Scanner: 输出文件已创建: %s"), *OutputFilePath);
        }
        else
        {
            UE_LOG(LogTemp, Error, TEXT("Scanner: 无法创建输出文件: %s"), *OutputFilePath);
        }
    }
    else
    {
        UE_LOG(LogTemp, Log, TEXT("Scanner: 使用现有输出文件: %s"), *OutputFilePath);
    }
}

void UObstacleScannerComponent::SaveScanResultToFile(const FVector& Location)
{
    // 计算从开始时间到现在的精确时间（微秒级）
    double CurrentTime = FPlatformTime::Seconds();
    double TimeFromStart = (CurrentTime - StartTime) * 1000000.0;  // 转换为微秒
    
    // 格式化坐标数据
    FString Data = FString::Printf(TEXT("%.3f,%.2f,%.2f,%.2f\n"),
        TimeFromStart,
        Location.X,
        Location.Y,
        Location.Z);
    
    // 添加到缓冲区
    DataBuffer.Add(Data);
    
    // 如果缓冲区达到指定大小，批量写入文件
    if (DataBuffer.Num() >= BUFFER_SIZE)
    {
        FlushBuffer();
    }
}

void UObstacleScannerComponent::FlushBuffer()
{
    if (DataBuffer.Num() > 0)
    {
        // 将缓冲区数据合并为一个字符串
        FString CombinedData;
        for (const FString& Data : DataBuffer)
        {
            CombinedData += Data;
        }
        
        // 批量写入文件
        if (!FFileHelper::SaveStringToFile(CombinedData, *OutputFilePath, 
            FFileHelper::EEncodingOptions::ForceUTF8, &IFileManager::Get(), FILEWRITE_Append))
        {
            UE_LOG(LogTemp, Error, TEXT("Scanner: 无法写入数据到文件: %s"), *OutputFilePath);
        }
        
        // 清空缓冲区
        DataBuffer.Empty();
    }
}

void UObstacleScannerComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    // 确保所有数据都被写入
    FlushBuffer();
    
    Super::EndPlay(EndPlayReason);
}

void UObstacleScannerComponent::OnScanTimer()
{
    AActor* Owner = GetOwner();
    if (Owner)
    {
        // 使用高精度时间戳
        double CurrentTime = FPlatformTime::Seconds();
        FString TimeString = FString::Printf(TEXT("%.3f"), CurrentTime);
        
        // 输出调试信息
        UE_LOG(LogTemp, Log, TEXT("Scanner: 执行定时扫描，时间戳: %s"), 
            *TimeString);
            
        ScanArea(Owner->GetActorLocation(), ScanRadius, ScanHeight);
    }
}

void UObstacleScannerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
    
    // 每帧检查是否需要刷新缓冲区
    if (DataBuffer.Num() > 0)
    {
        FlushBuffer();
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
    
    // 记录开始时间
    double ScanStartTime = FPlatformTime::Seconds();
    
    FRotator OwnerRotation = Owner->GetActorRotation();
    FVector ForwardVector = OwnerRotation.Vector();
    FVector RightVector = FVector::CrossProduct(FRotator(0, OwnerRotation.Yaw, 0).Vector(), FVector::UpVector);
    FVector UpVector = FVector::CrossProduct(ForwardVector, RightVector);

    // 使用可配置的高度偏移
    FVector ScanStartPoint = Center + FVector(0, 0, ScanStartHeight);

    // 计算扫描范围
    float StartDistance = 50.0f;  // 从扫描起点前方50厘米开始扫描
    float EndDistance = Radius;   // 扫描到最大半径
    
    // 计算垂直角度步长
    float VerticalStep = FMath::DegreesToRadians(VerticalScanAngle) / (NumScanLines - 1);
    float VerticalStart = -FMath::DegreesToRadians(VerticalScanAngle) / 2.0f;
    
    // 计算水平角度步长
    float HorizontalStep = FMath::DegreesToRadians(HorizontalScanAngle) / (PointsPerLine - 1);
    float HorizontalStart = -FMath::DegreesToRadians(HorizontalScanAngle) / 2.0f;

    // 创建扫描数据数组
    TArray<FScanData> ScanDataArray;
    ScanDataArray.SetNum(NumScanLines * PointsPerLine);

    // 记录并行处理开始时间
    double ParallelStartTime = FPlatformTime::Seconds();

    // 使用ParallelFor并行处理扫描线
    ParallelFor(NumScanLines, [&](int32 LineIndex)
    {
        
        double ThreadStartTime = FPlatformTime::Seconds();
        
        // 计算当前线的垂直角度
        float VerticalAngle = VerticalStart + LineIndex * VerticalStep;
        
        // 计算当前线的垂直方向（相对于水平面）
        FVector VerticalDirection = FVector::UpVector * FMath::Sin(VerticalAngle);
        
        // 对当前线进行水平扫描
        for (int32 PointIndex = 0; PointIndex < PointsPerLine; PointIndex++)
        {
            // 计算当前点的水平角度
            float HorizontalAngle = HorizontalStart + PointIndex * HorizontalStep;
            
            // 计算水平方向（相对于前向方向）
            FVector HorizontalDirection = ForwardVector * FMath::Cos(HorizontalAngle) + RightVector * FMath::Sin(HorizontalAngle);
            
            // 组合垂直和水平方向
            FVector Direction = (HorizontalDirection + VerticalDirection).GetSafeNormal();
            
            // 计算扫描起点和终点
            FVector Start = ScanStartPoint + Direction * StartDistance;
            FVector End = ScanStartPoint + Direction * EndDistance;

            // 执行射线检测
            FHitResult HitResult;
            FCollisionQueryParams QueryParams;
            QueryParams.AddIgnoredActor(Owner);
            QueryParams.bTraceComplex = false;
            QueryParams.bReturnPhysicalMaterial = false;

            bool bHit = GetWorld()->LineTraceSingleByChannel(
                HitResult,
                Start,
                End,
                ECC_Visibility,
                QueryParams
            );

            // 存储扫描结果
            int32 ArrayIndex = LineIndex * PointsPerLine + PointIndex;
            ScanDataArray[ArrayIndex].bHit = bHit;
            if (bHit)
            {
                ScanDataArray[ArrayIndex].HitLocation = HitResult.Location;
            }
        }

        // 记录每个线程的结束时间
        // double ThreadEndTime = FPlatformTime::Seconds();
        // UE_LOG(LogTemp, Log, TEXT("Thread %d processed line %d in %.3f ms"), 
        //     FPlatformTLS::GetCurrentThreadId(), 
        //     LineIndex, 
        //     (ThreadEndTime - ThreadStartTime) * 1000.0);
    });

    // 记录并行处理结束时间
    // double ParallelEndTime = FPlatformTime::Seconds();
    // UE_LOG(LogTemp, Log, TEXT("Parallel processing took %.3f ms"), (ParallelEndTime - ParallelStartTime) * 1000.0);

    // 处理扫描结果
    for (const FScanData& ScanData : ScanDataArray)
    {
        if (ScanData.bHit)
        {
            // 保存扫描结果到文件
            SaveScanResultToFile(ScanData.HitLocation);

            // 调试可视化
            if (bShowDebugVisualization)
            {
                DrawDebugSphere(
                    GetWorld(),        // 当前世界对象
                    ScanData.HitLocation,  // 球体中心位置(障碍物击中点)
                    1.0f,            // 球体半径(厘米)
                    8,                // 球体分段数(越高越精细)
                    FColor::Red,      // 球体颜色(红色)
                    false,            // 是否持久化(否)
                    ScanInterval,     // 显示持续时间(与扫描间隔相同)
                    0,                // 深度优先级(0表示最前)
                    1.0f              // 线条粗细
                );
            }
        }
    }

    // 记录总处理时间
    double EndTime = FPlatformTime::Seconds();
    UE_LOG(LogTemp, Log, TEXT("Total scan processing took %.3f ms"), (EndTime - ScanStartTime) * 1000.0);
}

void UObstacleScannerComponent::PerformRaycast(const FVector& Start, const FVector& End)
{
    if (!GetWorld())
        return;

    FHitResult HitResult;
    FCollisionQueryParams QueryParams;
    QueryParams.AddIgnoredActor(GetOwner());
    QueryParams.bTraceComplex = false;  // 使用简单碰撞检测以提高性能
    QueryParams.bReturnPhysicalMaterial = false;  // 不需要物理材质信息

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
                ScanInterval,  // 使用扫描间隔作为持续时间
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
                ScanInterval,  // 使用扫描间隔作为持续时间
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
    }
}

float UObstacleScannerComponent::GetGroundHeight(const FVector& Location) const
{
    // 检查网格地图是否有效
    if (!GridMap)
        return 0.0f;

    // 设置射线检测的起点和终点
    FVector Start = Location;
    FVector End = Start;
    End.Z -= GroundCheckDistance;  // 向下延伸检测距离

    // 设置射线检测参数
    FHitResult HitResult;
    FCollisionQueryParams QueryParams;
    QueryParams.AddIgnoredActor(GetOwner());  // 忽略组件所属的Actor

    // 执行射线检测
    if (GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Visibility, QueryParams))
    {
        return HitResult.Location.Z;  // 返回检测到的地面高度
    }

    return 0.0f;  // 未检测到地面时返回0
}

bool UObstacleScannerComponent::IsLocationOccupied(const FVector& Location) const
{
    if (!GridMap)
        return false;

    return GridMap->IsOccupied(Location);
}


UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ObstacleScanner")
float ScanInterval = 0.5f; // 修改为5.0秒

UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ObstacleScanner")
float ScanStartHeight = 100.0f;  // 默认1米