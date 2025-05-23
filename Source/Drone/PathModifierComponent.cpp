#include "CoreMinimal.h"
#include "PathModifierComponent.h"
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
    UE_LOG(LogTemp, Warning, TEXT("[PathModifier] SetPath 被调用，路径点数: %d"), CurrentPath.Num());
    for (int32 i = 0; i < CurrentPath.Num(); ++i)
    {
        UE_LOG(LogTemp, Warning, TEXT("[PathModifier] 路径点[%d]: %s"), i, *CurrentPath[i].ToString());
    }
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
    
    // 检查路径是否被障碍物阻挡
    bool bIsPathBlocked = false;
    for (int32 i = 0; i < CurrentPath.Num(); ++i)
    {
        if (GridMap->IsOccupied(CurrentPath[i]))
        {
            bIsPathBlocked = true;
            UE_LOG(LogTemp, Warning, TEXT("[PathModifier] 检测到障碍物阻挡路径，位置: %s"), *CurrentPath[i].ToString());
        }
        // else
        // {
        //     UE_LOG(LogTemp, Log, TEXT("[PathModifier] 路径点未被阻挡，位置: %s"), *CurrentPath[i].ToString());
        // }
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

    // 1. 获取当前位置（无人机Actor的位置）
    FVector CurrentPos = GetOwner() ? GetOwner()->GetActorLocation() : CurrentPath[0];

    // 2. 找到原路径上距离当前位置最近且未被阻挡的点索引
    int32 ClosestIdx = 0;
    float MinDist = TNumericLimits<float>::Max();
    for (int32 i = 0; i < CurrentPath.Num(); ++i)
    {
        if (!GridMap->IsOccupied(CurrentPath[i]))
        {
            float Dist = FVector::Dist(CurrentPath[i], CurrentPos);
            if (Dist < MinDist)
            {
                MinDist = Dist;
                ClosestIdx = i;
            }
        }
    }

    // 3. 保存数组1（起点到当前位置的路径）
    TArray<FVector> PathToCurrent;
    for (int32 i = 0; i <= ClosestIdx; ++i)
    {
        PathToCurrent.Add(CurrentPath[i]);
    }

    // 4. 重新规划当前位置到终点的路径
    FVector Goal = CurrentPath.Last();
    TArray<FVector> NewPath;
    if (AStar->FindPath(CurrentPos, Goal, NewPath) && NewPath.Num() > 0)
    {
        // 5. 拼接
        // 如果PathToCurrent最后一个点和NewPath第一个点很近，去掉NewPath第一个点
        if (PathToCurrent.Num() > 0 && FVector::Dist(PathToCurrent.Last(), NewPath[0]) < 1.0f)
        {
            NewPath.RemoveAt(0);
        }
        PathToCurrent.Append(NewPath);

        // 日志：输出新路径点
        UE_LOG(LogTemp, Warning, TEXT("[PathModifier] 调用AStar->UpdateStoredPath，路径点数: %d"), PathToCurrent.Num());
        for (int32 i = 0; i < PathToCurrent.Num(); ++i)
        {
            UE_LOG(LogTemp, Warning, TEXT("[PathModifier] 新路径点[%d]: %s"), i, *PathToCurrent[i].ToString());
        }
        AStar->UpdateStoredPath(PathToCurrent);
        UE_LOG(LogTemp, Warning, TEXT("[PathModifier] 已调用AStar->UpdateStoredPath"));
        AStar->CreatePathSpline();
        UE_LOG(LogTemp, Warning, TEXT("[PathModifier] 已调用AStar->CreatePathSpline()，轨迹已更新"));
        CurrentPath = PathToCurrent;
        UE_LOG(LogTemp, Warning, TEXT("[PathModifier] 路径重规划并拼接完成，总点数: %d"), CurrentPath.Num());
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
