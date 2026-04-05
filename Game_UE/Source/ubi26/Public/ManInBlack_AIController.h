// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Perception/AIPerceptionTypes.h"
#include "ManInBlack_AIController.generated.h"

class UAIPerceptionComponent;
class UAISenseConfig_Sight;

/**
 * 
 */
UCLASS()
class UBI26_API AManInBlack_AIController : public AAIController
{
	GENERATED_BODY()
	
public:
	AManInBlack_AIController();

protected:
	virtual void BeginPlay() override;
	virtual void OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result) override;
	virtual void OnPossess(APawn* InPawn) override;

	// the function that fires when the FOV detects a target
	UFUNCTION()
	void OnTargetDetected(AActor* Actor, FAIStimulus Stimulus);

	UFUNCTION()
	void UpdateChase();

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UAIPerceptionComponent* AIPerception;

	UPROPERTY()
	UAISenseConfig_Sight* SightConfig;

	// drop target points here for patrol route
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI Patrol")
	TArray<AActor*> PatrolPoints;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI Patrol")
	float WaitTime = 2.0f; // Time to wait at each patrol point

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI Patrol")
	float PatrolSpeed = 200.0f; // Speed to move between patrol points

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI Patrol")
	float ChaseSpeed = 300.0f; // Speed to chase the alien

	// AI Catch Settings
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI Catch")
	float CatchDistance = 100.0f; // Distance at which the AI will "catch" the alien

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI Catch")
	float CatchCooldown = 3.0f; // Time after catching before the AI can catch again

	UFUNCTION(BlueprintImplementableEvent, Category = "AI Catch")
	void OnGameOver();

	UFUNCTION(BlueprintImplementableEvent, Category = "AI Events")
	void OnAlienDetected(AActor* DetectedAlien);

	UFUNCTION(BlueprintImplementableEvent, Category = "AI Events")
	void OnAlienCaught(AActor* CaughtAlien);

	void CatchAlien(AActor* CaughtAlien);
	void FinishCatchSequence();
	void ResetCatchCooldown();

	// AI Proximity Radar
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI Patrol")
	float ProximityRadius = 250.0f; // Radius for the proximity radar 


	UFUNCTION(BlueprintCallable, Category = "AI Patrol")
	void MoveToNextPatrolPoint();

private:
	AActor* TargetAlien = nullptr;
	int32 CurrentPatrolIndex = 0;

	FTimerHandle WaitTimerHandle;

	FTimerHandle ChaseTimerHandle;

	FTimerHandle CatchSequenceTimerHandle;
	FTimerHandle CatchCooldownTimerHandle;
	AActor* CurrentlyCaughtAlien = nullptr;
	bool bIsCatchOnCooldown = false;

	FTimerHandle ProximityTimerHandle;

	UFUNCTION()
	void CheckProximity();
};
