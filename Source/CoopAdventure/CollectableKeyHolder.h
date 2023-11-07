// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CollectableKey.h"
#include "GameFramework/Actor.h"
#include "RotatorComponent.h"
#include "CollectableKeyHolder.generated.h"


UCLASS()
class COOPADVENTURE_API ACollectableKeyHolder : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACollectableKeyHolder();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	UStaticMeshComponent* ColumnMesh;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	UStaticMeshComponent* KeyMesh;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	USceneComponent* Root;

	UFUNCTION()
	void ActivateKeyMesh();

};
