// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/Actor.h"
#include "Components/AudioComponent.h"
#include "CollectableKey.generated.h"

UCLASS()
class COOPADVENTURE_API ACollectableKey : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACollectableKey();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	USceneComponent* Root;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* Mesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UCapsuleComponent* Capsule;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UAudioComponent* CollectAudio;

	UPROPERTY(ReplicatedUsing = OnRep_IsCollected)
	bool IsCollected;

	UFUNCTION()
	void OnRep_IsCollected();

	
};
