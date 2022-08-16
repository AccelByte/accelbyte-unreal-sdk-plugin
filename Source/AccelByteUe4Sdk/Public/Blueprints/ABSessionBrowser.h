// Copyright (c) 2021 AccelByte, Inc. All rights reserved.
// This is licensed software from Accelbyte Inc, for limitations
// and restrictions contact your company contract manager

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Core/AccelByteMultiRegistry.h"
#include "Models/AccelByteSessionBrowserModels.h"
#include "JsonObjectWrapper.h"
#include "ABSessionBrowser.generated.h"

using namespace AccelByte;

#pragma region MODEL_AND_DELEGATE_FOR_REQUEST_RESPONSE

DECLARE_DYNAMIC_DELEGATE_OneParam(FDModelsSessionBrowserDataResponse, FAccelByteModelsSessionBrowserData, Response);

DECLARE_DYNAMIC_DELEGATE_OneParam(FDModelsSessionBrowserGetResultResponse, FAccelByteModelsSessionBrowserGetResult, Response);

DECLARE_DYNAMIC_DELEGATE_OneParam(FDModelsSessionBrowserGetResultByUserIdsResponse, FAccelByteModelsSessionBrowserGetByUserIdsResult, Response);

DECLARE_DYNAMIC_DELEGATE_OneParam(FDModelsSessionBrowserAddPlayerResponse, FAccelByteModelsSessionBrowserAddPlayerResponse, Response);

DECLARE_DYNAMIC_DELEGATE_OneParam(FDModelsSessionBrowserRecentPlayerGetResultResponse, FAccelByteModelsSessionBrowserRecentPlayerGetResult, Response);

#pragma endregion

UCLASS(BlueprintType, Blueprintable)
class UABSessionBrowser : public UObject
{
	GENERATED_BODY()
	
public:
	void SetApiClient(FApiClientPtr const& NewApiClientPtr);

	UFUNCTION(BlueprintCallable, Category = "AccelByte | SessionBrowser | Api")
	void CreateGameSessionPublic(
		FString const& GameMode,
		FString const& GameMapName,
		FString const& GameVersion,
		int32 BotCount,
		int32 MaxPlayer,
		FJsonObjectWrapper OtherSettings,
		FDModelsSessionBrowserDataResponse const& OnSuccess,
		FDErrorHandler const& OnError
	);
	
	UFUNCTION(BlueprintCallable, Category = "AccelByte | SessionBrowser | Api")
	void CreateGameSessionPrivate(
		FString const& GameMode,
		FString const& GameMapName,
		FString const& GameVersion,
		int32 BotCount,
		int32 MaxPlayer,
		int32 MaxSpectator,
		FString const& Password,
		FJsonObjectWrapper OtherSettings,
		FDModelsSessionBrowserDataResponse const& OnSuccess,
		FDErrorHandler const& OnError
	);
	
	UFUNCTION(BlueprintCallable, Category = "AccelByte | SessionBrowser | Api")
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
		FDModelsSessionBrowserDataResponse const& OnSuccess,
		FDErrorHandler const& OnError
	);

	UFUNCTION(BlueprintCallable, Category = "AccelByte | SessionBrowser | Api")
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
		FDModelsSessionBrowserDataResponse const& OnSuccess,
		FDErrorHandler const& OnError
	);

	UFUNCTION(BlueprintCallable, Category = "AccelByte | SessionBrowser | Api")
	void CreateGameSessionTypeSpecificByStruct(
		FAccelByteModelsSessionBrowserCreateRequest CreateSessionRequest,
		FDModelsSessionBrowserDataResponse const& OnSuccess,
		FDErrorHandler const& OnError
	);

	UFUNCTION(BlueprintCallable, Category = "AccelByte | SessionBrowser | Api")
	void UpdateGameSession(
		FString const& SessionId,
		int32 MaxPlayer,
		int32 CurrentPlayerCount,
		FDModelsSessionBrowserDataResponse const& OnSuccess,
		FDErrorHandler const& OnError
	);

	UFUNCTION(BlueprintCallable, Category = "AccelByte | SessionBrowser | Api")
	void UpdateGameSessionByStruct(
		FString const& SessionId,
		FAccelByteModelsSessionBrowserUpdateRequest const& UpdateSessionRequest,
		FDModelsSessionBrowserDataResponse const& OnSuccess,
		FDErrorHandler const& OnError
	);

	UFUNCTION(BlueprintCallable, Category = "AccelByte | SessionBrowser | Api")
	void UpdateGameSettings(
		FString const& SessionId,
		TMap<FString, FString> Settings,
		FDModelsSessionBrowserDataResponse const& OnSuccess,
		FDErrorHandler const& OnError
	);

	UFUNCTION(BlueprintCallable, Category = "AccelByte | SessionBrowser | Api")
	void UpdateGameSettingsByJsonObject(
		FString const& SessionId,
		FJsonObjectWrapper Settings,
		FDModelsSessionBrowserDataResponse const& OnSuccess,
		FDErrorHandler const& OnError
	);

	UFUNCTION(BlueprintCallable, Category = "AccelByte | SessionBrowser | Api")
	void RemoveGameSession(
		FString const& SessionId,
		FDModelsSessionBrowserDataResponse const& OnSuccess,
		FDErrorHandler const& OnError
	);

	UFUNCTION(BlueprintCallable, Category = "AccelByte | SessionBrowser | Api")
	void GetGameSessionsByTypeString(
		FString const& SessionTypeString,
		FString const& GameMode,
		FDModelsSessionBrowserGetResultResponse const& OnSuccess,
		FDErrorHandler const& OnError,
		int32 Offset = 0,
		int32 Limit = 10
	);

	UFUNCTION(BlueprintCallable, Category = "AccelByte | SessionBrowser | Api")
	void GetGameSessionsByTypeEnum(
		EAccelByteSessionType SessionType,
		FString const& GameMode,
		FDModelsSessionBrowserGetResultResponse const& OnSuccess,
		FDErrorHandler const& OnError,
		int32 Offset = 0,
		int32 Limit = 10
	);

	UFUNCTION(BlueprintCallable, Category = "AccelByte | SessionBrowser | Api")
	void GetGameSessionsByTypeStringAndMatchExist(
		FString const& SessionTypeString,
		FString const& GameMode,
		FString const& MatchExist,
		FDModelsSessionBrowserGetResultResponse const& OnSuccess,
		FDErrorHandler const& OnError,
		int32 Offset = 0,
		int32 Limit = 10
	);

	UFUNCTION(BlueprintCallable, Category = "AccelByte | SessionBrowser | Api")
	void GetGameSessionsByTypeEnumAndMatchExist(
		EAccelByteSessionType SessionType,
		FString const& GameMode,
		FString const& MatchExist,
		FDModelsSessionBrowserGetResultResponse const& OnSuccess,
		FDErrorHandler const& OnError,
		int32 Offset = 0,
		int32 Limit = 10
	);

	UFUNCTION(BlueprintCallable, Category = "AccelByte | SessionBrowser | Api")
	void GetGameSessionsByUserIds(
		TArray<FString> UserIds,
		FDModelsSessionBrowserGetResultByUserIdsResponse const& OnSuccess,
		FDErrorHandler const& OnError
	);

	UFUNCTION(BlueprintCallable, Category = "AccelByte | SessionBrowser | Api")
	void RegisterPlayer(
		FString const& SessionId,
		FString const& PlayerToAdd,
		bool AsSpectator,
		FDModelsSessionBrowserAddPlayerResponse const& OnSuccess,
		FDErrorHandler const& OnError
	);

	UFUNCTION(BlueprintCallable, Category = "AccelByte | SessionBrowser | Api")
	void UnregisterPlayer(
		FString const& SessionId,
		FString const& PlayerToRemove,
		FDModelsSessionBrowserAddPlayerResponse const& OnSuccess,
		FDErrorHandler const& OnError
	);

	UFUNCTION(BlueprintCallable, Category = "AccelByte | SessionBrowser | Api")
	void GetRecentPlayer(
		FString const& UserId,
		FDModelsSessionBrowserRecentPlayerGetResultResponse const& OnSuccess,
		FDErrorHandler const& OnError,
		int32 Offset = 0,
		int32 Limit = 10
	);

	UFUNCTION(BlueprintCallable, Category = "AccelByte | SessionBrowser | Api")
	void JoinSession(
		FString const& SessionId,
		FString const& Password,
		FDModelsSessionBrowserDataResponse const& OnSuccess,
		FDErrorHandler const& OnError
	);
	
	UFUNCTION(BlueprintCallable, Category = "AccelByte | SessionBrowser | Api")
	void GetGameSession(
		FString const& SessionId,
		FDModelsSessionBrowserDataResponse const& OnSuccess,
		FDErrorHandler const& OnError
	);
	
private:
	FApiClientPtr ApiClientPtr;
};
