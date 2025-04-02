#include "OscillatorPattern.h"
#include "CellularAutomataManager.h"

AOscillatorPattern::AOscillatorPattern()
{
    // Oscillator pattern — assign PatternMesh in editor
}

void AOscillatorPattern::ApplyPattern_Implementation(ACellularAutomataManager* Manager)
{
    if (!Manager)
        return;

    if (PatternMesh)
    {
        MeshComponent->SetStaticMesh(PatternMesh);
    }

    // Define a horizontal blinker: {-1, 0}, {0, 0}, {1, 0}
    TArray<FIntPoint> LocalOffsets;
    LocalOffsets.Add(FIntPoint(-1, 0));
    LocalOffsets.Add(FIntPoint(0, 0));
    LocalOffsets.Add(FIntPoint(1, 0));

    // Populate SeededIndices
    ComputeSeededIndicesFromOffsets(Manager, LocalOffsets);

    // Mark these cells alive
    for (int32 Idx : SeededIndices)
    {
        if (Manager->CellGrid.IsValidIndex(Idx))
        {
            Manager->CellGrid[Idx] = 1;
        }
    }
}
