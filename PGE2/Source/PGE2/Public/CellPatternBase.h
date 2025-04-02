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

    // BlueprintNativeEvent for applying the pattern to the grid (should be overridden by subclasses).
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Cell Pattern")
    void ApplyPattern(ACellularAutomataManager* Manager);
    virtual void ApplyPattern_Implementation(ACellularAutomataManager* Manager);

    // Returns the grid indices that this pattern has seeded.
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

    // MeshComponent to display this pattern actor (if desired).
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Cell Pattern")
    UStaticMeshComponent* MeshComponent;

protected:
    // Cached indices where this pattern applied itself to the grid.
    UPROPERTY()
    TArray<int32> SeededIndices;

    /**
     * Utility for subclasses to populate SeededIndices from local offsets.
     * The LocalOffsets array should contain the (x,y) offsets (in grid units)
     * from the pattern's computed grid origin.
     */
    void ComputeSeededIndicesFromOffsets(const ACellularAutomataManager* Manager, const TArray<FIntPoint>& LocalOffsets);
};
