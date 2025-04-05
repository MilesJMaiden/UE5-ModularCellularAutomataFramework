#include "BeehivePattern.h"
#include "CellularAutomataManager.h"

ABeehivePattern::ABeehivePattern()
{
    // Assign PatternMesh via the editor.
}

void ABeehivePattern::ApplyPattern_Implementation(ACellularAutomataManager* Manager)
{
    if (!Manager)
        return;

    if (PatternMesh)
    {
        MeshComponent->SetStaticMesh(PatternMesh);
    }

    // Beehive (still life) offsets:
    // Common layout: (1,0), (2,0), (0,1), (3,1), (1,2), (2,2)
    TArray<FIntPoint> LocalOffsets;
    LocalOffsets.Add(FIntPoint(1, 0));
    LocalOffsets.Add(FIntPoint(2, 0));
    LocalOffsets.Add(FIntPoint(0, 1));
    LocalOffsets.Add(FIntPoint(3, 1));
    LocalOffsets.Add(FIntPoint(1, 2));
    LocalOffsets.Add(FIntPoint(2, 2));

    ComputeSeededIndicesFromOffsets(Manager, LocalOffsets);

    // Mark these grid cells as active.
    for (int32 Idx : SeededIndices)
    {
        if (Manager->CellGrid.IsValidIndex(Idx))
            Manager->CellGrid[Idx] = 1;
    }

    // Refresh pattern mesh instances.
    RefreshMeshInstances(Manager);
}
