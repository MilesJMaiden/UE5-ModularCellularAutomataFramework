#pragma once

#include "CoreMinimal.h"
#include "CellPatternBase.h"
#include "OscillatorPattern.generated.h"

/**
 * Represents an Oscillator pattern (e.g., a blinker).
 */
UCLASS(Blueprintable)
class PGE2_API AOscillatorPattern : public ACellPatternBase
{
    GENERATED_BODY()

public:
    AOscillatorPattern();

    // Override to apply an Oscillator pattern.
    virtual void ApplyPattern_Implementation(ACellularAutomataManager* Manager) override;
};
