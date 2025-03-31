#include "StillLifePattern.h"
#include "CellularAutomataManager.h"

AStillLifePattern::AStillLifePattern()
{
}

void AStillLifePattern::ApplyPattern_Implementation(ACellularAutomataManager* Manager)
{
    if (!Manager)
        return;

    // Example: Create a 2x2 block (a Still Life) at the pattern's origin.
    FVector Origin = GetActorLocation();
    int32 GridX = FMath::RoundToInt((Origin.X - Manager->GetActorLocation().X) / 100.0f);
    int32 GridY = FMath::RoundToInt((Origin.Y - Manager->GetActorLocation().Y) / 100.0f);

    if (Manager->CellGrid.IsValidIndex(GridY) && Manager->CellGrid[GridY].IsValidIndex(GridX))
        Manager->CellGrid[GridY][GridX] = 1;
    if (Manager->CellGrid.IsValidIndex(GridY) && Manager->CellGrid[GridY].IsValidIndex(GridX + 1))
        Manager->CellGrid[GridY][GridX + 1] = 1;
    if (Manager->CellGrid.IsValidIndex(GridY + 1) && Manager->CellGrid[GridY + 1].IsValidIndex(GridX))
        Manager->CellGrid[GridY + 1][GridX] = 1;
    if (Manager->CellGrid.IsValidIndex(GridY + 1) && Manager->CellGrid[GridY + 1].IsValidIndex(GridX + 1))
        Manager->CellGrid[GridY + 1][GridX + 1] = 1;
}
