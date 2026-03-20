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


	UFUNCTION(BlueprintCallable, Category = "AI Patrol")
	void MoveToNextPatrolPoint();

private:
	AActor* TargetAlien = nullptr;
	int32 CurrentPatrolIndex = 0;

	FTimerHandle WaitTimerHandle;

	FTimerHandle ChaseTimerHandle;
};
