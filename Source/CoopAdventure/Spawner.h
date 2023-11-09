// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MoveableActor.h"
#include "GameFramework/Actor.h"
#include "Spawner.generated.h"

UCLASS()
class COOPADVENTURE_API ASpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASpawner();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY()
	USceneComponent* Root;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* Mesh;
	
	UPROPERTY(EditAnywhere)
	TArray<AActor*> TriggerActors;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FVector SpawnLocation;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FRotator SpawnRotation;

	FActorSpawnParameters SpawnParams;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AActor> SpawnedActorClass;

	UPROPERTY(VisibleAnywhere)
	AActor* SpawnedActor;

	UFUNCTION()
	void OnTriggerActorActivated();

};
