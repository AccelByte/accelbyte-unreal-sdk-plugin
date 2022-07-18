// Copyright (c) 2021 AccelByte, Inc. All rights reserved.
// This is licensed software from Accelbyte Inc, for limitations
// and restrictions contact your company contract manager

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Core/AccelByteMultiRegistry.h"
#include "Models/AccelByteSessionBrowserModels.h"
#include "AccelByteUe4Sdk/Public/Core/AccelByteError.h"
#include "JsonObjectWrapper.h"
#include "ABServerSessionBrowser.generated.h"

using namespace AccelByte;
using namespace AccelByte::Api;
using namespace AccelByte::GameServerApi;

#pragma region MODEL_AND_DELEGATE_FOR_REQUEST_RESPONSE

DECLARE_DYNAMIC_DELEGATE_OneParam(FDServerSessionBrowserDataResponse, FAccelByteModelsSessionBrowserData, Response);

DECLARE_DYNAMIC_DELEGATE_OneParam(FDServerSessionBrowserGetResultResponse, FAccelByteModelsSessionBrowserGetResult, Response);

DECLARE_DYNAMIC_DELEGATE_OneParam(FDServerSessionBrowserAddPlayerResponse, FAccelByteModelsSessionBrowserAddPlayerResponse, Response);

DECLARE_DYNAMIC_DELEGATE_OneParam(FDServerSessionBrowserRecentPlayerGetResultResponse, FAccelByteModelsSessionBrowserRecentPlayerGetResult, Response);

#pragma endregion

UCLASS(BlueprintType, Blueprintable)
class UABServerSessionBrowser final : public UObject
{
	GENERATED_BODY()
	
public:
	void SetServerApiClient(FServerApiClientPtr NewServerApiClientPtr);

	UFUNCTION(BlueprintCallable, Category = "AccelByte | Server | SessionBrowser | Api")
	void CreateGameSessionPublic(
		FString const& GameMode,
		FString const& GameMapName,
		FString const& GameVersion,
		int32 BotCount,
		int32 MaxPlayer,
		FJsonObjectWrapper OtherSettings,
		FDServerSessionBrowserDataResponse const& OnSuccess,
		FDErrorHandler const& OnError
	);
	
	UFUNCTION(BlueprintCallable, Category = "AccelByte | Server | SessionBrowser | Api")
	void CreateGameSessionPrivate(
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
	);
	
	UFUNCTION(BlueprintCallable, Category = "AccelByte | Server | SessionBrowser | Api")
	void CreateGameSessionTypeSpecifiedByString(
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
	);
	
	UFUNCTION(BlueprintCallable, Category = "AccelByte | Server | SessionBrowser | Api")
	void CreateGameSessionTypeSpecifiedByEnum(
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
	);
	
	UFUNCTION(BlueprintCallable, Category = "AccelByte | Server | SessionBrowser | Api")
	void CreateGameSessionTypeSpecificByStruct(
		FAccelByteModelsSessionBrowserCreateRequest CreateSessionRequest,
		FDServerSessionBrowserDataResponse const& OnSuccess,
		FDErrorHandler const& OnError
	);
	
	UFUNCTION(BlueprintCallable, Category = "AccelByte | Server | SessionBrowser | Api")
	void UpdateGameSession(
		FString const& SessionId,
		int32 MaxPlayer,
		int32 CurrentPlayerCount,
		FDServerSessionBrowserDataResponse const& OnSuccess,
		FDErrorHandler const& OnError
	);
	
	UFUNCTION(BlueprintCallable, Category = "AccelByte | Server | SessionBrowser | Api")
	void UpdateGameSessionByStruct(
		FString const& SessionId,
		FAccelByteModelsSessionBrowserUpdateRequest const& UpdateSessionRequest,
		FDServerSessionBrowserDataResponse const& OnSuccess,
		FDErrorHandler const& OnError
	);
	
	UFUNCTION(BlueprintCallable, Category = "AccelByte | Server | SessionBrowser | Api")
	void RemoveGameSession(
		FString const& SessionId,
		FDServerSessionBrowserDataResponse const& OnSuccess,
		FDErrorHandler const& OnError
	);
	
	UFUNCTION(BlueprintCallable, Category = "AccelByte | Server | SessionBrowser | Api")
	void GetGameSessionsByTypeString(
		FString const& SessionTypeString,
		FString const& GameMode,
		FDServerSessionBrowserGetResultResponse const& OnSuccess,
		FDErrorHandler const& OnError,
		int32 Offset = 0,
		int32 Limit = 10
	);
	
	UFUNCTION(BlueprintCallable, Category = "AccelByte | Server | SessionBrowser | Api")
	void GetGameSessionsByTypeEnum(
		EAccelByteSessionType SessionType,
		FString const& GameMode,
		FDServerSessionBrowserGetResultResponse const& OnSuccess,
		FDErrorHandler const& OnError,
		int32 Offset = 0,
		int32 Limit = 10
	);
	
	UFUNCTION(BlueprintCallable, Category = "AccelByte | Server | SessionBrowser | Api")
	void GetGameSessionsByTypeStringAndMatchExist(
		FString const& SessionTypeString,
		FString const& GameMode,
		FString const& MatchExist,
		FDServerSessionBrowserGetResultResponse const& OnSuccess,
		FDErrorHandler const& OnError,
		int32 Offset = 0,
		int32 Limit = 10
	);
	
	UFUNCTION(BlueprintCallable, Category = "AccelByte | Server | SessionBrowser | Api")
	void GetGameSessionsByTypeEnumAndMatchExist(
		EAccelByteSessionType SessionType,
		FString const& GameMode,
		FString const& MatchExist,
		FDServerSessionBrowserGetResultResponse const& OnSuccess,
		FDErrorHandler const& OnError,
		int32 Offset = 0,
		int32 Limit = 10
	);
	
	UFUNCTION(BlueprintCallable, Category = "AccelByte | Server | SessionBrowser | Api")
	void RegisterPlayer(
		FString const& SessionId,
		FString const& PlayerToAdd,
		bool AsSpectator,
		FDServerSessionBrowserAddPlayerResponse const& OnSuccess,
		FDErrorHandler const& OnError
	);
	
	UFUNCTION(BlueprintCallable, Category = "AccelByte | Server | SessionBrowser | Api")
	void UnregisterPlayer(
		FString const& SessionId,
		FString const& PlayerToRemove,
		FDServerSessionBrowserAddPlayerResponse const& OnSuccess,
		FDErrorHandler const& OnError
	);
	
	UFUNCTION(BlueprintCallable, Category = "AccelByte | Server | SessionBrowser | Api")
	void GetRecentPlayer(
		FString const& UserId,
		FDServerSessionBrowserRecentPlayerGetResultResponse const& OnSuccess,
		FDErrorHandler const& OnError,
		int32 Offset = 0,
		int32 Limit = 10
	);
	
	UFUNCTION(BlueprintCallable, Category = "AccelByte | Server | SessionBrowser | Api")
	void JoinSession(
		FString const& SessionId,
		FString const& Password,
		FDServerSessionBrowserDataResponse const& OnSuccess,
		FDErrorHandler const& OnError
	);

private:
	FServerApiClientPtr ApiClientPtr;
};
