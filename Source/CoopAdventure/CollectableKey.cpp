// Fill out your copyright notice in the Description page of Project Settings.


#include "CollectableKey.h"
#include "CoopAdventureCharacter.h"
#include "Net/UnrealNetwork.h"

// Sets default values
ACollectableKey::ACollectableKey()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bReplicates = true;
	SetReplicateMovement(true);
	
	Root = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	SetRootComponent(Root);

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(Root);
	Mesh->SetIsReplicated(true);
	Mesh->SetCollisionProfileName(FName("OverlapAllDynamic"));

	CollectAudio = CreateDefaultSubobject<UAudioComponent>(TEXT("CollectAudio"));
	CollectAudio->SetupAttachment(Root);
	CollectAudio->SetAutoActivate(false);

	Capsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComponent"));
	Capsule->SetupAttachment(Root);
	Capsule->SetIsReplicated(true);
	Capsule->SetCollisionProfileName(FName("OverlapAllDynamic"));
	Capsule->SetCapsuleHalfHeight(150.0f);
	Capsule->SetCapsuleRadius(100.0f);

	RotatorComponent = CreateDefaultSubobject<URotatorComponent>(TEXT("RotatorComponent"));
	RotatorComponent->Speed = 100.0f;
}

void ACollectableKey::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ACollectableKey, IsCollected);
}

// Called when the game starts or when spawned
void ACollectableKey::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACollectableKey::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(HasAuthority())
	{
		TArray<AActor*> OverlappingActors;
		Capsule->GetOverlappingActors(OverlappingActors, ACoopAdventureCharacter::StaticClass());
		
		if(!OverlappingActors.IsEmpty())
		{
			if(!IsCollected)
			{
				IsCollected = true;
				OnRep_IsCollected();
			}
		}
	}
}

void ACollectableKey::OnRep_IsCollected()
{
	if(HasAuthority())
	{
		//UE_LOG(LogTemp, Display, TEXT("OnRep_IsCollected called on server"));

		if(IsCollected)
		{
			OnCollected.Broadcast();
		}
	}
	else
	{
		//UE_LOG(LogTemp, Display, TEXT("OnRep_IsCollected called on client"));
	}

	Mesh->SetVisibility(!IsCollected);
	CollectAudio->Play();

	if(IsCollected)
	{
		if(KeyHolderRef)
		{
			KeyHolderRef->ActivateKeyMesh();
		}
	}
}

