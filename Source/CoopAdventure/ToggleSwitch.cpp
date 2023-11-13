// Fill out your copyright notice in the Description page of Project Settings.


#include "ToggleSwitch.h"

// Sets default values
AToggleSwitch::AToggleSwitch()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bReplicates = true;
	SetReplicateMovement(true);

	Activated = false;
	Root = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	SetRootComponent(Root);

	TriggerShape = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TriggerMesh"));
	TriggerShape->SetupAttachment(RootComponent);
	TriggerShape->SetIsReplicated(true);

	auto TriggerMeshAsset = ConstructorHelpers::FObjectFinder<UStaticMesh>(TEXT("/Game/StarterContent/Shapes/Shape_Cylinder.Shape_Cylinder"));
	if(TriggerMeshAsset.Succeeded())
	{
		TriggerShape->SetStaticMesh(TriggerMeshAsset.Object);
		TriggerShape->SetRelativeScale3D(FVector(3.3f, 3.3f, 0.2f));
		TriggerShape->SetRelativeLocation(FVector(0.0f, 0.0f, 10.0f));
	}

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(RootComponent);
	Mesh->SetIsReplicated(true);

	auto MeshAsset = ConstructorHelpers::FObjectFinder<UStaticMesh>(TEXT("/Game/Stylized_Egypt/Meshes/building/SM_building_part_07"));
	if(MeshAsset.Succeeded())
	{
		Mesh->SetStaticMesh(MeshAsset.Object);
		Mesh->SetRelativeScale3D(FVector(3.0f, 3.0f, 0.25f));
		Mesh->SetRelativeLocation(FVector(0.0f, 0.0f, 7.2f));
	}

	Transporter = CreateDefaultSubobject<UTransporter>(TEXT("Transporter"));
	Transporter->MoveTime = 0.25f;
	Transporter->OwnerIsTriggerActor = true;
}

// Called when the game starts or when spawned
void AToggleSwitch::BeginPlay()
{
	Super::BeginPlay();

	Activated = ActivatedOnStart;
	
	CanActivate = true;

	TriggerShape->SetVisibility(false);
	TriggerShape->SetCollisionProfileName(FName("OverlapAll"));

	FVector Point1 = GetActorLocation();
	FVector Point2 = Point1 + FVector(0.0f, 0.0f, -10.0f);
	Transporter->SetPoints(Point1, Point2);
}

// Called every frame
void AToggleSwitch::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(HasAuthority())
	{
		TArray<AActor*> OverlappingActors;
		AActor* TriggerActor = 0;
		TriggerShape->GetOverlappingActors(OverlappingActors);

		for(int Actor = 0; Actor < OverlappingActors.Num(); Actor++)
		{
			AActor* A = OverlappingActors[Actor];
			if(A->ActorHasTag("TriggerActor"))
			{
				TriggerActor = A;
				break;
			}

			//GEngine->AddOnScreenDebugMessage(-1, 1, FColor::White, FString::Printf(TEXT("Name: %s"), *A->GetName()));
		}

		if(TriggerActor)
		{
			if(CanActivate)
			{
				if(!Activated)
				{
					GEngine->AddOnScreenDebugMessage(-1, 1, FColor::White, "Activated");
					Activated = true;
					OnActivated.Broadcast();
					CanActivate = false;
				}
				else
				{
					GEngine->AddOnScreenDebugMessage(-1, 1, FColor::White, "Deactivated");
					Activated = false;
					OnDeactivated.Broadcast();
					CanActivate = false;
				}
				
			}
		}
		else
		{
			CanActivate = true;
		}
	}
}

