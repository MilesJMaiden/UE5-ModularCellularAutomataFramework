#include "SpaceshipPattern.h"
#include "CellularAutomataManager.h"

ASpaceshipPattern::ASpaceshipPattern()
{
}

void ASpaceshipPattern::ApplyPattern_Implementation(ACellularAutomataManager* Manager)
{
    if (!Manager)
        return;

    FVector Origin = GetActorLocation();
    int32 GridX = FMath::RoundToInt((Origin.X - Manager->GetActorLocation().X) / 100.0f);
    int32 GridY = FMath::RoundToInt((Origin.Y - Manager->GetActorLocation().Y) / 100.0f);

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
        if (PosX >= 0 && PosX < Manager->GridWidth && PosY >= 0 && PosY < Manager->GridHeight)
        {
            int32 Index = PosY * Manager->GridWidth + PosX;
            Manager->CellGrid[Index] = 1;
        }
    }
}
