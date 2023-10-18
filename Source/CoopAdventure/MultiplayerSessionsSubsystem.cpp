// Fill out your copyright notice in the Description page of Project Settings.


#include "MultiplayerSessionsSubsystem.h"
#include "OnlineSubsystem.h"


void PrintString(const FString& string)
{
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Cyan, string);
	}
}

UMultiplayerSessionsSubsystem::UMultiplayerSessionsSubsystem()
{
	//PrintString("MSS Constructor");
}

void UMultiplayerSessionsSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	//PrintString("MSS Initialize");
	IOnlineSubsystem* OnlineSubsystem = IOnlineSubsystem::Get();

	if (OnlineSubsystem)
	{
		FString SubsystemName = OnlineSubsystem->GetSubsystemName().ToString();
		PrintString(SubsystemName);

		SessionInterface = OnlineSubsystem->GetSessionInterface();
		if (SessionInterface.IsValid())
		{
			PrintString("Session Interface is valid!");
		}
	}
}

void UMultiplayerSessionsSubsystem::Deinitialize()
{
	//UE_LOG(LogTemp, Warning, TEXT("MSS deinitialize"));
}
