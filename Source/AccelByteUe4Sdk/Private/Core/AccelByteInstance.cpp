// Copyright (c) 2024 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "Core/AccelByteInstance.h"

#include "AccelByteUe4SdkModule.h"

FAccelByteInstance::FAccelByteInstance(class AccelByte::Settings& InSettings
	, class AccelByte::ServerSettings& InServerSettings
	, TSharedPtr<AccelByte::IAccelByteDataStorage> InLocalDataStorage
	, AccelByte::FAccelByteTimeManagerPtr InTimeManager
	, int32 RegistryIndex)
	: Settings(MakeShared<AccelByte::Settings, ESPMode::ThreadSafe>(InSettings))
	, ServerSettings(MakeShared<AccelByte::ServerSettings, ESPMode::ThreadSafe>(InServerSettings))
	, PlatformPtr(nullptr)
	, LocalDataStorage(InLocalDataStorage)
	, Index(RegistryIndex)
{
	if (!InServerSettings.BaseUrl.IsEmpty())
	{
		PlatformPtr = IAccelByteUe4SdkModuleInterface::Get().GetAccelBytePlatform(ServerSettings);
	}
	else if (!InSettings.BaseUrl.IsEmpty())
	{
		PlatformPtr = IAccelByteUe4SdkModuleInterface::Get().GetAccelBytePlatform(Settings);
	}
	FlightId = FGuid::NewGuid().ToString().ToLower();
	DeviceId = GenerateDeviceId(Index);
	EncodedDeviceId = FAccelByteUtilities::EncodeHMACBase64(DeviceId, Settings->PublisherNamespace);
}

FAccelByteInstance::~FAccelByteInstance()
{
	ClearApiClient();
	ClearServerApiClient();

	{
		FScopeLock Lock (&EnvChangeHandleMtx);
		if(OnEnvironmentChangeHandle.IsValid() && IAccelByteUe4SdkModuleInterface::IsAvailable())
		{
			IAccelByteUe4SdkModuleInterface::Get().OnEnvironmentChanged().Remove(OnEnvironmentChangeHandle);
		}
	}

	{
		FScopeLock Lock (&OnDestroyedDelegateMtx);
		if (OnDestroyedDelegate.IsBound())
		{
			OnDestroyedDelegate.Broadcast(Index);
			OnDestroyedDelegate.Clear();
		}
	}
}

AccelByte::FApiClientPtr FAccelByteInstance::GetApiClient(FString const& Key, bool bCreateIfNotFound /* = true */)
{
	FScopeLock Lock(&ApiClientMtx);
	if(ApiClients.Contains(Key))
	{
		return ApiClients[Key];
	}
	else 
	{
		if (!bCreateIfNotFound)
		{
			return nullptr;
		}

		AccelByte::FApiClientPtr NewClient = nullptr;
		{
			AccelByte::SettingsPtr SettingsBuf;
			{
				FReadScopeLock SettingsReadLock(SettingsMtx);
				SettingsBuf = Settings;
			}
			NewClient = MakeShared<AccelByte::FApiClient, ESPMode::ThreadSafe>(SettingsBuf, PlatformPtr->GetTimeManager(), AsShared());
		}
		if(!NewClient.IsValid())
		{
			return nullptr;
		}
		NewClient->Init();

		if(!IsRunningDedicatedServer())
		{
			// Only do QoS latency check if not disabled in settings
			if(!NewClient->Settings->bDisableAutoGetQosLatencies)
			{
				TSharedPtr<AccelByte::Api::Qos, ESPMode::ThreadSafe> QoSApi = NewClient->GetQosApi().Pin();
				if(QoSApi.IsValid())
				{
					QoSApi->GetServerLatencies(nullptr, nullptr);
				}
			}
		}
		ApiClients.Emplace(Key, NewClient);
		return ApiClients[Key];
	}
}

bool FAccelByteInstance::RegisterApiClient(FString const& Key, AccelByte::FApiClientPtr ApiClient)
{
	FScopeLock Lock(&ApiClientMtx);
	if (!Key.IsEmpty())
	{
		ApiClients.Add(Key, ApiClient);
		return true;
	}
	else
	{
		return false;
	}
}

bool FAccelByteInstance::RemoveApiClient(const FString& Key)
{
	FScopeLock Lock(&ApiClientMtx);
	if (!Key.IsEmpty())
	{
		const int32 RemovedNum = ApiClients.Remove(Key);
		return RemovedNum > 0;
	}
	else 
	{
		return false;
	}
}

void FAccelByteInstance::ClearApiClient()
{
	FScopeLock Lock(&ApiClientMtx);
	for (auto& ApiClient : ApiClients)
	{
		ApiClient.Value.Reset();
	}
	ApiClients.Empty();
}

void FAccelByteInstance::ClearServerApiClient()
{
	FScopeLock Lock(&ServerApiClientMtx);
	for (auto& ServerApiClient : ServerApiClients)
	{
		ServerApiClient.Value.Reset();
	}
	ServerApiClients.Empty();
}

AccelByte::FServerApiClientPtr FAccelByteInstance::GetServerApiClient(FString const& Key)
{
	FScopeLock Lock(&ServerApiClientMtx);
	if(ServerApiClients.Contains(Key))
	{
		return ServerApiClients[Key];
	}
	else
	{
		AccelByte::FServerApiClientPtr NewClient = nullptr;
		{
			AccelByte::ServerSettingsPtr ServerSettingsBuf;
			{
				FReadScopeLock SettingsReadLock(SettingsMtx);
				ServerSettingsBuf = ServerSettings;
			}
			NewClient = MakeShared<AccelByte::FServerApiClient, ESPMode::ThreadSafe>(ServerSettingsBuf, PlatformPtr->GetTimeManager(), AsShared());
		}
		if(!NewClient.IsValid())
		{
			return nullptr;
		}
		NewClient->Init();

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
		return ServerApiClients[Key];
	}
}

bool FAccelByteInstance::RemoveServerApiClient(const FString& Key)
{
	FScopeLock Lock(&ServerApiClientMtx);
	if (!Key.IsEmpty())
	{
		const int32 RemovedNum = ServerApiClients.Remove(Key);
		return RemovedNum > 0;
	}
	else 
	{
		return false;
	}
}

void FAccelByteInstance::AddOnDestroyedDelegate(TFunction<void(uint32)> Fn)
{
	FScopeLock Lock(&OnDestroyedDelegateMtx);
	OnDestroyedDelegate.AddLambda(MoveTemp(Fn));
}

void FAccelByteInstance::OnSettingsEnvironmentChanges(ESettingsEnvironment SettingsEnvironment)
{
	FWriteScopeLock WriteLock(SettingsMtx);
	Settings->Reset(SettingsEnvironment);
	ServerSettings->Reset(SettingsEnvironment);
}

void FAccelByteInstance::SetEnvironmentChangeDelegate()
{
	FScopeLock Lock(&EnvChangeHandleMtx);
	OnEnvironmentChangeHandle = IAccelByteUe4SdkModuleInterface::Get().OnEnvironmentChanged().AddThreadSafeSP(AsShared(), &FAccelByteInstance::OnSettingsEnvironmentChanges);
}

AccelByte::SettingsPtr FAccelByteInstance::GetSettings() const
{
	FReadScopeLock ReadLock(SettingsMtx);
	return Settings;
}

AccelByte::ServerSettingsPtr FAccelByteInstance::GetServerSettings() const
{
	FReadScopeLock ReadLock(SettingsMtx);
	return ServerSettings;
}

AccelByte::FAccelBytePlatformPtr FAccelByteInstance::GetAccelBytePlatform() const
{
	return PlatformPtr;
}

TWeakPtr<AccelByte::FAccelByteTimeManager, ESPMode::ThreadSafe> FAccelByteInstance::GetTimeManager() const
{
	return PlatformPtr->GetTimeManager();
}

FString FAccelByteInstance::GetFlightId() const
{
	return FlightId;
}

FString FAccelByteInstance::GetDeviceId(bool bIsEncoded /*= true*/) const
{
	if(bIsEncoded)
	{
		return EncodedDeviceId;
	}
	return DeviceId;
}

FString FAccelByteInstance::GetMacAddress(bool bEncoded) const
{
	FString MacAddressString = TEXT("");
	PRAGMA_DISABLE_DEPRECATION_WARNINGS
		const TArray<uint8> MacAddr = FPlatformMisc::GetMacAddress();
	PRAGMA_ENABLE_DEPRECATION_WARNINGS
	for (auto it = MacAddr.begin(); it != MacAddr.end(); ++it)
	{
		MacAddressString += FString::Printf(TEXT("%02x"), *it);
	}

	FReadScopeLock ReadLock(SettingsMtx);
	return (bEncoded && !MacAddressString.IsEmpty()) ? FAccelByteUtilities::EncodeHMACBase64(MacAddressString, Settings->PublisherNamespace) : MacAddressString;
}

FString FAccelByteInstance::GenerateDeviceId(int32 InIndex)
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
			FString PlainMacAddress = GetMacAddress(false);
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

	// append index to deviceID so each PIE instance have different deviceID
	if(InIndex > 0)
	{
		Output.Append(FString::FromInt(InIndex));
	}

	return Output;
}
