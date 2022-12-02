// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "TBSCameraPawn.generated.h"

class UCameraComponent;
class USpringArmComponent;
class USceneComponent;

UCLASS()
class TURNBASESTRATEGY_API ATBSCameraPawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ATBSCameraPawn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Camera)
	UCameraComponent* CameraComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Camera)
	USpringArmComponent* SpringArmComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Camera)
	USceneComponent* SceneComponent;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Camera, meta = (ClampMin="0", ClampMax="100", AllowPrivateAccess=true))
	float MoveSpeedCamera = 0.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Camera, meta = (ClampMin="0", ClampMax="100", AllowPrivateAccess=true))
	float SpeedZoomCamera = 0.0f;

	FVector NextCameraLocation;
	float NextCameraZoomPosition = 0.0f;

	float ThisDeltaTime = 0.0f;
	bool IsCameraRotate = false;
	bool IsMoveCameraMouse = false;
	char DirectionCameraMove;

	void MoveCameraHorizontal(float Amount);
	void MoveCameraVertical(float Amount);
	void CameraZoom(float Amount);
	void StartCameraRotate();
	void StopCameraRotate();
	void TurnCamera(float Amount);
	void MouseCameraHorizontalDirection(float Amount);
	void MouseCameraVerticalDirection(float Amount);
	void MouseCameraMove();

	FVector2D GetMousePosition();
	FVector2D GetGameViewportSize();

};
