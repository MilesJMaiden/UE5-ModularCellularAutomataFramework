#include "SpaceshipPattern.h"
#include "CellularAutomataManager.h"

ASpaceshipPattern::ASpaceshipPattern()
{
}

void ASpaceshipPattern::ApplyPattern_Implementation(ACellularAutomataManager* Manager)
{
    if (!Manager)
        return;

    // Example: Create a glider pattern.
    FVector Origin = GetActorLocation();
    int32 GridX = FMath::RoundToInt((Origin.X - Manager->GetActorLocation().X) / 100.0f);
    int32 GridY = FMath::RoundToInt((Origin.Y - Manager->GetActorLocation().Y) / 100.0f);

    // Offsets for a basic glider pattern.
    TArray<FIntPoint> Offsets = {
        FIntPoint(1, 0),
        FIntPoint(2, 1),
        FIntPoint(0, 2),
        FIntPoint(1, 2),
        FIntPoint(2, 2)
    };

    for (const FIntPoint& Offset : Offsets)
    {
        int32 PosX = GridX + Offset.X;
        int32 PosY = GridY + Offset.Y;
        if (Manager->CellGrid.IsValidIndex(PosY) && Manager->CellGrid[PosY].IsValidIndex(PosX))
        {
            Manager->CellGrid[PosY][PosX] = 1;
        }
    }
}