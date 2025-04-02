#include "CellularAutomataManager.h"
#include "CellPatternBase.h"
#include "Engine/World.h"
#include "Engine/StaticMeshActor.h"
#include "Components/StaticMeshComponent.h"
#include "Materials/MaterialInstanceDynamic.h"

ACellularAutomataManager::ACellularAutomataManager()
{
    PrimaryActorTick.bCanEverTick = true;
    GridWidth = 50;
    GridHeight = 50;
    TimeStepInterval = 0.5f;
    TimeAccumulator = 0.0f;
}

void ACellularAutomataManager::BeginPlay()
{
    Super::BeginPlay();
    InitializeGrid();

    // Automatically spawn pattern actors based on the PatternSpawnInfos array.
    for (const FPatternSpawnInfo& Info : PatternSpawnInfos)
    {
        for (int32 i = 0; i < Info.InstanceCount; i++)
        {
            int32 RandX = FMath::RandRange(0, GridWidth - 1);
            int32 RandY = FMath::RandRange(0, GridHeight - 1);
            FVector PatternOrigin = GetActorLocation() + FVector(RandX * 100.0f, RandY * 100.0f, 0.0f);
            ApplyPattern(Info.PatternClass, PatternOrigin);
        }
    }
}

void ACellularAutomataManager::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    TimeAccumulator += DeltaTime;
    if (TimeAccumulator >= TimeStepInterval)
    {
        UpdateSimulation();
        TimeAccumulator = 0.0f;
    }
}

void ACellularAutomataManager::InitializeGrid()
{
    int32 TotalCells = GridWidth * GridHeight;
    CellGrid.Empty();
    CellGrid.SetNum(TotalCells);

    // Initialize CellIntensity to 0 (all cells start dead)
    CellIntensity.Empty();
    CellIntensity.SetNum(TotalCells);
    for (int32 i = 0; i < TotalCells; i++)
    {
        CellIntensity[i] = 0.0f;
    }

    CellActors.Empty();
    CellActors.SetNum(TotalCells);

    CellMeshOverrides.Empty();
    CellMeshOverrides.Init(nullptr, TotalCells);

    for (int32 y = 0; y < GridHeight; y++)
    {
        for (int32 x = 0; x < GridWidth; x++)
        {
            int32 Index = y * GridWidth + x;
            CellGrid[Index] = 0;
            SpawnCell(x, y, false);
        }
    }
}

int32 ACellularAutomataManager::GetLiveNeighborCountForCell(int32 X, int32 Y) const
{
    int32 Count = 0;
    for (int32 OffsetY = -1; OffsetY <= 1; OffsetY++)
    {
        for (int32 OffsetX = -1; OffsetX <= 1; OffsetX++)
        {
            if (OffsetX == 0 && OffsetY == 0)
                continue;
            int32 NeighborX = X + OffsetX;
            int32 NeighborY = Y + OffsetY;
            if (NeighborX >= 0 && NeighborX < GridWidth && NeighborY >= 0 && NeighborY < GridHeight)
            {
                int32 Index = NeighborY * GridWidth + NeighborX;
                Count += CellGrid[Index];
            }
        }
    }
    return Count;
}

void ACellularAutomataManager::UpdateSimulation()
{
    // 1. Update grid state using Conway's Game of Life rules.
    TArray<int32> NewGrid = CellGrid;
    for (int32 y = 0; y < GridHeight; y++)
    {
        for (int32 x = 0; x < GridWidth; x++)
        {
            int32 Index = y * GridWidth + x;
            int32 LiveNeighbors = GetLiveNeighborCountForCell(x, y);
            if (CellGrid[Index] == 1)
            {
                if (LiveNeighbors < 2 || LiveNeighbors > 3)
                    NewGrid[Index] = 0;
            }
            else
            {
                if (LiveNeighbors == 3)
                    NewGrid[Index] = 1;
            }
        }
    }
    CellGrid = NewGrid;

    // 2. Update CellIntensity for fade in/out.
    // For alive cells, at the start of the time step (TimeAccumulator = 0), intensity is 1.
    // Then, it decays linearly to 0 over the fade duration (FadeTime) of the owning pattern.
    // For simplicity, if a cell isn't owned by any pattern, we'll use 1.0.
    for (int32 i = 0; i < CellGrid.Num(); i++)
    {
        // Only update intensity for alive cells.
        if (CellGrid[i] == 1)
        {
            // We'll compute a fade value per cell later using the owning pattern's FadeTime.
            // For now, leave intensity at 1 (and let the update loop override it if needed).
            // (Alternatively, you could decay all cells uniformly; however, we want per-pattern fade.)
        }
        else
        {
            // Decay intensity for dead cells.
            // (If you want all cells to fade out regardless, you can update this.)
        }
    }

    // 3. Update each cell actor's dynamic material parameters.
    for (int32 i = 0; i < CellGrid.Num(); i++)
    {
        if (!CellActors.IsValidIndex(i) || !CellActors[i])
            continue;

        AStaticMeshActor* Actor = CellActors[i];
        UStaticMeshComponent* MeshComp = Actor->GetStaticMeshComponent();

        if (CellGrid[i] == 1)
        {
            FLinearColor DesiredColor = FLinearColor::White; // default color
            float DesiredOpacity = 1.0f; // default full opacity

            // Determine the owning pattern.
            for (ACellPatternBase* Pattern : ActivePatternActors)
            {
                if (!Pattern)
                    continue;
                TArray<int32> AffectedIndices = Pattern->GetAffectedIndices(this);
                if (AffectedIndices.Contains(i))
                {
                    DesiredColor = Pattern->PatternColor;
                    // Compute opacity decay: 
                    // Opacity = 1.0 - (TimeAccumulator / FadeTime), clamped to [0,1].
                    // Use the pattern's FadeTime; if zero or negative, assume full opacity.
                    if (Pattern->FadeTime > 0.0f)
                    {
                        DesiredOpacity = 1.0f - FMath::Clamp(TimeAccumulator / Pattern->FadeTime, 0.0f, 1.0f);
                    }
                    else
                    {
                        DesiredOpacity = 1.0f;
                    }
                    break; // first matching pattern wins
                }
            }

            Actor->SetActorHiddenInGame(false);
            UMaterialInstanceDynamic* DynMat = Cast<UMaterialInstanceDynamic>(MeshComp->GetMaterial(0));
            if (!DynMat && BaseCellMaterial)
            {
                DynMat = UMaterialInstanceDynamic::Create(BaseCellMaterial, this);
                MeshComp->SetMaterial(0, DynMat);
            }
            if (DynMat)
            {
                DynMat->SetVectorParameterValue(FName("BaseColor"), DesiredColor);
                DynMat->SetScalarParameterValue(FName("Opacity"), DesiredOpacity);
            }
        }
        else
        {
            Actor->SetActorHiddenInGame(true);
            UMaterialInstanceDynamic* DynMat = Cast<UMaterialInstanceDynamic>(MeshComp->GetMaterial(0));
            if (DynMat)
            {
                DynMat->SetScalarParameterValue(FName("Opacity"), 0.0f);
            }
        }
    }
}


void ACellularAutomataManager::ApplyPattern(TSubclassOf<ACellPatternBase> PatternClass, FVector Origin)
{
    if (PatternClass)
    {
        ACellPatternBase* Pattern = GetWorld()->SpawnActor<ACellPatternBase>(PatternClass, Origin, FRotator::ZeroRotator);
        if (Pattern)
        {
            Pattern->ApplyPattern(this);
            ActivePatternActors.Add(Pattern);
        }
    }
}

void ACellularAutomataManager::SpawnCell(int32 X, int32 Y, bool bIsAlive)
{
    FVector Location = GetActorLocation() + FVector(X * 100.0f, Y * 100.0f, 0.0f);
    FTransform Transform;
    Transform.SetLocation(Location);

    AStaticMeshActor* CellActor = GetWorld()->SpawnActor<AStaticMeshActor>(AStaticMeshActor::StaticClass(), Transform);
    if (CellActor && CellMesh)
    {
        CellActor->SetActorHiddenInGame(false);
        CellActor->GetStaticMeshComponent()->SetVisibility(true);
        CellActor->GetStaticMeshComponent()->SetStaticMesh(CellMesh);
        CellActor->GetStaticMeshComponent()->SetMobility(EComponentMobility::Movable);

        if (BaseCellMaterial)
        {
            UMaterialInstanceDynamic* DynMat = UMaterialInstanceDynamic::Create(BaseCellMaterial, this);
            CellActor->GetStaticMeshComponent()->SetMaterial(0, DynMat);
        }
    }
    int32 Index = Y * GridWidth + X;
    if (CellActors.IsValidIndex(Index))
    {
        CellActors[Index] = CellActor;
    }
    else
    {
        CellActors.Add(CellActor);
    }
}
