#include "BeehivePattern.h"
#include "CellularAutomataManager.h"

ABeehivePattern::ABeehivePattern()
{
    // Optionally, hide the actor’s root MeshComponent so that only the spawned instances are visible.
    MeshComponent->SetVisibility(false);
}

void ABeehivePattern::ApplyPattern_Implementation(ACellularAutomataManager* Manager)
{
    if (!Manager)
        return;

    // (Optional) Set the MeshComponent’s static mesh for debugging—but then hide it.
    if (PatternMesh)
    {
        MeshComponent->SetStaticMesh(PatternMesh);
    }
    MeshComponent->SetVisibility(false); // Hide the root mesh.

    // Beehive (still life) offsets:
    // Common layout: (1,0), (2,0), (0,1), (3,1), (1,2), (2,2)
    TArray<FIntPoint> LocalOffsets;
    LocalOffsets.Add(FIntPoint(1, 0));
    LocalOffsets.Add(FIntPoint(2, 0));
    LocalOffsets.Add(FIntPoint(0, 1));
    LocalOffsets.Add(FIntPoint(3, 1));
    LocalOffsets.Add(FIntPoint(1, 2));
    LocalOffsets.Add(FIntPoint(2, 2));

    // Calculate which grid cells this pattern covers.
    ComputeSeededIndicesFromOffsets(Manager, LocalOffsets);

    // Mark these grid cells as active.
    for (int32 Idx : SeededIndices)
    {
        if (Manager->CellGrid.IsValidIndex(Idx))
        {
            Manager->CellGrid[Idx] = 1;
        }
    }

    // Remove any old mesh instances and re-spawn new ones so that there is exactly one mesh per active tile.
    RefreshMeshInstances(Manager);
}
