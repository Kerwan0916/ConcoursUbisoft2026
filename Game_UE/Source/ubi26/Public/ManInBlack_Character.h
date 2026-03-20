// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ManInBlack_Character.generated.h"

UCLASS()
class UBI26_API AManInBlack_Character : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AManInBlack_Character();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// We use EditInstanceOnly so you can assign points to individual guards in your level!
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "AI Patrol")
	TArray<AActor*> PatrolRoute;
};