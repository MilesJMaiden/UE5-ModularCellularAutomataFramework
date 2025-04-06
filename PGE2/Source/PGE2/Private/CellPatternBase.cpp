#include "CellPatternBase.h"
#include "Components/StaticMeshComponent.h"
#include "CellularAutomataManager.h"

ACellPatternBase::ACellPatternBase()
{
    PrimaryActorTick.bCanEverTick = false;

    // Create the default MeshComponent for the pattern actor.
    MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
    RootComponent = MeshComponent;

    PatternMesh = nullptr;
    PatternMaterial = nullptr;
    PatternColor = FLinearColor::White;
    FadeTime = 0.5f;

    SeededIndices.Empty();
    PatternMeshInstances.Empty();
}

void ACellPatternBase::ApplyPattern_Implementation(ACellularAutomataManager* Manager)
{
    // Default implementation does nothing—override in subclasses.
}

TArray<int32> ACellPatternBase::GetAffectedIndices(const ACellularAutomataManager* Manager) const
{
    return SeededIndices;
}

void ACellPatternBase::ComputeSeededIndicesFromOffsets(const ACellularAutomataManager* Manager, const TArray<FIntPoint>& LocalOffsets)
{
    if (!Manager)
        return;

    FVector Origin = GetActorLocation();
    int32 GridX = FMath::FloorToInt((Origin.X - Manager->GetActorLocation().X) / 100.0f);
    int32 GridY = FMath::FloorToInt((Origin.Y - Manager->GetActorLocation().Y) / 100.0f);

    SeededIndices.Empty();
    for (const FIntPoint& Offset : LocalOffsets)
    {
        int32 X = GridX + Offset.X;
        int32 Y = GridY + Offset.Y;
        if (X >= 0 && X < Manager->GridWidth && Y >= 0 && Y < Manager->GridHeight)
        {
            int32 Index = Y * Manager->GridWidth + X;
            SeededIndices.AddUnique(Index); // Prevent duplicates.
        }
    }
}


void ACellPatternBase::SpawnPatternMeshInstances(const ACellularAutomataManager* Manager)
{
    // Destroy any existing mesh instances.
    for (UStaticMeshComponent* Comp : PatternMeshInstances)
    {
        if (Comp)
        {
            Comp->DestroyComponent();
        }
    }
    PatternMeshInstances.Empty();

    if (!Manager || !PatternMesh)
        return;

    // Instead of spawning one mesh per seeded index, spawn only for those cells that are active.
    TArray<int32> ActiveSeededIndices;
    for (int32 Index : SeededIndices)
    {
        if (Manager->CellGrid.IsValidIndex(Index) && Manager->CellGrid[Index] == 1)
        {
            ActiveSeededIndices.Add(Index);
        }
    }

    // Spawn one mesh instance for each active seeded index.
    for (int32 Index : ActiveSeededIndices)
    {
        UStaticMeshComponent* NewMeshComp = NewObject<UStaticMeshComponent>(this);
        if (NewMeshComp)
        {
            NewMeshComp->RegisterComponent();
            NewMeshComp->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
            NewMeshComp->SetStaticMesh(PatternMesh);
            if (PatternMaterial)
            {
                NewMeshComp->SetMaterial(0, PatternMaterial);
            }
            PatternMeshInstances.Add(NewMeshComp);
        }
    }
}

void ACellPatternBase::UpdateMeshPosition(const ACellularAutomataManager* Manager)
{
    if (!Manager)
        return;

    // Get active seeded indices (only for cells that are active).
    TArray<int32> ActiveSeededIndices;
    for (int32 Index : SeededIndices)
    {
        if (Manager->CellGrid.IsValidIndex(Index) && Manager->CellGrid[Index] == 1)
        {
            ActiveSeededIndices.Add(Index);
        }
    }

    // For each active seeded index, update the corresponding mesh instance’s location.
    const float Spacing = 100.0f;
    FVector ManagerOrigin = Manager->GetActorLocation();
    for (int32 i = 0; i < ActiveSeededIndices.Num(); i++)
    {
        int32 Index = ActiveSeededIndices[i];
        int32 GridX = Index % Manager->GridWidth;
        int32 GridY = Index / Manager->GridWidth;
        // Center of cell: add 0.5 * Spacing.
        FVector NewLocation = ManagerOrigin + FVector((GridX + 0.5f) * Spacing, (GridY + 0.5f) * Spacing, 0.0f);
        if (PatternMeshInstances.IsValidIndex(i) && PatternMeshInstances[i])
        {
            // Set world location directly (or interpolate if smooth transition is desired).
            PatternMeshInstances[i]->SetWorldLocation(NewLocation);
        }
    }
}

void ACellPatternBase::RefreshMeshInstances(const ACellularAutomataManager* Manager)
{
    // Destroy and re-spawn the mesh instances so that they match exactly the active cells.
    for (UStaticMeshComponent* Comp : PatternMeshInstances)
    {
        if (Comp)
        {
            Comp->DestroyComponent();
        }
    }
    PatternMeshInstances.Empty();
    SpawnPatternMeshInstances(Manager);
    UpdateMeshPosition(Manager);
}
