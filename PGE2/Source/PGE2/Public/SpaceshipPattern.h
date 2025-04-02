#pragma once

#include "CoreMinimal.h"
#include "CellPatternBase.h"
#include "SpaceshipPattern.generated.h"

/**
 * Represents a Spaceship pattern (e.g., a glider).
 */
UCLASS(Blueprintable)
class PGE2_API ASpaceshipPattern : public ACellPatternBase
{
    GENERATED_BODY()

public:
    ASpaceshipPattern();

    virtual void ApplyPattern_Implementation(ACellularAutomataManager* Manager) override;
};
