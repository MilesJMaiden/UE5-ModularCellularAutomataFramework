#include "CellPatternBase.h"
#include "Components/StaticMeshComponent.h"
#include "CellularAutomataManager.h"

ACellPatternBase::ACellPatternBase()
{
    PrimaryActorTick.bCanEverTick = false;

    // Create the mesh component and set it as the RootComponent.
    MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
    RootComponent = MeshComponent;

    PatternMesh = nullptr;
    PatternMaterial = nullptr;
    PatternColor = FLinearColor::White; // default color

    // SeededIndices will be filled by subclasses.
}

void ACellPatternBase::ApplyPattern_Implementation(ACellularAutomataManager* Manager)
{
    // Default implementation does nothing — subclasses must override this.
}

TArray<int32> ACellPatternBase::GetAffectedIndices(const ACellularAutomataManager* Manager) const
{
    // Return the cached seeded indices.
    return SeededIndices;
}

void ACellPatternBase::ComputeSeededIndicesFromOffsets(const ACellularAutomataManager* Manager, const TArray<FIntPoint>& LocalOffsets)
{
    if (!Manager) return;

    FVector Origin = GetActorLocation();
    // Compute grid coordinates based on the manager's origin and an assumed spacing of 100 units.
    int32 GridX = FMath::FloorToInt((Origin.X - Manager->GetActorLocation().X) / 100.0f);
    int32 GridY = FMath::FloorToInt((Origin.Y - Manager->GetActorLocation().Y) / 100.0f);

    // Clear any existing indices.
    SeededIndices.Empty();

    // For each offset, compute the absolute grid index and add if valid.
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
