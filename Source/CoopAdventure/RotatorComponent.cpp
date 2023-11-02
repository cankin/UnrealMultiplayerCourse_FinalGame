// Fill out your copyright notice in the Description page of Project Settings.


#include "RotatorComponent.h"

// Sets default values for this component's properties
URotatorComponent::URotatorComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	bCanSpin = true;
	Speed = 3.0f;
}


// Called when the game starts
void URotatorComponent::BeginPlay()
{
	Super::BeginPlay();
	
}


// Called every frame
void URotatorComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	if(GetOwner()->HasAuthority())
	{
		if(bCanSpin)
		{
			GetOwner()->AddActorLocalRotation(FRotator(0.0f, Speed * DeltaTime, 0.0f));
		}
	}
}

