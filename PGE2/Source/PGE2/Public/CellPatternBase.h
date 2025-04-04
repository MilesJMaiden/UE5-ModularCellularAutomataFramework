#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CellPatternBase.generated.h"

class ACellularAutomataManager;

/**
 * Abstract base class for cellular automata patterns.
 * This class holds common visual properties and manages the 3D mesh instances for a pattern.
 * Mesh instances are spawned (and later removed) so that at each simulation update
 * the pattern’s 3D meshes match exactly the active grid tiles.
 */
UCLASS(Abstract, Blueprintable)
class PGE2_API ACellPatternBase : public AActor
{
    GENERATED_BODY()

public:
    ACellPatternBase();

    // BlueprintNativeEvent for applying the pattern to the grid (should be overridden by subclasses).
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Cell Pattern")
    void ApplyPattern(ACellularAutomataManager* Manager);
    virtual void ApplyPattern_Implementation(ACellularAutomataManager* Manager);

    // Returns the grid indices that this pattern has seeded.
    UFUNCTION(BlueprintCallable, Category = "Cell Pattern")
    virtual TArray<int32> GetAffectedIndices(const ACellularAutomataManager* Manager) const;

    // The pattern's specific 3D mesh (assigned in the editor).
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cell Pattern")
    UStaticMesh* PatternMesh;

    // (Optional) Material for this pattern.
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cell Pattern")
    UMaterialInterface* PatternMaterial;

    // The pattern's color (used to drive material parameters such as BaseColor).
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cell Pattern")
    FLinearColor PatternColor;

    // The fade time (in seconds) over which the opacity decays from 1 to 0.
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cell Pattern")
    float FadeTime;

    // MeshComponent to display this pattern actor (if desired).
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Cell Pattern")
    UStaticMeshComponent* MeshComponent;

    // Array of mesh components spawned for each active seeded grid cell.
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Cell Pattern")
    TArray<UStaticMeshComponent*> PatternMeshInstances;

    // Refreshes the pattern’s mesh instances: deletes any existing ones and spawns new ones based on active grid cells.
    UFUNCTION(BlueprintCallable, Category = "Cell Pattern")
    void RefreshMeshInstances(const ACellularAutomataManager* Manager);

    // Updates the positions of all mesh instances so that they are centered on their grid tiles.
    UFUNCTION(BlueprintCallable, Category = "Cell Pattern")
    void UpdateMeshPosition(const ACellularAutomataManager* Manager);

protected:
    // Cached grid indices where this pattern is seeded.
    UPROPERTY()
    TArray<int32> SeededIndices;

    /**
     * Utility for subclasses to populate SeededIndices from local offsets (in grid units)
     * relative to the pattern’s computed grid origin.
     */
    UFUNCTION(BlueprintCallable, Category = "Cell Pattern")
    void ComputeSeededIndicesFromOffsets(const ACellularAutomataManager* Manager, const TArray<FIntPoint>& LocalOffsets);

    /**
     * Spawns one mesh instance for each seeded grid cell (later filtered for active cells).
     * Each instance is a new StaticMeshComponent attached to this actor.
     */
    UFUNCTION(BlueprintCallable, Category = "Cell Pattern")
    void SpawnPatternMeshInstances(const ACellularAutomataManager* Manager);
};
