#include "SpaceshipPattern.h"
#include "CellularAutomataManager.h"

ASpaceshipPattern::ASpaceshipPattern()
{
    // Assign a PatternMesh in the editor
}

void ASpaceshipPattern::ApplyPattern_Implementation(ACellularAutomataManager* Manager)
{
    if (!Manager)
        return;

    if (PatternMesh)
    {
        MeshComponent->SetStaticMesh(PatternMesh);
    }

    // Glider pattern offsets (e.g., moving diagonally)
    TArray<FIntPoint> LocalOffsets = {
        FIntPoint(1, 0),
        FIntPoint(2, 1),
        FIntPoint(0, 2),
        FIntPoint(1, 2),
        FIntPoint(2, 2)
    };

    // Cache the affected tile indices
    ComputeSeededIndicesFromOffsets(Manager, LocalOffsets);

    // Seed the grid with living cells at those indices
    for (int32 Idx : SeededIndices)
    {
        if (Manager->CellGrid.IsValidIndex(Idx))
        {
            Manager->CellGrid[Idx] = 1;
        }
    }
}
