#pragma once

#include "CoreMinimal.h"
#include "CellPatternBase.h"
#include "BlinkerPattern.generated.h"

/**
 * Represents an Blinker pattern.
 */
UCLASS(Blueprintable)
class PGE2_API ABlinkerPattern : public ACellPatternBase
{
    GENERATED_BODY()

public:
    ABlinkerPattern();

    virtual void ApplyPattern_Implementation(ACellularAutomataManager* Manager) override;
};
