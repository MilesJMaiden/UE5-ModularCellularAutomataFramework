#pragma once

#include "CoreMinimal.h"
#include "CellPatternBase.h"
#include "StillLifePattern.generated.h"

UCLASS(Blueprintable)
class PGE2_API AStillLifePattern : public ACellPatternBase
{
    GENERATED_BODY()

public:
    AStillLifePattern();

    // Optional custom mesh for cells in this pattern (for example, a Sphere from BasicShapes).
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cell Pattern")
    UStaticMesh* CustomCellMesh;

    virtual void ApplyPattern_Implementation(ACellularAutomataManager* Manager) override;
};
