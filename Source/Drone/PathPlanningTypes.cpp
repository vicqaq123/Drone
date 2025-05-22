// PathPlanningTypes.cpp
#include "PathPlanningTypes.h"

FPathPoint UPathPlanningFunctionLibrary::MakePathPoint(const FVector& Position, const FVector& Velocity, const FVector& Acceleration)
{
	return FPathPoint(Position, Velocity, Acceleration);
}

FPathPoint UPathPlanningFunctionLibrary::SampleTrajectory(const FTrajectory& Trajectory, float Time)
{
	if (Trajectory.Points.Num() == 0)
	{
		return FPathPoint();
	}
    
	// Clamp time to valid range
	Time = FMath::Clamp(Time, 0.0f, Trajectory.TotalTime);
    
	// Handle edge cases
	if (Time <= 0.0f)
	{
		return Trajectory.Points[0];
	}
    
	if (Time >= Trajectory.TotalTime)
	{
		return Trajectory.Points.Last();
	}
    
	// Find the two points to interpolate between
	int Index = 0;
	for (int i = 0; i < Trajectory.TimePoints.Num(); i++)
	{
		if (Trajectory.TimePoints[i] > Time)
		{
			Index = i;
			break;
		}
	}
    
	if (Index == 0)
	{
		return Trajectory.Points[0];
	}
    
	// Linear interpolation between points
	float Alpha = (Time - Trajectory.TimePoints[Index-1]) / (Trajectory.TimePoints[Index] - Trajectory.TimePoints[Index-1]);
    
	FPathPoint Result;
	Result.Position = FMath::Lerp(Trajectory.Points[Index-1].Position, Trajectory.Points[Index].Position, Alpha);
	Result.Velocity = FMath::Lerp(Trajectory.Points[Index-1].Velocity, Trajectory.Points[Index].Velocity, Alpha);
	Result.Acceleration = FMath::Lerp(Trajectory.Points[Index-1].Acceleration, Trajectory.Points[Index].Acceleration, Alpha);
    
	return Result;
}

void UPathPlanningFunctionLibrary::TrajectoryToSplinePoints(const FTrajectory& Trajectory, TArray<FVector>& OutSplinePoints)
{
	OutSplinePoints.Empty();
    
	for (const FPathPoint& Point : Trajectory.Points)
	{
		OutSplinePoints.Add(Point.Position);
	}
}