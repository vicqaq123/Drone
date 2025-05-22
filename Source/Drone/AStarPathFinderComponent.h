// AStarPathFinderComponent.h
#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Components/SplineComponent.h"
#include "GridMapComponent.h"
#include "AStarPathFinderComponent.generated.h"

UCLASS(ClassGroup=(PathPlanning), meta=(BlueprintSpawnableComponent))
class DRONE_API UAStarPathFinderComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    // 设置默认值
    UAStarPathFinderComponent();

    // 设置网格地图组件引用
    UFUNCTION(BlueprintCallable, Category="PathPlanning|AStar")
    void SetGridMap(UGridMapComponent* InGridMap);
    
    // 寻找从起点到终点的路径
    UFUNCTION(BlueprintCallable, Category="PathPlanning|AStar")
    bool FindPath(const FVector& Start, const FVector& Goal, TArray<FVector>& OutPath);
    
    // 获取保存的路径
    UFUNCTION(BlueprintCallable, Category="PathPlanning|AStar")
    TArray<FVector> GetSearchedPath();
    
    // 可视化路径
    UFUNCTION(BlueprintCallable, Category="PathPlanning|AStar")
    void VisualizePath(float Duration = 0.0f);
    
    // 将路径转换为Spline
    UFUNCTION(BlueprintCallable, Category="PathPlanning|AStar")
    USplineComponent* CreatePathSpline();
    
    // 设置路径平滑度
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="PathPlanning|AStar")
    float PathSmoothingFactor = 0.5f;
    
    // 设置路径点间距
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="PathPlanning|AStar")
    float PathPointSpacing = 100.0f;
    
    // 设置是否自动生成Spline
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="PathPlanning|AStar")
    bool bAutoCreateSpline = true;
    
    // 设置Spline颜色
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="PathPlanning|AStar")
    FLinearColor SplineColor = FLinearColor::Green;
    
    // 设置Spline宽度
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="PathPlanning|AStar")
    float SplineWidth = 1.0f;
    
    // 设置是否使用细线条模式
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="PathPlanning|AStar")
    bool bUseThinLine = true;
    
    // 设置线条透明度
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="PathPlanning|AStar", meta=(ClampMin="0.0", ClampMax="1.0"))
    float LineOpacity = 0.8f;
    
    // 获取存储的样条曲线点
    UFUNCTION(BlueprintCallable, Category = "PathPlanning|AStar")
    TArray<FVector> GetStoredSplinePoints() const;
    
    // 获取存储的样条曲线长度
    UFUNCTION(BlueprintCallable, Category = "PathPlanning|AStar")
    float GetStoredSplineLength() const;
    
    // 获取样条曲线上的点
    UFUNCTION(BlueprintCallable, Category = "PathPlanning|AStar")
    FVector GetLocationAlongSpline(float Distance) const;
    
    // 获取样条曲线上的位置和方向
    UFUNCTION(BlueprintCallable, Category = "PathPlanning|AStar")
    FTransform GetTransformAlongSpline(float Distance) const;
    
    // 获取样条曲线上的速度
    UFUNCTION(BlueprintCallable, Category = "PathPlanning|AStar")
    FVector GetVelocityAlongSpline(float Distance, float DeltaTime) const;

    // 设置无人机尺寸
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="PathPlanning|AStar")
    float DroneRadius = 10.0f;  // 无人机半径（厘米）
    
    // 设置安全距离系数
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="PathPlanning|AStar", meta=(ClampMin="1.0", ClampMax="3.0"))
    float SafetyFactor = 1.1f;  // 安全距离系数，实际安全距离 = DroneRadius * SafetyFactor

    // Chaikin曲线细分平滑算法声明
    void ChaikinSmoothPath(TArray<FVector>& Path, int Iterations);

protected:
    virtual void BeginPlay() override;
    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
    // 网格地图组件的引用
    UPROPERTY()
    UGridMapComponent* GridMap;
    
    // 存储的路径
    TArray<FVector> StoredPath;
    
    // Spline组件引用
    UPROPERTY()
    USplineComponent* PathSpline;
    
    // 存储的样条曲线数据
    TArray<FVector> StoredSplinePoints;
    float StoredSplineLength;
    
    // 启发式函数
    float GetDiagonalHeuristic(int X1, int Y1, int Z1, int X2, int Y2, int Z2);
    float GetManhattanHeuristic(int X1, int Y1, int Z1, int X2, int Y2, int Z2);
    float GetEuclideanHeuristic(int X1, int Y1, int Z1, int X2, int Y2, int Z2);
    
    // 计算两个节点间的距离
    float Distance(struct FAStarNode* A, struct FAStarNode* B);
    
    // 获取周围的临近节点（优化版本）
    void GetNeighborsOptimized(struct FAStarNode* Node, TArray<struct FAStarNode*>& OutNeighbors, int32 GoalX, int32 GoalY, int32 GoalZ);
    
    // 重建路径
    void ReconstructPath(struct FAStarNode* GoalNode, TArray<FVector>& OutPath);
    
    // 平滑路径点
    void SmoothPath(TArray<FVector>& Path);
    
    // 存储样条曲线数据
    void StoreSplineData();

    // 检查路径点是否安全
    bool IsPathPointSafe(const FVector& Point) const;

    // 获取路径点的安全距离
    float GetSafetyDistance() const { return DroneRadius * SafetyFactor; }
};