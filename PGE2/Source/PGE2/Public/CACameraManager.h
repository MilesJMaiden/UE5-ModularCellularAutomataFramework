#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CACameraManager.generated.h"

UCLASS(Blueprintable)
class PGE2_API ACACameraManager : public AActor
{
    GENERATED_BODY()

public:
    ACACameraManager();

    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;

    // Camera component for level viewing.
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
    class UCameraComponent* CameraComponent;

    // Movement speed exposed to Blueprints.
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
    float MovementSpeed;

    // Zoom speed exposed to Blueprints.
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
    float ZoomSpeed;

    // Moves the camera in a specified direction.
    UFUNCTION(BlueprintCallable, Category = "Camera")
    void MoveCamera(FVector Direction, float DeltaTime);

    // Zooms the camera based on an input axis value.
    UFUNCTION(BlueprintCallable, Category = "Camera")
    void ZoomCamera(float AxisValue, float DeltaTime);
};
