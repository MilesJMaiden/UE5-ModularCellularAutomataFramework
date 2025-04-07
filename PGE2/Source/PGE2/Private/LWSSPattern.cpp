#include "LWSSPattern.h"
#include "CellularAutomataManager.h"

ALWSSPattern::ALWSSPattern()
{
    // Assign PatternMesh via the editor.
}

void ALWSSPattern::ApplyPattern_Implementation(ACellularAutomataManager* Manager)
{
    if (!Manager)
        return;

    if (PatternMesh)
    {
        MeshComponent->SetStaticMesh(PatternMesh);
    }
    MeshComponent->SetVisibility(false); // Hide the root mesh.

    // Lightweight spaceship (LWSS) offsets (9 cells):
    TArray<FIntPoint> LocalOffsets;
    LocalOffsets.Add(FIntPoint(1, 0));
    LocalOffsets.Add(FIntPoint(2, 0));
    LocalOffsets.Add(FIntPoint(3, 0));
    LocalOffsets.Add(FIntPoint(4, 0));
    LocalOffsets.Add(FIntPoint(0, 1));
    LocalOffsets.Add(FIntPoint(4, 1));
    LocalOffsets.Add(FIntPoint(4, 2));
    LocalOffsets.Add(FIntPoint(0, 3));
    LocalOffsets.Add(FIntPoint(3, 3));

    ComputeSeededIndicesFromOffsets(Manager, LocalOffsets);

    for (int32 Idx : SeededIndices)
    {
        if (Manager->CellGrid.IsValidIndex(Idx))
            Manager->CellGrid[Idx] = 1;
    }

    RefreshMeshInstances(Manager);
}
