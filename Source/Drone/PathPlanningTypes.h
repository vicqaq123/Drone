// PathPlanningTypes.h
#pragma once
#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "PathPlanningTypes.generated.h"

USTRUCT(BlueprintType)
struct FPathPoint
{
    GENERATED_BODY()
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="PathPlanning")
    FVector Position;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="PathPlanning")
    FVector Velocity;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="PathPlanning")
    FVector Acceleration;
    
    FPathPoint() : Position(FVector::ZeroVector), Velocity(FVector::ZeroVector), Acceleration(FVector::ZeroVector) {}
    
    FPathPoint(const FVector& InPos) : Position(InPos), Velocity(FVector::ZeroVector), Acceleration(FVector::ZeroVector) {}
    
    FPathPoint(const FVector& InPos, const FVector& InVel, const FVector& InAcc) 
        : Position(InPos), Velocity(InVel), Acceleration(InAcc) {}
};

USTRUCT(BlueprintType)
struct FTrajectory
{
    GENERATED_BODY()
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="PathPlanning")
    TArray<FPathPoint> Points;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="PathPlanning")
    float TotalTime;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="PathPlanning")
    TArray<float> TimePoints;
    
    FTrajectory() : TotalTime(0.0f) {}
};

UCLASS(BlueprintType)
class UPathPlanningFunctionLibrary : public UBlueprintFunctionLibrary
{
    GENERATED_BODY()
    
public:
    // Utility function to create a path point
    UFUNCTION(BlueprintPure, Category="PathPlanning")
    static FPathPoint MakePathPoint(const FVector& Position, const FVector& Velocity = FVector::ZeroVector, const FVector& Acceleration = FVector::ZeroVector);
    
    // Utility function to sample a trajectory at a specific time
    UFUNCTION(BlueprintPure, Category="PathPlanning")
    static FPathPoint SampleTrajectory(const FTrajectory& Trajectory, float Time);
    
    // Utility function to convert a trajectory to a spline
    UFUNCTION(BlueprintCallable, Category="PathPlanning")
    static void TrajectoryToSplinePoints(const FTrajectory& Trajectory, TArray<FVector>& OutSplinePoints);
};