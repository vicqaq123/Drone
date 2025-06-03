// GridMapComponent.cpp
#include "GridMapComponent.h"
#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "Components/PrimitiveComponent.h"
#include "GameFramework/Actor.h"
#include "Engine/StaticMeshActor.h"

UGridMapComponent::UGridMapComponent()
{
    PrimaryComponentTick.bCanEverTick = true;
    
    // Default values
    MapOrigin = FVector::ZeroVector;
    MapSize = FVector(50.0f, 50.0f, 10.0f);
    CellSize = 10.0f;
    InflationRadius = 0.3f;
    
    GridDimX = 0;
    GridDimY = 0;
    GridDimZ = 0;
    
    bAutomaticObstacleDetection = true;
}

void UGridMapComponent::BeginPlay()
{
    Super::BeginPlay();
    
    if (GetOwner())
    {
        // 初始化地图，以所有者为中心
        FVector NewMapSize = FVector(1000.0f, 1000.0f, 200.0f);  // 更大的地图尺寸
        float NewCellSize = 5.0f;  // 更大的网格尺寸
        InitializeMap(GetOwner()->GetActorLocation(), NewMapSize, NewCellSize);
        
        UE_LOG(LogTemp, Warning, TEXT("GridMapComponent初始化完成 - 地图大小: (%.1f, %.1f, %.1f), 网格尺寸: %.1f"), 
            NewMapSize.X, NewMapSize.Y, NewMapSize.Z, NewCellSize);
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("GridMapComponent初始化失败 - 无法获取Owner"));
    }
}

void UGridMapComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
    
    // Automatic obstacle detection if enabled
    // if (bAutomaticObstacleDetection)
    // {
    //     UpdateObstaclesFromScene(MapSize.GetMax());
    // }
}

void UGridMapComponent::InitializeMap(FVector Origin, FVector Size, float Resolution)
{
    MapOrigin = Origin - Size / 2.0f; // Center the map at Origin
    MapSize = Size;
    CellSize = Resolution;
    
    // Calculate grid dimensions
    GridDimX = FMath::CeilToInt(Size.X / CellSize);
    GridDimY = FMath::CeilToInt(Size.Y / CellSize);
    GridDimZ = FMath::CeilToInt(Size.Z / CellSize);
    
    // Initialize 3D grid
    OccupancyGrid.SetNum(GridDimX);
    for (int x = 0; x < GridDimX; x++)
    {
        OccupancyGrid[x].SetNum(GridDimY);
        for (int y = 0; y < GridDimY; y++)
        {
            OccupancyGrid[x][y].SetNum(GridDimZ);
            for (int z = 0; z < GridDimZ; z++)
            {
                OccupancyGrid[x][y][z] = false; // Not occupied by default
            }
        }
    }
    if (GetOwner())
        UE_LOG(LogTemp, Warning, TEXT("GridMapComponent Owner: %s"), *GetOwner()->GetName());
    UE_LOG(LogTemp, Log, TEXT("Grid map initialized: %d x %d x %d cells"), GridDimX, GridDimY, GridDimZ);
    UE_LOG(LogTemp, Warning, TEXT("MapOrigin: (%.2f, %.2f, %.2f), MapSize: (%.2f, %.2f, %.2f)"),
    MapOrigin.X, MapOrigin.Y, MapOrigin.Z, MapSize.X, MapSize.Y, MapSize.Z);
    
}


void UGridMapComponent::AddCylindricalObstacles(const TArray<FVector>& Positions, float Radius, float Height, float InInflationRadius)
{
    if (GridDimX == 0 || GridDimY == 0 || GridDimZ == 0)
    {
        UE_LOG(LogTemp, Warning, TEXT("Grid map not initialized"));
        return;
    }
    
    // 设置膨胀半径
    this->InflationRadius = InInflationRadius;
    
    // 遍历所有位置，为每个位置添加圆柱形障碍物
    for (const FVector& Position : Positions)
    {
        // 保存真实障碍物参数
        FCylinderObstacle Obstacle;
        Obstacle.Center = Position;
        Obstacle.Radius = Radius;
        Obstacle.Height = Height;
        CylinderObstacles.Add(Obstacle);
        
        // 计算底部和顶部的Z坐标
        float BottomZ = Position.Z - Height/2;
        float TopZ = Position.Z + Height/2;
        
        // 获取XY平面上圆的范围
        int MinGridX, MinGridY, MinGridZ;
        int MaxGridX, MaxGridY, MaxGridZ;
        
        // 圆柱的最小外包盒
        FVector MinPoint(Position.X - Radius, Position.Y - Radius, BottomZ);
        FVector MaxPoint(Position.X + Radius, Position.Y + Radius, TopZ);
        
        // 转换为网格坐标
        WorldToGrid(MinPoint, MinGridX, MinGridY, MinGridZ);
        WorldToGrid(MaxPoint, MaxGridX, MaxGridY, MaxGridZ);
        
        // 限制在网格范围内
        MinGridX = FMath::Clamp(MinGridX, 0, GridDimX-1);
        MinGridY = FMath::Clamp(MinGridY, 0, GridDimY-1);
        MinGridZ = FMath::Clamp(MinGridZ, 0, GridDimZ-1);
        
        MaxGridX = FMath::Clamp(MaxGridX, 0, GridDimX-1);
        MaxGridY = FMath::Clamp(MaxGridY, 0, GridDimY-1);
        MaxGridZ = FMath::Clamp(MaxGridZ, 0, GridDimZ-1);
        
        // 圆柱体中心在XY平面的网格坐标
        int CenterGridX, CenterGridY, TempZ;
        WorldToGrid(Position, CenterGridX, CenterGridY, TempZ);
        
        // 半径的平方(网格单位)
        float RadiusSquaredInCells = FMath::Square(Radius / CellSize);
        
        // 遍历并标记圆柱体内的所有网格
        for (int x = MinGridX; x <= MaxGridX; x++)
        {
            for (int y = MinGridY; y <= MaxGridY; y++)
            {
                // 计算当前点到圆心XY平面距离的平方
                float distSquared = FMath::Square(x - CenterGridX) + FMath::Square(y - CenterGridY);
                
                // 如果在圆内
                if (distSquared <= RadiusSquaredInCells)
                {
                    // 标记该垂直线上在高度范围内的所有网格
                    for (int z = MinGridZ; z <= MaxGridZ; z++)
                    {
                        OccupancyGrid[x][y][z] = true;
                    }
                }
            }
        }
    }
    
    UE_LOG(LogTemp, Log, TEXT("Added %d cylindrical obstacles with radius %f and height %f"), 
        Positions.Num(), Radius, Height);
    
    // 应用膨胀
    InflateObstacles(InInflationRadius);
    
    if (OnGridMapUpdated.IsBound())
    {
        OnGridMapUpdated.Broadcast(FVector::ZeroVector); // 或合适的位置参数
    }
}

bool UGridMapComponent::IsOccupied(const FVector& Position)
{
    int32 GridX, GridY, GridZ;
    if (WorldToGrid(Position, GridX, GridY, GridZ))
    {
        return OccupancyGrid[GridX][GridY][GridZ];
    }
    return false;
}

void UGridMapComponent::MarkAsOccupied(const FVector& Position)
{
    int32 GridX, GridY, GridZ;
    if (WorldToGrid(Position, GridX, GridY, GridZ))
    {
        OccupancyGrid[GridX][GridY][GridZ] = true;
        UE_LOG(LogTemp, Warning, TEXT("已经更新障碍物 (%.2f, %.2f, %.2f)"),Position.X, Position.Y, Position.Z);
    }
    
    if (OnGridMapUpdated.IsBound())
    {
        OnGridMapUpdated.Broadcast(Position);
        // UE_LOG(LogTemp, Warning, TEXT("GridMap: 广播地图更新事件，位置: (%.2f, %.2f, %.2f)"), 
        //     Position.X, Position.Y, Position.Z);
    }
}

void UGridMapComponent::MarkAsGround(const FVector& Position)
{
    int32 GridX, GridY, GridZ;
    if (WorldToGrid(Position, GridX, GridY, GridZ))
    {
        // 将指定位置标记为地面
        if (GridX >= 0 && GridX < GridDimX &&
            GridY >= 0 && GridY < GridDimY &&
            GridZ >= 0 && GridZ < GridDimZ)
        {
            // 将网格标记为地面（非障碍物）
            OccupancyGrid[GridX][GridY][GridZ] = false;
            UE_LOG(LogTemp, Log, TEXT("GridMap: 位置 (%.2f, %.2f, %.2f) 已标记为地面"), 
                Position.X, Position.Y, Position.Z);
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("GridMap: 无效的网格索引 (%d, %d, %d)"), 
                GridX, GridY, GridZ);
        }
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("GridMap: 无法将位置 (%.2f, %.2f, %.2f) 转换为网格索引"), 
            Position.X, Position.Y, Position.Z);
    }
}

void UGridMapComponent::VisualizeMap(float Duration)
{
    if (!GetWorld() || GridDimX == 0 || GridDimY == 0 || GridDimZ == 0)
        return;
    
    for (int x = 0; x < GridDimX; x++)
    {
        for (int y = 0; y < GridDimY; y++)
        {
            for (int z = 0; z < GridDimZ; z++)
            {
                if (OccupancyGrid[x][y][z])
                {
                    FVector WorldPos = GridToWorld(x, y, z);
                    DrawDebugBox(
                        GetWorld(),
                        WorldPos,
                        FVector(CellSize/2, CellSize/2, CellSize/2),
                        FQuat::Identity,
                        FColor::Red,
                        false,
                        Duration,
                        0,
                        1.0f
                    );
                }
            }
        }
    }
}

void UGridMapComponent::GetMapBounds(FVector& OutOrigin, FVector& OutSize)
{
    OutOrigin = MapOrigin + MapSize / 2.0f; // Return center of map
    OutSize = MapSize;
}

bool UGridMapComponent::WorldToGrid(const FVector& WorldPos, int32& GridX, int32& GridY, int32& GridZ)
{
    // 计算相对于地图原点的偏移
    FVector RelativePos = WorldPos - MapOrigin;
    
    // 转换为网格坐标
    GridX = FMath::FloorToInt(RelativePos.X / CellSize);
    GridY = FMath::FloorToInt(RelativePos.Y / CellSize);
    GridZ = FMath::FloorToInt(RelativePos.Z / CellSize);
    
    // 检查是否在网格范围内
    if (GridX < 0 || GridX >= GridDimX ||
        GridY < 0 || GridY >= GridDimY ||
        GridZ < 0 || GridZ >= GridDimZ)
    {
        return false;
    }
    
    return true;
}

FVector UGridMapComponent::GridToWorld(int32 GridX, int32 GridY, int32 GridZ)
{
    // 计算网格中心点的世界坐标
    FVector WorldPos;
    WorldPos.X = MapOrigin.X + (GridX + 0.5f) * CellSize;
    WorldPos.Y = MapOrigin.Y + (GridY + 0.5f) * CellSize;
    WorldPos.Z = MapOrigin.Z + (GridZ + 0.5f) * CellSize;
    
    return WorldPos;
}


bool UGridMapComponent::InflateObstacles(float Radius)
{
    bool bHasUpdate = false;
    int InflationCells = FMath::CeilToInt(Radius / CellSize);
    
    // Create a copy of the grid
    TArray<TArray<TArray<bool>>> OriginalGrid = OccupancyGrid;
    
    // Apply inflation
    for (int x = 0; x < GridDimX; x++)
    {
        for (int y = 0; y < GridDimY; y++)
        {
            for (int z = 0; z < GridDimZ; z++)
            {
                if (OriginalGrid[x][y][z])
                {
                    // Inflate in all directions
                    for (int dx = -InflationCells; dx <= InflationCells; dx++)
                    {
                        for (int dy = -InflationCells; dy <= InflationCells; dy++)
                        {
                            for (int dz = -InflationCells; dz <= InflationCells; dz++)
                            {
                                int nx = x + dx;
                                int ny = y + dy;
                                int nz = z + dz;
                                
                                // Check bounds
                                if (nx >= 0 && nx < GridDimX &&
                                    ny >= 0 && ny < GridDimY &&
                                    nz >= 0 && nz < GridDimZ)
                                {
                                    // Check if within inflation radius
                                    float distance = FMath::Sqrt((float)(dx*dx + dy*dy + dz*dz)) * CellSize;
                                    if (distance <= Radius && !OccupancyGrid[nx][ny][nz])
                                    {
                                        OccupancyGrid[nx][ny][nz] = true;
                                        bHasUpdate = true;
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    return bHasUpdate;
}

void UGridMapComponent::VisualizeRealObstacles(float Duration)
{
    if (!GetWorld()) return;
    for (const FCylinderObstacle& Obstacle : CylinderObstacles)
    {
        FVector Start = Obstacle.Center - FVector(0, 0, Obstacle.Height/2);
        FVector End   = Obstacle.Center + FVector(0, 0, Obstacle.Height/2);
        DrawDebugCylinder(
            GetWorld(),
            Start,
            End,
            Obstacle.Radius,
            24, // 圆的分段数
            FColor::Blue, // 真实障碍物用蓝色
            false,
            Duration,
            0,
            2.0f // 线宽
        );
    }
}

void UGridMapComponent::ClearObstacles()
{
    for (int x = 0; x < GridDimX; x++)
        for (int y = 0; y < GridDimY; y++)
            for (int z = 0; z < GridDimZ; z++)
                OccupancyGrid[x][y][z] = false;
    if (OnGridMapUpdated.IsBound())
        OnGridMapUpdated.Broadcast(FVector::ZeroVector);
}
