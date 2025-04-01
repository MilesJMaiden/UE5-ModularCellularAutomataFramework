#include "CACameraManager.h"
#include "Camera/CameraComponent.h"

ACACameraManager::ACACameraManager()
{
    PrimaryActorTick.bCanEverTick = true;

    // Create and attach the camera component.
    CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
    RootComponent = CameraComponent;

    MovementSpeed = 600.0f;
    ZoomSpeed = 200.0f;
}

void ACACameraManager::BeginPlay()
{
    Super::BeginPlay();
}

void ACACameraManager::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void ACACameraManager::MoveCamera(FVector Direction, float DeltaTime)
{
    AddActorWorldOffset(Direction * MovementSpeed * DeltaTime, true);
}

void ACACameraManager::ZoomCamera(float AxisValue, float DeltaTime)
{
    FVector ZoomMovement = CameraComponent->GetForwardVector() * AxisValue * ZoomSpeed * DeltaTime;
    AddActorWorldOffset(ZoomMovement, true);
}
