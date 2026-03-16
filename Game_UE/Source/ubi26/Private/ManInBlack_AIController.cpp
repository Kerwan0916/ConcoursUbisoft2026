// Fill out your copyright notice in the Description page of Project Settings.


#include "ManInBlack_AIController.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Navigation/PathFollowingComponent.h"
#include "TimerManager.h"
#include "ManInBlack_Character.h"

AManInBlack_AIController::AManInBlack_AIController()
{
	AIPerception = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerception"));
	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("SightConfig"));

	// Set up the FOV Cone
	SightConfig->SightRadius = 1200.0f; // How far he can see
	SightConfig->LoseSightRadius = 1500.0f; // How far the alien has to run to escape
	SightConfig->PeripheralVisionAngleDegrees = 60.0f; // 120 degree total cone of vision
	SightConfig->DetectionByAffiliation.bDetectEnemies = true;
	SightConfig->DetectionByAffiliation.bDetectNeutrals = true;
	SightConfig->DetectionByAffiliation.bDetectFriendlies = true;

	AIPerception->ConfigureSense(*SightConfig);
	AIPerception->SetDominantSense(SightConfig->GetSenseImplementation());

	// Hook up our detection function
	AIPerception->OnTargetPerceptionUpdated.AddDynamic(this, &AManInBlack_AIController::OnTargetDetected);
}

void AManInBlack_AIController::BeginPlay()
{
	Super::BeginPlay();

	//MoveToNextPatrolPoint();
}

void AManInBlack_AIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	// Cast the pawn we just possessed to our custom C++ Character
	if (AManInBlack_Character* MIBCharacter = Cast<AManInBlack_Character>(InPawn))
	{
		// Grab the array from the character's body and give it to the brain
		PatrolPoints = MIBCharacter->PatrolRoute;

		// Kick off the patrol!
		MoveToNextPatrolPoint();
	}
}

void AManInBlack_AIController::OnTargetDetected(AActor* Actor, FAIStimulus Stimulus)
{
	if (Actor == nullptr)
	{
		return;
	}

	// Check if the thing he just saw has the "Alien" tag
	if (Actor->ActorHasTag("Alien"))
	{
		if (Stimulus.WasSuccessfullySensed())
		{
			// He sees the alien! Cancel the timer and chase it!
			GetWorld()->GetTimerManager().ClearTimer(WaitTimerHandle);

			TargetAlien = Actor;
			MoveToActor(TargetAlien, 100.0f);
		}
		else
		{
			// The alien ran out of his FOV. Go back to patrolling.
			TargetAlien = nullptr;
			MoveToNextPatrolPoint();
		}
	}
}

void AManInBlack_AIController::OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result)
{
	Super::OnMoveCompleted(RequestID, Result);

	// STACK OVERFLOW FIX: 
	// If he is chasing the alien, Unreal automatically tracks the moving target for us.
	// We don't need to loop it. If this fires while chasing, it means he either caught you or got stuck.
	if (TargetAlien != nullptr)
	{
		return;
	}

	// PATROL LOGIC (Only runs if he is NOT chasing an alien)
	if (PatrolPoints.Num() > 0)
	{
		// Only increment the patrol index if he successfully reached his destination
		if (Result.IsSuccess())
		{
			CurrentPatrolIndex = (CurrentPatrolIndex + 1) % PatrolPoints.Num();
		}

		// Start the stopwatch!
		GetWorld()->GetTimerManager().SetTimer(WaitTimerHandle, this, &AManInBlack_AIController::MoveToNextPatrolPoint, WaitTime, false);
	}
}

void AManInBlack_AIController::MoveToNextPatrolPoint()
{
	// This will show up in the top left of your game screen
	if (GEngine)
	{
		FString DebugMsg = FString::Printf(TEXT("Patrol Points Count: %d | Current Index: %d"), PatrolPoints.Num(), CurrentPatrolIndex);
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, DebugMsg);
	}

	if (TargetAlien == nullptr && PatrolPoints.Num() > 0)
	{
		// Safety check: is the specific point in the array valid?
		if (PatrolPoints[CurrentPatrolIndex])
		{
			MoveToActor(PatrolPoints[CurrentPatrolIndex], 50.0f);
		}
		else
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("ERROR: Patrol Point at Index is NULL!"));
		}
	}
}

