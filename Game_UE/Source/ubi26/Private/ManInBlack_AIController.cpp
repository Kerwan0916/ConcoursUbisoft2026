// Fill out your copyright notice in the Description page of Project Settings.


#include "ManInBlack_AIController.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Navigation/PathFollowingComponent.h"
#include "TimerManager.h"
#include "ManInBlack_Character.h"
#include "Perception/AISense.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"

AManInBlack_AIController::AManInBlack_AIController()
{
	AIPerception = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerception"));
	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("SightConfig"));

	// Set up the FOV Cone
	SightConfig->SightRadius = 500.0f; // How far he can see
	SightConfig->LoseSightRadius = 700.0f; // How far the alien has to run to escape
	SightConfig->PeripheralVisionAngleDegrees = 60.0f; // 120 degree total cone of vision
	SightConfig->DetectionByAffiliation.bDetectEnemies = true;
	SightConfig->DetectionByAffiliation.bDetectNeutrals = true;
	SightConfig->DetectionByAffiliation.bDetectFriendlies = true;

	AIPerception->ConfigureSense(*SightConfig);
	AIPerception->SetDominantSense(SightConfig->GetSenseImplementation());

	// Hook up detection function
	AIPerception->OnTargetPerceptionUpdated.AddDynamic(this, &AManInBlack_AIController::OnTargetDetected);
}

void AManInBlack_AIController::BeginPlay()
{
	Super::BeginPlay();

	// start the 360 proximity radar to check for nearby aliens every 0.5 seconds
	GetWorld()->GetTimerManager().SetTimer(ProximityTimerHandle, this, &AManInBlack_AIController::CheckProximity, 0.5f, true);
}

void AManInBlack_AIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	// Cast the pawn we just possessed to our custom C++ Character
	if (AManInBlack_Character* MIBCharacter = Cast<AManInBlack_Character>(InPawn))
	{
		// Grab the array from the character's body and give it to the brain
		PatrolPoints = MIBCharacter->PatrolRoute;

		// Kick off the patrol
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
	if (Actor->ActorHasTag("Alien") && Stimulus.WasSuccessfullySensed())
	{
		// Cancel the patrol wait timer
		GetWorld()->GetTimerManager().ClearTimer(WaitTimerHandle);

		// If the chase loop isn't already running, start it (Runs every 0.2 seconds)
		if (!GetWorld()->GetTimerManager().IsTimerActive(ChaseTimerHandle))
		{
			GetWorld()->GetTimerManager().SetTimer(ChaseTimerHandle, this, &AManInBlack_AIController::UpdateChase, 0.2f, true);

			// Call it immediately the first time so there is no delay
			UpdateChase();
		}

		// previous implementation
		//if (Stimulus.WasSuccessfullySensed())
		//{
		//	// He sees the alien, chases
		//	GetWorld()->GetTimerManager().ClearTimer(WaitTimerHandle);

		//	TargetAlien = Actor;
		//	MoveToActor(TargetAlien, 100.0f);
		//}
		//else
		//{
		//	// The alien ran out of his FOV, back to patrolling.
		//	TargetAlien = nullptr;
		//	MoveToNextPatrolPoint();
		//}
	}
}

void AManInBlack_AIController::OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result)
{
	Super::OnMoveCompleted(RequestID, Result);

	// STACK OVERFLOW FIX: 
	// If he is chasing the alien Unreal automatically tracks the moving target
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

		// Start the stopwatch
		GetWorld()->GetTimerManager().SetTimer(WaitTimerHandle, this, &AManInBlack_AIController::MoveToNextPatrolPoint, WaitTime, false);
	}
}

void AManInBlack_AIController::MoveToNextPatrolPoint()
{
	// This will show up in the top left of the game screen
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
			// Set speed to walking
			if (ACharacter* MIBCharacter = Cast<ACharacter>(GetPawn()))
			{
				MIBCharacter->GetCharacterMovement()->MaxWalkSpeed = PatrolSpeed;
			}
			MoveToActor(PatrolPoints[CurrentPatrolIndex], 50.0f);
		}
		else
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("ERROR: Patrol Point at Index is NULL!"));
		}
	}
}

void AManInBlack_AIController::UpdateChase()
{
	// Prevent crash if the AI temporarily loses its pawn or hasn't fully possessed it
	APawn* ControlledPawn = GetPawn();
	if (!ControlledPawn)
	{
		return;
	}

	// Safety check to ensure the perception component exists
	if (!AIPerception)
	{
		return;
	}

	TArray<AActor*> PerceivedActors;

	// Use the static class instead of the SightConfig pointer for no dead memory access if the config gets destroyed
	AIPerception->GetCurrentlyPerceivedActors(UAISense_Sight::StaticClass(), PerceivedActors);

	AActor* ClosestAlien = nullptr;
	float ShortestDistance = 999999.0f; // Start with a massive number

	// Loop through everything he sees
	for (AActor* PerceivedActor : PerceivedActors)
	{
		if (PerceivedActor && PerceivedActor->ActorHasTag("Alien"))
		{
			// Calculate the distance using our safe ControlledPawn reference
			float Distance = FVector::Dist(ControlledPawn->GetActorLocation(), PerceivedActor->GetActorLocation());

			// did he catch the alien? If he's within the catch distance, call the catch function and skip the rest of the chase logic
			if (!bIsCatchOnCooldown && Distance <= CatchDistance)
			{
				CatchAlien(PerceivedActor);
				return; // stop the chase logic here, we don't want him to keep chasing while he's in the middle of the catch sequence
			}

			if (Distance < ShortestDistance)
			{
				ShortestDistance = Distance;
				ClosestAlien = PerceivedActor;
			}
		}
	}

	// Did we find an alien
	if (ClosestAlien != nullptr)
	{
		TargetAlien = ClosestAlien;

		// Set speed to sprinting!
		if (ACharacter* MIBCharacter = Cast<ACharacter>(ControlledPawn))
		{
			MIBCharacter->GetCharacterMovement()->MaxWalkSpeed = ChaseSpeed;
		}

		MoveToActor(TargetAlien, 10.0f);
	}
	else
	{
		// if he loses sight of alien but alien is still in the array, it means the alien is just out of his FOV but still close. 
		// In this case, we want him to keep chasing the last known location of the alien until he reaches it or sees it again.
		if (TargetAlien != nullptr)
		{
			float DistToOldTarget = FVector::Dist(ControlledPawn->GetActorLocation(), TargetAlien->GetActorLocation());

			// if the alien is within 600 units, man in black spins and keeps chasing
			if (DistToOldTarget < 600.0f)
			{
				MoveToActor(TargetAlien, 10.0f);
				return; // stop here so the patrol logic doesn't kick in and change his destination while he's still chasing the last known location of the alien
			}
		}
		// The array was empty so all aliens escaped his vision.
		// Clear the target, kill the chase loop, and resume patrol
		TargetAlien = nullptr;
		GetWorld()->GetTimerManager().ClearTimer(ChaseTimerHandle);

		// Find the closest patrol point to his current location 
		if (PatrolPoints.Num() > 0 && GetPawn() != nullptr)
		{
			float ShortestPatrolDist = 999999.0f;
			int32 ClosestIndex = CurrentPatrolIndex;
			FVector MyLocation = GetPawn()->GetActorLocation();

			for (int32 i = 0; i < PatrolPoints.Num(); i++)
			{
				if (PatrolPoints[i])
				{
					float Dist = FVector::Dist(MyLocation, PatrolPoints[i]->GetActorLocation());

					// If this point is closer than the others, save it
					if (Dist < ShortestPatrolDist)
					{
						ShortestPatrolDist = Dist;
						ClosestIndex = i;
					}
				}
			}

			// Update his brain to target the closest point we just found
			CurrentPatrolIndex = ClosestIndex;
		}
		// Resume patrol heading to the new closest point
		MoveToNextPatrolPoint();
	}
}

void AManInBlack_AIController::CatchAlien(AActor* CaughtAlien)
{
	if (!CaughtAlien) return;

	// Stop AI Chase & Trigger Cooldown
	GetWorld()->GetTimerManager().ClearTimer(ChaseTimerHandle);
	TargetAlien = nullptr;
	StopMovement();
	CurrentlyCaughtAlien = CaughtAlien;

	bIsCatchOnCooldown = true;
	GetWorld()->GetTimerManager().SetTimer(CatchCooldownTimerHandle, this, &AManInBlack_AIController::ResetCatchCooldown, CatchCooldown, false);

	// Lock the player controls and trigger the camera fade
	if (APawn* CaughtPawn = Cast<APawn>(CaughtAlien))
	{
		if (APlayerController* PC = Cast<APlayerController>(CaughtPawn->GetController()))
		{
			CaughtPawn->DisableInput(PC); // The alien is now officially stuck

			if (PC->PlayerCameraManager)
			{
				// Fade to black over 1 second
				PC->PlayerCameraManager->StartCameraFade(0.0f, 1.0f, 1.0f, FLinearColor::Black, false, true);
			}
		}
	}

	// Swap the ID badges so the AI ignores this player from now on
	CaughtAlien->Tags.Remove("Alien");
	CaughtAlien->Tags.Add("Prisoner");

	// GAME OVER CHECK Are both players currently in prison?
	TArray<AActor*> Prisoners;
	UGameplayStatics::GetAllActorsWithTag(GetWorld(), "Prisoner", Prisoners);

	if (Prisoners.Num() >= 2)
	{
		// BOTH CAUGHT! Call the Blueprint event to show the UI Widget!
		OnGameOver();
		return;
	}

	// If the game isn't over, set a timer to teleport them 1 second later
	GetWorld()->GetTimerManager().SetTimer(CatchSequenceTimerHandle, this, &AManInBlack_AIController::FinishCatchSequence, 1.0f, false);
}

void AManInBlack_AIController::FinishCatchSequence()
{
	if (!CurrentlyCaughtAlien) return;

	// Dynamic Cell Matching 
	FName CellTagToFind = NAME_None;

	// Check the alien's ID tags to figure out which incubator they belong in
	if (CurrentlyCaughtAlien->ActorHasTag("GreenAlien"))
	{
		CellTagToFind = "GreenCell";
	}
	else if (CurrentlyCaughtAlien->ActorHasTag("PurpleAlien"))
	{
		CellTagToFind = "PurpleCell";
	}
	else
	{
		// Fallback just in case someone forgot to add the tags!
		CellTagToFind = "PrisonCell";
	}

	// Find the specific location matching the alien's color
	TArray<AActor*> PrisonCells;
	UGameplayStatics::GetAllActorsWithTag(GetWorld(), CellTagToFind, PrisonCells);

	// Teleport the alien to their specific incubator
	if (PrisonCells.Num() > 0)
	{
		CurrentlyCaughtAlien->SetActorLocation(PrisonCells[0]->GetActorLocation());
	}

	// Trigger the White Fade-In
	if (APawn* CaughtPawn = Cast<APawn>(CurrentlyCaughtAlien))
	{
		if (APlayerController* PC = Cast<APlayerController>(CaughtPawn->GetController()))
		{
			if (PC->PlayerCameraManager)
			{
				PC->PlayerCameraManager->StartCameraFade(1.0f, 0.0f, 1.0f, FLinearColor::White, false, false);
			}
		}
	}

	// Clean up and tell the Man in Black to go back to patrolling
	CurrentlyCaughtAlien = nullptr;
	MoveToNextPatrolPoint();
}

void AManInBlack_AIController::ResetCatchCooldown()
{
	bIsCatchOnCooldown = false;
}

void AManInBlack_AIController::CheckProximity()
{
	// don't bother scanning if he's already chasing an alien
	if (TargetAlien != nullptr || GetPawn() == nullptr) return;

	// grab all free aliens on the map
	TArray<AActor*> Aliens;
	UGameplayStatics::GetAllActorsWithTag(GetWorld(), "Alien", Aliens);

	// measure distance to each alien
	for (AActor* Alien : Aliens)
	{
		if (Alien)
		{
			float Distance = FVector::Dist(GetPawn()->GetActorLocation(), Alien->GetActorLocation());

			// if any are within the proximity radius
			if (Distance <= ProximityRadius)
			{
				// lock onto the sneaky alien and start chasing
				TargetAlien = Alien;
				GetWorld()->GetTimerManager().ClearTimer(WaitTimerHandle); // stop patrolling
				if (!GetWorld()->GetTimerManager().IsTimerActive(ChaseTimerHandle))
				{
					GetWorld()->GetTimerManager().SetTimer(ChaseTimerHandle, this, &AManInBlack_AIController::UpdateChase, 0.2f, true);
					UpdateChase();
				}
				return; // stop checking other aliens, we only want to chase one at a time
			}
		}
	}
}

