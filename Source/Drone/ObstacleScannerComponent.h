// ObstacleScannerComponent.h
#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GridMapComponent.h"
#include "ObstacleScannerComponent.generated.h"

UCLASS(ClassGroup=(PathPlanning), meta=(BlueprintSpawnableComponent))
class DRONE_API UObstacleScannerComponent : public UActorComponent
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



    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ObstacleScanner")
    float ObstacleInflationRadius2 = 30.0f; // 障碍物膨胀半径

    // 扫描间隔（秒）
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ObstacleScanner")
    float ScanInterval = 0.5f;

    // 自动扫描开关
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ObstacleScanner")
    bool bAutoScan = true;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ObstacleScanner")
    float ScannerObstacleInflationRadius = 60.0f; // 扫描器障碍物膨胀半径

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ObstacleScanner")
    float ObstacleUpdateTolerance = 10.0f; // 障碍物更新容差范围

    // 绘制所有障碍物
    void DrawAllObstacles();

protected:
    virtual void BeginPlay() override;
    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
    // 网格地图组件引用
    UPROPERTY()
    UGridMapComponent* GridMap;

    // 上次扫描后经过的时间
    float TimeSinceLastScan = 0.0f;


    // 执行射线检测
    void PerformRaycast(const FVector& Start, const FVector& End);

    // 更新网格地图
    void UpdateGridMap(const FVector& HitLocation, bool bIsObstacle);



    // 可视化扫描结果
    void VisualizeScanResults(const FVector& Center, float Radius, float Height);
}; 