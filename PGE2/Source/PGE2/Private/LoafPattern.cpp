#include "LoafPattern.h"
#include "CellularAutomataManager.h"

ALoafPattern::ALoafPattern()
{
    // Assign PatternMesh via the editor.
}

void ALoafPattern::ApplyPattern_Implementation(ACellularAutomataManager* Manager)
{
    if (!Manager)
        return;

    if (PatternMesh)
    {
        MeshComponent->SetStaticMesh(PatternMesh);
    }
    MeshComponent->SetVisibility(false);

    // Loaf pattern (still life) offsets:
    TArray<FIntPoint> LocalOffsets;
    LocalOffsets.Add(FIntPoint(1, 0));
    LocalOffsets.Add(FIntPoint(2, 0));
    LocalOffsets.Add(FIntPoint(0, 1));
    LocalOffsets.Add(FIntPoint(3, 1));
    LocalOffsets.Add(FIntPoint(1, 2));
    LocalOffsets.Add(FIntPoint(3, 2));
    LocalOffsets.Add(FIntPoint(2, 3));

    ComputeSeededIndicesFromOffsets(Manager, LocalOffsets);

    for (int32 Idx : SeededIndices)
    {
        if (Manager->CellGrid.IsValidIndex(Idx))
            Manager->CellGrid[Idx] = 1;
    }

    RefreshMeshInstances(Manager);
}
