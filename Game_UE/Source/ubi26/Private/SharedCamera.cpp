// Fill out your copyright notice in the Description page of Project Settings.


#include "SharedCamera.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/GameViewportClient.h"
#include "Engine/World.h"

ASharedCamera::ASharedCamera()
{
	PrimaryActorTick.bCanEverTick = true;

	// fix for the camera not spawning in the right place and breaking the screen when it does
	// this stops the level floor from overwriting the spring arm's angle and zoom when the camera spawns in
	USceneComponent* RootComp = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultRoot"));
	RootComponent = RootComp;

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);

	// fix : angle the camera down and stop the floor from breaking the zoom
	CameraBoom->bDoCollisionTest = false;
	CameraBoom->bUsePawnControlRotation = false;
	CameraBoom->SetRelativeRotation(FRotator(-50.0f, 0.0f, 0.0f));

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom);
}

void ASharedCamera::BeginPlay()
{
	Super::BeginPlay();

	// Force shared screen at the very start
	if (UGameViewportClient* Viewport = GetWorld()->GetGameViewport())
	{
		Viewport->SetForceDisableSplitscreen(true);
	}
}

void ASharedCamera::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	APlayerController* PC1 = UGameplayStatics::GetPlayerController(this, 0);
	APlayerController* PC2 = UGameplayStatics::GetPlayerController(this, 1);

	if (!PC1 || !PC2) return;

	// FIX 1: Continually grab the actual possessed bodies every frame!
	// This guarantees it never locks onto an invisible spectator spawn point.
	Player1 = PC1->GetPawn();
	Player2 = PC2->GetPawn();

	if (!Player1 || !Player2) return;

	float Distance = FVector::Dist(Player1->GetActorLocation(), Player2->GetActorLocation());

	if (Distance > SplitDistance)
	{
		if (!bIsSplitScreen || bFirstStart)
		{
			// Split screens
			if (UGameViewportClient* Viewport = GetWorld()->GetGameViewport())
			{
				Viewport->SetForceDisableSplitscreen(false);
			}
			PC1->SetViewTargetWithBlend(Player1, TransitionTime);
			PC2->SetViewTargetWithBlend(Player2, TransitionTime);

			bIsSplitScreen = true;
			bFirstStart = false;
		}
	}
	else
	{
		if (bIsSplitScreen || bFirstStart)
		{
			// Merge screens
			if (UGameViewportClient* Viewport = GetWorld()->GetGameViewport())
			{
				Viewport->SetForceDisableSplitscreen(true);
			}
			PC1->SetViewTargetWithBlend(this, TransitionTime);
			PC2->SetViewTargetWithBlend(this, TransitionTime);
			bIsSplitScreen = false;
			bFirstStart = false;
		}

		// Calculate Midpoint and Move
		FVector Midpoint = (Player1->GetActorLocation() + Player2->GetActorLocation()) / 2.0f;
		SetActorLocation(Midpoint);

		// Calculate Zoom
		FVector2D DistanceRange(0.0f, SplitDistance);
		FVector2D ZoomRange(MinZoom, MaxZoom);
		float TargetZoom = FMath::GetMappedRangeValueClamped(DistanceRange, ZoomRange, Distance);
		CameraBoom->TargetArmLength = FMath::FInterpTo(CameraBoom->TargetArmLength, TargetZoom, DeltaTime, ZoomSpeed);
	}
}
