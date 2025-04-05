#pragma once

#include "CoreMinimal.h"
#include "CellPatternBase.h"
#include "BeehivePattern.generated.h"

/**
 * Represents a Beehive pattern still life.
 */
UCLASS(Blueprintable)
class PGE2_API ABeehivePattern : public ACellPatternBase
{
    GENERATED_BODY()

public:
    ABeehivePattern();

    virtual void ApplyPattern_Implementation(ACellularAutomataManager* Manager) override;
};
