// Copyright (c) 2018 - 2021 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "AccelByteUe4SdkModule.h"
#include "Core/Version.h"
#include "Interfaces/IPluginManager.h"
#include "Core/AccelByteSettings.h"
#include "Core/AccelByteServerSettings.h"
#include "Core/AccelByteRegistry.h"
#include "Core/AccelByteHttpRetryScheduler.h"
#include "Core/AccelByteReport.h"
#include "Core/AccelByteDataStorageBinaryFile.h"
#include "Core/AccelByteInstance.h"
#include "Core/Platform/AccelBytePlatformHandler.h"
#include "Api/AccelByteGameTelemetryApi.h"
#include "Api/AccelByteHeartBeatApi.h"
#include "Api/AccelBytePredefinedEventApi.h"
#include "Api/AccelByteGameStandardEventApi.h"
#include "Api/AccelByteQos.h"
#include "Api/AccelByteLobbyApi.h"
#include "Core/AccelByteMultiRegistry.h"
#include "Core/ServerTime/AccelByteTimeManager.h"
#include "GameServerApi/AccelByteServerAMSApi.h"
#include "Engine/GameInstance.h"

#if UE_SERVER
#include "Core/AccelByteSignalHandler.h"
#endif

#if WITH_EDITOR
#include "ISettingsModule.h"
#include "ISettingsSection.h"
#endif

class FAccelByteUe4SdkModule : public IAccelByteUe4SdkModuleInterface
{
public:

    virtual void StartupModule() override;
	virtual void ShutdownModule() override;

	virtual void SetEnvironment(ESettingsEnvironment const Environment) override;
	virtual AccelByte::Settings const& GetClientSettings() const override;
	virtual AccelByte::ServerSettings const& GetServerSettings() const override;
	virtual ESettingsEnvironment const& GetSettingsEnvironment() const override;
	virtual FEnvironmentChangedDelegate& OnEnvironmentChanged() override;
	virtual AccelByte::IAccelByteDataStorage * GetLocalDataStorage() override;
	virtual AccelByte::FAccelBytePlatformHandler& GetPlatformHandler() override;

private:
	AccelByte::Settings GlobalClientSettings{};
	AccelByte::ServerSettings GlobalServerSettings{};
	ESettingsEnvironment SettingsEnvironment{ESettingsEnvironment::Default};
	FEnvironmentChangedDelegate EnvironmentChangedDelegate{};
	TSharedPtr<AccelByte::IAccelByteDataStorage> LocalDataStorage = nullptr;
	AccelByte::FAccelBytePlatformHandler PlatformHandler;
	FThreadSafeCounter GameInstanceCount {0};

	FAccelByteTimeManagerPtr TimeManager;

    // For registering settings in UE4 editor
	void RegisterSettings();
	void UnregisterSettings();
	
	void PostStartup();
	FDelegateHandle PostStartupDelegateHandle{};

	FDelegateHandle GameInstanceStartHandle{};

	FDelegateHandle OnPreExitHandle;

	bool LoadClientSettings(ESettingsEnvironment const Environment);
	bool LoadServerSettings(ESettingsEnvironment const Environment);
	bool LoadSettingsFromConfigUObject();
	bool LoadServerSettingsFromConfigUObject();
	bool NullCheckConfig(FString const& Value, FString const& ConfigField);
	static FVersion GetPluginVersion();
	void GetVersionInfo(FString const& Url, TFunction<void(FVersionInfo)> Callback) const;
	void CheckServicesCompatibility() const;
	void SetDefaultHttpCustomHeader(FString const& Namespace);

	void OnGameInstanceCreated(UGameInstance* GameInstance);
	void OnPreExit();

public:
	virtual FAccelByteInstancePtr CreateAccelByteInstance() override;

protected:
};

void FAccelByteUe4SdkModule::StartupModule()
{
#if WITH_EDITOR
	FModuleManager::Get().LoadModuleChecked("Settings");
#endif
	FModuleManager::Get().LoadModuleChecked("HTTP");
	FModuleManager::Get().LoadModuleChecked("WebSockets");
	FModuleManager::Get().LoadModuleChecked("Json");
	FModuleManager::Get().LoadModuleChecked("JsonUtilities");
	FModuleManager::Get().LoadModuleChecked("Projects");

	SettingsEnvironment = ESettingsEnvironment::Default;

	RegisterSettings();
	LoadSettingsFromConfigUObject();
	LoadServerSettingsFromConfigUObject();

	LocalDataStorage = MakeShared<AccelByte::DataStorageBinaryFile>();
	TimeManager = MakeShared<AccelByte::FAccelByteTimeManager, ESPMode::ThreadSafe>();

#ifdef TEMPORARY_ENABLE_COMPAT_CHECK
#if UE_BUILD_DEVELOPMENT && TEMPORARY_ENABLE_COMPAT_CHECK
	CheckServicesCompatibility();
#endif // UE_BUILD_DEVELOPMENT && TEMPORARY_ENABLE_COMPAT_CHECK
#endif // defined(TEMPORARY_ENABLE_COMPAT_CHECK)

	AccelByte::FRegistry::HttpRetryScheduler.Startup();
	AccelByte::FRegistry::CredentialsRef->Startup();
	AccelByte::FRegistry::QosPtr->Startup();
	AccelByte::FRegistry::GameTelemetryPtr->Startup();
	AccelByte::FRegistry::PredefinedEventPtr->Startup();
	AccelByte::FRegistry::GameStandardEventPtr->Startup();
	AccelByte::FRegistry::LobbyPtr->Startup();
#if !UE_SERVER
	AccelByte::FRegistry::HeartBeat.Startup();
#endif
	AccelByte::FRegistry::ServerCredentialsRef->Startup();

#if UE_SERVER
	FAccelByteSignalHandler::Initialize();
#endif

	GameInstanceStartHandle = FWorldDelegates::OnStartGameInstance.AddRaw(this, &FAccelByteUe4SdkModule::OnGameInstanceCreated);

	PostStartupDelegateHandle = FCoreDelegates::OnBeginFrame.AddRaw(this, &FAccelByteUe4SdkModule::PostStartup);

	if (!OnPreExitHandle.IsValid())
	{
		OnPreExitHandle = FCoreDelegates::OnPreExit.AddRaw(this, &FAccelByteUe4SdkModule::OnPreExit);
	}
}

void FAccelByteUe4SdkModule::PostStartup()
{
	auto StoragePtr = GetLocalDataStorage();
	if (StoragePtr != nullptr)
	{
		UE_LOG(LogAccelByte, Log, TEXT("LocalDataStorageUtility obtained, executing the cache migration."));
		StoragePtr->ConvertExistingCache(
			FAccelByteUtilities::AccelByteStorageFile(),//The old storage name
			FAccelByteUtilities::GetCacheFilenameTelemetry(),
			FAccelByteUtilities::GetCacheFilenameGeneralPurpose());
	}
	else
	{
		UE_LOG(LogAccelByte, Warning, TEXT("LocalDataStorageUtility can't be obtained, skipping the cache migration."));
	}

	FCoreDelegates::OnBeginFrame.Remove(this->PostStartupDelegateHandle);
}

void FAccelByteUe4SdkModule::ShutdownModule()
{
	UnregisterSettings();

	FWorldDelegates::OnStartGameInstance.Remove(GameInstanceStartHandle);

	if (OnPreExitHandle.IsValid())
	{
		FCoreDelegates::OnPreExit.Remove(OnPreExitHandle);
		OnPreExitHandle.Reset();
	}
}

void FAccelByteUe4SdkModule::SetEnvironment(ESettingsEnvironment const Environment)
{
	LoadClientSettings(Environment);
	LoadServerSettings(Environment);
	SettingsEnvironment = Environment;
	if (EnvironmentChangedDelegate.IsBound())
	{
		EnvironmentChangedDelegate.Broadcast(Environment);
	}
}

AccelByte::Settings const& FAccelByteUe4SdkModule::GetClientSettings() const
{
	return GlobalClientSettings;
}

AccelByte::ServerSettings const& FAccelByteUe4SdkModule::GetServerSettings() const
{
	return GlobalServerSettings;
}

ESettingsEnvironment const& FAccelByteUe4SdkModule::GetSettingsEnvironment() const
{
	return SettingsEnvironment;
}

void FAccelByteUe4SdkModule::RegisterSettings()
{
#if WITH_EDITOR
	if (ISettingsModule* SettingsModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings"))
	{
		ISettingsSectionPtr SettingsSection = SettingsModule->RegisterSettings(TEXT("Project"), TEXT("Plugins"), TEXT("AccelByte Unreal Engine 4 Client SDK"),
			FText::FromName(TEXT("AccelByte Unreal Engine 4 Client SDK")),
			FText::FromName(TEXT("Setup your plugin.")),
			GetMutableDefault<UAccelByteSettings>()
		);
		if (SettingsSection.IsValid())
		{
			SettingsSection->OnModified().BindRaw(this, &FAccelByteUe4SdkModule::LoadSettingsFromConfigUObject);
			SettingsSection->OnSave().BindRaw(this, &FAccelByteUe4SdkModule::LoadSettingsFromConfigUObject);
			SettingsSection->OnSaveDefaults().BindRaw(this, &FAccelByteUe4SdkModule::LoadSettingsFromConfigUObject);
		}

		ISettingsSectionPtr ServerSettingsSection = SettingsModule->RegisterSettings(TEXT("Project"), TEXT("Plugins"), TEXT("AccelByte Unreal Engine 4 Server SDK"),
			FText::FromName(TEXT("AccelByte Unreal Engine 4 Server SDK")),
			FText::FromName(TEXT("Setup your plugin.")),
			GetMutableDefault<UAccelByteServerSettings>()
		);
		if (ServerSettingsSection.IsValid())
		{
			ServerSettingsSection->OnModified().BindRaw(this, &FAccelByteUe4SdkModule::LoadServerSettingsFromConfigUObject);
			ServerSettingsSection->OnSave().BindRaw(this, &FAccelByteUe4SdkModule::LoadServerSettingsFromConfigUObject);
			ServerSettingsSection->OnSaveDefaults().BindRaw(this, &FAccelByteUe4SdkModule::LoadServerSettingsFromConfigUObject);
		}
	}
#endif
}

void FAccelByteUe4SdkModule::UnregisterSettings()
{
#if WITH_EDITOR
	if (ISettingsModule* SettingsModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings"))
	{
		SettingsModule->UnregisterSettings(TEXT("Project"), TEXT("Plugins"), TEXT("AccelByte Unreal Engine 4 SDK"));
	}
#endif
}

bool FAccelByteUe4SdkModule::LoadClientSettings(ESettingsEnvironment const Environment)
{
	bool bResult = true;
	GlobalClientSettings.Reset(Environment);
	
	bResult &= NullCheckConfig(GlobalClientSettings.ClientId, TEXT("Client ID"));
	bResult &= NullCheckConfig(GlobalClientSettings.Namespace, TEXT("Namespace"));
	bResult &= NullCheckConfig(GlobalClientSettings.BaseUrl, TEXT("Base URL"));

	if (!bResult)
	{
		return bResult;
	}
	
	AccelByte::FRegistry::Settings.Reset(Environment);
	AccelByte::FRegistry::CredentialsRef->SetClientCredentials(Environment);
	SetDefaultHttpCustomHeader(GlobalClientSettings.Namespace);

	return bResult;
}

bool FAccelByteUe4SdkModule::LoadServerSettings(ESettingsEnvironment const Environment)
{
	bool bResult = true;

	GlobalServerSettings.Reset(Environment);

	bResult &= NullCheckConfig(GlobalServerSettings.ClientId, TEXT("Client ID"));
	bResult &= NullCheckConfig(GlobalServerSettings.Namespace, TEXT("Namespace"));
	bResult &= NullCheckConfig(GlobalServerSettings.BaseUrl, TEXT("Base URL"));
	
	if (!bResult)
	{
		return bResult;
	}
	
	AccelByte::FRegistry::ServerSettings.Reset(Environment);
	AccelByte::FRegistry::ServerCredentialsRef->SetClientCredentials(Environment);
	SetDefaultHttpCustomHeader(GlobalServerSettings.Namespace);
	
	return bResult;
}

bool FAccelByteUe4SdkModule::LoadSettingsFromConfigUObject()
{
	return LoadClientSettings(SettingsEnvironment);
}

bool FAccelByteUe4SdkModule::LoadServerSettingsFromConfigUObject()
{
	bool bResult = false;
	bool bEnableSettings = false;

#if UE_BUILD_DEVELOPMENT
	GConfig->GetBool(TEXT("/Script/AccelByteUe4Sdk.AccelByteServerSettings"), TEXT("ForceEnableSettings"), bEnableSettings, GEngineIni);
#endif

#if WITH_EDITOR || UE_SERVER
	bEnableSettings = true;
#endif

	if (bEnableSettings)
	{
		bResult = LoadServerSettings(SettingsEnvironment);
	}
		
	return bResult;
}

bool FAccelByteUe4SdkModule::NullCheckConfig(FString const& Value, FString const& ConfigField)
{
	if (Value.IsEmpty())
	{
		UE_LOG(LogAccelByte, Warning, TEXT("\"%s\" is not configured yet.\nCheck DefaultEngine.ini or Edit/ProjectSettings/Plugins/"), *ConfigField);
		return false;
	}

	return true;
}

FVersion FAccelByteUe4SdkModule::GetPluginVersion()
{
	FString const PluginName = "AccelByteUe4Sdk";
 
	TSharedPtr<IPlugin> const Plugin = IPluginManager::Get().FindPlugin("AccelByteUe4Sdk");
	const FPluginDescriptor& Descriptor = Plugin->GetDescriptor();

	return Descriptor.VersionName;
}

void FAccelByteUe4SdkModule::GetVersionInfo(FString const& Url, TFunction<void(FVersionInfo)> Callback) const
{
	FHttpRequestPtr const Request = FHttpModule::Get().CreateRequest();
	Request->SetVerb("GET");
	Request->SetURL(FString::Printf(TEXT("%s/version"), *Url));
	Request->OnProcessRequestComplete().BindLambda(
		[Callback](FHttpRequestPtr const RequestPtr, FHttpResponsePtr const ResponsePtr, bool const bFinished)
		{
			if (!bFinished || !ResponsePtr.IsValid())
			{
				UE_LOG(LogAccelByte, Warning, TEXT("Getting version info failed:%s"), *RequestPtr->GetURL());
				return;
			}

			FString const Content = ResponsePtr->GetContentAsString();

			if (Content.IsEmpty())
			{
				UE_LOG(LogAccelByte, Warning, TEXT("Version info empty:%s"), *RequestPtr->GetURL());
				return;
			}

			FVersionInfo VersionInfo;
			FJsonObjectConverter::JsonObjectStringToUStruct(Content, &VersionInfo, 0, 0);

			if (Callback)
			{
				Callback(VersionInfo);
			}
		});

	Request->ProcessRequest();

}

void FAccelByteUe4SdkModule::CheckServicesCompatibility() const
{
	if (GetPluginVersion().Compare(FVersion{TEXT("4.0.0")}) <= 0)
	{
		return;
	}

	FString const Path = FPaths::ProjectPluginsDir() / "AccelByteUe4Sdk/Content/CompatibilityMap.json";
	FString String;
	FFileHelper::LoadFileToString(String, *Path);
	auto CompatibilityMapPtr = MakeShared<FServiceCompatibilityMap>(FServiceCompatibilityMap::FromJson(String));

	for (auto const& ServiceName : CompatibilityMapPtr->GetServices())
	{
		if (ServiceName.IsEmpty())
		{
			return;
		}

		GetVersionInfo(
			AccelByte::FRegistry::Settings.BaseUrl / ServiceName,
			[CompatibilityMapPtr, ServiceName](FVersionInfo const VersionInfo)
			{
				FResult const Result = CompatibilityMapPtr->Check(ServiceName, VersionInfo.Version, true);
				if (Result.bIsError)
				{
					UE_LOG(LogAccelByte, Warning, TEXT("[Compatibility] %s"), *Result.Message);
				}
			});
	}
}

FEnvironmentChangedDelegate& FAccelByteUe4SdkModule::OnEnvironmentChanged()
{
	return EnvironmentChangedDelegate;
}

AccelByte::IAccelByteDataStorage* FAccelByteUe4SdkModule::GetLocalDataStorage()
{
	if (LocalDataStorage.IsValid())
	{
		return LocalDataStorage.Get();
	}
	else
	{
		return nullptr;
	}
}

AccelByte::FAccelBytePlatformHandler& FAccelByteUe4SdkModule::GetPlatformHandler()
{
	return this->PlatformHandler;
}

void FAccelByteUe4SdkModule::SetDefaultHttpCustomHeader(FString const& Namespace)
{
	AccelByte::FHttpRetryScheduler::SetHeaderNamespace(Namespace);
	AccelByte::FHttpRetryScheduler::SetHeaderSDKVersion(GetPluginVersion().ToString());
	FString ProjectVersion;
	GConfig->GetString(TEXT("/Script/EngineSettings.GeneralProjectSettings"), TEXT("ProjectVersion"), ProjectVersion, GGameIni);
	AccelByte::FHttpRetryScheduler::SetHeaderGameClientVersion(ProjectVersion);
}

void FAccelByteUe4SdkModule::OnGameInstanceCreated(UGameInstance* GameInstance)
{
	if (!GameInstance->IsValidLowLevel())
	{
		return;
	}

	GameInstanceCount.Increment();
	
	AccelByte::FRegistry::TimeManager.GetServerTime({}, {});
	TimeManager->GetServerTime({}, {});
}

void FAccelByteUe4SdkModule::OnPreExit()
{
	AccelByte::FRegistry::ServerCredentialsRef->Shutdown();
#if !UE_SERVER
	AccelByte::FRegistry::HeartBeat.Shutdown();
#endif
	AccelByte::FRegistry::GameTelemetryPtr->Shutdown();
	AccelByte::FRegistry::PredefinedEventPtr->Shutdown();
	AccelByte::FRegistry::GameStandardEventPtr->Shutdown();
	AccelByte::FRegistry::CredentialsRef->Shutdown();
	AccelByte::FRegistry::HttpRetryScheduler.Shutdown();

	FMultiRegistry::Shutdown();
}

FAccelByteInstancePtr FAccelByteUe4SdkModule::CreateAccelByteInstance()
{
	return MakeShared<FAccelByteInstance, ESPMode::ThreadSafe>(GlobalClientSettings, GlobalServerSettings, LocalDataStorage, TimeManager, GameInstanceCount.GetValue());
}

IMPLEMENT_MODULE(FAccelByteUe4SdkModule, AccelByteUe4Sdk)
