#pragma once

#include "CoreMinimal.h"
#include "CellPatternBase.h"
#include "GliderPattern.generated.h"

/**
 * Represents a glider Spaceship pattern.
 */
UCLASS(Blueprintable)
class PGE2_API AGliderPattern : public ACellPatternBase
{
    GENERATED_BODY()

public:
    AGliderPattern();

    virtual void ApplyPattern_Implementation(ACellularAutomataManager* Manager) override;
};
