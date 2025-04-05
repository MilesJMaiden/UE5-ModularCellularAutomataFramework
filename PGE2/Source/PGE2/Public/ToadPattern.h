#pragma once

#include "CoreMinimal.h"
#include "CellPatternBase.h"
#include "ToadPattern.generated.h"

/**
 * Represents a Toad oscillator pattern.
 */
UCLASS(Blueprintable)
class PGE2_API AToadPattern : public ACellPatternBase
{
    GENERATED_BODY()

public:
    AToadPattern();

    virtual void ApplyPattern_Implementation(ACellularAutomataManager* Manager) override;
};
