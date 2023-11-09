// Fill out your copyright notice in the Description page of Project Settings.


#include "Spawner.h"

#include "CollectableKey.h"
#include "PressurePlate.h"

// Sets default values
ASpawner::ASpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bReplicates = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(Root);
	auto MeshAsset = ConstructorHelpers::FObjectFinder<UStaticMesh>(TEXT("/Game/StarterContent/Shapes/Shape_Sphere"));
	if (MeshAsset.Object != nullptr)
	{
		Mesh->SetStaticMesh(MeshAsset.Object);
	}
}

// Called when the game starts or when spawned
void ASpawner::BeginPlay()
{
	Super::BeginPlay();

	for(AActor* TA : TriggerActors)
	{
		APressurePlate* PressurePlateActor = Cast<APressurePlate>(TA);
		if(PressurePlateActor)
		{
			PressurePlateActor->OnActivated.AddDynamic(this, &ASpawner::OnTriggerActorActivated);
			continue;
		}

		ACollectableKey* KeyActor = Cast<ACollectableKey>(TA);
		if(KeyActor)
		{
			KeyActor->OnCollected.AddDynamic(this, &ASpawner::OnTriggerActorActivated);
		}
	}

	SpawnLocation = GetActorLocation();
	SpawnRotation = GetActorRotation();
}

// Called every frame
void ASpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASpawner::OnTriggerActorActivated()
{
	if(HasAuthority())
	{
		if(SpawnedActor)
		{
			SpawnedActor->Destroy();
		}
		SpawnedActor = GetWorld()->SpawnActor<AActor>(SpawnedActorClass, SpawnLocation, SpawnRotation, SpawnParams);
	}
}

