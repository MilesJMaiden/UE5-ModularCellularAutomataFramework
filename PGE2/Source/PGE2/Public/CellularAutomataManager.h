#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CellularAutomataManager.generated.h"

UCLASS(Blueprintable)
class PGE2_API ACellularAutomataManager : public AActor
{
    GENERATED_BODY()

public:
    ACellularAutomataManager();

    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;

    // Grid dimensions for the cellular automata
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cellular Automata")
    int32 GridWidth;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cellular Automata")
    int32 GridHeight;

    // Time between simulation steps
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cellular Automata")
    float TimeStepInterval;

    // Mesh used to visualize each cell (assign a Cube mesh in the editor)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cellular Automata")
    UStaticMesh* CellMesh;

    // Initializes the grid with all dead cells
    UFUNCTION(BlueprintCallable, Category = "Cellular Automata")
    void InitializeGrid();

    // Updates the simulation state (using Game of Life rules)
    UFUNCTION(BlueprintCallable, Category = "Cellular Automata")
    void UpdateSimulation();

    // Applies a pattern (Still Life, Oscillator, or Spaceship) at a specified origin.
    UFUNCTION(BlueprintCallable, Category = "Cellular Automata")
    void ApplyPattern(TSubclassOf<class ACellPatternBase> PatternClass, FVector Origin);

    // The grid storing the cell states (0 = dead, 1 = alive)
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Cellular Automata")
    TArray<TArray<int32>> CellGrid;

protected:
    float TimeAccumulator;

    // Helper function to spawn a visual cell at the grid coordinates.
    void SpawnCell(int32 X, int32 Y, bool bIsAlive);
};
