#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CellPatternBase.generated.h"

UCLASS(Abstract, Blueprintable)
class PGE2_API ACellPatternBase : public AActor
{
    GENERATED_BODY()

public:
    ACellPatternBase();

    // BlueprintNativeEvent for applying a pattern.
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Cell Pattern")
    void ApplyPattern(class ACellularAutomataManager* Manager);
    virtual void ApplyPattern_Implementation(ACellularAutomataManager* Manager);
};
