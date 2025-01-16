// Copyright (c) 2024 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "Core/AccelByteInstance.h"

FAccelByteInstance::FAccelByteInstance(
	class Settings& InSettings
	, class ServerSettings& InServerSettings
	, TSharedPtr<IAccelByteDataStorage> InLocalDataStorage
	, FAccelByteTimeManagerPtr InTimeManager
	,int32 RegistryIndex)
	: Settings(MakeShared<class Settings, ESPMode::ThreadSafe>(InSettings))
	, ServerSettings(MakeShared<class ServerSettings, ESPMode::ThreadSafe>(InServerSettings))
	, LocalDataStorage(InLocalDataStorage)
	, TimeManager(InTimeManager)
	, Index(RegistryIndex)
{
	FlightId = FGuid::NewGuid().ToString().ToLower();

	DeviceId = GenerateDeviceId(Index, true);
}

FApiClientPtr FAccelByteInstance::GetApiClient(FString const& Key, bool bCreateIfNotFound /* = true */)
{
	if (!ApiClients.Contains(Key))
	{
		if (!bCreateIfNotFound)
		{
			return nullptr;
		}

		FApiClientPtr NewClient = nullptr;
		NewClient = MakeShared<FApiClient, ESPMode::ThreadSafe>(Settings, TimeManager);

		if(!IsRunningDedicatedServer())
		{
			// only do QoS latency check if not disabled in settings
			if(!NewClient->Settings->bDisableAutoGetQosLatencies)
			{
				TSharedPtr<Api::Qos, ESPMode::ThreadSafe> QoSApi = NewClient->GetQosApi().Pin();
				if(QoSApi.IsValid())
				{
					QoSApi->GetServerLatencies(nullptr, nullptr);
				}
			}
		}
		
		ApiClients.Emplace(Key, NewClient);
	}

	return ApiClients[Key];
}

SettingsPtr FAccelByteInstance::GetSettings()
{
	return Settings;
}

bool FAccelByteInstance::RegisterApiClient(FString const& Key, FApiClientPtr ApiClient)
{
	bool bResult = false;

	if (!Key.IsEmpty())
	{
		ApiClients.Add(Key, ApiClient);
		bResult = true;
	}

	return bResult;
}

bool FAccelByteInstance::RemoveApiClient(const FString& Key)
{
	bool bResult = false;

	if (!Key.IsEmpty())
	{
		const int32 RemovedNum = ApiClients.Remove(Key);
		bResult = RemovedNum > 0;
	}

	return bResult;
}

void FAccelByteInstance::ClearApiClient()
{
	for (auto ApiClient : ApiClients)
	{
		ApiClient.Value.Reset();
	}

	ApiClients.Empty();
}

void FAccelByteInstance::ClearServerApiClient()
{
	for (auto ServerApiClient : ServerApiClients)
	{
		ServerApiClient.Value.Reset();
	}

	ServerApiClients.Empty();
}

FServerApiClientPtr FAccelByteInstance::GetServerApiClient(FString const& Key)
{
	if (!ServerApiClients.Contains(Key))
	{
		FServerApiClientPtr NewClient = nullptr;
		NewClient = MakeShared<FServerApiClient, ESPMode::ThreadSafe>(ServerSettings);

		if (IsRunningDedicatedServer())
		{
			if (NewClient->ServerSettings->bServerUseAMS)
			{
				if (!NewClient->ServerSettings->LoadAMSSettings())
				{
					UE_LOG(LogAccelByte, Log, TEXT("Dedicated server could not connect to AMS. Some mandatory settings were not configured correctly."));
				}
				else
				{
					NewClient->ServerAMS.Connect();
				}
			}
			else 
			{
				UE_LOG(LogAccelByte, Log, TEXT("Dedicated server will NOT use AMS because the feature flag is not enabled. Please configure bServerUseAMS to use AMS."));
			}
		}

		ServerApiClients.Emplace(Key, NewClient);
	}
	
	return ServerApiClients[Key];
}

ServerSettingsPtr FAccelByteInstance::GetServerSettings()
{
	return ServerSettings;
}

bool FAccelByteInstance::RemoveServerApiClient(const FString& Key)
{
	bool bResult = false;
	
	if (!Key.IsEmpty())
	{
		const int32 RemovedNum = ServerApiClients.Remove(Key);
		bResult = RemovedNum > 0;
	}

	return bResult;
}

TWeakPtr<FAccelByteTimeManager, ESPMode::ThreadSafe> FAccelByteInstance::GetTimeManager()
{
	return TimeManager;
}

FString FAccelByteInstance::GetFlightId()
{
	return FlightId;
}

FString FAccelByteInstance::GetDeviceId()
{
	return DeviceId;
}

FString FAccelByteInstance::GenerateDeviceId(int32 InIndex, bool bIsDeviceIdRequireEncode)
{
	FString Output = FString();

	FString PlatformDeviceId = FPlatformMisc::GetDeviceId();
	if (PlatformDeviceId.IsEmpty())
	{
		bool bIsCached = false;
		LocalDataStorage->GetItem(FAccelByteUtilities::AccelByteStoredKeyDeviceId()
			, THandler<TPair<FString, FString>>::CreateLambda(
				[&Output, &bIsCached](TPair<FString, FString> SavedDeviceId)
				{
					if (SavedDeviceId.Key.IsEmpty() || SavedDeviceId.Value.IsEmpty())
					{
						return;
					}
					Output = SavedDeviceId.Value;
					bIsCached = true;

				})
			, FAccelByteUtilities::GetCacheFilenameGeneralPurpose());
		if (!bIsCached)
		{
			FString PlainMacAddress = FAccelByteUtilities::GetMacAddress(false);
			if (PlainMacAddress.IsEmpty())
			{
				Output = FGuid::NewGuid().ToString();
			}
			else
			{
				Output = PlainMacAddress;
			}
			LocalDataStorage->SaveItem(FAccelByteUtilities::AccelByteStoredKeyDeviceId()
				, Output
				, THandler<bool>::CreateLambda([](bool bIsSuccess){})
				, FAccelByteUtilities::GetCacheFilenameGeneralPurpose());
		}
	}
	else //IF Platform-specific DeviceID available
	{
		Output = PlatformDeviceId;
	}
	
	bool bIsClientDevMode = FAccelByteUtilities::IsRunningDevMode() && !IsRunningDedicatedServer();

	if (bIsClientDevMode)
	{
		Output = FAccelByteUtilities::GetDevModeDeviceId(Output);
	}

	if (bIsDeviceIdRequireEncode || !bIsClientDevMode)
	{
		Output = FAccelByteUtilities::EncodeHMACBase64(Output, Settings->PublisherNamespace);
	}

	// append index to deviceID so each PIE instance have different deviceID
	if(InIndex > 0)
	{
		Output.Append(FString::FromInt(InIndex));
	}
	return Output;
}
