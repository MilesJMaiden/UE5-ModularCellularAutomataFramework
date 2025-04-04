#include "OscillatorPattern.h"
#include "CellularAutomataManager.h"

AOscillatorPattern::AOscillatorPattern()
{
    // Assign PatternMesh via the editor.
}

void AOscillatorPattern::ApplyPattern_Implementation(ACellularAutomataManager* Manager)
{
    if (!Manager)
        return;

    if (PatternMesh)
    {
        this->MeshComponent->SetStaticMesh(PatternMesh);
    }

    // Define horizontal blinker offsets: (-1,0), (0,0), (1,0)
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
