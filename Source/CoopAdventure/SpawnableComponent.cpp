// Fill out your copyright notice in the Description page of Project Settings.


#include "SpawnableComponent.h"
#include "CollectableKey.h"
#include "PressurePlate.h"

// Sets default values for this component's properties
USpawnableComponent::USpawnableComponent()
{
	SpawnLocation = GetOwner()->GetActorLocation();
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void USpawnableComponent::BeginPlay()
{
	Super::BeginPlay();

	for(AActor* TA : TriggerActors)
	{
		APressurePlate* PressurePlateActor = Cast<APressurePlate>(TA);
		if(PressurePlateActor)
		{
			PressurePlateActor->OnActivated.AddDynamic(this, &USpawnableComponent::OnTriggerActorActivated);
			continue;
		}

		ACollectableKey* KeyActor = Cast<ACollectableKey>(TA);
		if(KeyActor)
		{
			KeyActor->OnCollected.AddDynamic(this, &USpawnableComponent::OnTriggerActorActivated);
		}
	}

	if(GetOwner()->HasAuthority())
	{
		GetOwner()->SetActorEnableCollision(false);
		GetOwner()->SetActorHiddenInGame(true);
	}
}


// Called every frame
void USpawnableComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void USpawnableComponent::OnTriggerActorActivated()
{
	if(GetOwner()->HasAuthority())
	{
		GetOwner()->SetActorEnableCollision(true);
		GetOwner()->SetActorHiddenInGame(false);
		GetOwner()->SetActorLocation(SpawnLocation);
	}
}

