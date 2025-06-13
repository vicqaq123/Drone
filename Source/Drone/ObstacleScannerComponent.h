// ObstacleScannerComponent.h
#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GridMapComponent.h"
#include "HAL/PlatformFileManager.h"
#include "HAL/FileManager.h"
#include "Misc/FileHelper.h"
#include "ObstacleScannerComponent.generated.h"

// 扫描数据结构体
struct FScanData
{
    bool bHit;
    FVector HitLocation;
};

UCLASS(ClassGroup=(PathPlanning), meta=(BlueprintSpawnableComponent))
class DRONE_API UObstacleScannerComponent : public USceneComponent
{
    GENERATED_BODY()

public:
    UObstacleScannerComponent();

    // 设置网格地图组件引用
    UFUNCTION(BlueprintCallable, Category="PathPlanning|Scanner")
    void SetGridMap(UGridMapComponent* InGridMap);

    // 扫描指定区域
    UFUNCTION(BlueprintCallable, Category="PathPlanning|Scanner")
    void ScanArea(const FVector& Center, float Radius, float Height);

    // 获取地面高度
    UFUNCTION(BlueprintCallable, Category="PathPlanning|Scanner")
    float GetGroundHeight(const FVector& Location) const;

    // 检查位置是否被占用
    UFUNCTION(BlueprintCallable, Category="PathPlanning|Scanner")
    bool IsLocationOccupied(const FVector& Location) const;

protected:
    virtual void BeginPlay() override;
    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
    // 文件操作相关
    FString OutputFilePath;
    IPlatformFile* PlatformFile;
    IFileHandle* FileHandle;
    TArray<FString> DataBuffer;
    static const int32 BUFFER_SIZE = 1000;  // 缓冲区大小
    double StartTime;  // 记录开始时间

    // 网格地图组件引用
    UPROPERTY()
    UGridMapComponent* GridMap;

    // 扫描计时器句柄
    FTimerHandle ScanTimerHandle;
    
    // 上次扫描后经过的时间
    float TimeSinceLastScan = 0.0f;

    // 私有函数
    void InitializeOutputFile();
    void SaveScanResultToFile(const FVector& Location);
    void FlushBuffer();
    void OnScanTimer();
    void PerformRaycast(const FVector& Start, const FVector& End);
    void UpdateGridMap(const FVector& HitLocation, bool bIsObstacle);

public:
    // 扫描参数
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="PathPlanning|Scanner")
    float ScanRadius = 400.0f;  // 扫描半径

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="PathPlanning|Scanner")
    float ScanHeight = 200.0f;   // 扫描高度

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="PathPlanning|Scanner")
    float RaycastSpacing = 30.0f;  // 射线间距

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="PathPlanning|Scanner")
    float GroundCheckDistance = 1000.0f;  // 地面检测距离

    // 调试可视化
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="PathPlanning|Scanner")
    bool bShowDebugVisualization = true;

    // 扫描间隔（秒）
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ObstacleScanner")
    float ScanInterval = 0.1f;  // 改为1秒扫描一次

    // 雷达线数
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ObstacleScanner", meta = (ClampMin = "1", ClampMax = "32"))
    int32 NumScanLines = 16;  // 默认8线雷达

    // 每线扫描点数
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ObstacleScanner", meta = (ClampMin = "1", ClampMax = "360"))
    int32 PointsPerLine = 300;  // 默认每线180个点

    // 垂直扫描角度范围（度）
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ObstacleScanner", meta = (ClampMin = "0", ClampMax = "180"))
    float VerticalScanAngle = 30.0f;  // 默认上下各15度

    // 水平扫描角度范围（度）
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ObstacleScanner", meta = (ClampMin = "0", ClampMax = "360"))
    float HorizontalScanAngle = 360.0f;  // 默认360度，实现全周扫描

    // 自动扫描开关
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ObstacleScanner")
    bool bAutoScan = true;

    // 扫描起点高度
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ObstacleScanner")
    float ScanStartHeight = 100.0f;  // 默认1米
};    
