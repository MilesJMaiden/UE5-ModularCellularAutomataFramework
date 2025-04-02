#include "CellPatternBase.h"
#include "Components/StaticMeshComponent.h"
#include "CellularAutomataManager.h"

ACellPatternBase::ACellPatternBase()
{
    PrimaryActorTick.bCanEverTick = false;

    MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
    RootComponent = MeshComponent;

    PatternMesh = nullptr;
    PatternMaterial = nullptr;
}

void ACellPatternBase::ApplyPattern_Implementation(ACellularAutomataManager* Manager)
{
    // Default implementation does nothing — should be overridden by subclasses.
}

TArray<int32> ACellPatternBase::GetAffectedIndices(const ACellularAutomataManager* Manager) const
{
    return SeededIndices;
}

void ACellPatternBase::ComputeSeededIndicesFromOffsets(const ACellularAutomataManager* Manager, const TArray<FIntPoint>& LocalOffsets)
{
    if (!Manager) return;

    FVector Origin = GetActorLocation();
    int32 GridX = FMath::FloorToInt((Origin.X - Manager->GetActorLocation().X) / 100.0f);
    int32 GridY = FMath::FloorToInt((Origin.Y - Manager->GetActorLocation().Y) / 100.0f);

    for (const FIntPoint& Offset : LocalOffsets)
    {
        int32 X = GridX + Offset.X;
        int32 Y = GridY + Offset.Y;

        if (X >= 0 && X < Manager->GridWidth && Y >= 0 && Y < Manager->GridHeight)
        {
            int32 Index = Y * Manager->GridWidth + X;
            SeededIndices.Add(Index);
        }
    }
}
