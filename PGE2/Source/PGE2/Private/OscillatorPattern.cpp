#include "OscillatorPattern.h"
#include "CellularAutomataManager.h"

AOscillatorPattern::AOscillatorPattern()
{
}

void AOscillatorPattern::ApplyPattern_Implementation(ACellularAutomataManager* Manager)
{
    if (!Manager)
        return;

    FVector Origin = GetActorLocation();
    int32 GridX = FMath::RoundToInt((Origin.X - Manager->GetActorLocation().X) / 100.0f);
    int32 GridY = FMath::RoundToInt((Origin.Y - Manager->GetActorLocation().Y) / 100.0f);

    // Create a horizontal blinker.
    if ((GridX - 1) >= 0 && (GridX - 1) < Manager->GridWidth && GridY >= 0 && GridY < Manager->GridHeight)
    {
        int32 Index = GridY * Manager->GridWidth + (GridX - 1);
        Manager->CellGrid[Index] = 1;
    }
    if (GridX >= 0 && GridX < Manager->GridWidth && GridY >= 0 && GridY < Manager->GridHeight)
    {
        int32 Index = GridY * Manager->GridWidth + GridX;
        Manager->CellGrid[Index] = 1;
    }
    if ((GridX + 1) >= 0 && (GridX + 1) < Manager->GridWidth && GridY >= 0 && GridY < Manager->GridHeight)
    {
        int32 Index = GridY * Manager->GridWidth + (GridX + 1);
        Manager->CellGrid[Index] = 1;
    }
}
