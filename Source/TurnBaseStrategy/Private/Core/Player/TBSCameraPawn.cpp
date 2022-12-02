// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/Player/TBSCameraPawn.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/SceneComponent.h"

// Sets default values
ATBSCameraPawn::ATBSCameraPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SceneComponent = CreateDefaultSubobject<UStaticMeshComponent>("DefaultSceneRootComponent");

	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>("SprintArmComponent");
	SpringArmComponent->TargetArmLength = 700.0f;
	SpringArmComponent->SetupAttachment(SceneComponent);

	CameraComponent = CreateDefaultSubobject<UCameraComponent>("CameraComponent");
	CameraComponent->SetupAttachment(SpringArmComponent);

	bUseControllerRotationYaw = true;

}

// Called when the game starts or when spawned
void ATBSCameraPawn::BeginPlay()
{
	Super::BeginPlay();

	NextCameraLocation = GetActorLocation();
	NextCameraZoomPosition = SpringArmComponent->TargetArmLength;
	
}

// Called every frame
void ATBSCameraPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (IsMoveCameraMouse)
	{
		MouseCameraMove();
	}

}

// Called to bind functionality to input
void ATBSCameraPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAxis("MoveCameraHorizontal", this, &ATBSCameraPawn::MoveCameraHorizontal);
	PlayerInputComponent->BindAxis("MoveCameraVertical", this, &ATBSCameraPawn::MoveCameraVertical);
	PlayerInputComponent->BindAxis("CameraZoom", this, &ATBSCameraPawn::CameraZoom);
	PlayerInputComponent->BindAxis("TurnCamera", this, &ATBSCameraPawn::TurnCamera);
	PlayerInputComponent->BindAction("CameraRotate", IE_Pressed, this, &ATBSCameraPawn::StartCameraRotate);
	PlayerInputComponent->BindAction("CameraRotate", IE_Released, this, &ATBSCameraPawn::StopCameraRotate);
	PlayerInputComponent->BindAxis("MouseMoveCameraHorizontal", this, &ATBSCameraPawn::MouseCameraHorizontalDirection);
	PlayerInputComponent->BindAxis("MouseMoveCameraVertical", this, &ATBSCameraPawn::MouseCameraVerticalDirection);

}

void ATBSCameraPawn::MoveCameraHorizontal(float Amount)
{
	ThisDeltaTime = GetWorld()->GetDeltaSeconds();
	NextCameraLocation = Amount * MoveSpeedCamera * GetActorRightVector() + NextCameraLocation;
	const FVector NewCameraLocation = FMath::VInterpTo(GetActorLocation(), NextCameraLocation, ThisDeltaTime, 2);
	SetActorLocation(NewCameraLocation);
}

void ATBSCameraPawn::MoveCameraVertical(float Amount)
{
	ThisDeltaTime = GetWorld()->GetDeltaSeconds();
	NextCameraLocation = Amount * MoveSpeedCamera * GetActorForwardVector() + NextCameraLocation;
	const FVector NewCameraLocation = FMath::VInterpTo(GetActorLocation(), NextCameraLocation, ThisDeltaTime, 2);
	SetActorLocation(NewCameraLocation);
}

void ATBSCameraPawn::CameraZoom(float Amount)
{
	ThisDeltaTime = GetWorld()->GetDeltaSeconds();
	NextCameraZoomPosition = Amount * SpeedZoomCamera * SpringArmComponent->TargetArmLength;
	const float NewCameraZoomPosition = SpringArmComponent->TargetArmLength + NextCameraZoomPosition;
	SpringArmComponent->TargetArmLength = FMath::FInterpTo(SpringArmComponent->TargetArmLength, FMath::Clamp(NewCameraZoomPosition, 300.f, 1000.f),
											ThisDeltaTime, 10);
}

void ATBSCameraPawn::StartCameraRotate()
{
	IsCameraRotate = true;
}

void ATBSCameraPawn::StopCameraRotate()
{
	IsCameraRotate = false;
}

void ATBSCameraPawn::TurnCamera(float Amount)
{
	if (IsCameraRotate)
	{
		AddControllerYawInput(Amount);
	}
}

void ATBSCameraPawn::MouseCameraHorizontalDirection(float Amount)
{
	if (!IsCameraRotate)
	{
		const FVector2D MousePosition = GetMousePosition();
		if (MousePosition.X / GetGameViewportSize().X >= 0.975f)
		{
			IsMoveCameraMouse = true;
			DirectionCameraMove = 'R';
		} else if (MousePosition.X / GetGameViewportSize().X <= 0.025f)
		{
			IsMoveCameraMouse = true;
			DirectionCameraMove = 'L';
		}
		else
		{
			IsMoveCameraMouse = false;
		}
	}
}

void ATBSCameraPawn::MouseCameraVerticalDirection(float Amount)
{
	if (!IsCameraRotate)
	{
		const FVector2D MousePosition = GetMousePosition();
		if (MousePosition.Y / GetGameViewportSize().Y >= 0.975f)
		{
			IsMoveCameraMouse = true;
			DirectionCameraMove = 'B';
		} else if (MousePosition.Y / GetGameViewportSize().Y <= 0.025f)
		{
			IsMoveCameraMouse = true;
			DirectionCameraMove = 'F';
		}
	
	}
}

void ATBSCameraPawn::MouseCameraMove()
{
	switch (DirectionCameraMove)
	{
	case 'F':
		MoveCameraVertical(1.0f);
		break;
	case 'B':
		MoveCameraVertical(-1.0f);
		break;
	case 'R':
		MoveCameraHorizontal(1.0f);
		break;
	case 'L':
		MoveCameraHorizontal(-1.0f);
		break;
	default:
		break;
	}
}

FVector2D ATBSCameraPawn::GetMousePosition()
{
	FVector2D MousePosition;
	if (GetWorld())
	{
		GetWorld()->GetGameViewport()->GetMousePosition(MousePosition);
	}
	return MousePosition;
}

FVector2D ATBSCameraPawn::GetGameViewportSize()
{
	FVector2D ViewportSize;
	if (GEngine && GEngine->GameViewport)
	{
		GEngine->GameViewport->GetViewportSize(ViewportSize);
	}

	return ViewportSize;
}

