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
    DefaultFadeTime = 1.0f;
    TimeAccumulator = 0.0f;
    TimeInStep = 0.0f;
    bEnableAnimations = true;
    bEnableFade = true;

    CellMesh = nullptr;
    BaseCellMaterial = nullptr;
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
            // Spawn the pattern at the center of the chosen grid tile.
            int32 RandX = FMath::RandRange(0, GridWidth - 1);
            int32 RandY = FMath::RandRange(0, GridHeight - 1);
            FVector PatternOrigin = GetActorLocation() + FVector((RandX + 0.5f) * 100.0f, (RandY + 0.5f) * 100.0f, 0.0f);
            ApplyPattern(Info.PatternClass, PatternOrigin);
        }
    }
}

void ACellularAutomataManager::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    TimeAccumulator += DeltaTime;
    TimeInStep += DeltaTime;

    // Update per-cell activation time.
    for (int32 i = 0; i < CellGrid.Num(); i++)
    {
        if (CellGrid[i] == 1)
            CellActivationTime[i] += DeltaTime;
        else
            CellActivationTime[i] = 0.0f;
    }

    if (TimeAccumulator >= TimeStepInterval)
    {
        // Save the old state to reset activation time for newly active cells.
        TArray<int32> OldGrid = CellGrid;
        UpdateSimulation();
        TimeAccumulator = 0.0f;
        for (int32 i = 0; i < CellGrid.Num(); i++)
        {
            if (CellGrid[i] == 1 && OldGrid[i] == 0)
                CellActivationTime[i] = 0.0f;
        }
    }
    else
    {
        // While between simulation updates, update opacity continuously if enabled.
        if (bEnableFade)
        {
            for (int32 i = 0; i < CellGrid.Num(); i++)
            {
                if (!CellActors.IsValidIndex(i) || !CellActors[i])
                    continue;
                if (CellGrid[i] == 1)
                {
                    float fadeTime = DefaultFadeTime;
                    // If a pattern covers this cell, use its fade time.
                    for (ACellPatternBase* Pattern : ActivePatternActors)
                    {
                        if (!Pattern)
                            continue;
                        TArray<int32> AffectedIndices = Pattern->GetAffectedIndices(this);
                        if (AffectedIndices.Contains(i))
                        {
                            if (Pattern->FadeTime > 0.0f)
                                fadeTime = Pattern->FadeTime;
                            break;
                        }
                    }
                    float DesiredOpacity = 1.0f - FMath::Clamp(CellActivationTime[i] / fadeTime, 0.0f, 1.0f);
                    CellIntensity[i] = DesiredOpacity;
                    AStaticMeshActor* Actor = CellActors[i];
                    UStaticMeshComponent* MeshComp = Actor->GetStaticMeshComponent();
                    UMaterialInstanceDynamic* DynMat = Cast<UMaterialInstanceDynamic>(MeshComp->GetMaterial(0));
                    if (DynMat)
                    {
                        DynMat->SetScalarParameterValue(FName("Opacity"), DesiredOpacity);
                    }
                }
            }
        }
        else
        {
            for (int32 i = 0; i < CellGrid.Num(); i++)
            {
                if (!CellActors.IsValidIndex(i) || !CellActors[i])
                    continue;
                if (CellGrid[i] == 1)
                {
                    CellIntensity[i] = 1.0f;
                    AStaticMeshActor* Actor = CellActors[i];
                    UStaticMeshComponent* MeshComp = Actor->GetStaticMeshComponent();
                    UMaterialInstanceDynamic* DynMat = Cast<UMaterialInstanceDynamic>(MeshComp->GetMaterial(0));
                    if (DynMat)
                    {
                        DynMat->SetScalarParameterValue(FName("Opacity"), 1.0f);
                    }
                }
            }
        }
    }

    // Update each cell actor's transform so that it is centered on its grid tile.
    for (int32 i = 0; i < CellActors.Num(); i++)
    {
        if (!CellActors.IsValidIndex(i) || !CellActors[i])
            continue;
        AStaticMeshActor* Actor = CellActors[i];
        int32 X = i % GridWidth;
        int32 Y = i / GridWidth;
        // Center-of-cell: (X + 0.5, Y + 0.5) * 100.
        FVector TargetLocation = GetActorLocation() + FVector((X + 0.5f) * 100.0f, (Y + 0.5f) * 100.0f, 0.0f);
        FVector CurrentLocation = Actor->GetActorLocation();
        float LerpSpeed = 5.0f;
        FVector NewLocation = FMath::VInterpTo(CurrentLocation, TargetLocation, GetWorld()->DeltaTimeSeconds, LerpSpeed);

        if (bEnableAnimations && CellGrid[i] == 1)
        {
            float ScaleFactor = 1.0f + 0.2f * FMath::Sin((TimeInStep + i * 0.1f) * PI * 2.0f / DefaultFadeTime);
            float RotationAngle = FMath::Fmod((TimeInStep + i * 0.05f) * 30.0f, 360.0f);
            FTransform CurrentTransform = Actor->GetActorTransform();
            CurrentTransform.SetLocation(NewLocation);
            CurrentTransform.SetScale3D(FVector(ScaleFactor));
            CurrentTransform.SetRotation(FQuat(FRotator(0, RotationAngle, 0)));
            Actor->SetActorTransform(CurrentTransform);
        }
        else
        {
            FTransform CurrentTransform = Actor->GetActorTransform();
            CurrentTransform.SetLocation(NewLocation);
            Actor->SetActorTransform(CurrentTransform);
        }
    }

    // For each active pattern actor, re-create its 3D mesh instances to exactly match the active grid tiles.
    for (ACellPatternBase* Pattern : ActivePatternActors)
    {
        if (Pattern)
        {
            Pattern->RefreshMeshInstances(this);
        }
    }
}

void ACellularAutomataManager::InitializeGrid()
{
    int32 TotalCells = GridWidth * GridHeight;
    CellGrid.Empty();
    CellGrid.SetNum(TotalCells);

    CellIntensity.Empty();
    CellIntensity.SetNum(TotalCells);
    CellActivationTime.Empty();
    CellActivationTime.SetNum(TotalCells);
    for (int32 i = 0; i < TotalCells; i++)
    {
        CellIntensity[i] = 0.0f;
        CellActivationTime[i] = 0.0f;
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
    TArray<int32> OldGrid = CellGrid;

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

    // 2. Update CellIntensity based on each cell's activation time.
    for (int32 i = 0; i < CellGrid.Num(); i++)
    {
        if (CellGrid[i] == 1)
        {
            float fadeTime = DefaultFadeTime;
            for (ACellPatternBase* Pattern : ActivePatternActors)
            {
                if (!Pattern)
                    continue;
                TArray<int32> AffectedIndices = Pattern->GetAffectedIndices(this);
                if (AffectedIndices.Contains(i))
                {
                    if (Pattern->FadeTime > 0.0f)
                        fadeTime = Pattern->FadeTime;
                    break;
                }
            }
            float DesiredOpacity = 1.0f - FMath::Clamp(CellActivationTime[i] / fadeTime, 0.0f, 1.0f);
            CellIntensity[i] = DesiredOpacity;
        }
        else
        {
            CellIntensity[i] = 0.0f;
        }
    }

    // 3. Update each cell actor's dynamic material parameters.
    for (int32 i = 0; i < CellGrid.Num(); i++)
    {
        if (!CellActors.IsValidIndex(i) || !CellActors[i])
            continue;

        AStaticMeshActor* Actor = CellActors[i];
        UStaticMeshComponent* MeshComp = Actor->GetStaticMeshComponent();
        UMaterialInstanceDynamic* DynMat = Cast<UMaterialInstanceDynamic>(MeshComp->GetMaterial(0));
        if (!DynMat && BaseCellMaterial)
        {
            DynMat = UMaterialInstanceDynamic::Create(BaseCellMaterial, this);
            MeshComp->SetMaterial(0, DynMat);
        }

        if (CellGrid[i] == 1)
        {
            FLinearColor DesiredColor = FLinearColor::White;
            for (ACellPatternBase* Pattern : ActivePatternActors)
            {
                if (!Pattern)
                    continue;
                TArray<int32> AffectedIndices = Pattern->GetAffectedIndices(this);
                if (AffectedIndices.Contains(i))
                {
                    DesiredColor = Pattern->PatternColor;
                    break;
                }
            }
            Actor->SetActorHiddenInGame(false);
            if (DynMat)
            {
                DynMat->SetVectorParameterValue(FName("BaseColor"), DesiredColor);
                DynMat->SetScalarParameterValue(FName("Opacity"), CellIntensity[i]);
            }
        }
        else
        {
            Actor->SetActorHiddenInGame(true);
            if (DynMat)
            {
                DynMat->SetScalarParameterValue(FName("Opacity"), 0.0f);
            }
        }
    }

    // 4. For each active pattern actor, remove and re-spawn its mesh instances so that they match the active tiles.
    for (ACellPatternBase* Pattern : ActivePatternActors)
    {
        if (Pattern)
        {
            Pattern->RefreshMeshInstances(this);
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
    // Spawn the cell actor at the center of the grid tile.
    FVector Location = GetActorLocation() + FVector((X + 0.5f) * 100.0f, (Y + 0.5f) * 100.0f, 0.0f);
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
