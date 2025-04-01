#pragma once

#include "CoreMinimal.h"
#include "CellPatternBase.h"
#include "StillLifePattern.generated.h"

UCLASS(Blueprintable)
class PGE2_API AStillLifePattern : public ACellPatternBase
{
    GENERATED_BODY()

public:
    AStillLifePattern();

    virtual void ApplyPattern_Implementation(ACellularAutomataManager* Manager) override;
};
