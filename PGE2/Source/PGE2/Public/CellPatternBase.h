#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CellPatternBase.generated.h"

class ACellularAutomataManager;

/**
 * Abstract base class for cellular automata patterns.
 * Contains common visual properties and a mesh component.
 */
UCLASS(Abstract, Blueprintable)
class PGE2_API ACellPatternBase : public AActor
{
    GENERATED_BODY()

public:
    ACellPatternBase();

    // BlueprintNativeEvent for applying the pattern to the grid (to be overridden).
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Cell Pattern")
    void ApplyPattern(ACellularAutomataManager* Manager);
    virtual void ApplyPattern_Implementation(ACellularAutomataManager* Manager);

    // Returns the grid indices that this pattern initially affected.
    virtual TArray<int32> GetAffectedIndices(const ACellularAutomataManager* Manager) const;

    // Pattern mesh to apply to affected tiles.
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cell Pattern")
    UStaticMesh* PatternMesh;

    // Optional override material.
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cell Pattern")
    UMaterialInterface* PatternMaterial;

    // Visual mesh component (optional for in-world marker).
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Cell Pattern")
    UStaticMeshComponent* MeshComponent;

protected:
    // Cached indices where this pattern applied itself to the grid.
    UPROPERTY()
    TArray<int32> SeededIndices;

    // Utility for subclasses to populate SeededIndices from local offsets.
    void ComputeSeededIndicesFromOffsets(const ACellularAutomataManager* Manager, const TArray<FIntPoint>& LocalOffsets);
};
