#pragma once

#include "CoreMinimal.h"
#include "CellPatternBase.h"
#include "StillLifePattern.generated.h"

/**
 * Represents a Still Life pattern (e.g., a 2x2 block).
 */
UCLASS(Blueprintable)
class PGE2_API AStillLifePattern : public ACellPatternBase
{
    GENERATED_BODY()

public:
    AStillLifePattern();

    virtual void ApplyPattern_Implementation(ACellularAutomataManager* Manager) override;
};
