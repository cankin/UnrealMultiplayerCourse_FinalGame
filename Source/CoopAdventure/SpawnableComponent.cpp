// Fill out your copyright notice in the Description page of Project Settings.


#include "SpawnableComponent.h"
#include "CollectableKey.h"
#include "PressurePlate.h"

// Sets default values for this component's properties
USpawnableComponent::USpawnableComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void USpawnableComponent::BeginPlay()
{
	Super::BeginPlay();
}


// Called every frame
void USpawnableComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
}

