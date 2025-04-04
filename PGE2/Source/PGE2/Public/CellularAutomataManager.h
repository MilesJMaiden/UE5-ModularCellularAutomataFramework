#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Engine/StaticMeshActor.h" // Full definition for AStaticMeshActor
#include "CellularAutomataManager.generated.h"

/**
 * Structure to hold pattern spawning settings.
 * Specify the pattern class and the total number of instances to spawn.
 */
USTRUCT(BlueprintType)
struct FPatternSpawnInfo
{
    GENERATED_BODY()

    // The pattern blueprint (or C++ class) to spawn.
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pattern")
    TSubclassOf<class ACellPatternBase> PatternClass;

    // Total number of instances to spawn randomly on the grid.
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pattern")
    int32 InstanceCount;
};

UCLASS(Blueprintable)
class PGE2_API ACellularAutomataManager : public AActor
{
    GENERATED_BODY()

public:
    ACellularAutomataManager();

    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;

    // Grid dimensions.
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cellular Automata")
    int32 GridWidth;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cellular Automata")
    int32 GridHeight;

    // Time between simulation steps.
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cellular Automata")
    float TimeStepInterval;

    // Default fade time (in seconds) used if no pattern-specific fade time is provided.
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cellular Automata")
    float DefaultFadeTime;

    // Default mesh for an alive cell.
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cellular Automata")
    UStaticMesh* CellMesh;

    // (Optional) Base material for cell actors (should use parameters "BaseColor" and "Opacity").
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cellular Automata")
    UMaterialInterface* BaseCellMaterial;

    // Initializes the grid with all dead cells.
    UFUNCTION(BlueprintCallable, Category = "Cellular Automata")
    void InitializeGrid();

    // Updates the simulation state and then updates cell visuals.
    UFUNCTION(BlueprintCallable, Category = "Cellular Automata")
    void UpdateSimulation();

    // Spawns a pattern actor at a specified origin.
    UFUNCTION(BlueprintCallable, Category = "Cellular Automata")
    void ApplyPattern(TSubclassOf<class ACellPatternBase> PatternClass, FVector Origin);

    // The grid state (0 = dead, 1 = alive) stored as a 1D array.
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Cellular Automata")
    TArray<int32> CellGrid;

    // References to spawned cell actors (ordered to match the grid).
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Cellular Automata")
    TArray<AStaticMeshActor*> CellActors;

    // For each grid cell, an override mesh (if a pattern seeded it).
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Cellular Automata")
    TArray<UStaticMesh*> CellMeshOverrides;

    // Array of pattern spawn infos to automatically seed the grid.
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cellular Automata|Patterns")
    TArray<FPatternSpawnInfo> PatternSpawnInfos;

    // Tracks active pattern actors.
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Cellular Automata")
    TArray<class ACellPatternBase*> ActivePatternActors;

    // Intensity for each cell (used to control opacity via dynamic material parameters).
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Cellular Automata")
    TArray<float> CellIntensity;

    // Per-cell activation time (records the time since a cell was activated).
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Cellular Automata")
    TArray<float> CellActivationTime;

    // --- New Boolean Options ---
    // Toggle for 3D mesh animations (rotation & pulsation).
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Visual Effects")
    bool bEnableAnimations;

    // Toggle for opacity fading. When disabled, all alive cells remain fully opaque.
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Visual Effects")
    bool bEnableFade;

protected:
    float TimeAccumulator;

    // Elapsed time within the current simulation step (for opacity fade).
    // Not reset at simulation update so that fade can extend beyond the simulation step.
    float TimeInStep;

    // Helper: spawn a cell actor at grid coordinates (X, Y).
    void SpawnCell(int32 X, int32 Y, bool bIsAlive);

    // Helper: count live neighbors for a given cell.
    int32 GetLiveNeighborCountForCell(int32 X, int32 Y) const;
};
