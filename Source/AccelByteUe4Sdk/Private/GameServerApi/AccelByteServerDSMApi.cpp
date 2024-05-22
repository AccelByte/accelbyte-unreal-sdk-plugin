// Copyright (c) 2019 - 2020 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "GameServerApi/AccelByteServerDSMApi.h"
#include "GameServerApi/AccelByteServerQosManagerApi.h"
#include "Core/AccelByteServerCredentials.h"
#include "Core/AccelByteRegistry.h"
#include "Core/AccelByteReport.h"
#include "Core/AccelByteServerSettings.h"
#include "Core/AccelByteHttpRetryScheduler.h"
#include "Core/AccelByteError.h"
#include "Core/AccelByteEnvironment.h"
#include "Core/AccelByteUtilities.h"
#include "Engine/GameEngine.h"
#include "GameFramework/GameState.h"

namespace AccelByte
{
namespace GameServerApi
{

FAccelByteTaskWPtr ServerDSM::RegisterServerToDSM(int32 Port
	, FVoidHandler const& OnSuccess
	, FErrorHandler const& OnError
	, FString const& CustomAttribute)
{
	FReport::Log(FString(__FUNCTION__));
	ParseCommandParam();

	if (ServerType != EServerType::NONE)
	{
		OnError.ExecuteIfBound(409, TEXT("Server already registered."));
		return nullptr;
	}

	ServerName = Environment::GetEnvironmentVariable("POD_NAME", 100);

	const FString Url = FString::Printf(TEXT("%s/namespaces/%s/servers/register")
		, *ServerSettingsRef.DSMControllerServerUrl
		, *ServerCredentialsRef->GetClientNamespace());

	const FAccelByteModelsRegisterServerRequest Register{
		GameVersion,
		DSPubIp,
		ServerName,
		Port,
		Provider,
		CustomAttribute
	};

	FReport::Log(TEXT("Starting DSM Register Request..."));

	const TDelegate<void(const FJsonObject&)> OnSuccessHttpClient = THandler<FJsonObject>::CreateLambda(
		[OnSuccess, this](FJsonObject const& JSONObject)
		{
			FTickerAlias::GetCoreTicker().RemoveTicker(AutoShutdownDelegateHandle);

			FString JSONString;
			const TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&JSONString);
			FJsonSerializer::Serialize(MakeShared<FJsonObject>(JSONObject), Writer);

			FAccelByteModelsServerInfo Result;
			if (FJsonObjectConverter::JsonObjectStringToUStruct(JSONString, &Result, 0, 0))
			{
				RegisteredServerInfo = Result;
				SetServerName(Result.Pod_name);
			}
			if (CountdownTimeStart != -1)
			{
				AutoShutdownDelegateHandle = FTickerAlias::GetCoreTicker().AddTicker(AutoShutdownDelegate, ShutdownTickSeconds);
			}
			SetServerType(EServerType::CLOUDSERVER);

			OnSuccess.ExecuteIfBound();
		});

	return HttpClient.ApiRequest(TEXT("POST"), Url, {}, Register, OnSuccessHttpClient, OnError);
}

FAccelByteTaskWPtr ServerDSM::RegisterServerToDSM(int32 Port
	, THandler<FAccelByteModelsServerInfo> const& OnSuccess
	, FErrorHandler const& OnError
	, FString const& CustomAttribute)
{
	FReport::Log(FString(__FUNCTION__));
	ParseCommandParam();

	if (ServerType != EServerType::NONE)
	{
		OnError.ExecuteIfBound(409, TEXT("Server already registered."));
		return nullptr;
	}

	ServerName = Environment::GetEnvironmentVariable("POD_NAME", 100);

	const FString Url = FString::Printf(TEXT("%s/namespaces/%s/servers/register")
		, *ServerSettingsRef.DSMControllerServerUrl
		, *ServerCredentialsRef->GetClientNamespace());

	const FAccelByteModelsRegisterServerRequest Register{
		GameVersion,
		DSPubIp,
		ServerName,
		Port,
		Provider,
		CustomAttribute
	};

	FReport::Log(TEXT("Starting DSM Register Request..."));

	const TDelegate<void(const FJsonObject&)> OnSuccessHttpClient = THandler<FJsonObject>::CreateLambda(
		[OnSuccess, this](FJsonObject const& JSONObject)
		{
			FTickerAlias::GetCoreTicker().RemoveTicker(AutoShutdownDelegateHandle);

			FString JSONString;
			const TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&JSONString);
			FJsonSerializer::Serialize(MakeShared<FJsonObject>(JSONObject), Writer);

			FAccelByteModelsServerInfo Result;
			if (FJsonObjectConverter::JsonObjectStringToUStruct(JSONString, &Result, 0, 0))
			{
				RegisteredServerInfo = Result;
				SetServerName(Result.Pod_name);
			}
			if (CountdownTimeStart != -1)
			{
				AutoShutdownDelegateHandle = FTickerAlias::GetCoreTicker().AddTicker(AutoShutdownDelegate, ShutdownTickSeconds);
			}
			SetServerType(EServerType::CLOUDSERVER);

			OnSuccess.ExecuteIfBound(Result);
		});

	return HttpClient.ApiRequest(TEXT("POST"), Url, {}, Register, OnSuccessHttpClient, OnError);
}

FAccelByteTaskWPtr ServerDSM::SendShutdownToDSM(bool KillMe
	, FString const& MatchId
	, FVoidHandler const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));
	if (ServerType == EServerType::LOCALSERVER)
	{
		OnError.ExecuteIfBound(409, TEXT("Server not registered as Cloud Server."));
		return nullptr;
	}

	const FString Url = FString::Printf(TEXT("%s/namespaces/%s/servers/shutdown")
		, *ServerSettingsRef.DSMControllerServerUrl
		, *ServerCredentialsRef->GetClientNamespace());

	const FAccelByteModelsShutdownServerRequest Shutdown{
		KillMe,
		ServerName,
		MatchId
	};

	FReport::Log(TEXT("Starting DSM Shutdown Request..."));
	FTickerAlias::GetCoreTicker().RemoveTicker(AutoShutdownDelegateHandle);
	ServerType = EServerType::NONE;
	RegisteredServerInfo = FAccelByteModelsServerInfo();

	return HttpClient.ApiRequest(TEXT("POST"), Url, {}, Shutdown, OnSuccess, OnError);
}

FAccelByteTaskWPtr ServerDSM::RegisterLocalServerToDSM(FString const& IPAddress
	, int32 Port
	, FString const& ServerName_
	, FVoidHandler const& OnSuccess
	, FErrorHandler const& OnError
	, FString const& CustomAttribute)
{
	FReport::Log(FString(__FUNCTION__));
	if (ServerType != EServerType::NONE)
	{
		OnError.ExecuteIfBound(409, TEXT("Server already registered."));
		return nullptr;
	}

	this->ServerName = ServerName_;

	const FString Url = FString::Printf(TEXT("%s/namespaces/%s/servers/local/register")
		, *ServerSettingsRef.DSMControllerServerUrl
		, *ServerCredentialsRef->GetClientNamespace());

	const FAccelByteModelsRegisterLocalServerRequest Register{
		IPAddress,
		ServerName_,
		Port,
		CustomAttribute
	};

	FReport::Log(TEXT("Starting DSM Register Local Request..."));

	const TDelegate<void(const FJsonObject&)> OnSuccessHttpClient = THandler<FJsonObject>::CreateLambda(
		[OnSuccess, this](FJsonObject const& JSONObject)
		{
			FTickerAlias::GetCoreTicker().RemoveTicker(AutoShutdownDelegateHandle);

			FString JSONString;
			const TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&JSONString);
			FJsonSerializer::Serialize(MakeShared<FJsonObject>(JSONObject), Writer);

			FAccelByteModelsServerInfo Result;
			if (FJsonObjectConverter::JsonObjectStringToUStruct(JSONString, &Result, 0, 0))
			{
				RegisteredServerInfo = Result;
			}
			if (CountdownTimeStart != -1)
			{
				AutoShutdownDelegateHandle = FTickerAlias::GetCoreTicker().AddTicker(AutoShutdownDelegate, ShutdownTickSeconds);
			}
			SetServerType(EServerType::LOCALSERVER);

			OnSuccess.ExecuteIfBound();
		});

	return HttpClient.ApiRequest(TEXT("POST"), Url, {}, Register, OnSuccessHttpClient, OnError);
}

FAccelByteTaskWPtr ServerDSM::RegisterLocalServerToDSM(int32 Port
	, FString const& ServerName_
	, FVoidHandler const& OnSuccess
	, FErrorHandler const& OnError
	, FString const& CustomAttribute)
{
	FReport::Log(FString(__FUNCTION__));
	FReport::LogDeprecated(
		FString(__FUNCTION__),
		TEXT("Please use RegisterLocalServerToDSM(const FString IPAddress, const int32 Port, const FString ServerName, FVoidHandler const& OnSuccess, FErrorHandler const& OnError)"));

	GetPubIpDelegate.BindLambda(
		[this, Port, ServerName_, OnSuccess, OnError, CustomAttribute](const FAccelByteModelsPubIp& Result)
		{
			RegisterLocalServerToDSM(Result.Ip, Port, ServerName_, OnSuccess, OnError, CustomAttribute);
		});
	FAccelByteNetUtilities::GetPublicIP(GetPubIpDelegate, OnError);
	return nullptr;
}

FAccelByteTaskWPtr ServerDSM::DeregisterLocalServerFromDSM(FString const& ServerName_
	, FVoidHandler const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));
	if (ServerType == EServerType::CLOUDSERVER)
	{
		OnError.ExecuteIfBound(409, TEXT("Server not registered as Local Server."));
		return nullptr;
	}

	const FString Url = FString::Printf(TEXT("%s/namespaces/%s/servers/local/deregister")
		, *ServerSettingsRef.DSMControllerServerUrl
			, *ServerCredentialsRef->GetClientNamespace());

	const FAccelByteModelsDeregisterLocalServerRequest Deregister{
		ServerName_
	};

	FReport::Log(TEXT("Starting DSM Deregister Request..."));
	FTickerAlias::GetCoreTicker().RemoveTicker(AutoShutdownDelegateHandle);
	ServerType = EServerType::NONE;

	RegisteredServerInfo = FAccelByteModelsServerInfo();

	return HttpClient.ApiRequest(TEXT("POST"), Url, {}, Deregister, OnSuccess, OnError);
}

FAccelByteTaskWPtr ServerDSM::RegisterServerGameSession(FString const& SessionId
    , FString const& GameMode
    , THandler<FAccelByteModelsServerCreateSessionResponse> const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	if(ServerType == EServerType::NONE)
	{
		OnError.ExecuteIfBound(400, TEXT("Server not Registered."));
		return nullptr;
	}

	FAccelByteModelsUser User;
	User.User_id = "0";
	FAccelByteModelsMatchingParty Party;
	Party.Party_id = "0";
	Party.Party_members = {User};
	
	FAccelByteModelsServerCreateSessionRequest Request;
	Request.Session_id = SessionId;
	Request.Client_version = RegisteredServerInfo.Game_version;
	Request.Game_mode = GameMode;
	Request.Configuration = "";
	Request.Deployment = RegisteredServerInfo.Deployment;
	Request.Region = RegisteredServerInfo.Region;
	Request.Namespace = FRegistry::ServerCredentialsRef->GetClientNamespace();
	Request.Matching_allies = {{{Party}}};
	
	if(ServerType == EServerType::LOCALSERVER)
	{
		Request.Pod_name = RegisteredServerInfo.Pod_name;
	}
	else if(ServerType == EServerType::CLOUDSERVER)
	{
		Request.Pod_name = "";
	}

	return RegisterServerGameSession(Request, OnSuccess, OnError);
}

FAccelByteTaskWPtr ServerDSM::RegisterServerGameSession(FAccelByteModelsServerCreateSessionRequest const& RequestContent
    , THandler<FAccelByteModelsServerCreateSessionResponse> const& OnSuccess
    , FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	const FString Url = FString::Printf(TEXT("%s/namespaces/%s/sessions")
		, *ServerSettingsRef.DSMControllerServerUrl
		, *ServerCredentialsRef->GetClientNamespace());

	return HttpClient.ApiRequest(TEXT("POST"), Url, {}, RequestContent, OnSuccess, OnError);
}

FAccelByteTaskWPtr ServerDSM::GetServerInfo(THandler<FAccelByteModelsServerInfo> const& OnSuccess
	, FErrorHandler const& OnError)
{
	if (!RegisteredServerInfo.Pod_name.IsEmpty())
	{
		OnSuccess.ExecuteIfBound(RegisteredServerInfo);
	}
	else
	{
		OnError.ExecuteIfBound(400, TEXT("No Server Registered."));
	}
	return nullptr;
}

FAccelByteTaskWPtr ServerDSM::GetSessionTimeout(THandler<FAccelByteModelsServerTimeoutResponse> const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	if (ServerType == EServerType::NONE)
	{
		OnError.ExecuteIfBound((int32)ErrorCodes::StatusBadRequest, TEXT("Server not Registered."));
		return nullptr;
	}
	
	const FString Url = FString::Printf(TEXT("%s/namespaces/%s/servers/%s/config/sessiontimeout")
		, *ServerSettingsRef.DSMControllerServerUrl
		, *ServerCredentialsRef->GetClientNamespace()
		, *ServerName);

	return HttpClient.ApiRequest(TEXT("GET"), Url, {}, FString(), OnSuccess, OnError);
}

FAccelByteTaskWPtr ServerDSM::ServerHeartbeat(FVoidHandler const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	if (ServerType == EServerType::NONE)
	{
		OnError.ExecuteIfBound((int32)ErrorCodes::StatusBadRequest, TEXT("Server not Registered."));
		return nullptr;
	}
	
	const FString Url = FString::Printf(TEXT("%s/namespaces/%s/servers/heartbeat")
		, *ServerSettingsRef.DSMControllerServerUrl
		, *ServerCredentialsRef->GetClientNamespace());

	FString Content = TEXT("");
	TSharedPtr<FJsonObject> JsonObject = MakeShared<FJsonObject>();
	JsonObject->SetStringField("podName", ServerName);
	TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&Content);
	FJsonSerializer::Serialize(JsonObject.ToSharedRef(), Writer);

	return HttpClient.ApiRequest(TEXT("PUT"), Url, {}, Content, OnSuccess, OnError);
	
}

FAccelByteTaskWPtr ServerDSM::GetSessionId(THandler<FAccelByteModelsServerSessionResponse> const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	if (ServerType == EServerType::NONE)
	{
		OnError.ExecuteIfBound(400, TEXT("Server not Registered."));
		return nullptr;
	}

	const FString Url = FString::Printf(TEXT("%s/namespaces/%s/servers/%s/session")
		, *ServerSettingsRef.DSMControllerServerUrl
		, *ServerCredentialsRef->GetClientNamespace()
		, *ServerName);

	TDelegate<void(FAccelByteModelsServerSessionResponse const&)> OnSuccessHttpClient =
		THandler<FAccelByteModelsServerSessionResponse>::CreateLambda(
			[OnSuccess, this](const FAccelByteModelsServerSessionResponse& Result)
			{
				if (!Result.Session_id.IsEmpty())
				{
					bIsDSClaimed = true;
				}
				OnSuccess.ExecuteIfBound(Result);
			});

	return HttpClient.ApiRequest(TEXT("GET"), Url, {}, FString(), OnSuccess, OnError);
}

bool ServerDSM::ShutdownTick(float DeltaTime)
{
	// For debugging purposes
	FReport::Log(FString::Printf(TEXT("%s: CountdownTimeStart:%d, bIsDSClaimed:%d, ServerType:%d, ShutdownCountdown:%d")
		, *FString(__FUNCTION__), CountdownTimeStart, bIsDSClaimed, ServerType, ShutdownCountdown));

	if (CountdownTimeStart != -1 && bIsDSClaimed && (ServerType == EServerType::CLOUDSERVER))
	{
		if (ShutdownCountdown <= 0)
		{
			FReport::Log(TEXT("ShutdownTick: Would have been shutdown"));
			SendShutdownToDSM(true, "", OnAutoShutdown, OnAutoShutdownError);
		}
		else
		{
			if (GetPlayerNum() == 0)
			{
				ShutdownCountdown -= ShutdownTickSeconds;
			}
			else
			{
				ShutdownCountdown = CountdownTimeStart;
			}
		}
	}
	return true;
}

bool ServerDSM::PeriodicHeartbeat(float DeltaTime)
{
	FReport::Log(FString(__FUNCTION__));
	ServerHeartbeat(OnHeartbeatSuccess, OnHeartbeatError);
	return true;
}

void ServerDSM::ConfigureAutoShutdown(uint32 TickSeconds
	, int CountdownStart)
{
	if (CountdownStart >= -1)
	{
		CountdownTimeStart = CountdownStart;
		ShutdownCountdown = CountdownStart;
		ShutdownTickSeconds = TickSeconds;
	}
	else
	{
		CountdownTimeStart = -1;
		ShutdownCountdown = -1;
	}
}

void ServerDSM::SetOnAutoShutdownResponse(FVoidHandler const& OnAutoShutdown_)
{
	OnAutoShutdown = OnAutoShutdown_;
}

void ServerDSM::SetOnAutoShutdownErrorDelegate(FErrorHandler const& OnError)
{
	OnAutoShutdownError = OnError;
}

// Should not be called from constructor, FCommandLine::Get() is not ready.
void ServerDSM::ParseCommandParam()
{
	FAccelByteUtilities::GetValueFromCommandLineSwitch("provider", Provider);
	FAccelByteUtilities::GetValueFromCommandLineSwitch("game_version", GameVersion);
	FAccelByteUtilities::GetValueFromCommandLineSwitch("region", Region);
}

void ServerDSM::SetOnHeartbeatSuccessDelegate(FVoidHandler const& OnSuccess)
{
	OnHeartbeatSuccess = OnSuccess;
}

void ServerDSM::SetOnHeartbeatErrorDelegate(FErrorHandler const& OnError)
{
	OnHeartbeatError = OnError;
}

void ServerDSM::StartHeartbeat(uint32 IntervalSeconds)
{
	HeartbeatTickSeconds = IntervalSeconds;
	HeartbeatDelegate = FTickerDelegate::CreateRaw(this, &ServerDSM::PeriodicHeartbeat);
	HeartbeatDelegateHandle = FTickerAlias::GetCoreTicker().AddTicker(HeartbeatDelegate, HeartbeatTickSeconds);
}

void ServerDSM::StopHeartbeat()
{
	FTickerAlias::GetCoreTicker().RemoveTicker(HeartbeatDelegateHandle);
}

int32 ServerDSM::GetPlayerNum()
{
	UGameEngine* GameEngine = CastChecked<UGameEngine>(GEngine);
	if (GameEngine)
	{
		UWorld* World = GameEngine->GetGameWorld();
		if (World)
		{
			AGameStateBase* GameState = World->GetGameState();
			if (GameState)
			{
				auto PlayerArray = GameState->PlayerArray;
				return PlayerArray.Num();
			}
			else
			{
				return 0;
			}
		}
		else
		{
			return 0;
		}
	}
	else
	{
		return 0;
	}
}

ServerDSM::ServerDSM(ServerCredentials const& InCredentialsRef
	, ServerSettings const& InSettingsRef
	, FHttpRetryScheduler& InHttpRef)
    : FServerApiBase(InCredentialsRef, InSettingsRef, InHttpRef)
{
	AutoShutdownDelegate = FTickerDelegate::CreateRaw(this, &ServerDSM::ShutdownTick);
}

ServerDSM::~ServerDSM() {}

} // Namespace GameServerApi
} // Namespace AccelByte
