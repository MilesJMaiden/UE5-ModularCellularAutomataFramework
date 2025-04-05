#pragma once

#include "CoreMinimal.h"
#include "CellPatternBase.h"
#include "BeaconPattern.generated.h"

/**
 * Represents a Beacon oscillator pattern.
 */
UCLASS(Blueprintable)
class PGE2_API ABeaconPattern : public ACellPatternBase
{
    GENERATED_BODY()

public:
    ABeaconPattern();

    virtual void ApplyPattern_Implementation(ACellularAutomataManager* Manager) override;
};
