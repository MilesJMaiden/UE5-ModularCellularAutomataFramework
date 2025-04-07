#include "ToadPattern.h"
#include "CellularAutomataManager.h"

AToadPattern::AToadPattern()
{
    // Assign PatternMesh via the editor.
}

void AToadPattern::ApplyPattern_Implementation(ACellularAutomataManager* Manager)
{
    if (!Manager)
        return;

    if (PatternMesh)
    {
        MeshComponent->SetStaticMesh(PatternMesh);
    }
    MeshComponent->SetVisibility(false); // Hide the root mesh.

    // Toad oscillator offsets:
    TArray<FIntPoint> LocalOffsets;
    LocalOffsets.Add(FIntPoint(1, 0));
    LocalOffsets.Add(FIntPoint(2, 0));
    LocalOffsets.Add(FIntPoint(3, 0));
    LocalOffsets.Add(FIntPoint(0, 1));
    LocalOffsets.Add(FIntPoint(1, 1));
    LocalOffsets.Add(FIntPoint(2, 1));

    ComputeSeededIndicesFromOffsets(Manager, LocalOffsets);

    for (int32 Idx : SeededIndices)
    {
        if (Manager->CellGrid.IsValidIndex(Idx))
            Manager->CellGrid[Idx] = 1;
    }

    RefreshMeshInstances(Manager);
}
