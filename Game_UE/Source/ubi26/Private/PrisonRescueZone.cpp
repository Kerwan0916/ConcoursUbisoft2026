// Fill out your copyright notice in the Description page of Project Settings.


#include "PrisonRescueZone.h"
#include "Components/BoxComponent.h"
#include "Engine/Engine.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/PlayerController.h"

APrisonRescueZone::APrisonRescueZone()
{
	PrimaryActorTick.bCanEverTick = true;

	// Create the box and make it the root of the actor
	RescueBox = CreateDefaultSubobject<UBoxComponent>(TEXT("RescueBox"));
	RootComponent = RescueBox;

	// Set it to a nice 600x600x200 unit box
	RescueBox->SetBoxExtent(FVector(300.0f, 300.0f, 100.0f));
	RescueBox->SetCollisionProfileName(TEXT("Trigger"));

	// Hook up the overlap functions
	RescueBox->OnComponentBeginOverlap.AddDynamic(this, &APrisonRescueZone::OnOverlapBegin);
	RescueBox->OnComponentEndOverlap.AddDynamic(this, &APrisonRescueZone::OnOverlapEnd);
}

void APrisonRescueZone::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	// scan the controller of anyone standing in the box
	for (auto& [Id, PC] : RescuerInZone)
	{
		if (IsValid(PC) && (PC->WasInputKeyJustPressed(EKeys::E) || PC->WasInputKeyJustPressed(EKeys::Gamepad_FaceButton_Right)))
		{
			ExecuteRescue();
			break;
		}
	}
}

void APrisonRescueZone::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// Only care if a FREE alien walks into the box
	if (OtherActor && OtherActor->ActorHasTag("Alien"))
	{
		if (APawn* AlienPawn = Cast<APawn>(OtherActor))
		{
			if (APlayerController* PC = Cast<APlayerController>(AlienPawn->GetController()))
			{
				// Add them to our tracking array
				RescuerInZone.Add(PC->GetUniqueID(), PC);
			}
		}
	}
}

void APrisonRescueZone::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor && OtherActor->ActorHasTag("Alien"))
	{
		if (APawn* AlienPawn = Cast<APawn>(OtherActor))
		{
			if (APlayerController* PC = Cast<APlayerController>(AlienPawn->GetController()))
			{
				// They left the box, stop tracking them
				RescuerInZone.Remove(PC->GetUniqueID());
			}
		}
	}
}

void APrisonRescueZone::ExecuteRescue()
{
	TArray<AActor*> Prisoners;
	UGameplayStatics::GetAllActorsWithTag(GetWorld(), "Prisoner", Prisoners);

	for (AActor* PrisonerActor : Prisoners)
	{
		if (PrisonerActor)
		{
			// 1. Swap the ID badges back
			PrisonerActor->Tags.Remove("Prisoner");
			PrisonerActor->Tags.Add("Alien");

			// 2. Give them their controls back
			if (APawn* PrisonerPawn = Cast<APawn>(PrisonerActor))
			{
				if (APlayerController* PC = Cast<APlayerController>(PrisonerPawn->GetController()))
				{
					PrisonerPawn->EnableInput(PC);

					// 3. Tell the blueprint
					OnAlienRescued(PrisonerActor);
				}
			}
		}
	}

	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, TEXT("JAILBREAK SUCCESSFUL!"));
	}
}

