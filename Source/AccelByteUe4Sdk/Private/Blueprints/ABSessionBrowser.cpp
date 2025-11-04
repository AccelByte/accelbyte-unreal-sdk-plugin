// Copyright (c) 2021 AccelByte, Inc. All rights reserved.
// This is licensed software from Accelbyte Inc, for limitations
// and restrictions contact your company contract manager

#if 1 // MMv1 Deprecation
#include "Blueprints/ABSessionBrowser.h"

DECLARE_LOG_CATEGORY_EXTERN(LogAccelByteSessionBrowser, Log, All);
DEFINE_LOG_CATEGORY(LogAccelByteSessionBrowser);

using namespace AccelByte;

void UABSessionBrowser::SetApiClient(FApiClientPtr const& NewApiClientPtr)
{
	ApiClientPtr = NewApiClientPtr;
}

void UABSessionBrowser::CreateGameSessionPublic(
	FString const& GameMode,
	FString const& GameMapName,
	FString const& GameVersion,
	int32 BotCount,
	int32 MaxPlayer,
	FJsonObjectWrapper const& OtherSettings,
	FDModelsSessionBrowserDataResponse const& OnSuccess,
	FDErrorHandler const& OnError
)
{
	FJsonObjectWrapper OtherSettingsCopy = OtherSettings;
	OtherSettingsCopy.JsonObjectFromString(OtherSettingsCopy.JsonString);
	auto SessionBrowser = ApiClientPtr->GetSessionBrowserApi().Pin();
	if (SessionBrowser.IsValid())
	{
		SessionBrowser->CreateGameSession(
			GameMode,
			GameMapName,
			GameVersion,
			BotCount,
			MaxPlayer,
			OtherSettingsCopy.JsonObject,
			THandler<FAccelByteModelsSessionBrowserData>::CreateLambda(
				[OnSuccess](FAccelByteModelsSessionBrowserData const& Response)
				{
					OnSuccess.ExecuteIfBound(Response);
				}
			),
			FErrorHandler::CreateLambda(
				[OnError](int32 Code, FString const& Message)
				{
					OnError.ExecuteIfBound(Code, Message);
				}
			)
		);
	}
	else
	{
		UE_LOG(LogAccelByteSessionBrowser, Warning, TEXT("Invalid SessionBrowser API from API Client"));
		OnError.ExecuteIfBound(static_cast<int32>(AccelByte::ErrorCodes::InvalidRequest), TEXT("SessionBrowser API already destroyed!"));
	}
}

void UABSessionBrowser::CreateGameSessionPrivate(
	FString const& GameMode,
	FString const& GameMapName,
	FString const& GameVersion,
	int32 BotCount,
	int32 MaxPlayer,
	int32 MaxSpectator,
	FString const& Password,
	FJsonObjectWrapper const& OtherSettings,
	FDModelsSessionBrowserDataResponse const& OnSuccess,
	FDErrorHandler const& OnError
)
{
	FJsonObjectWrapper OtherSettingsCopy = OtherSettings;
	OtherSettingsCopy.JsonObjectFromString(OtherSettingsCopy.JsonString);
	auto SessionBrowser = ApiClientPtr->GetSessionBrowserApi().Pin();
	if (SessionBrowser.IsValid())
	{
		SessionBrowser->CreateGameSession(
			GameMode,
			GameMapName,
			GameVersion,
			BotCount,
			MaxPlayer,
			MaxSpectator,
			Password,
			OtherSettingsCopy.JsonObject,
			THandler<FAccelByteModelsSessionBrowserData>::CreateLambda(
				[OnSuccess](FAccelByteModelsSessionBrowserData const& Response)
				{
					OnSuccess.ExecuteIfBound(Response);
				}
			),
			FErrorHandler::CreateLambda(
				[OnError](int32 Code, FString const& Message)
				{
					OnError.ExecuteIfBound(Code, Message);
				}
			)
		);
	}
	else
	{
		UE_LOG(LogAccelByteSessionBrowser, Warning, TEXT("Invalid SessionBrowser API from API Client"));
		OnError.ExecuteIfBound(static_cast<int32>(AccelByte::ErrorCodes::InvalidRequest), TEXT("SessionBrowser API already destroyed!"));
	}
}

void UABSessionBrowser::CreateGameSessionTypeSpecifiedByString(
	FString const& SessionTypeString,
	FString const& GameMode,
	FString const& GameMapName,
	FString const& GameVersion,
	int32 BotCount,
	int32 MaxPlayer,
	int32 MaxSpectator,
	FString const& Password,
	FJsonObjectWrapper const& OtherSettings,
	FDModelsSessionBrowserDataResponse const& OnSuccess,
	FDErrorHandler const& OnError
)
{
	FJsonObjectWrapper OtherSettingsCopy = OtherSettings;
	OtherSettingsCopy.JsonObjectFromString(OtherSettingsCopy.JsonString);
	auto SessionBrowser = ApiClientPtr->GetSessionBrowserApi().Pin();
	if (SessionBrowser.IsValid())
	{
		SessionBrowser->CreateGameSession(
			SessionTypeString,
			GameMode,
			GameMapName,
			GameVersion,
			BotCount,
			MaxPlayer,
			MaxSpectator,
			Password,
			OtherSettingsCopy.JsonObject,
			THandler<FAccelByteModelsSessionBrowserData>::CreateLambda(
				[OnSuccess](FAccelByteModelsSessionBrowserData const& Response)
				{
					OnSuccess.ExecuteIfBound(Response);
				}
			),
			FErrorHandler::CreateLambda(
				[OnError](int32 Code, FString const& Message)
				{
					OnError.ExecuteIfBound(Code, Message);
				}
			)
		);
	}
	else
	{
		UE_LOG(LogAccelByteSessionBrowser, Warning, TEXT("Invalid SessionBrowser API from API Client"));
		OnError.ExecuteIfBound(static_cast<int32>(AccelByte::ErrorCodes::InvalidRequest), TEXT("SessionBrowser API already destroyed!"));
	}
}

void UABSessionBrowser::CreateGameSessionTypeSpecifiedByEnum(
	EAccelByteSessionType SessionType,
	FString const& GameMode,
	FString const& GameMapName,
	FString const& GameVersion,
	int32 BotCount,
	int32 MaxPlayer,
	int32 MaxSpectator,
	FString const& Password,
	FJsonObjectWrapper const& OtherSettings,
	FDModelsSessionBrowserDataResponse const& OnSuccess,
	FDErrorHandler const& OnError
)
{
	FJsonObjectWrapper OtherSettingsCopy = OtherSettings;
	OtherSettingsCopy.JsonObjectFromString(OtherSettingsCopy.JsonString);
	auto SessionBrowser = ApiClientPtr->GetSessionBrowserApi().Pin();
	if (SessionBrowser.IsValid())
	{
		SessionBrowser->CreateGameSession(
			SessionType,
			GameMode,
			GameMapName,
			GameVersion,
			BotCount,
			MaxPlayer,
			MaxSpectator,
			Password,
			OtherSettingsCopy.JsonObject,
			THandler<FAccelByteModelsSessionBrowserData>::CreateLambda(
				[OnSuccess](FAccelByteModelsSessionBrowserData const& Response)
				{
					OnSuccess.ExecuteIfBound(Response);
				}
			),
			FErrorHandler::CreateLambda(
				[OnError](int32 Code, FString const& Message)
				{
					OnError.ExecuteIfBound(Code, Message);
				}
			)
		);
	}
	else
	{
		UE_LOG(LogAccelByteSessionBrowser, Warning, TEXT("Invalid SessionBrowser API from API Client"));
		OnError.ExecuteIfBound(static_cast<int32>(AccelByte::ErrorCodes::InvalidRequest), TEXT("SessionBrowser API already destroyed!"));
	}
}

void UABSessionBrowser::CreateGameSessionTypeSpecificByStruct(
	FAccelByteModelsSessionBrowserCreateRequest const& CreateSessionRequest,
	FDModelsSessionBrowserDataResponse const& OnSuccess,
	FDErrorHandler const& OnError
)
{
	FAccelByteModelsSessionBrowserCreateRequest CreateSessionRequestCopy = CreateSessionRequest;
	CreateSessionRequestCopy.Game_session_setting.Settings.JsonObjectFromString(CreateSessionRequestCopy.Game_session_setting.Settings.JsonString);
	FAccelByteModelsSessionBrowserCreateRequest const& CreateSessionRequestConst = CreateSessionRequestCopy;
	auto SessionBrowser = ApiClientPtr->GetSessionBrowserApi().Pin();
	if (SessionBrowser.IsValid())
	{
		SessionBrowser->CreateGameSession(
			CreateSessionRequestConst,
			THandler<FAccelByteModelsSessionBrowserData>::CreateLambda(
				[OnSuccess](FAccelByteModelsSessionBrowserData const& Response)
				{
					OnSuccess.ExecuteIfBound(Response);
				}
			),
			FErrorHandler::CreateLambda(
				[OnError](int32 Code, FString const& Message)
				{
					OnError.ExecuteIfBound(Code, Message);
				}
			)
		);
	}
	else
	{
		UE_LOG(LogAccelByteSessionBrowser, Warning, TEXT("Invalid SessionBrowser API from API Client"));
		OnError.ExecuteIfBound(static_cast<int32>(AccelByte::ErrorCodes::InvalidRequest), TEXT("SessionBrowser API already destroyed!"));
	}
}

void UABSessionBrowser::UpdateGameSession(
	FString const& SessionId,
	int32 MaxPlayer,
	int32 CurrentPlayerCount,
	FDModelsSessionBrowserDataResponse const& OnSuccess,
	FDErrorHandler const& OnError
)
{
	auto SessionBrowser = ApiClientPtr->GetSessionBrowserApi().Pin();
	if (SessionBrowser.IsValid())
	{
		SessionBrowser->UpdateGameSession(
			SessionId,
			MaxPlayer,
			CurrentPlayerCount,
			THandler<FAccelByteModelsSessionBrowserData>::CreateLambda(
				[OnSuccess](FAccelByteModelsSessionBrowserData const& Response)
				{
					OnSuccess.ExecuteIfBound(Response);
				}
			),
			FErrorHandler::CreateLambda(
				[OnError](int32 Code, FString const& Message)
				{
					OnError.ExecuteIfBound(Code, Message);
				}
			)
		);
	}
	else
	{
		UE_LOG(LogAccelByteSessionBrowser, Warning, TEXT("Invalid SessionBrowser API from API Client"));
		OnError.ExecuteIfBound(static_cast<int32>(AccelByte::ErrorCodes::InvalidRequest), TEXT("SessionBrowser API already destroyed!"));
	}
}

void UABSessionBrowser::UpdateGameSessionByStruct(
	FString const& SessionId,
	FAccelByteModelsSessionBrowserUpdateRequest const& UpdateSessionRequest,
	FDModelsSessionBrowserDataResponse const& OnSuccess,
	FDErrorHandler const& OnError
)
{
	auto SessionBrowser = ApiClientPtr->GetSessionBrowserApi().Pin();
	if (SessionBrowser.IsValid())
	{
		SessionBrowser->UpdateGameSession(
			SessionId,
			UpdateSessionRequest,
			THandler<FAccelByteModelsSessionBrowserData>::CreateLambda(
				[OnSuccess](FAccelByteModelsSessionBrowserData const& Response)
				{
					OnSuccess.ExecuteIfBound(Response);
				}
			),
			FErrorHandler::CreateLambda(
				[OnError](int32 Code, FString const& Message)
				{
					OnError.ExecuteIfBound(Code, Message);
				}
			)
		);
	}
	else
	{
		UE_LOG(LogAccelByteSessionBrowser, Warning, TEXT("Invalid SessionBrowser API from API Client"));
		OnError.ExecuteIfBound(static_cast<int32>(AccelByte::ErrorCodes::InvalidRequest), TEXT("SessionBrowser API already destroyed!"));
	}
}

void UABSessionBrowser::UpdateGameSettings(
	FString const& SessionId,
	TMap<FString, FString> const& Settings,
	FDModelsSessionBrowserDataResponse const& OnSuccess,
	FDErrorHandler const& OnError
)
{
	auto SessionBrowser = ApiClientPtr->GetSessionBrowserApi().Pin();
	if (SessionBrowser.IsValid())
	{
		SessionBrowser->UpdateGameSettings(
			SessionId,
			Settings,
			THandler<FAccelByteModelsSessionBrowserData>::CreateLambda(
				[OnSuccess](FAccelByteModelsSessionBrowserData const& Response)
				{
					OnSuccess.ExecuteIfBound(Response);
				}
			),
			FErrorHandler::CreateLambda(
				[OnError](int32 Code, FString const& Message)
				{
					OnError.ExecuteIfBound(Code, Message);
				}
			)
		);
	}
	else
	{
		UE_LOG(LogAccelByteSessionBrowser, Warning, TEXT("Invalid SessionBrowser API from API Client"));
		OnError.ExecuteIfBound(static_cast<int32>(AccelByte::ErrorCodes::InvalidRequest), TEXT("SessionBrowser API already destroyed!"));
	}
}

void UABSessionBrowser::UpdateGameSettingsByJsonObject(
	FString const& SessionId,
	FJsonObjectWrapper const& Settings,
	FDModelsSessionBrowserDataResponse const& OnSuccess,
	FDErrorHandler const& OnError
)
{
	FJsonObjectWrapper SettingsCopy = Settings;
	SettingsCopy.JsonObjectFromString(SettingsCopy.JsonString);
	auto SessionBrowser = ApiClientPtr->GetSessionBrowserApi().Pin();
	if (SessionBrowser.IsValid())
	{
		SessionBrowser->UpdateGameSettings(
			SessionId,
			SettingsCopy.JsonObject,
			THandler<FAccelByteModelsSessionBrowserData>::CreateLambda(
				[OnSuccess](FAccelByteModelsSessionBrowserData const& Response)
				{
					OnSuccess.ExecuteIfBound(Response);
				}
			),
			FErrorHandler::CreateLambda(
				[OnError](int32 Code, FString const& Message)
				{
					OnError.ExecuteIfBound(Code, Message);
				}
			)
		);
	}
	else
	{
		UE_LOG(LogAccelByteSessionBrowser, Warning, TEXT("Invalid SessionBrowser API from API Client"));
		OnError.ExecuteIfBound(static_cast<int32>(AccelByte::ErrorCodes::InvalidRequest), TEXT("SessionBrowser API already destroyed!"));
	}
}

void UABSessionBrowser::RemoveGameSession(
	FString const& SessionId,
	FDModelsSessionBrowserDataResponse const& OnSuccess,
	FDErrorHandler const& OnError
)
{
	auto SessionBrowser = ApiClientPtr->GetSessionBrowserApi().Pin();
	if (SessionBrowser.IsValid())
	{
		SessionBrowser->RemoveGameSession(
			SessionId,
			THandler<FAccelByteModelsSessionBrowserData>::CreateLambda(
				[OnSuccess](FAccelByteModelsSessionBrowserData const& Response)
				{
					OnSuccess.ExecuteIfBound(Response);
				}
			),
			FErrorHandler::CreateLambda(
				[OnError](int32 Code, FString const& Message)
				{
					OnError.ExecuteIfBound(Code, Message);
				}
			)
		);
	}
	else
	{
		UE_LOG(LogAccelByteSessionBrowser, Warning, TEXT("Invalid SessionBrowser API from API Client"));
		OnError.ExecuteIfBound(static_cast<int32>(AccelByte::ErrorCodes::InvalidRequest), TEXT("SessionBrowser API already destroyed!"));
	}
}

void UABSessionBrowser::GetGameSessionsByTypeString(
	FString const& SessionTypeString,
	FString const& GameMode,
	FDModelsSessionBrowserGetResultResponse const& OnSuccess,
	FDErrorHandler const& OnError,
	int32 Offset,
	int32 Limit
)
{
	auto SessionBrowser = ApiClientPtr->GetSessionBrowserApi().Pin();
	if (SessionBrowser.IsValid())
	{
		SessionBrowser->GetGameSessions(
			SessionTypeString,
			GameMode,
			THandler<FAccelByteModelsSessionBrowserGetResult>::CreateLambda(
				[OnSuccess](FAccelByteModelsSessionBrowserGetResult const& Response)
				{
					OnSuccess.ExecuteIfBound(Response);
				}
			),
			FErrorHandler::CreateLambda(
				[OnError](int32 Code, FString const& Message)
				{
					OnError.ExecuteIfBound(Code, Message);
				}
			),
			Offset,
			Limit
		);
	}
	else
	{
		UE_LOG(LogAccelByteSessionBrowser, Warning, TEXT("Invalid SessionBrowser API from API Client"));
		OnError.ExecuteIfBound(static_cast<int32>(AccelByte::ErrorCodes::InvalidRequest), TEXT("SessionBrowser API already destroyed!"));
	}
}

void UABSessionBrowser::GetGameSessionsByTypeEnum(
	EAccelByteSessionType SessionType,
	FString const& GameMode,
	FDModelsSessionBrowserGetResultResponse const& OnSuccess,
	FDErrorHandler const& OnError,
	int32 Offset,
	int32 Limit
)
{
	auto SessionBrowser = ApiClientPtr->GetSessionBrowserApi().Pin();
	if (SessionBrowser.IsValid())
	{
		SessionBrowser->GetGameSessions(
			SessionType,
			GameMode,
			THandler<FAccelByteModelsSessionBrowserGetResult>::CreateLambda(
				[OnSuccess](FAccelByteModelsSessionBrowserGetResult const& Response)
				{
					OnSuccess.ExecuteIfBound(Response);
				}
			),
			FErrorHandler::CreateLambda(
				[OnError](int32 Code, FString const& Message)
				{
					OnError.ExecuteIfBound(Code, Message);
				}
			),
			Offset,
			Limit
		);
	}
	else
	{
		UE_LOG(LogAccelByteSessionBrowser, Warning, TEXT("Invalid SessionBrowser API from API Client"));
		OnError.ExecuteIfBound(static_cast<int32>(AccelByte::ErrorCodes::InvalidRequest), TEXT("SessionBrowser API already destroyed!"));
	}
}

void UABSessionBrowser::GetGameSessionsByTypeStringAndMatchExist(
	FString const& SessionTypeString,
	FString const& GameMode,
	FString const& MatchExist,
	FDModelsSessionBrowserGetResultResponse const& OnSuccess,
	FDErrorHandler const& OnError,
	int32 Offset,
	int32 Limit
)
{
	auto SessionBrowser = ApiClientPtr->GetSessionBrowserApi().Pin();
	if (SessionBrowser.IsValid())
	{
		SessionBrowser->GetGameSessions(
			SessionTypeString,
			GameMode,
			MatchExist,
			THandler<FAccelByteModelsSessionBrowserGetResult>::CreateLambda(
				[OnSuccess](FAccelByteModelsSessionBrowserGetResult const& Response)
				{
					OnSuccess.ExecuteIfBound(Response);
				}
			),
			FErrorHandler::CreateLambda(
				[OnError](int32 Code, FString const& Message)
				{
					OnError.ExecuteIfBound(Code, Message);
				}
			),
			Offset,
			Limit
		);
	}
	else
	{
		UE_LOG(LogAccelByteSessionBrowser, Warning, TEXT("Invalid SessionBrowser API from API Client"));
		OnError.ExecuteIfBound(static_cast<int32>(AccelByte::ErrorCodes::InvalidRequest), TEXT("SessionBrowser API already destroyed!"));
	}
}

void UABSessionBrowser::GetGameSessionsByTypeEnumAndMatchExist(
	EAccelByteSessionType SessionType,
	FString const& GameMode,
	FString const& MatchExist,
	FDModelsSessionBrowserGetResultResponse const& OnSuccess,
	FDErrorHandler const& OnError,
	int32 Offset,
	int32 Limit
)
{
	auto SessionBrowser = ApiClientPtr->GetSessionBrowserApi().Pin();
	if (SessionBrowser.IsValid())
	{
		SessionBrowser->GetGameSessions(
			SessionType,
			GameMode,
			MatchExist,
			THandler<FAccelByteModelsSessionBrowserGetResult>::CreateLambda(
				[OnSuccess](FAccelByteModelsSessionBrowserGetResult const& Response)
				{
					OnSuccess.ExecuteIfBound(Response);
				}
			),
			FErrorHandler::CreateLambda(
				[OnError](int32 Code, FString const& Message)
				{
					OnError.ExecuteIfBound(Code, Message);
				}
			),
			Offset,
			Limit
		);
	}
	else
	{
		UE_LOG(LogAccelByteSessionBrowser, Warning, TEXT("Invalid SessionBrowser API from API Client"));
		OnError.ExecuteIfBound(static_cast<int32>(AccelByte::ErrorCodes::InvalidRequest), TEXT("SessionBrowser API already destroyed!"));
	}
}

void UABSessionBrowser::GetGameSessionsByUserIds(TArray<FString> const& UserIds,
	FDModelsSessionBrowserGetResultByUserIdsResponse const& OnSuccess,
	FDErrorHandler const& OnError)
{
	auto SessionBrowser = ApiClientPtr->GetSessionBrowserApi().Pin();
	if (SessionBrowser.IsValid())
	{
		SessionBrowser->GetGameSessionsByUserIds(
			UserIds,
			THandler<FAccelByteModelsSessionBrowserGetByUserIdsResult>::CreateLambda(
				[OnSuccess](FAccelByteModelsSessionBrowserGetByUserIdsResult const& Response)
				{
					OnSuccess.ExecuteIfBound(Response);
				}
			),
			FErrorHandler::CreateLambda(
				[OnError](int32 Code, FString const& Message)
				{
					OnError.ExecuteIfBound(Code, Message);
				}
			)
		);
	}
	else
	{
		UE_LOG(LogAccelByteSessionBrowser, Warning, TEXT("Invalid SessionBrowser API from API Client"));
		OnError.ExecuteIfBound(static_cast<int32>(AccelByte::ErrorCodes::InvalidRequest), TEXT("SessionBrowser API already destroyed!"));
	}
}

void UABSessionBrowser::RegisterPlayer(
	FString const& SessionId,
	FString const& PlayerToAdd,
	bool AsSpectator,
	FDModelsSessionBrowserAddPlayerResponse const& OnSuccess,
	FDErrorHandler const& OnError
)
{
	auto SessionBrowser = ApiClientPtr->GetSessionBrowserApi().Pin();
	if (SessionBrowser.IsValid())
	{
		SessionBrowser->RegisterPlayer(
			SessionId,
			PlayerToAdd,
			AsSpectator,
			THandler<FAccelByteModelsSessionBrowserAddPlayerResponse>::CreateLambda(
				[OnSuccess](FAccelByteModelsSessionBrowserAddPlayerResponse const& Response)
				{
					OnSuccess.ExecuteIfBound(Response);
				}
			),
			FErrorHandler::CreateLambda(
				[OnError](int32 Code, FString const& Message)
				{
					OnError.ExecuteIfBound(Code, Message);
				}
			)
		);
	}
	else
	{
		UE_LOG(LogAccelByteSessionBrowser, Warning, TEXT("Invalid SessionBrowser API from API Client"));
		OnError.ExecuteIfBound(static_cast<int32>(AccelByte::ErrorCodes::InvalidRequest), TEXT("SessionBrowser API already destroyed!"));
	}
}

void UABSessionBrowser::UnregisterPlayer(
	FString const& SessionId,
	FString const& PlayerToRemove,
	FDModelsSessionBrowserAddPlayerResponse const& OnSuccess,
	FDErrorHandler const& OnError
)
{
	auto SessionBrowser = ApiClientPtr->GetSessionBrowserApi().Pin();
	if (SessionBrowser.IsValid())
	{
		SessionBrowser->UnregisterPlayer(
			SessionId,
			PlayerToRemove,
			THandler<FAccelByteModelsSessionBrowserAddPlayerResponse>::CreateLambda(
				[OnSuccess](FAccelByteModelsSessionBrowserAddPlayerResponse const& Response)
				{
					OnSuccess.ExecuteIfBound(Response);
				}
			),
			FErrorHandler::CreateLambda(
				[OnError](int32 Code, FString const& Message)
				{
					OnError.ExecuteIfBound(Code, Message);
				}
			)
		);
	}
	else
	{
		UE_LOG(LogAccelByteSessionBrowser, Warning, TEXT("Invalid SessionBrowser API from API Client"));
		OnError.ExecuteIfBound(static_cast<int32>(AccelByte::ErrorCodes::InvalidRequest), TEXT("SessionBrowser API already destroyed!"));
	}
}

void UABSessionBrowser::GetRecentPlayer(
	FString const& UserId,
	FDModelsSessionBrowserRecentPlayerGetResultResponse const& OnSuccess,
	FDErrorHandler const& OnError,
	int32 Offset,
	int32 Limit
)
{
	auto SessionBrowser = ApiClientPtr->GetSessionBrowserApi().Pin();
	if (SessionBrowser.IsValid())
	{
		SessionBrowser->GetRecentPlayer(
			UserId,
			THandler<FAccelByteModelsSessionBrowserRecentPlayerGetResult>::CreateLambda(
				[OnSuccess](FAccelByteModelsSessionBrowserRecentPlayerGetResult const& Response)
				{
					OnSuccess.ExecuteIfBound(Response);
				}
			),
			FErrorHandler::CreateLambda(
				[OnError](int32 Code, FString const& Message)
				{
					OnError.ExecuteIfBound(Code, Message);
				}
			),
			Offset,
			Limit
		);
	}
	else
	{
		UE_LOG(LogAccelByteSessionBrowser, Warning, TEXT("Invalid SessionBrowser API from API Client"));
		OnError.ExecuteIfBound(static_cast<int32>(AccelByte::ErrorCodes::InvalidRequest), TEXT("SessionBrowser API already destroyed!"));
	}
}

void UABSessionBrowser::JoinSession(
	FString const& SessionId,
	FString const& Password,
	FDModelsSessionBrowserDataResponse const& OnSuccess,
	FDErrorHandler const& OnError
)
{
	auto SessionBrowser = ApiClientPtr->GetSessionBrowserApi().Pin();
	if (SessionBrowser.IsValid())
	{
		SessionBrowser->JoinSession(
			SessionId,
			Password,
			THandler<FAccelByteModelsSessionBrowserData>::CreateLambda(
				[OnSuccess](FAccelByteModelsSessionBrowserData const& Response)
				{
					OnSuccess.ExecuteIfBound(Response);
				}
			),
			FErrorHandler::CreateLambda(
				[OnError](int32 Code, FString const& Message)
				{
					OnError.ExecuteIfBound(Code, Message);
				}
			)
		);
	}
	else
	{
		UE_LOG(LogAccelByteSessionBrowser, Warning, TEXT("Invalid SessionBrowser API from API Client"));
		OnError.ExecuteIfBound(static_cast<int32>(AccelByte::ErrorCodes::InvalidRequest), TEXT("SessionBrowser API already destroyed!"));
	}
}

void UABSessionBrowser::GetGameSession(
	FString const& SessionId,
	FDModelsSessionBrowserDataResponse const& OnSuccess,
	FDErrorHandler const& OnError
)
{
	auto SessionBrowser = ApiClientPtr->GetSessionBrowserApi().Pin();
	if (SessionBrowser.IsValid())
	{
		SessionBrowser->GetGameSession(
			SessionId,
			THandler<FAccelByteModelsSessionBrowserData>::CreateLambda(
				[OnSuccess](FAccelByteModelsSessionBrowserData const& Response)
				{
					OnSuccess.ExecuteIfBound(Response);
				}
			),
			FErrorHandler::CreateLambda(
				[OnError](int32 Code, FString const& Message)
				{
					OnError.ExecuteIfBound(Code, Message);
				}
			)
		);
	}
	else
	{
		UE_LOG(LogAccelByteSessionBrowser, Warning, TEXT("Invalid SessionBrowser API from API Client"));
		OnError.ExecuteIfBound(static_cast<int32>(AccelByte::ErrorCodes::InvalidRequest), TEXT("SessionBrowser API already destroyed!"));
	}
}

#endif