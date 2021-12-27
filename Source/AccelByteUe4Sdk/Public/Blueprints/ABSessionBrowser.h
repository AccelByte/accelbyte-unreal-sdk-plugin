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

DECLARE_DYNAMIC_DELEGATE_OneParam(FDModelsSessionBrowserAddPlayerResponse, FAccelByteModelsSessionBrowserAddPlayerResponse, Response);

DECLARE_DYNAMIC_DELEGATE_OneParam(FDModelsSessionBrowserRecentPlayerGetResultResponse, FAccelByteModelsSessionBrowserRecentPlayerGetResult, Response);

#pragma endregion

UCLASS(BlueprintType, Blueprintable)
class UABSessionBrowser : public UObject
{
	GENERATED_BODY()
	
public:
	void SetApiClient(FApiClientPtr const& NewApiClientPtr);

	/*UFUNCTION(BlueprintCallable, Category = "AccelByte | SessinBrowser")
	int JsonTest(
		FJsonObjectWrapper OtherSettings
	)
	{
		auto SettingJson = MakeShared<FJsonObject>();
		SettingJson->SetStringField(TEXT("CUSTOM1"), TEXT("CUSTOM1 UwU"));
		SettingJson->SetNumberField(TEXT("CUSTOM2"), 20);

		return (*SettingJson).GetStringField("CUSTOM1");
		OtherSettings.JsonObjectFromString(OtherSettings.JsonString);
		return (*SettingJson).GetIntegerField("CUSTOM2");
	}*/

	UFUNCTION(BlueprintCallable, Category = "AccelByte | SessinBrowser")
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
	
	UFUNCTION(BlueprintCallable, Category = "AccelByte | SessinBrowser")
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
	
	UFUNCTION(BlueprintCallable, Category = "AccelByte | SessinBrowser")
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

	UFUNCTION(BlueprintCallable, Category = "AccelByte | SessinBrowser")
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

	UFUNCTION(BlueprintCallable, Category = "AccelByte | SessinBrowser")
	void CreateGameSessionTypeSpecificByStruct(
		FAccelByteModelsSessionBrowserCreateRequest CreateSessionRequest,
		FDModelsSessionBrowserDataResponse const& OnSuccess,
		FDErrorHandler const& OnError
	);

	UFUNCTION(BlueprintCallable, Category = "AccelByte | SessinBrowser")
	void UpdateGameSession(
		FString const& SessionId,
		int32 MaxPlayer,
		int32 CurrentPlayerCount,
		FDModelsSessionBrowserDataResponse const& OnSuccess,
		FDErrorHandler const& OnError
	);

	UFUNCTION(BlueprintCallable, Category = "AccelByte | SessinBrowser")
	void UpdateGameSessionByStruct(
		FString const& SessionId,
		FAccelByteModelsSessionBrowserUpdateRequest const& UpdateSessionRequest,
		FDModelsSessionBrowserDataResponse const& OnSuccess,
		FDErrorHandler const& OnError
	);

	UFUNCTION(BlueprintCallable, Category = "AccelByte | SessinBrowser")
	void RemoveGameSession(
		FString const& SessionId,
		FDModelsSessionBrowserDataResponse const& OnSuccess,
		FDErrorHandler const& OnError
	);

	UFUNCTION(BlueprintCallable, Category = "AccelByte | SessinBrowser")
	void GetGameSessionsByTypeString(
		FString const& SessionTypeString,
		FString const& GameMode,
		FDModelsSessionBrowserGetResultResponse const& OnSuccess,
		FDErrorHandler const& OnError,
		int32 Offset = 0,
		int32 Limit = 10
	);

	UFUNCTION(BlueprintCallable, Category = "AccelByte | SessinBrowser")
	void GetGameSessionsByTypeEnum(
		EAccelByteSessionType SessionType,
		FString const& GameMode,
		FDModelsSessionBrowserGetResultResponse const& OnSuccess,
		FDErrorHandler const& OnError,
		int32 Offset = 0,
		int32 Limit = 10
	);

	UFUNCTION(BlueprintCallable, Category = "AccelByte | SessinBrowser")
	void GetGameSessionsByTypeStringAndMatchExist(
		FString const& SessionTypeString,
		FString const& GameMode,
		FString const& MatchExist,
		FDModelsSessionBrowserGetResultResponse const& OnSuccess,
		FDErrorHandler const& OnError,
		int32 Offset = 0,
		int32 Limit = 10
	);

	UFUNCTION(BlueprintCallable, Category = "AccelByte | SessinBrowser")
	void GetGameSessionsByTypeEnumAndMatchExist(
		EAccelByteSessionType SessionType,
		FString const& GameMode,
		FString const& MatchExist,
		FDModelsSessionBrowserGetResultResponse const& OnSuccess,
		FDErrorHandler const& OnError,
		int32 Offset = 0,
		int32 Limit = 10
	);

	UFUNCTION(BlueprintCallable, Category = "AccelByte | SessinBrowser")
	void RegisterPlayer(
		FString const& SessionId,
		FString const& PlayerToAdd,
		bool AsSpectator,
		FDModelsSessionBrowserAddPlayerResponse const& OnSuccess,
		FDErrorHandler const& OnError
	);

	UFUNCTION(BlueprintCallable, Category = "AccelByte | SessinBrowser")
	void UnregisterPlayer(
		FString const& SessionId,
		FString const& PlayerToRemove,
		FDModelsSessionBrowserAddPlayerResponse const& OnSuccess,
		FDErrorHandler const& OnError
	);

	UFUNCTION(BlueprintCallable, Category = "AccelByte | SessinBrowser")
	void GetRecentPlayer(
		FString const& UserId,
		FDModelsSessionBrowserRecentPlayerGetResultResponse const& OnSuccess,
		FDErrorHandler const& OnError,
		int32 Offset = 0,
		int32 Limit = 10
	);

	UFUNCTION(BlueprintCallable, Category = "AccelByte | SessinBrowser")
	void JoinSession(
		FString const& SessionId,
		FString const& Password,
		FDModelsSessionBrowserDataResponse const& OnSuccess,
		FDErrorHandler const& OnError
	);
	
	UFUNCTION(BlueprintCallable, Category = "AccelByte | SessinBrowser")
	void GetGameSession(
		FString const& SessionId,
		FDModelsSessionBrowserDataResponse const& OnSuccess,
		FDErrorHandler const& OnError
	);
	
private:
	FApiClientPtr ApiClientPtr;
};
