#pragma once

#include "CoreMinimal.h"
#include "CellPatternBase.h"
#include "StillLifePattern.generated.h"

/**
 * Represents a Still Life pattern (e.g., a block).
 */
UCLASS(Blueprintable)
class PGE2_API AStillLifePattern : public ACellPatternBase
{
    GENERATED_BODY()

public:
    AStillLifePattern();

    // Override to apply a Still Life pattern.
    virtual void ApplyPattern_Implementation(ACellularAutomataManager* Manager) override;
};
