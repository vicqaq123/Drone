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
}

void UAStarPathFinderComponent::SetGridMap(UGridMapComponent* InGridMap)
{
    GridMap = InGridMap;
    if (GridMap)
    {
        UE_LOG(LogTemp, Log, TEXT("AStarPathFinder: GridMap successfully set"));
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("AStarPathFinder: GridMap set to null"));
    }
}

bool UAStarPathFinderComponent::FindPath(const FVector& Start, const FVector& Goal, TArray<FVector>& OutPath)
{
    if (!GridMap)
    {
        UE_LOG(LogTemp, Error, TEXT("AStarPathFinder: GridMap is not set"));
        return false;
    }
    
    UE_LOG(LogTemp, Log, TEXT("AStarPathFinder: Starting path finding from (%.2f, %.2f, %.2f) to (%.2f, %.2f, %.2f)"),
        Start.X, Start.Y, Start.Z, Goal.X, Goal.Y, Goal.Z);
    
    // Clear previous path
    StoredPath.Empty();
    OutPath.Empty();
    
    // 获取网格地图的边界信息
    FVector MapOrigin = GridMap->GetMapOrigin();
    FVector MapSize = GridMap->GetMapSize();
    
    UE_LOG(LogTemp, Warning, TEXT("Start World: (%.2f, %.2f, %.2f)"), Start.X, Start.Y, Start.Z);
    UE_LOG(LogTemp, Warning, TEXT("Goal World: (%.2f, %.2f, %.2f)"), Goal.X, Goal.Y, Goal.Z);
    UE_LOG(LogTemp, Warning, TEXT("MapOrigin: (%.2f, %.2f, %.2f), MapSize: (%.2f, %.2f, %.2f)"),
        MapOrigin.X, MapOrigin.Y, MapOrigin.Z, MapSize.X, MapSize.Y, MapSize.Z);
    
    // Convert start and goal to grid coordinates
    int StartX, StartY, StartZ;
    int GoalX, GoalY, GoalZ;
    
    if (!GridMap->WorldToGrid(Start, StartX, StartY, StartZ))
    {
        UE_LOG(LogTemp, Warning, TEXT("AStarPathFinder: Start position is outside the grid"));
        return false;
    }
    
    if (!GridMap->WorldToGrid(Goal, GoalX, GoalY, GoalZ))
    {
        UE_LOG(LogTemp, Warning, TEXT("AStarPathFinder: Goal position is outside the grid"));
        return false;
    }
    
    UE_LOG(LogTemp, Warning, TEXT("Start Grid: (%d, %d, %d)"), StartX, StartY, StartZ);
    UE_LOG(LogTemp, Warning, TEXT("Goal Grid: (%d, %d, %d)"), GoalX, GoalY, GoalZ);
    
    // Check if start or goal is occupied
    if (GridMap->IsOccupied(Start))
    {
        UE_LOG(LogTemp, Warning, TEXT("AStarPathFinder: Start position is occupied"));
        return false;
    }
    
    if (GridMap->IsOccupied(Goal))
    {
        UE_LOG(LogTemp, Warning, TEXT("AStarPathFinder: Goal position is occupied"));
        return false;
    }
    
    UE_LOG(LogTemp, Log, TEXT("FindPath called: Start=(%.2f, %.2f, %.2f), Goal=(%.2f, %.2f, %.2f)"),
        Start.X, Start.Y, Start.Z, Goal.X, Goal.Y, Goal.Z);
    UE_LOG(LogTemp, Log, TEXT("Grid coordinates: Start=(%d, %d, %d), Goal=(%d, %d, %d)"),
        StartX, StartY, StartZ, GoalX, GoalY, GoalZ);
    
    // 使用优先队列替代TArray
    TArray<FAStarNode*> OpenSet;
    TArray<FAStarNode*> ClosedSet;
    TMap<int32, FAStarNode*> NodeMap; // 用于快速查找节点
    
    // 创建起始节点
    FAStarNode* StartNode = new FAStarNode(Start, StartX, StartY, StartZ);
    StartNode->GScore = 0;
    float InitialHScore = GetDiagonalHeuristic(StartX, StartY, StartZ, GoalX, GoalY, GoalZ);
    StartNode->FScore = InitialHScore;
    
    UE_LOG(LogTemp, Warning, TEXT("Start node: GScore=%.2f, HScore=%.2f, FScore=%.2f"), 
        StartNode->GScore, InitialHScore, StartNode->FScore);
    
    // 添加到开放集
    OpenSet.Add(StartNode);
    
    // 记录开始时间
    float StartTime = GetWorld()->GetTimeSeconds();
    int32 Steps = 0;
    
    // 主A*循环
    while (OpenSet.Num() > 0)
    {
        // 检查是否超时
        if (GetWorld()->GetTimeSeconds() - StartTime > MAX_SEARCH_TIME)
        {
            UE_LOG(LogTemp, Warning, TEXT("AStarPathFinder: Search timeout after %d steps"), Steps);
            // 清理内存
            for (FAStarNode* Node : OpenSet) delete Node;
            for (FAStarNode* Node : ClosedSet) delete Node;
            return false;
        }
        
        // 检查步数限制
        if (++Steps > MAX_SEARCH_STEPS)
        {
            UE_LOG(LogTemp, Warning, TEXT("AStarPathFinder: Exceeded max steps %d"), MAX_SEARCH_STEPS);
            // 清理内存
            for (FAStarNode* Node : OpenSet) delete Node;
            for (FAStarNode* Node : ClosedSet) delete Node;
            return false;
        }
        
        // 获取F值最小的节点
        int32 BestIndex = 0;
        float BestFScore = OpenSet[0]->FScore;
        for (int32 i = 1; i < OpenSet.Num(); i++)
        {
            if (OpenSet[i]->FScore < BestFScore)
            {
                BestFScore = OpenSet[i]->FScore;
                BestIndex = i;
            }
        }
        
        FAStarNode* Current = OpenSet[BestIndex];
        OpenSet.RemoveAt(BestIndex);
        if(Steps%10000==0)
        {UE_LOG(LogTemp, Log, TEXT("Step %d: Current node at grid (%d, %d, %d), world (%.2f, %.2f, %.2f), FScore=%.2f"), 
            Steps, Current->GridX, Current->GridY, Current->GridZ, 
            Current->Position.X, Current->Position.Y, Current->Position.Z, Current->FScore);}
        
        
        
        // 检查是否到达目标
        const float GoalTolerance = 1.0f; // 允许1个网格单位的误差
        float DistanceToGoal = FVector::Dist(
            FVector(Current->GridX, Current->GridY, Current->GridZ),
            FVector(GoalX, GoalY, GoalZ)
        );
        
        if (DistanceToGoal <= GoalTolerance)
        {
            // 如果非常接近目标，直接使用目标点作为终点
            if (DistanceToGoal > 0.0f)
            {
                FVector GoalWorldPos = GridMap->GridToWorld(GoalX, GoalY, GoalZ);
                Current->Position = GoalWorldPos;
                Current->GridX = GoalX;
                Current->GridY = GoalY;
                Current->GridZ = GoalZ;
            }
            
            // 重建路径
            ReconstructPath(Current, OutPath);
            // 对路径进行平滑处理
            ChaikinSmoothPath(OutPath, 2); // 2次迭代
            
            // 清理内存
            for (FAStarNode* Node : OpenSet) delete Node;
            for (FAStarNode* Node : ClosedSet) delete Node;
            
            StoredPath = OutPath;
            UE_LOG(LogTemp, Log, TEXT("FindPath: Found path with %d points in %d steps, final distance to goal: %.2f"), 
                OutPath.Num(), Steps, DistanceToGoal);
            
            // 在成功找到路径后自动同步到PathModifierComponent
            if (OutPath.Num() > 0)
            {
                AActor* Owner = GetOwner();
                if (Owner)
                {
                    UPathModifierComponent* PathMod = Owner->FindComponentByClass<UPathModifierComponent>();
                    if (PathMod)
                    {
                        PathMod->SetPath(OutPath);
                        UE_LOG(LogTemp, Warning, TEXT("[AStar] 已自动同步路径到PathModifierComponent，点数: %d"), OutPath.Num());
                    }
                    else
                    {
                        UE_LOG(LogTemp, Error, TEXT("[AStar] 未找到PathModifierComponent组件"));
                    }
                }
                else
                {
                    UE_LOG(LogTemp, Error, TEXT("[AStar] 未找到有效的Owner Actor"));
                }
            }
            return true;
        }
        
        // 添加到关闭集
        ClosedSet.Add(Current);
        
        // 获取邻居节点
        TArray<FAStarNode*> Neighbors;
        GetNeighborsOptimized(Current, Neighbors, GoalX, GoalY, GoalZ);
        
        // 处理邻居节点
        for (FAStarNode* Neighbor : Neighbors)
        {
            // 计算从当前节点到相邻节点的代价
            float StaticCost = FMath::Sqrt(static_cast<float>(
                FMath::Square(Neighbor->GridX - Current->GridX) +
                FMath::Square(Neighbor->GridY - Current->GridY) +
                FMath::Square(Neighbor->GridZ - Current->GridZ)
            ));
            float TentativeGScore = Current->GScore + StaticCost * GridMap->GetResolution();
            
            // 检查是否在关闭集中
            bool InClosedSet = false;
            for (FAStarNode* ClosedNode : ClosedSet)
            {
                if (*Neighbor == *ClosedNode)
                {
                    InClosedSet = true;
                    break;
                }
            }
            
            if (InClosedSet)
            {
                delete Neighbor;
                continue;
            }
            
            // 检查是否在开放集中
            bool InOpenSet = false;
            FAStarNode* OpenNode = nullptr;
            for (FAStarNode* Node : OpenSet)
            {
                if (*Node == *Neighbor)
                {
                    InOpenSet = true;
                    OpenNode = Node;
                    break;
                }
            }
            
            if (!InOpenSet)
            {
                // 新节点
                Neighbor->GScore = TentativeGScore;
                float NeighborHScore = GetDiagonalHeuristic(Neighbor->GridX, Neighbor->GridY, Neighbor->GridZ, GoalX, GoalY, GoalZ);
                Neighbor->FScore = TentativeGScore + NeighborHScore;
                Neighbor->Parent = Current;
                OpenSet.Add(Neighbor);
                if(Steps%10000==0)
                {
                UE_LOG(LogTemp, Log, TEXT("Step %d: Added new node at grid (%d, %d, %d), GScore=%.2f, HScore=%.2f, FScore=%.2f"), 
                    Steps, Neighbor->GridX, Neighbor->GridY, Neighbor->GridZ, 
                    Neighbor->GScore, NeighborHScore, Neighbor->FScore);
                }
            }
            else if (TentativeGScore < OpenNode->GScore)
            {
                // 更新现有节点
                OpenNode->GScore = TentativeGScore;
                float OpenNodeHScore = GetDiagonalHeuristic(OpenNode->GridX, OpenNode->GridY, OpenNode->GridZ, GoalX, GoalY, GoalZ);
                OpenNode->FScore = TentativeGScore + OpenNodeHScore;
                OpenNode->Parent = Current;
                delete Neighbor;
                if(Steps%10000==0)
                {
                UE_LOG(LogTemp, Log, TEXT("Step %d: Updated existing node at grid (%d, %d, %d), new GScore=%.2f, HScore=%.2f, FScore=%.2f"), 
                    Steps, OpenNode->GridX, OpenNode->GridY, OpenNode->GridZ, 
                    OpenNode->GScore, OpenNodeHScore, OpenNode->FScore);
                }
            }
            else
            {
                delete Neighbor;
            }
        }
    }
    
    // 清理内存
    for (FAStarNode* Node : ClosedSet) delete Node;
    
    UE_LOG(LogTemp, Warning, TEXT("AStarPathFinder: No path found after %d steps"), Steps);
    return false;
}

TArray<FVector> UAStarPathFinderComponent::GetSearchedPath()
{
    return StoredPath;
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
    
    UE_LOG(LogTemp, Log, TEXT("VisualizePath: Starting visualization with %d points"), StoredPath.Num());
    
    // 在起点绘制红色小球
    DrawDebugSphere(
        GetWorld(),
        StoredPath[0],  // 起点位置
        20.0f,          // 球体半径
        12,             // 球体分段数
        FColor::Red,    // 红色
        false,          // 不持久化
        Duration,
        0,
        2.0f           // 线条粗细
    );
    
    // 在终点绘制绿色小球
    DrawDebugSphere(
        GetWorld(),
        StoredPath.Last(),  // 终点位置
        20.0f,              // 球体半径
        12,                 // 球体分段数
        FColor::Green,      // 绿色
        false,              // 不持久化
        Duration,
        0,
        2.0f               // 线条粗细
    );
    
    // Draw lines between path points
    for (int i = 0; i < StoredPath.Num() - 1; i++)
    {
        // 打印每个点的坐标
        UE_LOG(LogTemp, Log, TEXT("Path[%d]: X=%.2f, Y=%.2f, Z=%.2f"), 
            i, 
            StoredPath[i].X, 
            StoredPath[i].Y, 
            StoredPath[i].Z);
        
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

float UAStarPathFinderComponent::Distance(FAStarNode* A, FAStarNode* B)
{
    // Euclidean distance
    return FVector::Dist(A->Position, B->Position);
}

// 优化后的邻居节点生成函数
void UAStarPathFinderComponent::GetNeighborsOptimized(FAStarNode* Node, TArray<FAStarNode*>& OutNeighbors, int32 GoalX, int32 GoalY, int32 GoalZ)
{
    if (!GridMap)
    {
        UE_LOG(LogTemp, Error, TEXT("GetNeighborsOptimized: GridMap is not set"));
        return;
    }
    
    // 26个方向：包括对角线移动
    const int32 Directions[26][3] = {
        // 6个主要方向
        {1,0,0}, {-1,0,0},  // 前后
        {0,1,0}, {0,-1,0},  // 左右
        {0,0,1}, {0,0,-1},  // 上下
        // 12个边线方向
        {1,1,0}, {1,-1,0}, {-1,1,0}, {-1,-1,0},  // XY平面
        {1,0,1}, {1,0,-1}, {-1,0,1}, {-1,0,-1},  // XZ平面
        {0,1,1}, {0,1,-1}, {0,-1,1}, {0,-1,-1},  // YZ平面
        // 8个对角线方向
        {1,1,1}, {1,1,-1}, {1,-1,1}, {1,-1,-1},
        {-1,1,1}, {-1,1,-1}, {-1,-1,1}, {-1,-1,-1}
    };

    int32 CurrentX = Node->GridX;
    int32 CurrentY = Node->GridY;
    int32 CurrentZ = Node->GridZ;

    int32 DirX = FMath::Sign(GoalX - CurrentX);
    int32 DirY = FMath::Sign(GoalY - CurrentY);
    int32 DirZ = FMath::Sign(GoalZ - CurrentZ);

    TArray<int32> DirectionOrder;
    if (DirX != 0 && DirY != 0 && DirZ != 0)
    {
        for (int32 i = 18; i < 26; i++)
        {
            if (Directions[i][0] == DirX && 
                Directions[i][1] == DirY && 
                Directions[i][2] == DirZ)
            {
                DirectionOrder.Add(i);
                break;
            }
        }
    }
    if (DirX != 0 && DirY != 0)
    {
        for (int32 i = 6; i < 10; i++)
        {
            if (Directions[i][0] == DirX && Directions[i][1] == DirY)
            {
                DirectionOrder.Add(i);
                break;
            }
        }
    }
    if (DirX != 0 && DirZ != 0)
    {
        for (int32 i = 10; i < 14; i++)
        {
            if (Directions[i][0] == DirX && Directions[i][2] == DirZ)
            {
                DirectionOrder.Add(i);
                break;
            }
        }
    }
    if (DirY != 0 && DirZ != 0)
    {
        for (int32 i = 14; i < 18; i++)
        {
            if (Directions[i][1] == DirY && Directions[i][2] == DirZ)
            {
                DirectionOrder.Add(i);
                break;
            }
        }
    }
    if (DirX != 0) DirectionOrder.Add(DirX > 0 ? 0 : 1);
    if (DirY != 0) DirectionOrder.Add(DirY > 0 ? 2 : 3);
    if (DirZ != 0) DirectionOrder.Add(DirZ > 0 ? 4 : 5);
    for (int32 i = 0; i < 26; i++)
    {
        if (!DirectionOrder.Contains(i))
        {
            DirectionOrder.Add(i);
        }
    }

    // 每个方向优先尝试5格，5格不可行时才尝试1格
    for (int32 i = 0; i < DirectionOrder.Num(); i++)
    {
        int32 DirectionIndex = DirectionOrder[i];
        bool bFoundValid = false;
        for (int StepIdx = 0; StepIdx < 2; ++StepIdx)
        {
            int32 StepSize = (StepIdx == 0) ? 5 : 1;
            int32 NX = CurrentX + Directions[DirectionIndex][0] * StepSize;
            int32 NY = CurrentY + Directions[DirectionIndex][1] * StepSize;
            int32 NZ = CurrentZ + Directions[DirectionIndex][2] * StepSize;
            if (NX < 0 || NX >= GridMap->GetGridDimX() ||
                NY < 0 || NY >= GridMap->GetGridDimY() ||
                NZ < 0 || NZ >= GridMap->GetGridDimZ())
            {
                continue;
            }
            FVector WorldPos = GridMap->GridToWorld(NX, NY, NZ);
            if (GridMap->IsOccupied(WorldPos) || !IsPathPointSafe(WorldPos))
            {
                continue;
            }
            // 只要大步长可行就不再尝试小步长
            FAStarNode* Neighbor = new FAStarNode(WorldPos, NX, NY, NZ);
            OutNeighbors.Add(Neighbor);
            bFoundValid = true;
            break;
        }
    }
}

void UAStarPathFinderComponent::ReconstructPath(FAStarNode* GoalNode, TArray<FVector>& OutPath)
{
    // Clear path
    OutPath.Empty();
    
    // Trace back from goal to start
    FAStarNode* Current = GoalNode;
    while (Current)
    {
        OutPath.Insert(Current->Position, 0);
        Current = Current->Parent;
    }
}

void UAStarPathFinderComponent::BeginPlay()
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
                UE_LOG(LogTemp, Error, TEXT("AStarPathFinder: No GridMap component found on owner actor"));
            }
            else
            {
                UE_LOG(LogTemp, Log, TEXT("AStarPathFinder: Successfully found GridMap component"));
            }
        }
        else
        {
            UE_LOG(LogTemp, Error, TEXT("AStarPathFinder: No valid owner actor"));
        }
    }
    
    // 检查组件是否有效
    if (!IsValid(this))
    {
        UE_LOG(LogTemp, Error, TEXT("AStarPathFinder: Component is not valid"));
    }
}

void UAStarPathFinderComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

USplineComponent* UAStarPathFinderComponent::CreatePathSpline()
{
    if (!GetWorld())
    {
        UE_LOG(LogTemp, Error, TEXT("CreatePathSpline: No valid world found!"));
        return nullptr;
    }
    
    if (!IsValid(this))
    {
        UE_LOG(LogTemp, Error, TEXT("CreatePathSpline: Component is not valid"));
        return nullptr;
    }
    
    UE_LOG(LogTemp, Log, TEXT("CreatePathSpline: Starting with %d path points"), StoredPath.Num());
    
    // 检查路径点数量
    if (StoredPath.Num() < 2)
    {
        UE_LOG(LogTemp, Warning, TEXT("CreatePathSpline: Not enough path points to create a valid spline (need at least 2 points)"));
        return nullptr;
    }
    
    // 如果已存在Spline组件，先移除
    if (PathSpline)
    {
        UE_LOG(LogTemp, Log, TEXT("CreatePathSpline: Removing existing spline component"));
        PathSpline->DestroyComponent();
        PathSpline = nullptr;
    }
    
    // 创建新的Spline组件
    AActor* Owner = GetOwner();
    if (!Owner)
    {
        UE_LOG(LogTemp, Error, TEXT("CreatePathSpline: No valid owner actor!"));
        return nullptr;
    }
    
    PathSpline = NewObject<USplineComponent>(Owner, TEXT("PathSpline"));
    if (!PathSpline)
    {
        UE_LOG(LogTemp, Error, TEXT("CreatePathSpline: Failed to create spline component!"));
        return nullptr;
    }
    
    // 注册组件
    PathSpline->RegisterComponent();
    Owner->AddInstanceComponent(PathSpline);
    
    // 确保组件被正确添加
    if (!Owner->GetComponents().Contains(PathSpline))
    {
        UE_LOG(LogTemp, Error, TEXT("CreatePathSpline: Failed to add spline component to owner"));
        return nullptr;
    }
    
    // 清空现有的点
    PathSpline->ClearSplinePoints();
    
    // 添加路径点
    for (int32 i = 0; i < StoredPath.Num(); i++)
    {
        PathSpline->AddSplinePoint(StoredPath[i], ESplineCoordinateSpace::World);
    }
    
    // 设置曲线类型和平滑度
    for (int32 i = 0; i < StoredPath.Num(); i++)
    {
        // 使用曲线类型使路径更平滑
        PathSpline->SetSplinePointType(i, ESplinePointType::Curve);
        
        // 设置到达和离开的切线
        if (i > 0 && i < StoredPath.Num() - 1)
        {
            FVector PrevPoint = StoredPath[i - 1];
            FVector CurrentPoint = StoredPath[i];
            FVector NextPoint = StoredPath[i + 1];
            
            // 计算平滑的切线
            FVector ArriveTangent = (CurrentPoint - PrevPoint).GetSafeNormal() * PathSmoothingFactor;
            FVector LeaveTangent = (NextPoint - CurrentPoint).GetSafeNormal() * PathSmoothingFactor;
            
            PathSpline->SetTangentsAtSplinePoint(i, ArriveTangent, LeaveTangent, ESplineCoordinateSpace::World);
        }
    }
    
    // 更新样条曲线
    PathSpline->UpdateSpline();
    
    // 设置更明显的材质
    if (UMaterialInterface* SplineMaterial = LoadObject<UMaterialInterface>(nullptr, TEXT("/Engine/BasicShapes/BasicShapeMaterial")))
    {
        UE_LOG(LogTemp, Log, TEXT("CreatePathSpline: Setting material"));
        PathSpline->SetMaterial(0, SplineMaterial);
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("CreatePathSpline: Failed to load material"));
    }
    
    // 存储样条曲线数据
    StoreSplineData();
    
    UE_LOG(LogTemp, Log, TEXT("CreatePathSpline: Successfully created spline component with %d points"), StoredPath.Num());
    return PathSpline;
}

// 添加新的函数来存储样条曲线数据
void UAStarPathFinderComponent::StoreSplineData()
{
    if (!PathSpline)
    {
        return;
    }
    
    // 存储样条曲线点
    StoredSplinePoints.Empty();
    for (int32 i = 0; i < PathSpline->GetNumberOfSplinePoints(); i++)
    {
        StoredSplinePoints.Add(PathSpline->GetLocationAtSplinePoint(i, ESplineCoordinateSpace::World));
    }
    
    // 存储样条曲线长度
    StoredSplineLength = PathSpline->GetSplineLength();
}

// 添加新的函数来获取存储的样条曲线点
TArray<FVector> UAStarPathFinderComponent::GetStoredSplinePoints() const
{
    return StoredSplinePoints;
}

// 添加新的函数来获取存储的样条曲线长度
float UAStarPathFinderComponent::GetStoredSplineLength() const
{
    return StoredSplineLength;
}

// 添加新的函数来获取样条曲线上的位置和方向
FTransform UAStarPathFinderComponent::GetTransformAlongSpline(float Distance) const
{
    if (!PathSpline || StoredSplinePoints.Num() < 2)
    {
        return FTransform::Identity;
    }
    
    // 确保距离在有效范围内
    float SplineLength = PathSpline->GetSplineLength();
    Distance = FMath::Clamp(Distance, 0.0f, SplineLength);
    
    // 获取位置
    FVector Location = PathSpline->GetLocationAtDistanceAlongSpline(Distance, ESplineCoordinateSpace::World);
    
    // 获取方向
    FVector Direction = PathSpline->GetDirectionAtDistanceAlongSpline(Distance, ESplineCoordinateSpace::World);
    
    // 创建变换
    FTransform Transform;
    Transform.SetLocation(Location);
    Transform.SetRotation(Direction.Rotation().Quaternion());
    
    return Transform;
}

// 添加新的函数来获取样条曲线上的速度
FVector UAStarPathFinderComponent::GetVelocityAlongSpline(float Distance, float DeltaTime) const
{
    if (!PathSpline || StoredSplinePoints.Num() < 2)
    {
        return FVector::ZeroVector;
    }
    
    // 确保距离在有效范围内
    float SplineLength = PathSpline->GetSplineLength();
    Distance = FMath::Clamp(Distance, 0.0f, SplineLength);
    
    // 获取当前位置
    FVector CurrentLocation = PathSpline->GetLocationAtDistanceAlongSpline(Distance, ESplineCoordinateSpace::World);
    
    // 获取下一位置
    float NextDistance = FMath::Min(Distance + DeltaTime, SplineLength);
    FVector NextLocation = PathSpline->GetLocationAtDistanceAlongSpline(NextDistance, ESplineCoordinateSpace::World);
    
    // 计算速度
    return (NextLocation - CurrentLocation) / DeltaTime;
}

void UAStarPathFinderComponent::SmoothPath(TArray<FVector>& Path)
{
    if (Path.Num() < 3)
        return;
        
    TArray<FVector> SmoothedPath = Path;
    
    for (int i = 1; i < Path.Num() - 1; i++)
    {
        FVector Prev = Path[i - 1];
        FVector Current = Path[i];
        FVector Next = Path[i + 1];
        
        // 使用加权平均进行平滑
        SmoothedPath[i] = Current * (1.0f - PathSmoothingFactor) + 
                         (Prev + Next) * 0.5f * PathSmoothingFactor;
    }
    
    Path = SmoothedPath;
}

// 添加新的启发式函数
float UAStarPathFinderComponent::GetDiagonalHeuristic(int X1, int Y1, int Z1, int X2, int Y2, int Z2)
{
    if (!GridMap)
    {
        UE_LOG(LogTemp, Error, TEXT("GetDiagonalHeuristic: GridMap is not set"));
        return 0.0f;
    }
    
    float DX = FMath::Abs(X2 - X1);
    float DY = FMath::Abs(Y2 - Y1);
    float DZ = FMath::Abs(Z2 - Z1);
    
    float H = 0.0f;
    float Diag = FMath::Min3(DX, DY, DZ);
    DX -= Diag;
    DY -= Diag;
    DZ -= Diag;
    
    if (DX == 0)
    {
        H = 1.0f * FMath::Sqrt(3.0f) * Diag + FMath::Sqrt(2.0f) * FMath::Min(DY, DZ) + 1.0f * FMath::Abs(DY - DZ);
    }
    else if (DY == 0)
    {
        H = 1.0f * FMath::Sqrt(3.0f) * Diag + FMath::Sqrt(2.0f) * FMath::Min(DX, DZ) + 1.0f * FMath::Abs(DX - DZ);
    }
    else if (DZ == 0)
    {
        H = 1.0f * FMath::Sqrt(3.0f) * Diag + FMath::Sqrt(2.0f) * FMath::Min(DX, DY) + 1.0f * FMath::Abs(DX - DY);
    }
    
    return H * GridMap->GetResolution();
}

float UAStarPathFinderComponent::GetManhattanHeuristic(int X1, int Y1, int Z1, int X2, int Y2, int Z2)
{
    if (!GridMap)
    {
        UE_LOG(LogTemp, Error, TEXT("GetManhattanHeuristic: GridMap is not set"));
        return 0.0f;
    }
    
    float DX = FMath::Abs(X2 - X1);
    float DY = FMath::Abs(Y2 - Y1);
    float DZ = FMath::Abs(Z2 - Z1);
    return (DX + DY + DZ) * GridMap->GetResolution();
}

float UAStarPathFinderComponent::GetEuclideanHeuristic(int X1, int Y1, int Z1, int X2, int Y2, int Z2)
{
    if (!GridMap)
    {
        UE_LOG(LogTemp, Error, TEXT("GetEuclideanHeuristic: GridMap is not set"));
        return 0.0f;
    }
    
    float DX = X2 - X1;
    float DY = Y2 - Y1;
    float DZ = Z2 - Z1;
    return FMath::Sqrt(DX*DX + DY*DY + DZ*DZ) * GridMap->GetResolution();
}

// 添加新的函数来获取样条曲线上的位置
FVector UAStarPathFinderComponent::GetLocationAlongSpline(float Distance) const
{
    if (!PathSpline || StoredSplinePoints.Num() < 2)
    {
        return FVector::ZeroVector;
    }
    
    // 确保距离在有效范围内
    float SplineLength = PathSpline->GetSplineLength();
    Distance = FMath::Clamp(Distance, 0.0f, SplineLength);
    
    // 获取位置
    return PathSpline->GetLocationAtDistanceAlongSpline(Distance, ESplineCoordinateSpace::World);
}

bool UAStarPathFinderComponent::IsPathPointSafe(const FVector& Point) const
{
    if (!GridMap)
    {
        return false;
    }
    
    // 获取安全距离
    float SafetyDistance = GetSafetyDistance();
    
    // 检查点周围的安全距离内是否有障碍物
    TArray<FVector> CheckPoints;
    int32 NumChecks = 8;  // 在水平面上检查8个方向
    float AngleStep = 2.0f * PI / NumChecks;
    
    for (int32 i = 0; i < NumChecks; i++)
    {
        float Angle = i * AngleStep;
        FVector Offset(
            FMath::Cos(Angle) * SafetyDistance,
            FMath::Sin(Angle) * SafetyDistance,
            0.0f
        );
        CheckPoints.Add(Point + Offset);
    }
    
    // 检查上下方向
    CheckPoints.Add(Point + FVector(0, 0, SafetyDistance));
    CheckPoints.Add(Point + FVector(0, 0, -SafetyDistance));
    
    // 检查所有点
    for (const FVector& CheckPoint : CheckPoints)
    {
        if (GridMap->IsOccupied(CheckPoint))
        {
            return false;
        }
    }
    
    return true;
}

// Chaikin曲线细分平滑算法
void UAStarPathFinderComponent::ChaikinSmoothPath(TArray<FVector>& Path, int Iterations)
{
    for (int iter = 0; iter < Iterations; ++iter)
    {
        if (Path.Num() < 3) return;
        TArray<FVector> NewPath;
        NewPath.Add(Path[0]); // 保留起点
        for (int i = 0; i < Path.Num() - 1; ++i)
        {
            FVector Q = Path[i] * 0.75f + Path[i + 1] * 0.25f;
            FVector R = Path[i] * 0.25f + Path[i + 1] * 0.75f;
            NewPath.Add(Q);
            NewPath.Add(R);
        }
        NewPath.Add(Path.Last()); // 保留终点
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
    StoredPath = NewPath;
    // 重新可视化路径
    VisualizePath(5.0f); // 可视化新路径，持续5秒
}

void UAStarPathFinderComponent::SetPath(const TArray<FVector>& InPath)
{
    CurrentPath = InPath;
    UE_LOG(LogTemp, Warning, TEXT("[PathModifier] SetPath 被调用，路径点数: %d"), CurrentPath.Num());
    for (int32 i = 0; i < CurrentPath.Num(); ++i)
    {
        UE_LOG(LogTemp, Warning, TEXT("[PathModifier] 路径点[%d]: %s"), i, *CurrentPath[i].ToString());
    }
    // 自动调用UpdateStoredPath以更新存储的路径并重新可视化
    UpdateStoredPath(CurrentPath);
}