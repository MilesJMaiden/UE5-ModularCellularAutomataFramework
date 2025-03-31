#include "OscillatorPattern.h"
#include "CellularAutomataManager.h"

AOscillatorPattern::AOscillatorPattern()
{
}

void AOscillatorPattern::ApplyPattern_Implementation(ACellularAutomataManager* Manager)
{
    if (!Manager)
        return;

    // Example: Create a horizontal blinker pattern.
    FVector Origin = GetActorLocation();
    int32 GridX = FMath::RoundToInt((Origin.X - Manager->GetActorLocation().X) / 100.0f);
    int32 GridY = FMath::RoundToInt((Origin.Y - Manager->GetActorLocation().Y) / 100.0f);

    if (Manager->CellGrid.IsValidIndex(GridY))
    {
        if (Manager->CellGrid[GridY].IsValidIndex(GridX - 1))
            Manager->CellGrid[GridY][GridX - 1] = 1;
        if (Manager->CellGrid[GridY].IsValidIndex(GridX))
            Manager->CellGrid[GridY][GridX] = 1;
        if (Manager->CellGrid[GridY].IsValidIndex(GridX + 1))
            Manager->CellGrid[GridY][GridX + 1] = 1;
    }
}
