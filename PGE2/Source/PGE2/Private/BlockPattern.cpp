#include "BlockPattern.h"
#include "CellularAutomataManager.h"

ABlockPattern::ABlockPattern()
{
    // Assign PatternMesh via the editor.
}

void ABlockPattern::ApplyPattern_Implementation(ACellularAutomataManager* Manager)
{
    if (!Manager)
        return;

    if (PatternMesh)
    {
        this->MeshComponent->SetStaticMesh(PatternMesh);
    }
    MeshComponent->SetVisibility(false); // Hide the root mesh.

    // Define a 2x2 block pattern: offsets (0,0), (1,0), (0,1), (1,1).
    TArray<FIntPoint> LocalOffsets;
    LocalOffsets.Add(FIntPoint(0, 0));
    LocalOffsets.Add(FIntPoint(1, 0));
    LocalOffsets.Add(FIntPoint(0, 1));
    LocalOffsets.Add(FIntPoint(1, 1));

    // Compute seeded indices based on these offsets.
    ComputeSeededIndicesFromOffsets(Manager, LocalOffsets);

    // Mark the corresponding grid cells as active.
    for (int32 Idx : SeededIndices)
    {
        if (Manager->CellGrid.IsValidIndex(Idx))
        {
            Manager->CellGrid[Idx] = 1;
        }
    }

    // Remove any old mesh instances and re-spawn new ones exactly where the grid is active.
    RefreshMeshInstances(Manager);
}
