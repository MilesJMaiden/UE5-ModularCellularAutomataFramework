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
    // Create a grid with all dead cells using a one-dimensional array.
    int32 TotalCells = GridWidth * GridHeight;
    CellGrid.Empty();
    CellGrid.SetNum(TotalCells);

    for (int32 y = 0; y < GridHeight; y++)
    {
        for (int32 x = 0; x < GridWidth; x++)
        {
            int32 Index = y * GridWidth + x;
            CellGrid[Index] = 0;
            // Spawn a visual cell at (x, y).
            SpawnCell(x, y, false);
        }
    }
}

void ACellularAutomataManager::UpdateSimulation()
{
    TArray<int32> NewGrid = CellGrid;

    auto GetLiveNeighborCount = [this](int32 X, int32 Y) -> int32
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
                        int32 Index = NeighborY * GridWidth + NeighborX;
                        Count += CellGrid[Index];
                    }
                }
            }
            return Count;
        };

    for (int32 y = 0; y < GridHeight; y++)
    {
        for (int32 x = 0; x < GridWidth; x++)
        {
            int32 Index = y * GridWidth + x;
            int32 LiveNeighbors = GetLiveNeighborCount(x, y);
            if (CellGrid[Index] == 1)
            {
                if (LiveNeighbors < 2 || LiveNeighbors > 3)
                    NewGrid[Index] = 0;
            }
            else
            {
                if (LiveNeighbors == 3)
                    NewGrid[Index] = 1;
            }
        }
    }

    CellGrid = NewGrid;
    // (Optional) Update visual representations here.
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
    // Convert grid coordinates to a world location (using 100 unit spacing).
    FVector Location = GetActorLocation() + FVector(X * 100.0f, Y * 100.0f, 0.0f);
    FTransform Transform;
    Transform.SetLocation(Location);

    AStaticMeshActor* CellActor = GetWorld()->SpawnActor<AStaticMeshActor>(AStaticMeshActor::StaticClass(), Transform);
    if (CellActor && CellMesh)
    {
        CellActor->GetStaticMeshComponent()->SetStaticMesh(CellMesh);
    }
}
