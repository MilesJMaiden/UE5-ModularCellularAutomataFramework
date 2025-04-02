#include "StillLifePattern.h"
#include "CellularAutomataManager.h"

AStillLifePattern::AStillLifePattern()
{
    // Assign PatternMesh via the editor
}

void AStillLifePattern::ApplyPattern_Implementation(ACellularAutomataManager* Manager)
{
    if (!Manager)
        return;

    if (PatternMesh)
    {
        MeshComponent->SetStaticMesh(PatternMesh);
    }

    // Define a 2x2 block: (0,0), (1,0), (0,1), (1,1)
    TArray<FIntPoint> LocalOffsets;
    LocalOffsets.Add(FIntPoint(0, 0));
    LocalOffsets.Add(FIntPoint(1, 0));
    LocalOffsets.Add(FIntPoint(0, 1));
    LocalOffsets.Add(FIntPoint(1, 1));

    // Populate SeededIndices
    ComputeSeededIndicesFromOffsets(Manager, LocalOffsets);

    // Activate those cells in the manager's grid
    for (int32 Idx : SeededIndices)
    {
        if (Manager->CellGrid.IsValidIndex(Idx))
        {
            Manager->CellGrid[Idx] = 1;
        }
    }
}
