#pragma once

#include "CoreMinimal.h"
#include "CellPatternBase.h"
#include "LoafPattern.generated.h"

/**
 * Represents a Loaf pattern still life.
 */
UCLASS(Blueprintable)
class PGE2_API ALoafPattern : public ACellPatternBase
{
    GENERATED_BODY()

public:
    ALoafPattern();

    virtual void ApplyPattern_Implementation(ACellularAutomataManager* Manager) override;
};
