#include "Transporter.h"

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


	
}


void UTransporter::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
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

