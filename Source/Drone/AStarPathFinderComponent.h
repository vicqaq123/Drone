// AStarPathFinderComponent.h
#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Components/SplineComponent.h"
#include "GridMapComponent.h"
#include "AStarPathFinderComponent.generated.h"

// A*算法节点状态枚举
enum class ENodeState
{
    UNEXPLORED,
    OPENSET,
    CLOSEDSET
};

// A*算法网格节点结构体
struct FGridNode
{
    FIntVector Index;      // 网格索引
    float GScore;          // 从起点到当前节点的代价
    float FScore;          // 总代价 (GScore + 启发式值)
    FGridNode* CameFrom;   // 父节点
    ENodeState State;      // 节点状态
    int32 Rounds;          // 搜索轮次

    FGridNode()
        : GScore(FLT_MAX)
        , FScore(FLT_MAX)
        , CameFrom(nullptr)
        , State(ENodeState::UNEXPLORED)
        , Rounds(0)
    {
    }
};

// A*算法结果枚举
enum class EAStarResult
{
    SUCCESS,
    INIT_ERR,
    SEARCH_ERR
};

UCLASS(ClassGroup=(PathPlanning), meta=(BlueprintSpawnableComponent))
class DRONE_API UAStarPathFinderComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    // 设置默认值
    UAStarPathFinderComponent();

    // 寻找从起点到终点的路径
    UFUNCTION(BlueprintCallable, Category="PathPlanning|AStar")
    bool FindPath(const FVector& Start, const FVector& Goal, TArray<FVector>& OutPath);
    
    // 可视化路径
    UFUNCTION(BlueprintCallable, Category="PathPlanning|AStar")
    void VisualizePath(float Duration = 100.0f);
    
    // 将路径转换为Spline
    UFUNCTION(BlueprintCallable, Category="PathPlanning|AStar")
    USplineComponent* CreatePathSpline();

    // 设置路径平滑度
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="PathPlanning|AStar")
    float PathSmoothingFactor = 0.5f;

    // 设置无人机尺寸
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="PathPlanning|AStar")
    float DroneRadius = 10.0f;  // 无人机半径（厘米）
    
    // 设置安全距离系数
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="PathPlanning|AStar", meta=(ClampMin="1.0", ClampMax="3.0"))
    float SafetyFactor = 1.1f;  // 安全距离系数，实际安全距离 = DroneRadius * SafetyFactor

    // 新增：用于外部更新存储路径
    UFUNCTION(BlueprintCallable, Category="PathPlanning|AStar")
    void UpdateStoredPath(const TArray<FVector>& NewPath);

    void SetPath(const TArray<FVector>& InPath);

    // 存储搜索过程中访问的所有路径点
    UPROPERTY()
    TArray<FVector> SearchedPath;

    // 获取搜索过程中访问的所有路径点
    UFUNCTION(BlueprintCallable, Category = "Path Finding")
    TArray<FVector> GetSearchedPath();

protected:
    virtual void BeginPlay() override;
    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
    // 网格地图组件的引用
    UPROPERTY()
    UGridMapComponent* GridMap;
    
    // 存储的路径
    TArray<FVector> StoredPath;
    TArray<FVector> CurrentPath;
    
    // Spline组件引用
    UPROPERTY()
    USplineComponent* PathSpline;
    
    // 存储的样条曲线数据
    TArray<FVector> StoredSplinePoints;
    float StoredSplineLength;

    // A*算法相关变量
    int32 Rounds;  // 搜索轮次
    TArray<TArray<TArray<FGridNode*>>> GridNodeMap;  // 三维网格节点地图
    TArray<FGridNode*> OpenSet;  // 开放列表
    TArray<FGridNode*> GridPath;  // 存储找到的路径
    FIntVector PoolSize;  // 网格大小
    FIntVector CenterIdx;  // 中心索引
    float GridResolution;  // 网格分辨率

    // A*算法相关函数
    EAStarResult Search(const FVector& StartPt, const FVector& EndPt);
    bool ConvertToIndexAndAdjustStartEndPoints(const FVector& StartPt, const FVector& EndPt, FIntVector& StartIdx, FIntVector& EndIdx);
    bool Coord2Index(const FVector& Coord, FIntVector& Index) const;
    FVector Index2Coord(const FIntVector& Index) const;
    float GetHeu(const FGridNode* Node1, const FGridNode* Node2) const;
    float GetDiagHeu(const FGridNode* Node1, const FGridNode* Node2) const;
    float GetManhHeu(const FGridNode* Node1, const FGridNode* Node2) const;
    float GetEuclHeu(const FGridNode* Node1, const FGridNode* Node2) const;
    TArray<FGridNode*> RetrievePath(FGridNode* Current);
    bool CheckOccupancy(const FVector& Position) const;
    void InitGridMap(const FVector& MapSize, float InGridResolution);

    // 样条曲线相关函数
    void StoreSplineData();
    void ChaikinSmoothPath(TArray<FVector>& Path, int Iterations);
};