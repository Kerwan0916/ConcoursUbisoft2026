// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PrisonRescueZone.generated.h"

class UBoxComponent;

UCLASS()
class UBI26_API APrisonRescueZone : public AActor
{
	GENERATED_BODY()
	
public:	
	APrisonRescueZone();

protected:
	// the physical zone the player must be in to rescue the prisoner
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Rescue")
	UBoxComponent* RescueBox;

	// Standard overlap events
	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	// The function that runs when 'E' is pressed
	void ExecuteRescue();
};
