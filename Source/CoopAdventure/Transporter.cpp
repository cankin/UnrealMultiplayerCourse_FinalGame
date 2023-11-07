#include "Transporter.h"
#include "CollectableKey.h"
#include "PressurePlate.h"

UTransporter::UTransporter()
{
	PrimaryComponentTick.bCanEverTick = true;

	SetIsReplicatedByDefault(true);
	MoveTime = 3.0f;
	ActivatedTriggerCount = 0;
	ArePointsSet = false;
	StartPoint = FVector(0.0f, 0.0f, 0.0f);
	EndPoint = FVector(0.0f, 0.0f, 0.0f);
}


void UTransporter::BeginPlay()
{
	Super::BeginPlay();
	
	if(OwnerIsTriggerActor)
	{
		TriggerActors.Add(GetOwner());
	}
	
	for(AActor* TA : TriggerActors)
	{
		APressurePlate* PressurePlateActor = Cast<APressurePlate>(TA);
		if(PressurePlateActor)
		{
			PressurePlateActor->OnActivated.AddDynamic(this, &UTransporter::OnTriggerActorActivated);
			PressurePlateActor->OnDeactivated.AddDynamic(this, &UTransporter::OnTriggerActorDeactivated);
			continue;
		}

		ACollectableKey* KeyActor = Cast<ACollectableKey>(TA);
		if(KeyActor)
		{
			KeyActor->OnCollected.AddDynamic(this, &UTransporter::OnTriggerActorActivated);
		}
	}
}


void UTransporter::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if(TriggerActors.Num() > 0)
	{
		AllTriggerActorsTriggered = (ActivatedTriggerCount >= TriggerActors.Num());
		//if(AllTriggerActorsTriggered)
		//{
		//	GEngine->AddOnScreenDebugMessage(-1, 5, FColor::White, FString("AllTriggerActorsTriggered"));
		//}
	}

	AActor* ActorToMove = GetOwner();
	if(ActorToMove && ActorToMove->HasAuthority() && ArePointsSet)
	{
		FVector CurrentLocation = ActorToMove->GetActorLocation();
		float Speed = FVector::Distance(StartPoint, EndPoint) / MoveTime;

		FVector TargetLocation = AllTriggerActorsTriggered ? EndPoint : StartPoint;

		if(!CurrentLocation.Equals(TargetLocation))
		{
			FVector NewLocation = FMath::VInterpConstantTo(CurrentLocation, TargetLocation, DeltaTime, Speed);
			ActorToMove->SetActorLocation(NewLocation);
		}
	}
}

void UTransporter::SetPoints(FVector Point1, FVector Point2)
{
	if(Point1.Equals(Point2))
	{
		return;
	}
	
	StartPoint = Point1;
	EndPoint = Point2;
	ArePointsSet = true;
}

void UTransporter::OnTriggerActorActivated()
{
	ActivatedTriggerCount++;

	//FString msg = FString::Printf(TEXT("Transporter Activated: %d"), ActivatedTriggerCount);
	//GEngine->AddOnScreenDebugMessage(-1, 5, FColor::White, msg);
}

void UTransporter::OnTriggerActorDeactivated()
{
	ActivatedTriggerCount--;

	//FString msg = FString::Printf(TEXT("Transporter Activated: %d"), ActivatedTriggerCount);
	//GEngine->AddOnScreenDebugMessage(-1, 5, FColor::White, msg);
}

