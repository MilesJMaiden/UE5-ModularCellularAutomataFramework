#pragma once

#include "CoreMinimal.h"
#include "CellPatternBase.h"
#include "BlockPattern.generated.h"

/**
 * Represents a block Still Life pattern.
 */
UCLASS(Blueprintable)
class PGE2_API ABlockPattern : public ACellPatternBase
{
    GENERATED_BODY()

public:
    ABlockPattern();

    virtual void ApplyPattern_Implementation(ACellularAutomataManager* Manager) override;
};
