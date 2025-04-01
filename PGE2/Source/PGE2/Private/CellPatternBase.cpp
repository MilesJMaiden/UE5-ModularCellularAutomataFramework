#include "CellPatternBase.h"
#include "CellularAutomataManager.h"

ACellPatternBase::ACellPatternBase()
{
    PrimaryActorTick.bCanEverTick = false;
}

void ACellPatternBase::ApplyPattern_Implementation(ACellularAutomataManager* Manager)
{
}
