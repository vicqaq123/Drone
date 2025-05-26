#include "PathModifierComponent.h"
#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "CoreMinimal.h"
#include "GridMapComponent.h"
#include "AStarPathFinderComponent.h"

UPathModifierComponent::UPathModifierComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void UPathModifierComponent::BeginPlay()
{
    Super::BeginPlay();

    AActor* Owner = GetOwner();
    if (Owner)
    {
        GridMap = Owner->FindComponentByClass<UGridMapComponent>();
        AStar = Owner->FindComponentByClass<UAStarPathFinderComponent>();

        if (GridMap)
        {
            UE_LOG(LogTemp, Log, TEXT("[PathModifier] GridMap component found"));
            GridMap->OnGridMapUpdated.AddDynamic(this, &UPathModifierComponent::OnGridUpdated);
        }
        else
        {
            UE_LOG(LogTemp, Error, TEXT("[PathModifier] GridMap component not found"));
        }

        if (AStar)
        {
            UE_LOG(LogTemp, Log, TEXT("[PathModifier] AStar component found"));
        }
        else
        {
            UE_LOG(LogTemp, Error, TEXT("[PathModifier] AStar component not found"));
        }
    }
}

void UPathModifierComponent::SetPath(const TArray<FVector>& InPath)
{
    CurrentPath = InPath;
    UE_LOG(LogTemp, Warning, TEXT("[PathModifier] SetPath 被调用"));
    // for (int32 i = 0; i < CurrentPath.Num(); ++i)
    // {
    //     UE_LOG(LogTemp, Warning, TEXT("[PathModifier] 路径点[%d]: %s"), i, *CurrentPath[i].ToString());
    // }
}


void UPathModifierComponent::OnGridUpdated(const FVector& UpdatedLocation)
{
    // 防止重复处理
    static bool bIsProcessing = false;
    if (bIsProcessing)
    {
        return;
    }
    
    bIsProcessing = true;
    // UE_LOG(LogTemp, Log, TEXT("[PathModifier] 开始处理地图更新"));
    
    // 检查路径是否被障碍物阻挡
    bool bIsPathBlocked = false;
    
    for (int32 i = 0; i < CurrentPath.Num(); ++i)
    {
        const FVector& PathPoint = CurrentPath[i];
            
        if (GridMap->IsOccupied(PathPoint))
        {
            bIsPathBlocked = true;
            UE_LOG(LogTemp, Warning, TEXT("[PathModifier] 检测到障碍物在安全距离内，位置: %s"), 
                *PathPoint.ToString());
            break;
        }
    }
    
    // 只有当路径被阻挡时，才进行路径重规划
    if (bIsPathBlocked)
    {
        UE_LOG(LogTemp, Warning, TEXT("[PathModifier] 检测到障碍物阻挡路径，执行路径重规划"));
        CheckAndModifyPath();
    }
    
    bIsProcessing = false;
}

void UPathModifierComponent::CheckAndModifyPath()
{
    if (!GridMap || !AStar || CurrentPath.Num() == 0)
        return;

    // 添加重规划次数限制
    static int32 ReplanCount = 0;
    if (ReplanCount >= 3)  // 最多重试3次
    {
        UE_LOG(LogTemp, Warning, TEXT("[PathModifier] 达到最大重规划次数，停止重规划"));
        ReplanCount = 0;
        return;
    }
    ReplanCount++;

    // 1. 获取当前位置（无人机Actor的位置）
    FVector CurrentPos = GetOwner() ? GetOwner()->GetActorLocation() : CurrentPath[0];
    int32 CurrentPosIndex = -1;  // 初始化为-1表示未找到有效索引
    float MinDist = TNumericLimits<float>::Max();
    
    // 找到距离当前位置最近的路径点索引，但只能在当前位置之前
    for (int32 i = 0; i < CurrentPath.Num(); ++i)
    {
        // 计算当前路径点到无人机位置的距离
        float Dist = FVector::Dist(CurrentPos, CurrentPath[i]);
        
        // 如果当前点比之前找到的最近点更近，且该点在当前位置之前
        if (Dist < MinDist && CurrentPath[i].X <= CurrentPos.X)
        {
            MinDist = Dist;
            CurrentPosIndex = i;
        }
    }
    
    // 如果没有找到有效索引，使用第一个点
    if (CurrentPosIndex == -1)
    {
        CurrentPosIndex = 0;
    }

    // 3. 保存数组1（起点到当前位置的路径）
    TArray<FVector> PathToCurrent;
    for (int32 i = 0; i <= CurrentPosIndex; ++i)
    {
        PathToCurrent.Add(CurrentPath[i]);
    }

    // 4. 重新规划当前位置到终点的路径
    FVector Goal = CurrentPath.Last();
    TArray<FVector> NewPath;
    if (AStar->FindPath(CurrentPath[CurrentPosIndex], Goal, NewPath) && NewPath.Num() > 0)
    {
        PathToCurrent.Append(NewPath);

        // 检查新路径是否安全
        bool bIsPathSafe = true;
        for (const FVector& Point : PathToCurrent)
        {
            if (GridMap->IsOccupied(Point))
            {
                bIsPathSafe = false;
                UE_LOG(LogTemp, Warning, TEXT("[PathModifier] 新路径上存在障碍物，位置: %s"), *Point.ToString());
                break;
            }
        }

        // 只有当路径安全时才更新
        if (bIsPathSafe)
        {
            AStar->UpdateStoredPath(PathToCurrent);
            AStar->CreatePathSpline();
            CurrentPath = PathToCurrent;
            ReplanCount = 0;  // 重置重规划计数
            UE_LOG(LogTemp, Warning, TEXT("[PathModifier] 路径重规划并拼接完成，总点数: %d"), PathToCurrent.Num());
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("[PathModifier] 新路径不安全，尝试寻找替代路径"));
            // 尝试寻找替代路径
            FVector AlternativeGoal = GetNextValidGoal();
            if (AlternativeGoal != CurrentPath[CurrentPosIndex])  // 确保不是当前点
            {
                TArray<FVector> AltPath;
                if (AStar->FindPath(CurrentPath[CurrentPosIndex], AlternativeGoal, AltPath) && AltPath.Num() > 0)
                {
                    PathToCurrent = AltPath;
                    AStar->UpdateStoredPath(PathToCurrent);
                    AStar->CreatePathSpline();
                    CurrentPath = PathToCurrent;
                    ReplanCount = 0;  // 重置重规划计数
                    UE_LOG(LogTemp, Warning, TEXT("[PathModifier] 找到替代路径，总点数: %d"), PathToCurrent.Num());
                }
            }
        }
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("[PathModifier] 当前位置到终点无法重新规划路径！"));
    }
}

bool UPathModifierComponent::IsPathBlocked(const TArray<FVector>& Path) const
{
    if (!GridMap)
        return false;
    for (const FVector& Point : Path)
    {
        if (GridMap->IsOccupied(Point))
        {
            UE_LOG(LogTemp, Warning, TEXT("[PathModifier] 路径点被障碍物阻挡: %s"), *Point.ToString());
            return true;
        }
    }
    return false;
}

FVector UPathModifierComponent::GetNextValidGoal() const
{
    // 默认选择原路径中最远的未被阻挡点作为新目标
    for (int32 i = CurrentPath.Num() - 1; i >= 0; --i)
    {
        if (!GridMap->IsOccupied(CurrentPath[i]))
        {
            return CurrentPath[i];
        }
    }

    // 全部被阻挡，退而求其次：使用当前起点
    return CurrentPath[0];
}
