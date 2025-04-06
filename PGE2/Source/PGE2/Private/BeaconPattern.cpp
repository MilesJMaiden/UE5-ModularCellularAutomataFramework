#include "BeaconPattern.h"
#include "CellularAutomataManager.h"

ABeaconPattern::ABeaconPattern()
{
    // Assign PatternMesh via the editor.
}

void ABeaconPattern::ApplyPattern_Implementation(ACellularAutomataManager* Manager)
{
    if (!Manager)
        return;

    if (PatternMesh)
    {
        MeshComponent->SetStaticMesh(PatternMesh);
    }
    MeshComponent->SetVisibility(false); // Hide the root mesh.

    // Beacon oscillator offsets:
    // One common layout: two 2x2 blocks separated by a gap.
    // Top block: (0,0), (1,0), (0,1), (1,1)
    // Bottom block: (2,2), (3,2), (2,3), (3,3)
    TArray<FIntPoint> LocalOffsets;
    LocalOffsets.Add(FIntPoint(0, 0));
    LocalOffsets.Add(FIntPoint(1, 0));
    LocalOffsets.Add(FIntPoint(0, 1));
    LocalOffsets.Add(FIntPoint(1, 1));
    LocalOffsets.Add(FIntPoint(2, 2));
    LocalOffsets.Add(FIntPoint(3, 2));
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
