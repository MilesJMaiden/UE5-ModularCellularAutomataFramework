#include "CellPatternBase.h"
#include "CellularAutomataManager.h"

ACellPatternBase::ACellPatternBase()
{
    PrimaryActorTick.bCanEverTick = false;
}

void ACellPatternBase::ApplyPattern_Implementation(ACellularAutomataManager* Manager)
{
    // Base implementation does nothing.
    // Derived classes will override this to apply specific patterns.
}
