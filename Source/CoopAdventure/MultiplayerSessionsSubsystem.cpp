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
	MySessionName = FName("Coop Adventure Session Name");
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
			SessionInterface->OnJoinSessionCompleteDelegates.AddUObject(this, &UMultiplayerSessionsSubsystem::OnJoinSessionComplete);

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
		FOnlineSessionSearchResult* CorrectResult = 0;
	
    	if (Results.Num() > 0)
    	{
			for(FOnlineSessionSearchResult Result : Results)
			{
				if(Result.IsValid())
				{
					FString ServerName = "No-name";
					Result.Session.SessionSettings.Get(FName("SERVER_NAME"), ServerName);

					if(ServerName.Equals(ServerNameToFind))
					{
						CorrectResult = &Result;

						FString msg2 = FString::Printf(TEXT("Found server with name: %s"), *ServerName);
						PrintString(msg2);
						break;
					}
				}
			}

    		if(CorrectResult)
    		{
    			SessionInterface->JoinSession(0, MySessionName, *CorrectResult);
    		}
    		else
    		{
    			PrintString(FString::Printf(TEXT("No server found with name: %s"), *ServerNameToFind));
    			ServerNameToFind = "";
    		}
    		
    		FString Msg = FString::Printf(TEXT("%d sessions found."), Results.Num());
    		PrintString(Msg);
    	}
    	else
    	{
    		PrintString("Zero sessions found.");
    	}
    
}

void UMultiplayerSessionsSubsystem::OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result)
{
	if(Result == EOnJoinSessionCompleteResult::Success)
	{
		PrintString(FString::Printf(TEXT("Successfully joined session: %s"), *SessionName.ToString()));

		FString Address = "";
		bool success = SessionInterface->GetResolvedConnectString(MySessionName, Address);
		if(success)
		{
			PrintString(FString::Printf(TEXT("Address: %s"), *Address));
			APlayerController* PlayerController = GetGameInstance()->GetFirstLocalPlayerController();
			if(PlayerController)
			{
				PlayerController->ClientTravel(Address, TRAVEL_Absolute);
			}
		}
		else
		{
			PrintString("Get resolved connect string returned false");
		}
	}
	else
	{
		PrintString("OnJoinSessionComplete failed");
	}
}

