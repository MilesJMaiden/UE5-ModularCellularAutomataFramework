#pragma once

#include "CoreMinimal.h"
#include "CellPatternBase.h"
#include "SpaceshipPattern.generated.h"

UCLASS(Blueprintable)
class PGE2_API ASpaceshipPattern : public ACellPatternBase
{
    GENERATED_BODY()

public:
    ASpaceshipPattern();

    virtual void ApplyPattern_Implementation(ACellularAutomataManager* Manager) override;
};
