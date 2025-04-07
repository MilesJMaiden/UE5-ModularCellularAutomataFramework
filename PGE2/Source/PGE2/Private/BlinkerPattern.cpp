#include "BlinkerPattern.h"
#include "CellularAutomataManager.h"

ABlinkerPattern::ABlinkerPattern()
{
    // Assign PatternMesh via the editor.
}

void ABlinkerPattern::ApplyPattern_Implementation(ACellularAutomataManager* Manager)
{
    if (!Manager)
        return;

    if (PatternMesh)
    {
        this->MeshComponent->SetStaticMesh(PatternMesh);
    }
    MeshComponent->SetVisibility(false); // Hide the root mesh.

    TArray<FIntPoint> LocalOffsets;
    LocalOffsets.Add(FIntPoint(-1, 0));
    LocalOffsets.Add(FIntPoint(0, 0));
    LocalOffsets.Add(FIntPoint(1, 0));

    ComputeSeededIndicesFromOffsets(Manager, LocalOffsets);

    // Mark these grid cells as active.
    for (int32 Idx : SeededIndices)
    {
        if (Manager->CellGrid.IsValidIndex(Idx))
        {
            Manager->CellGrid[Idx] = 1;
        }
    }

    // Remove and re-spawn mesh instances for each seeded cell.
    RefreshMeshInstances(Manager);
}
