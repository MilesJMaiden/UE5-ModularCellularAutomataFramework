#include "SpaceshipPattern.h"
#include "CellularAutomataManager.h"

ASpaceshipPattern::ASpaceshipPattern()
{
    // Assign PatternMesh via the editor.
}

void ASpaceshipPattern::ApplyPattern_Implementation(ACellularAutomataManager* Manager)
{
    if (!Manager)
        return;

    if (PatternMesh)
    {
        this->MeshComponent->SetStaticMesh(PatternMesh);
    }

    // Define glider offsets.
    TArray<FIntPoint> LocalOffsets = {
        FIntPoint(1, 0),
        FIntPoint(2, 1),
        FIntPoint(0, 2),
        FIntPoint(1, 2),
        FIntPoint(2, 2)
    };

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
