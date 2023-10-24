// Fill out your copyright notice in the Description page of Project Settings.


#include "MultiplayerSessionsSubsystem.h"
#include "Online/OnlineSessionNames.h"


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
	OnlineSubsystem = IOnlineSubsystem::Get();
	createServerAfterDestroy = false;
	destroyServerName = "";
	ServerNameToFind = "";
}

void UMultiplayerSessionsSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	//PrintString("MSS Initialize")

	if (OnlineSubsystem)
	{
		FString SubsystemName = OnlineSubsystem->GetSubsystemName().ToString();
		PrintString(SubsystemName);

		SessionInterface = OnlineSubsystem->GetSessionInterface();
		if (SessionInterface.IsValid())
		{
			SessionInterface->OnCreateSessionCompleteDelegates.AddUObject(this, &UMultiplayerSessionsSubsystem::OnCreateSessionComplete);
			SessionInterface->OnDestroySessionCompleteDelegates.AddUObject(this, &UMultiplayerSessionsSubsystem::OnDestroySessionComplete);
			SessionInterface->OnFindSessionsCompleteDelegates.AddUObject(this, &UMultiplayerSessionsSubsystem::OnFindSessionsComplete);

		}
	}
}

void UMultiplayerSessionsSubsystem::Deinitialize()
{
	//UE_LOG(LogTemp, Warning, TEXT("MSS deinitialize"));
}

void UMultiplayerSessionsSubsystem::CreateServer(FString ServerName)
{
	PrintString("Create Server");

	if(ServerName.IsEmpty())
	{
		PrintString("Server name cannot be empty");
		return;
	}

	FName MySessionName = FName("Coop Adventure Session Name");

	FNamedOnlineSession* ExistingSession = SessionInterface->GetNamedSession(MySessionName);
	if(ExistingSession)
	{
		PrintString(FString::Printf(TEXT("Session named %s already exists. Destroying it."), *MySessionName.ToString()));
		createServerAfterDestroy = true;
		destroyServerName = ServerName;
		SessionInterface->DestroySession(MySessionName);
		return;
	}

	FOnlineSessionSettings SessionSettings;
	SessionSettings.bAllowJoinInProgress = true;
	SessionSettings.bIsDedicated = false;
	SessionSettings.bShouldAdvertise = true;
	SessionSettings.NumPublicConnections = 2;
	SessionSettings.bUseLobbiesIfAvailable = true;
	SessionSettings.bUsesPresence = true;
	SessionSettings.bAllowJoinViaPresence = true;

	if(OnlineSubsystem->GetSubsystemName() == "NULL")
	{
		SessionSettings.bIsLANMatch = true;
	}
	else
	{
		SessionSettings.bIsLANMatch = false;
	}
	
	SessionSettings.Set(FName("SERVER_NAME"), ServerName, EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);
	
	SessionInterface->CreateSession(0, MySessionName, SessionSettings);
}

void UMultiplayerSessionsSubsystem::FindServer(FString ServerName)
{
	PrintString("FindServer");

	if (ServerName.IsEmpty())
	{
		PrintString("Server name cannot be empty!");
		return;
	}

	SessionSearch = MakeShareable(new FOnlineSessionSearch());
	bool IsLAN = false;
	if (IOnlineSubsystem::Get()->GetSubsystemName() == "NULL")
	{
		IsLAN = true;
	}
	SessionSearch->bIsLanQuery = IsLAN;
	SessionSearch->MaxSearchResults = 9999;
	SessionSearch->QuerySettings.Set(SEARCH_PRESENCE, true, EOnlineComparisonOp::Equals);

	ServerNameToFind = ServerName;

	SessionInterface->FindSessions(0, SessionSearch.ToSharedRef());
}

void UMultiplayerSessionsSubsystem::OnCreateSessionComplete(FName SessionName, bool bWasSucessful)
{
	PrintString(FString::Printf(TEXT("OnCreateSessionComplete: %d"), bWasSucessful));

	if(bWasSucessful)
	{
		GetWorld()->ServerTravel("/Game/ThirdPerson/Maps/ThirdPersonMap?listen");
	}
}

void UMultiplayerSessionsSubsystem::OnDestroySessionComplete(FName SessionName, bool bWasSucessful)
{
	PrintString(FString::Printf(TEXT("OnDestroySessionComplete session name: %s Sucess: %d"), *SessionName.ToString(), bWasSucessful));

	if(createServerAfterDestroy)
	{
		createServerAfterDestroy = false;
		CreateServer(destroyServerName);
	}
}

void UMultiplayerSessionsSubsystem::OnFindSessionsComplete(bool WasSuccessful)
{
	if (!WasSuccessful) return;
	if (ServerNameToFind.IsEmpty()) return;
	
    if(!SessionSearch.IsValid())
    {
    	return;
    }
    	TArray<FOnlineSessionSearchResult> Results = SessionSearch->SearchResults;
    	if (Results.Num() > 0)
    	{
			for(FOnlineSessionSearchResult Result : Results)
			{
				if(Result.IsValid())
				{
					FString ServerName = "No-name";
					Result.Session.SessionSettings.Get(FName("SERVER_NAME"), ServerName);

					FString msg2 = FString::Printf(TEXT("Server name: %s"), *ServerName);
					PrintString(msg2);
				}
			}
    		
    		FString Msg = FString::Printf(TEXT("%d sessions found."), Results.Num());
    		PrintString(Msg);
    	}
    	else
    	{
    		PrintString("Zero sessions found.");
    	}
    
}

