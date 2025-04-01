#include "CellularAutomataManager.h"
#include "CellPatternBase.h"
#include "Engine/World.h"
#include "Engine/StaticMeshActor.h"
#include "Materials/MaterialInstanceDynamic.h"

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

    // Clear previous cell actors, if any.
    CellActors.Empty();
    CellActors.SetNum(TotalCells);

    for (int32 y = 0; y < GridHeight; y++)
    {
        for (int32 x = 0; x < GridWidth; x++)
        {
            int32 Index = y * GridWidth + x;
            CellGrid[Index] = 0;
            // Spawn a visual cell at (x, y) and store its reference.
            SpawnCell(x, y, false);
        }
    }
}

int32 ACellularAutomataManager::GetLiveNeighborCountForCell(int32 X, int32 Y) const
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
}

void ACellularAutomataManager::UpdateSimulation()
{
    // Create a temporary copy of the grid.
    TArray<int32> NewGrid = CellGrid;

    // Update the simulation state using Conway's Game of Life rules.
    for (int32 y = 0; y < GridHeight; y++)
    {
        for (int32 x = 0; x < GridWidth; x++)
        {
            int32 Index = y * GridWidth + x;
            int32 LiveNeighbors = GetLiveNeighborCountForCell(x, y);
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

    // Update the simulation grid.
    CellGrid = NewGrid;

    // Update the dynamic material parameters for each cell.
    for (int32 y = 0; y < GridHeight; y++)
    {
        for (int32 x = 0; x < GridWidth; x++)
        {
            int32 Index = y * GridWidth + x;
            if (CellActors.IsValidIndex(Index) && CellActors[Index])
            {
                // Get the dynamic material instance from the cell's StaticMeshComponent.
                UMaterialInstanceDynamic* DynMat = Cast<UMaterialInstanceDynamic>(CellActors[Index]->GetStaticMeshComponent()->GetMaterial(0));
                if (DynMat)
                {
                    // Set a parameter (e.g., "LifeState") to indicate whether the cell is alive (1.0) or dead (0.0).
                    DynMat->SetScalarParameterValue(FName("LifeState"), CellGrid[Index] == 1 ? 1.0f : 0.0f);

                    // Also update a parameter for the neighbor count.
                    int32 LiveNeighbors = GetLiveNeighborCountForCell(x, y);
                    DynMat->SetScalarParameterValue(FName("NeighborCount"), (float)LiveNeighbors);
                }
            }
        }
    }
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

    // Spawn a StaticMeshActor to represent the cell.
    AStaticMeshActor* CellActor = GetWorld()->SpawnActor<AStaticMeshActor>(AStaticMeshActor::StaticClass(), Transform);
    if (CellActor && CellMesh)
    {
        CellActor->GetStaticMeshComponent()->SetStaticMesh(CellMesh);

        // Create a dynamic material instance based on the BaseCellMaterial (if assigned).
        if (BaseCellMaterial)
        {
            UMaterialInstanceDynamic* DynMat = UMaterialInstanceDynamic::Create(BaseCellMaterial, this);
            if (DynMat)
            {
                // Optionally initialize parameters.
                DynMat->SetScalarParameterValue(FName("LifeState"), 0.0f);
                DynMat->SetScalarParameterValue(FName("NeighborCount"), 0.0f);
                CellActor->GetStaticMeshComponent()->SetMaterial(0, DynMat);
            }
        }
    }

    // Store the spawned actor reference in the same order as the grid.
    int32 Index = Y * GridWidth + X;
    if (CellActors.IsValidIndex(Index))
    {
        CellActors[Index] = CellActor;
    }
    else
    {
        CellActors.Add(CellActor);
    }
}
