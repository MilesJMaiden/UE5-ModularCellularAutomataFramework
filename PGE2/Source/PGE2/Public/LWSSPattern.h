#pragma once

#include "CoreMinimal.h"
#include "CellPatternBase.h"
#include "LWSSPattern.generated.h"

/**
 * Represents a Lightweight Spaceship (LWSS) pattern.
 */
UCLASS(Blueprintable)
class PGE2_API ALWSSPattern : public ACellPatternBase
{
    GENERATED_BODY()

public:
    ALWSSPattern();

    virtual void ApplyPattern_Implementation(ACellularAutomataManager* Manager) override;
};
