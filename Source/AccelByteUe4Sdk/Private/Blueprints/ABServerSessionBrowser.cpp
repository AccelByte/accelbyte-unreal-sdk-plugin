// Copyright (c) 2021 AccelByte, Inc. All rights reserved.
// This is licensed software from Accelbyte Inc, for limitations
// and restrictions contact your company contract manager

#include "Blueprints/ABServerSessionBrowser.h"

void UABServerSessionBrowser::SetServerApiClient(FServerApiClientPtr NewServerApiClientPtr)
{
	ApiClientPtr = NewServerApiClientPtr;
}

void UABServerSessionBrowser::CreateGameSessionPublic(
	FString const& GameMode,
	FString const& GameMapName,
	FString const& GameVersion,
	int32 BotCount,
	int32 MaxPlayer,
	FJsonObjectWrapper OtherSettings,
	FDServerSessionBrowserDataResponse const& OnSuccess,
	FDErrorHandler const& OnError
)
{
	OtherSettings.JsonObjectFromString(OtherSettings.JsonString);
	ApiClientPtr->ServerSessionBrowser.CreateGameSession(
		GameMode,
		GameMapName,
		GameVersion,
		BotCount,
		MaxPlayer,
		OtherSettings.JsonObject,
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

void UABServerSessionBrowser::CreateGameSessionPrivate(
	FString const& GameMode,
	FString const& GameMapName,
	FString const& GameVersion,
	int32 BotCount,
	int32 MaxPlayer,
	int32 MaxSpectator,
	FString const& Password,
	FJsonObjectWrapper OtherSettings,
	FDServerSessionBrowserDataResponse const& OnSuccess,
	FDErrorHandler const& OnError
)
{
	OtherSettings.JsonObjectFromString(OtherSettings.JsonString);
	ApiClientPtr->ServerSessionBrowser.CreateGameSession(
		GameMode,
		GameMapName,
		GameVersion,
		BotCount,
		MaxPlayer,
		MaxSpectator,
		Password,
		OtherSettings.JsonObject,
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

void UABServerSessionBrowser::CreateGameSessionTypeSpecifiedByString(
	FString const& SessionTypeString,
	FString const& GameMode,
	FString const& GameMapName,
	FString const& GameVersion,
	int32 BotCount,
	int32 MaxPlayer,
	int32 MaxSpectator,
	FString const& Password,
	FJsonObjectWrapper OtherSettings,
	FDServerSessionBrowserDataResponse const& OnSuccess,
	FDErrorHandler const& OnError
)
{
	OtherSettings.JsonObjectFromString(OtherSettings.JsonString);
	ApiClientPtr->ServerSessionBrowser.CreateGameSession(
		SessionTypeString,
		GameMode,
		GameMapName,
		GameVersion,
		BotCount,
		MaxPlayer,
		MaxSpectator,
		Password,
		OtherSettings.JsonObject,
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

void UABServerSessionBrowser::CreateGameSessionTypeSpecifiedByEnum(
	EAccelByteSessionType SessionType,
	FString const& GameMode,
	FString const& GameMapName,
	FString const& GameVersion,
	int32 BotCount,
	int32 MaxPlayer,
	int32 MaxSpectator,
	FString const& Password,
	FJsonObjectWrapper OtherSettings,
	FDServerSessionBrowserDataResponse const& OnSuccess,
	FDErrorHandler const& OnError
)
{
	OtherSettings.JsonObjectFromString(OtherSettings.JsonString);
	ApiClientPtr->ServerSessionBrowser.CreateGameSession(
		SessionType,
		GameMode,
		GameMapName,
		GameVersion,
		BotCount,
		MaxPlayer,
		MaxSpectator,
		Password,
		OtherSettings.JsonObject,
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

void UABServerSessionBrowser::CreateGameSessionTypeSpecificByStruct(
	FAccelByteModelsSessionBrowserCreateRequest CreateSessionRequest,
	FDServerSessionBrowserDataResponse const& OnSuccess,
	FDErrorHandler const& OnError
)
{
	CreateSessionRequest.Game_session_setting.Settings.JsonObjectFromString(CreateSessionRequest.Game_session_setting.Settings.JsonString);
	FAccelByteModelsSessionBrowserCreateRequest const& CreateSessionRequestConst = CreateSessionRequest;
	ApiClientPtr->ServerSessionBrowser.CreateGameSession(
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

void UABServerSessionBrowser::UpdateGameSession(
	FString const& SessionId,
	int32 MaxPlayer,
	int32 CurrentPlayerCount,
	FDServerSessionBrowserDataResponse const& OnSuccess,
	FDErrorHandler const& OnError
)
{
	ApiClientPtr->ServerSessionBrowser.UpdateGameSession(
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

void UABServerSessionBrowser::UpdateGameSessionByStruct(
	FString const& SessionId,
	FAccelByteModelsSessionBrowserUpdateRequest const& UpdateSessionRequest,
	FDServerSessionBrowserDataResponse const& OnSuccess,
	FDErrorHandler const& OnError
)
{
	ApiClientPtr->ServerSessionBrowser.UpdateGameSession(
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

void UABServerSessionBrowser::RemoveGameSession(
	FString const& SessionId,
	FDServerSessionBrowserDataResponse const& OnSuccess,
	FDErrorHandler const& OnError
)
{
	ApiClientPtr->ServerSessionBrowser.RemoveGameSession(
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

void UABServerSessionBrowser::GetGameSessionsByTypeString(
	FString const& SessionTypeString,
	FString const& GameMode,
	FDServerSessionBrowserGetResultResponse const& OnSuccess,
	FDErrorHandler const& OnError,
	int32 Offset,
	int32 Limit
)
{
	ApiClientPtr->ServerSessionBrowser.GetGameSessions(
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

void UABServerSessionBrowser::GetGameSessionsByTypeEnum(
	EAccelByteSessionType SessionType,
	FString const& GameMode,
	FDServerSessionBrowserGetResultResponse const& OnSuccess,
	FDErrorHandler const& OnError,
	int32 Offset,
	int32 Limit
)
{
	ApiClientPtr->ServerSessionBrowser.GetGameSessions(
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

void UABServerSessionBrowser::GetGameSessionsByTypeStringAndMatchExist(
	FString const& SessionTypeString,
	FString const& GameMode,
	FString const& MatchExist,
	FDServerSessionBrowserGetResultResponse const& OnSuccess,
	FDErrorHandler const& OnError,
	int32 Offset,
	int32 Limit
)
{
	ApiClientPtr->ServerSessionBrowser.GetGameSessions(
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

void UABServerSessionBrowser::GetGameSessionsByTypeEnumAndMatchExist(
	EAccelByteSessionType SessionType,
	FString const& GameMode,
	FString const& MatchExist,
	FDServerSessionBrowserGetResultResponse const& OnSuccess,
	FDErrorHandler const& OnError,
	int32 Offset,
	int32 Limit
)
{
	ApiClientPtr->ServerSessionBrowser.GetGameSessions(
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

void UABServerSessionBrowser::RegisterPlayer(
	FString const& SessionId,
	FString const& PlayerToAdd,
	bool AsSpectator,
	FDServerSessionBrowserAddPlayerResponse const& OnSuccess,
	FDErrorHandler const& OnError
)
{
	ApiClientPtr->ServerSessionBrowser.RegisterPlayer(
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

void UABServerSessionBrowser::UnregisterPlayer(
	FString const& SessionId,
	FString const& PlayerToRemove,
	FDServerSessionBrowserAddPlayerResponse const& OnSuccess,
	FDErrorHandler const& OnError
)
{
	ApiClientPtr->ServerSessionBrowser.UnregisterPlayer(
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

void UABServerSessionBrowser::GetRecentPlayer(
	FString const& UserId,
	FDServerSessionBrowserRecentPlayerGetResultResponse const& OnSuccess,
	FDErrorHandler const& OnError,
	int32 Offset,
	int32 Limit
)
{
	ApiClientPtr->ServerSessionBrowser.GetRecentPlayer(
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

void UABServerSessionBrowser::JoinSession(
	FString const& SessionId,
	FString const& Password,
	FDServerSessionBrowserDataResponse const& OnSuccess,
	FDErrorHandler const& OnError
)
{
	ApiClientPtr->ServerSessionBrowser.JoinSession(
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