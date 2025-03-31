#include "CellularAutomataManager.h"
#include "CellPatternBase.h"
#include "Engine/World.h"
#include "Engine/StaticMeshActor.h"

ACellularAutomataManager::ACellularAutomataManager()
{
    PrimaryActorTick.bCanEverTick = true;
    GridWidth = 50;
    GridHeight = 50;
    TimeStepInterval = 0.5f;
    TimeAccumulator = 0.0f;
}

void ACellularAutomataManager::BeginPlay()
{
    Super::BeginPlay();
    InitializeGrid();
}

void ACellularAutomataManager::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    TimeAccumulator += DeltaTime;
    if (TimeAccumulator >= TimeStepInterval)
    {
        UpdateSimulation();
        TimeAccumulator = 0.0f;
    }
}

void ACellularAutomataManager::InitializeGrid()
{
    // Create a grid with all dead cells.
    CellGrid.Empty();
    for (int32 i = 0; i < GridHeight; i++)
    {
        TArray<int32> Row;
        for (int32 j = 0; j < GridWidth; j++)
        {
            Row.Add(0);
            // Spawn a visual representation for each grid cell.
            SpawnCell(j, i, false);
        }
        CellGrid.Add(Row);
    }
}

void ACellularAutomataManager::UpdateSimulation()
{
    // Create a temporary grid (double-buffer) for state update.
    TArray<TArray<int32>> NewGrid = CellGrid;

    auto GetLiveNeighborCount = [&](int32 X, int32 Y) -> int32
        {
            int32 Count = 0;
            for (int32 OffsetY = -1; OffsetY <= 1; OffsetY++)
            {
                for (int32 OffsetX = -1; OffsetX <= 1; OffsetX++)
                {
                    if (OffsetX == 0 && OffsetY == 0)
                        continue;
                    int32 NeighborX = X + OffsetX;
                    int32 NeighborY = Y + OffsetY;
                    if (NeighborX >= 0 && NeighborX < GridWidth && NeighborY >= 0 && NeighborY < GridHeight)
                    {
                        Count += CellGrid[NeighborY][NeighborX];
                    }
                }
            }
            return Count;
        };

    // Apply Conway's Game of Life rules.
    for (int32 Y = 0; Y < GridHeight; Y++)
    {
        for (int32 X = 0; X < GridWidth; X++)
        {
            int32 LiveNeighbors = GetLiveNeighborCount(X, Y);
            if (CellGrid[Y][X] == 1)
            {
                if (LiveNeighbors < 2 || LiveNeighbors > 3)
                    NewGrid[Y][X] = 0;
            }
            else
            {
                if (LiveNeighbors == 3)
                    NewGrid[Y][X] = 1;
            }
        }
    }

    CellGrid = NewGrid;
    // Optionally, update visual representations based on new states.
}

void ACellularAutomataManager::ApplyPattern(TSubclassOf<ACellPatternBase> PatternClass, FVector Origin)
{
    if (PatternClass)
    {
        ACellPatternBase* Pattern = GetWorld()->SpawnActor<ACellPatternBase>(PatternClass, Origin, FRotator::ZeroRotator);
        if (Pattern)
        {
            Pattern->ApplyPattern(this);
        }
    }
}

void ACellularAutomataManager::SpawnCell(int32 X, int32 Y, bool bIsAlive)
{
    // Convert grid coordinates to world location (assuming 100 unit spacing).
    FVector Location = GetActorLocation() + FVector(X * 100.0f, Y * 100.0f, 0.0f);
    FTransform Transform;
    Transform.SetLocation(Location);

    // Spawn a StaticMeshActor to represent the cell.
    AStaticMeshActor* CellActor = GetWorld()->SpawnActor<AStaticMeshActor>(AStaticMeshActor::StaticClass(), Transform);
    if (CellActor && CellMesh)
    {
        CellActor->GetStaticMeshComponent()->SetStaticMesh(CellMesh);
        // Optionally, set materials or collision settings.
    }
}
