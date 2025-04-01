#include "StillLifePattern.h"
#include "CellularAutomataManager.h"

AStillLifePattern::AStillLifePattern()
{
}

void AStillLifePattern::ApplyPattern_Implementation(ACellularAutomataManager* Manager)
{
    if (!Manager)
        return;

    FVector Origin = GetActorLocation();
    int32 GridX = FMath::RoundToInt((Origin.X - Manager->GetActorLocation().X) / 100.0f);
    int32 GridY = FMath::RoundToInt((Origin.Y - Manager->GetActorLocation().Y) / 100.0f);

    // Set a 2x2 block.
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
    if (GridX >= 0 && GridX < Manager->GridWidth && (GridY + 1) >= 0 && (GridY + 1) < Manager->GridHeight)
    {
        int32 Index = (GridY + 1) * Manager->GridWidth + GridX;
        Manager->CellGrid[Index] = 1;
    }
    if ((GridX + 1) >= 0 && (GridX + 1) < Manager->GridWidth && (GridY + 1) >= 0 && (GridY + 1) < Manager->GridHeight)
    {
        int32 Index = (GridY + 1) * Manager->GridWidth + (GridX + 1);
        Manager->CellGrid[Index] = 1;
    }
}
