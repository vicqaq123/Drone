// GridMapComponent.h
#pragma once
#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GridMapComponent.generated.h"

// 障碍物结构体
USTRUCT()
struct FCylinderObstacle
{
    GENERATED_BODY();
    FVector Center;
    float Radius;
    float Height;
};

UCLASS(ClassGroup=(PathPlanning), meta=(BlueprintSpawnableComponent))
class DRONE_API UGridMapComponent : public UActorComponent
{
    GENERATED_BODY()
    
public:
    UGridMapComponent();
    
    virtual void BeginPlay() override;
    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
    
    // Initialize the grid map
    UFUNCTION(BlueprintCallable, Category="PathPlanning|GridMap")
    void InitializeMap(FVector Origin, FVector Size, float Resolution);
    
    // Update obstacles from scene
    UFUNCTION(BlueprintCallable, Category="PathPlanning|GridMap")
    void UpdateObstaclesFromScene(float DetectionRadius);
    
    // Manually add obstacles
    UFUNCTION(BlueprintCallable, Category="PathPlanning|GridMap")
    void AddObstacles(const TArray<FVector>& ObstaclePositions, float InflationRadius = 0.3f);

    // 在GridMapComponent.h中添加新方法声明
    UFUNCTION(BlueprintCallable, Category="PathPlanning|GridMap")
    void AddCylindricalObstacle(const FVector& Position, float Radius, float Height, float InflationRadius = 0.3f);
    
    // 添加批量添加圆柱形障碍物的函数声明
    UFUNCTION(BlueprintCallable, Category="PathPlanning|GridMap")
    void AddCylindricalObstacles(const TArray<FVector>& Positions, float Radius, float Height, float InflationRadius = 0.3f);
        
    // Check if position is occupied
    UFUNCTION(BlueprintCallable, Category="PathPlanning|GridMap")
    bool IsOccupied(const FVector& Position);
    
    // 标记位置为障碍物
    UFUNCTION(BlueprintCallable, Category="PathPlanning|GridMap")
    void MarkAsOccupied(const FVector& Position);
    
    // 标记位置为地面
    UFUNCTION(BlueprintCallable, Category="PathPlanning|GridMap")
    void MarkAsGround(const FVector& Position);
    
    // Visualize the map
    UFUNCTION(BlueprintCallable, Category="PathPlanning|GridMap")
    void VisualizeMap(float Duration = 0.0f);
    
    // Visualize real obstacles
    UFUNCTION(BlueprintCallable, Category="PathPlanning|GridMap")
    void VisualizeRealObstacles(float Duration = 0.0f);
    
    // Get map bounds
    UFUNCTION(BlueprintCallable, Category="PathPlanning|GridMap")
    void GetMapBounds(FVector& OutOrigin, FVector& OutSize);
    
    // Convert between world and grid coordinates
    UFUNCTION(BlueprintCallable, Category="PathPlanning|GridMap")
    bool WorldToGrid(const FVector& WorldPos, int32& GridX, int32& GridY, int32& GridZ);
    
    UFUNCTION(BlueprintCallable, Category="PathPlanning|GridMap")
    FVector GridToWorld(int32 GridX, int32 GridY, int32 GridZ);
    
    // Get resolution
    UFUNCTION(BlueprintPure, Category="PathPlanning|GridMap")
    float GetResolution() const { return CellSize; }
    
    // Get grid dimensions
    UFUNCTION(BlueprintPure, Category="PathPlanning|GridMap")
    int32 GetGridDimX() const { return GridDimX; }
    
    UFUNCTION(BlueprintPure, Category="PathPlanning|GridMap")
    int32 GetGridDimY() const { return GridDimY; }
    
    UFUNCTION(BlueprintPure, Category="PathPlanning|GridMap")
    int32 GetGridDimZ() const { return GridDimZ; }
    
    // Config properties
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="PathPlanning|GridMap")
    float InflationRadius = 0.3f;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="PathPlanning|GridMap")
    bool bAutomaticObstacleDetection = true;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="PathPlanning|GridMap")
    TArray<AActor*> IgnoredActors;
    
    FORCEINLINE FVector GetMapOrigin() const { return MapOrigin; }
    FORCEINLINE FVector GetMapSize() const { return MapSize; }
    
private:
    // Grid map data
    TArray<TArray<TArray<bool>>> OccupancyGrid;
    
    FVector MapOrigin;
    FVector MapSize;
    float CellSize;
    
    int32 GridDimX;
    int32 GridDimY;
    int32 GridDimZ;
    
    // 保存所有真实圆柱障碍物
    TArray<FCylinderObstacle> CylinderObstacles;
    
    // Helper methods
    void MarkCellsInBox(const FVector& Min, const FVector& Max);
    void InflateObstacles(float Radius);
};