// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Transporter.h"
#include "GameFramework/Actor.h"
#include "ToggleSwitch.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FToggleSwitchOnActivated);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FToggleSwitchOnDeactivated);

UCLASS()
class COOPADVENTURE_API AToggleSwitch : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AToggleSwitch();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	USceneComponent* Root;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	UStaticMeshComponent* TriggerShape;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	UStaticMeshComponent* Mesh;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	bool Activated;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool ActivatedOnStart;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	bool CanActivate;

	UPROPERTY(BlueprintAssignable)
	FToggleSwitchOnActivated OnActivated;

	UPROPERTY(BlueprintAssignable)
	FToggleSwitchOnDeactivated OnDeactivated;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UTransporter* Transporter;

};
