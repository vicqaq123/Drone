#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PathModifierComponent.generated.h"

class UGridMapComponent;
class UAStarPathFinderComponent;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class DRONE_API UPathModifierComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UPathModifierComponent();

    virtual void BeginPlay() override;

    // 设置原始路径（由A*设置）
    UFUNCTION(BlueprintCallable)
    void SetPath(const TArray<FVector>& InPath);

    // 检查路径是否可行并自动修正
    UFUNCTION(BlueprintCallable)
    void CheckAndModifyPath();

    // // 检查点是否在安全距离内
    // UFUNCTION(BlueprintCallable)
    // bool IsPointSafeWithDistance(const FVector& Point, float SafetyDistance) const;

protected:
    UPROPERTY()
    UGridMapComponent* GridMap;

    UPROPERTY()
    UAStarPathFinderComponent* AStar;

    TArray<FVector> CurrentPath;

    UFUNCTION()
    void OnGridUpdated(const FVector& UpdatedLocation);  // 接收网格更新通知

    bool IsPathBlocked(const TArray<FVector>& Path) const;

    FVector GetNextValidGoal() const;
};
