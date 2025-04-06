#include "MWSSPattern.h"
#include "CellularAutomataManager.h"

AMWSSPattern::AMWSSPattern()
{
    // Assign PatternMesh via the editor.
}

void AMWSSPattern::ApplyPattern_Implementation(ACellularAutomataManager* Manager)
{
    if (!Manager)
        return;

    if (PatternMesh)
    {
        MeshComponent->SetStaticMesh(PatternMesh);
    }
    MeshComponent->SetVisibility(false); // Hide the root mesh.

    // Middleweight spaceship (MWSS) offsets (10 cells):
    // One possible layout:
    // Row0: (2,0), (3,0), (4,0), (5,0)
    // Row1: (1,1), (5,1)
    // Row2: (5,2)
    // Row3: (1,3), (2,3), (3,3)
    TArray<FIntPoint> LocalOffsets;
    LocalOffsets.Add(FIntPoint(2, 0));
    LocalOffsets.Add(FIntPoint(3, 0));
    LocalOffsets.Add(FIntPoint(4, 0));
    LocalOffsets.Add(FIntPoint(5, 0));
    LocalOffsets.Add(FIntPoint(1, 1));
    LocalOffsets.Add(FIntPoint(5, 1));
    LocalOffsets.Add(FIntPoint(5, 2));
    LocalOffsets.Add(FIntPoint(1, 3));
    LocalOffsets.Add(FIntPoint(2, 3));
    LocalOffsets.Add(FIntPoint(3, 3));

    ComputeSeededIndicesFromOffsets(Manager, LocalOffsets);

    for (int32 Idx : SeededIndices)
    {
        if (Manager->CellGrid.IsValidIndex(Idx))
            Manager->CellGrid[Idx] = 1;
    }

    RefreshMeshInstances(Manager);
}
