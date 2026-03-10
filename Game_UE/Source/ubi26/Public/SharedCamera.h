// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SharedCamera.generated.h"

class USpringArmComponent;
class UCameraComponent;

UCLASS()
class ASharedCamera : public AActor
{
	GENERATED_BODY()

public:
	ASharedCamera();
	virtual void Tick(float DeltaTime) override;

	// Variables for designers to tweak in the editor
	UPROPERTY(EditAnywhere, Category = "Coop Camera Settings")
	float SplitDistance = 700.0f;

	UPROPERTY(EditAnywhere, Category = "Coop Camera Settings")
	float MinZoom = 700.0f;

	UPROPERTY(EditAnywhere, Category = "Coop Camera Settings")
	float MaxZoom = 1300.0f;

	UPROPERTY(EditAnywhere, Category = "Coop Camera Settings")
	float ZoomSpeed = 2.0f;

	UPROPERTY(EditAnywhere, Category = "Coop Camera Settings")
	float TransitionTime = 0.5f; 

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UCameraComponent* FollowCamera;

private:
	// Tracks if the screen is currently split so we don't spam the command
	bool bIsSplitScreen = false;
	bool bFirstStart = true;

	AActor* Player1 = nullptr;
	AActor* Player2 = nullptr;
};