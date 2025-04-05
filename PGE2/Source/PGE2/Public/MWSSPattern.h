#pragma once

#include "CoreMinimal.h"
#include "CellPatternBase.h"
#include "MWSSPattern.generated.h"

/**
 * Represents a Middleweight Spaceship (MWSS) pattern.
 */
UCLASS(Blueprintable)
class PGE2_API AMWSSPattern : public ACellPatternBase
{
    GENERATED_BODY()

public:
    AMWSSPattern();

    virtual void ApplyPattern_Implementation(ACellularAutomataManager* Manager) override;
};
