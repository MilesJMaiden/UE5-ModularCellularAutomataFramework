#include "MWSSPattern.h"
#include "CellularAutomataManager.h"

AMWSSPattern::AMWSSPattern()
{
    // Assign PatternMesh via the editor if desired.
}

void AMWSSPattern::ApplyPattern_Implementation(ACellularAutomataManager* Manager)
{
    if (!Manager)
        return;

    // Optional: Set this actor's MeshComponent to the PatternMesh, but hide it so only instances show.
    if (PatternMesh)
    {
        MeshComponent->SetStaticMesh(PatternMesh);
    }
    MeshComponent->SetVisibility(false);

    // Middleweight Spaceship (MWSS) offsets (5 rows, 10 total cells):
    // Row 0: (2,0), (3,0), (4,0), (5,0)
    // Row 1: (1,1), (5,1)
    // Row 2: (5,2)
    // Row 3: (1,3), (4,3)
    // Row 4: (2,4)
    TArray<FIntPoint> LocalOffsets;
    LocalOffsets.Add(FIntPoint(1, 0));
    LocalOffsets.Add(FIntPoint(2, 0));
    LocalOffsets.Add(FIntPoint(3, 0));
    LocalOffsets.Add(FIntPoint(4, 0));
    LocalOffsets.Add(FIntPoint(5, 0));

    LocalOffsets.Add(FIntPoint(0, 1));
    LocalOffsets.Add(FIntPoint(5, 1));

    LocalOffsets.Add(FIntPoint(5, 2));

    LocalOffsets.Add(FIntPoint(0, 3));
    LocalOffsets.Add(FIntPoint(4, 3));

    LocalOffsets.Add(FIntPoint(2, 4));

    // Compute which grid cells are occupied, based on our local offsets.
    ComputeSeededIndicesFromOffsets(Manager, LocalOffsets);

    // Mark those cells as active in the manager's grid.
    for (int32 Idx : SeededIndices)
    {
        if (Manager->CellGrid.IsValidIndex(Idx))
        {
            Manager->CellGrid[Idx] = 1;
        }
    }

    // Spawn one mesh instance for each active cell, centered on the tile.
    RefreshMeshInstances(Manager);
}
