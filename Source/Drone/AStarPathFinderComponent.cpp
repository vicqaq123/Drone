// AStarPathFinderComponent.cpp
#include "AStarPathFinderComponent.h"
#include "DrawDebugHelpers.h"
#include "PathModifierComponent.h"

// Helper struct for A* algorithm
struct FAStarNode
{
    FVector Position;      // World position
    int GridX, GridY, GridZ; // Grid coordinates
    float GScore;          // Cost from start
    float FScore;          // Total cost (GScore + Heuristic)
    FAStarNode* Parent;    // Parent node
    
    FAStarNode(FVector InPos, int X, int Y, int Z)
        : Position(InPos), GridX(X), GridY(Y), GridZ(Z),
          GScore(FLT_MAX), FScore(FLT_MAX), Parent(nullptr)
    {
    }
    
    bool operator==(const FAStarNode& Other) const
    {
        return GridX == Other.GridX && GridY == Other.GridY && GridZ == Other.GridZ;
    }
};

// Node comparison operator for priority queue
struct FNodeCompare
{
    bool operator()(const FAStarNode& A, const FAStarNode& B) const
    {
        return A.FScore > B.FScore; // Lower FScore has higher priority
    }
};

// 添加最大搜索步数和超时时间常量
const int32 MAX_SEARCH_STEPS = 100000;
const float MAX_SEARCH_TIME = 1.0f; // 1秒超时

UAStarPathFinderComponent::UAStarPathFinderComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
    Rounds = 0;
    GridResolution = 0.0f;
}

void UAStarPathFinderComponent::InitGridMap(const FVector& MapSize, float InGridResolution)
{
    GridResolution = InGridResolution;
    PoolSize = FIntVector(
        FMath::CeilToInt(MapSize.X / GridResolution),
        FMath::CeilToInt(MapSize.Y / GridResolution),
        FMath::CeilToInt(MapSize.Z / GridResolution)
    );
    CenterIdx = PoolSize / 2;

    UE_LOG(LogTemp, Log, TEXT("AStarPathFinder: 初始化网格地图 - 分辨率: %.2f, 大小: (%d, %d, %d), 中心: (%d, %d, %d)"),
        GridResolution, PoolSize.X, PoolSize.Y, PoolSize.Z, CenterIdx.X, CenterIdx.Y, CenterIdx.Z);

    // 初始化三维网格地图
    GridNodeMap.SetNum(PoolSize.X);
    for (int32 i = 0; i < PoolSize.X; i++)
    {
        GridNodeMap[i].SetNum(PoolSize.Y);
        for (int32 j = 0; j < PoolSize.Y; j++)
        {
            GridNodeMap[i][j].SetNum(PoolSize.Z);
            for (int32 k = 0; k < PoolSize.Z; k++)
            {
                GridNodeMap[i][j][k] = new FGridNode();
            }
        }
    }
    UE_LOG(LogTemp, Log, TEXT("AStarPathFinder: 网格地图初始化完成"));
}

bool UAStarPathFinderComponent::FindPath(const FVector& Start, const FVector& Goal, TArray<FVector>& OutPath)
{
    // 尝试获取 GridMap 组件
    if (!GridMap)
    {
        // 从同一 Actor 获取
        GridMap = GetOwner()->FindComponentByClass<UGridMapComponent>();
        
        if (!GridMap)
        {
            UE_LOG(LogTemp, Error, TEXT("AStarPathFinder: 无法找到 GridMap 组件"));
            return false;
        }
        else
        {
            UE_LOG(LogTemp, Log, TEXT("AStarPathFinder: 成功获取 GridMap 组件"));
        }
    }
    
    UE_LOG(LogTemp, Log, TEXT("AStarPathFinder: 开始寻路 - 起点: (%.2f, %.2f, %.2f), 终点: (%.2f, %.2f, %.2f)"),
        Start.X, Start.Y, Start.Z, Goal.X, Goal.Y, Goal.Z);

    // 初始化网格地图
    InitGridMap(GridMap->GetMapSize(), GridMap->GetResolution());

    // 执行A*搜索
    EAStarResult Result = Search(Start, Goal);
    if (Result != EAStarResult::SUCCESS)
    {
        UE_LOG(LogTemp, Warning, TEXT("AStarPathFinder: 路径搜索失败 - 错误代码: %d"), static_cast<int32>(Result));
        return false;
    }
    
    // 转换路径格式
    OutPath.Empty();
    for (FGridNode* Node : GridPath)
    {
        OutPath.Add(Index2Coord(Node->Index));
    }
    Algo::Reverse(OutPath);

    UE_LOG(LogTemp, Log, TEXT("AStarPathFinder: 找到路径 - 路径点数: %d"), OutPath.Num());

    // // 对路径进行平滑处理
    // ChaikinSmoothPath(OutPath, 2);
    // UE_LOG(LogTemp, Log, TEXT("AStarPathFinder: 路径平滑完成"));

    // 更新存储的路径
    StoredPath = OutPath;

    // 更新 PathModifier 组件的路径
    if (AActor* Owner = GetOwner())
    {
        if (UPathModifierComponent* PathModifier = Owner->FindComponentByClass<UPathModifierComponent>())
        {
            PathModifier->SetPath(OutPath);
            UE_LOG(LogTemp, Warning, TEXT("AStarPathFinder: 已更新 PathModifier 的路径"));
        }
    }

    // 清理内存
    for (int32 i = 0; i < PoolSize.X; i++)
    {
        for (int32 j = 0; j < PoolSize.Y; j++)
        {
            for (int32 k = 0; k < PoolSize.Z; k++)
            {
                delete GridNodeMap[i][j][k];
            }
        }
    }

    return true;
}

EAStarResult UAStarPathFinderComponent::Search(const FVector& StartPt, const FVector& EndPt)
{
    Rounds++;
    UE_LOG(LogTemp, Log, TEXT("AStarPathFinder: 开始第 %d 轮搜索"), Rounds);

    FIntVector StartIdx, EndIdx;
    if (!ConvertToIndexAndAdjustStartEndPoints(StartPt, EndPt, StartIdx, EndIdx))
    {
        UE_LOG(LogTemp, Error, TEXT("AStarPathFinder: 起点或终点转换失败"));
        return EAStarResult::INIT_ERR;
    }

    UE_LOG(LogTemp, Log, TEXT("AStarPathFinder: 起点索引: (%d, %d, %d), 终点索引: (%d, %d, %d)"),
        StartIdx.X, StartIdx.Y, StartIdx.Z, EndIdx.X, EndIdx.Y, EndIdx.Z);

    // 初始化起点和终点
    FGridNode* StartPtr = GridNodeMap[StartIdx.X][StartIdx.Y][StartIdx.Z];
    FGridNode* EndPtr = GridNodeMap[EndIdx.X][EndIdx.Y][EndIdx.Z];

    // 清空开放列表
    OpenSet.Empty();
    GridPath.Empty();

    // 设置起点参数
    StartPtr->Index = StartIdx;
    StartPtr->Rounds = Rounds;
    StartPtr->GScore = 0.0f;
    StartPtr->FScore = GetHeu(StartPtr, EndPtr);
    StartPtr->State = ENodeState::OPENSET;
    StartPtr->CameFrom = nullptr;
    OpenSet.Add(StartPtr);

    int32 SearchSteps = 0;
    // A*主循环
    while (OpenSet.Num() > 0)
    {
        SearchSteps++;
        if (SearchSteps % 1000 == 0)
        {
            // UE_LOG(LogTemp, Log, TEXT("AStarPathFinder: 搜索步数: %d, 开放列表大小: %d"), SearchSteps, OpenSet.Num());
        }

        // 获取F值最小的节点
        FGridNode* Current = OpenSet[0];
        OpenSet.RemoveAt(0);

        // 检查是否到达终点
        if (Current->Index == EndPtr->Index)
        {
            GridPath = RetrievePath(Current);
            UE_LOG(LogTemp, Log, TEXT("AStarPathFinder: 找到路径 - 总步数: %d"), SearchSteps);
            return EAStarResult::SUCCESS;
        }

        Current->State = ENodeState::CLOSEDSET;

        // 遍历相邻节点
        for (int32 dx = -1; dx <= 1; dx++)
        {
            for (int32 dy = -1; dy <= 1; dy++)
            {
                for (int32 dz = -1; dz <= 1; dz++)
                {
                    if (dx == 0 && dy == 0 && dz == 0)
                        continue;

                    FIntVector NeighborIdx = Current->Index + FIntVector(dx, dy, dz);

                    // 检查边界
                    if (NeighborIdx.X < 1 || NeighborIdx.X >= PoolSize.X - 1 ||
                        NeighborIdx.Y < 1 || NeighborIdx.Y >= PoolSize.Y - 1 ||
                        NeighborIdx.Z < 1 || NeighborIdx.Z >= PoolSize.Z - 1)
                    {
                        continue;
                    }

                    FGridNode* NeighborPtr = GridNodeMap[NeighborIdx.X][NeighborIdx.Y][NeighborIdx.Z];
                    NeighborPtr->Index = NeighborIdx;

                    // 检查节点状态
                    bool bExplored = NeighborPtr->Rounds == Rounds;
                    if (bExplored && NeighborPtr->State == ENodeState::CLOSEDSET)
                        continue;

                    NeighborPtr->Rounds = Rounds;

                    // 检查是否是障碍物
                    if (GridMap && GridMap->IsOccupied(Index2Coord(NeighborPtr->Index)))
                    {
                        // UE_LOG(LogTemp,Warning,TEXT("(%.2f,%.2f,%.2f)"),Index2Coord(NeighborPtr->Index).X,Index2Coord(NeighborPtr->Index).Y,Index2Coord(NeighborPtr->Index).Z);
                        continue;
                    }

                    // 计算代价
                    float StaticCost = FVector(dx, dy, dz).Size();
                    float TentativeGScore = Current->GScore + StaticCost;

                    if (!bExplored)
                    {
                        NeighborPtr->State = ENodeState::OPENSET;
                        NeighborPtr->CameFrom = Current;
                        NeighborPtr->GScore = TentativeGScore;
                        NeighborPtr->FScore = TentativeGScore + GetEuclHeu(NeighborPtr, EndPtr);
                        OpenSet.Add(NeighborPtr);
                    }
                    else if (TentativeGScore < NeighborPtr->GScore)
                    {
                        NeighborPtr->CameFrom = Current;
                        NeighborPtr->GScore = TentativeGScore;
                        NeighborPtr->FScore = TentativeGScore + GetEuclHeu(NeighborPtr, EndPtr);
                    }
                }
            }
        }
    }

    UE_LOG(LogTemp, Warning, TEXT("AStarPathFinder: 搜索失败 - 未找到路径, 总步数: %d"), SearchSteps);
    return EAStarResult::SEARCH_ERR;
}

bool UAStarPathFinderComponent::ConvertToIndexAndAdjustStartEndPoints(const FVector& StartPt, const FVector& EndPt, FIntVector& StartIdx, FIntVector& EndIdx)
{
    FVector Start = StartPt;
    FVector End = EndPt;
    if (!Coord2Index(Start, StartIdx) || !Coord2Index(End, EndIdx))
    {
        UE_LOG(LogTemp, Error, TEXT("AStarPathFinder: 坐标转换失败 - 起点: (%.2f, %.2f, %.2f), 终点: (%.2f, %.2f, %.2f)"),
            Start.X, Start.Y, Start.Z, End.X, End.Y, End.Z);
        return false;
    }

    // 检查起点是否在障碍物内
    if (CheckOccupancy(Index2Coord(StartIdx)))
    {
        UE_LOG(LogTemp, Warning, TEXT("AStarPathFinder: 起点在障碍物内，尝试调整"));
        FVector Direction = (End - Start).GetSafeNormal();
        Start += Direction * GridResolution;
        if (!Coord2Index(Start, StartIdx))
        {
            UE_LOG(LogTemp, Error, TEXT("AStarPathFinder: 起点调整失败"));
            return false;
        }
        UE_LOG(LogTemp, Log, TEXT("AStarPathFinder: 起点已调整到: (%.2f, %.2f, %.2f)"), Start.X, Start.Y, Start.Z);
    }

    // 检查终点是否在障碍物内
    if (CheckOccupancy(Index2Coord(EndIdx)))
    {
        UE_LOG(LogTemp, Warning, TEXT("AStarPathFinder: 终点在障碍物内，尝试调整"));
        FVector Direction = (Start - End).GetSafeNormal();
        End += Direction * GridResolution;
        if (!Coord2Index(End, EndIdx))
        {
            UE_LOG(LogTemp, Error, TEXT("AStarPathFinder: 终点调整失败"));
            return false;
        }
        UE_LOG(LogTemp, Log, TEXT("AStarPathFinder: 终点已调整到: (%.2f, %.2f, %.2f)"), End.X, End.Y, End.Z);
    }

    return true;
}

bool UAStarPathFinderComponent::Coord2Index(const FVector& Coord, FIntVector& Index) const
{
    Index.X = FMath::RoundToInt(Coord.X / GridResolution) + CenterIdx.X;
    Index.Y = FMath::RoundToInt(Coord.Y / GridResolution) + CenterIdx.Y;
    Index.Z = FMath::RoundToInt(Coord.Z / GridResolution) + CenterIdx.Z;
    
    bool bIsValid = (Index.X >= 0 && Index.X < PoolSize.X &&
                     Index.Y >= 0 && Index.Y < PoolSize.Y &&
                     Index.Z >= 0 && Index.Z < PoolSize.Z);
                     
    if (!bIsValid)
    {
        UE_LOG(LogTemp, Warning, TEXT("AStarPathFinder: 坐标超出范围 - 坐标: (%.2f, %.2f, %.2f), 索引: (%d, %d, %d), 范围: (%d, %d, %d)"),
            Coord.X, Coord.Y, Coord.Z, Index.X, Index.Y, Index.Z, PoolSize.X, PoolSize.Y, PoolSize.Z);
    }
    
    return bIsValid;
}

FVector UAStarPathFinderComponent::Index2Coord(const FIntVector& Index) const
{
    return FVector(
        (Index.X - CenterIdx.X) * GridResolution,
        (Index.Y - CenterIdx.Y) * GridResolution,
        (Index.Z - CenterIdx.Z) * GridResolution
    );
}

float UAStarPathFinderComponent::GetHeu(const FGridNode* Node1, const FGridNode* Node2) const
{
    if (!Node1 || !Node2) return FLT_MAX;
    
    // 使用欧几里得距离作为启发式函数
    return GetEuclHeu(Node1, Node2);
}

float UAStarPathFinderComponent::GetDiagHeu(const FGridNode* Node1, const FGridNode* Node2) const
{
    if (!Node1 || !Node2) return FLT_MAX;
    
    float dx = static_cast<float>(FMath::Abs(Node1->Index.X - Node2->Index.X));
    float dy = static_cast<float>(FMath::Abs(Node1->Index.Y - Node2->Index.Y));
    float dz = static_cast<float>(FMath::Abs(Node1->Index.Z - Node2->Index.Z));
    
    float min = FMath::Min3(dx, dy, dz);
    float max = FMath::Max3(dx, dy, dz);
    float mid = dx + dy + dz - min - max;
    
    // 修复 Sqrt 调用的类型转换问题
    const float sqrt3 = FMath::Sqrt(3.0f);
    const float sqrt2 = FMath::Sqrt(2.0f);
    return (sqrt3 - sqrt2) * min + (sqrt2 - 1.0f) * mid + max;
}

float UAStarPathFinderComponent::GetManhHeu(const FGridNode* Node1, const FGridNode* Node2) const
{
    if (!Node1 || !Node2) return FLT_MAX;
    
    return static_cast<float>(FMath::Abs(Node1->Index.X - Node2->Index.X) +
           FMath::Abs(Node1->Index.Y - Node2->Index.Y) +
           FMath::Abs(Node1->Index.Z - Node2->Index.Z));
}

float UAStarPathFinderComponent::GetEuclHeu(const FGridNode* Node1, const FGridNode* Node2) const
{
    if (!Node1 || !Node2) return FLT_MAX;
    
    float dx = static_cast<float>(Node1->Index.X - Node2->Index.X);
    float dy = static_cast<float>(Node1->Index.Y - Node2->Index.Y);
    float dz = static_cast<float>(Node1->Index.Z - Node2->Index.Z);
    
    // 使用 FVector 的 Size 函数来计算欧几里得距离
    return FVector(dx, dy, dz).Size();
}

TArray<FGridNode*> UAStarPathFinderComponent::RetrievePath(FGridNode* Current)
{
    TArray<FGridNode*> Path;
    while (Current)
    {
        Path.Add(Current);
        Current = Current->CameFrom;
    }
    return Path;
}

bool UAStarPathFinderComponent::CheckOccupancy(const FVector& Position) const
{
    // if (!GridMap)
    // {
    //     UE_LOG(LogTemp, Error, TEXT("AStarPathFinder: GridMap未设置，无法检查占用"));
    //     return true;
    // }
    bool bIsOccupied = GridMap->IsOccupied(Position);
    // if (bIsOccupied)
    // {
    //     UE_LOG(LogTemp, Log, TEXT("AStarPathFinder: 位置被占用 - (%.2f, %.2f, %.2f)"), Position.X, Position.Y, Position.Z);
    // }
    return bIsOccupied;
}

void UAStarPathFinderComponent::VisualizePath(float Duration)
{
    if (!GetWorld())
    {
        UE_LOG(LogTemp, Error, TEXT("VisualizePath: No valid world found!"));
        return;
    }
    // FlushPersistentDebugLines(GetWorld());

    if (StoredPath.Num() < 2)
    {
        UE_LOG(LogTemp, Warning, TEXT("VisualizePath: Not enough path points! Current points: %d"), StoredPath.Num());
        return;
    }
    
    UE_LOG(LogTemp, Log, TEXT("VisualizePath: Starting visualization"));
    
    
    // Draw lines between path points
    for (int i = 0; i < StoredPath.Num() - 1; i++)
    {
        // 使用白色线条
        DrawDebugLine(
            GetWorld(),
            StoredPath[i],
            StoredPath[i+1],
            FColor::White,  // 白色线条
            false,
            Duration,
            0,
            1.0f  // 细线条
        );
    }
    
    UE_LOG(LogTemp, Log, TEXT("VisualizePath: Visualization completed"));
}

// Chaikin曲线细分平滑算法
void UAStarPathFinderComponent::ChaikinSmoothPath(TArray<FVector>& Path, int Iterations)
{
    if (Path.Num() < 2) return;
    
    // 在平滑之前，先移除太近的点
    TArray<FVector> FilteredPath;
    FilteredPath.Add(Path[0]);
    
    const float MinDistance = 10.0f; // 最小距离阈值
    for (int32 i = 1; i < Path.Num(); ++i)
    {
        float Distance = FVector::Distance(FilteredPath.Last(), Path[i]);
        if (Distance >= MinDistance)
        {
            FilteredPath.Add(Path[i]);
        }
    }
    
    Path = FilteredPath;
    
    // 执行 Chaikin 平滑
    for (int32 i = 0; i < Iterations; ++i)
    {
        TArray<FVector> NewPath;
        NewPath.Add(Path[0]); // 保持起点不变
        
        for (int32 j = 0; j < Path.Num() - 1; ++j)
        {
            FVector P0 = Path[j];
            FVector P1 = Path[j + 1];
            
            // 计算新的控制点
            FVector Q = P0 + (P1 - P0) * PathSmoothingFactor;
            FVector R = P1 + (P0 - P1) * PathSmoothingFactor;
            
            // 检查新点是否会导致方向突变
            if (j > 0)
            {
                FVector PrevDir = (P0 - Path[j-1]).GetSafeNormal();
                FVector NewDir = (Q - P0).GetSafeNormal();
                float DotProduct = FVector::DotProduct(PrevDir, NewDir);
                
                // 如果方向变化太大，调整新点的位置
                if (DotProduct < 0.7f) // 约45度
                {
                    Q = P0 + (P1 - P0) * (PathSmoothingFactor * 0.5f);
                }
            }
            
            NewPath.Add(Q);
            NewPath.Add(R);
        }
        
        NewPath.Add(Path.Last()); // 保持终点不变
        Path = NewPath;
    }
}

void UAStarPathFinderComponent::UpdateStoredPath(const TArray<FVector>& NewPath)
{
    // 清空之前的可视化
    if (PathSpline)
    {
        PathSpline->DestroyComponent();
        PathSpline = nullptr;
    }
    
    // 清除所有调试线条
    if (GetWorld())
    {
        FlushPersistentDebugLines(GetWorld());
    }
    
    StoredPath = NewPath;
    
    // 重新可视化路径
    VisualizePath(50.0f); // 可视化新路径，持续5秒
}

void UAStarPathFinderComponent::SetPath(const TArray<FVector>& InPath)
{
    CurrentPath = InPath;
    UE_LOG(LogTemp, Warning, TEXT("[PathModifier] SetPath 被调用，路径点数: %d"), CurrentPath.Num());
    for (int32 i = 0; i < CurrentPath.Num(); ++i)
    // {
    //     UE_LOG(LogTemp, Warning, TEXT("[PathModifier] 路径点[%d]: %s"), i, *CurrentPath[i].ToString());
    // }
    // 自动调用UpdateStoredPath以更新存储的路径并重新可视化
    UpdateStoredPath(CurrentPath);
}

TArray<FVector> UAStarPathFinderComponent::GetSearchedPath()
{
    return SearchedPath;
}

void UAStarPathFinderComponent::BeginPlay()
{
    Super::BeginPlay();
    
    // 初始化组件
    if (!GridMap)
    {
        UE_LOG(LogTemp, Warning, TEXT("AStarPathFinder: GridMap组件未设置"));
    }
}

void UAStarPathFinderComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
    
    // 如果需要，可以在这里添加每帧更新的逻辑
}

USplineComponent* UAStarPathFinderComponent::CreatePathSpline()
{
    if (!GetOwner())
    {
        UE_LOG(LogTemp, Error, TEXT("AStarPathFinder: 无法创建路径样条线 - 没有有效的所有者"));
        return nullptr;
    }

    // 如果已经存在样条线组件，先销毁它
    if (PathSpline)
    {
        PathSpline->DestroyComponent();
        PathSpline = nullptr;
    }

    // 创建新的样条线组件
    PathSpline = NewObject<USplineComponent>(GetOwner(), TEXT("PathSpline"));
    if (PathSpline)
    {
        PathSpline->RegisterComponent();
        PathSpline->SetMobility(EComponentMobility::Movable);
        PathSpline->SetWorldLocation(GetOwner()->GetActorLocation());
        
        // 设置样条线属性
        PathSpline->SetSplinePoints(StoredPath, ESplineCoordinateSpace::World);
        PathSpline->SetSplinePointType(0, ESplinePointType::Linear);
        PathSpline->SetSplinePointType(StoredPath.Num() - 1, ESplinePointType::Linear);
        
        UE_LOG(LogTemp, Log, TEXT("AStarPathFinder: 成功创建路径样条线"));
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("AStarPathFinder: 创建路径样条线失败"));
    }

    return PathSpline;
}