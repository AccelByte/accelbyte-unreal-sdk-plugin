// Copyright (c) 2019 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Models/AccelByteGameProfileModels.h"
#include "AccelByteGameProfileBlueprints.generated.h"

UCLASS(Blueprintable, BlueprintType)
class UAccelByteBlueprintsGameProfile : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	DECLARE_DYNAMIC_DELEGATE_TwoParams(FBlueprintErrorHandler, int32, ErrorCode, const FString&, ErrorMessage);

	DECLARE_DYNAMIC_DELEGATE_OneParam(FBatchGetPublicGameProfilesSuccess, const TArray<FAccelByteModelsPublicGameProfile>&, Result);
	UFUNCTION(BlueprintCallable, Category = "AccelByte | GameProfile | Api")
	static void BatchGetPublicGameProfiles(TArray<FString> UserIds, const FBatchGetPublicGameProfilesSuccess& OnSuccess, const FBlueprintErrorHandler& OnError);

	DECLARE_DYNAMIC_DELEGATE_OneParam(FGetAllGameProfilesSuccess, const TArray<FAccelByteModelsGameProfile>&, Result);
	UFUNCTION(BlueprintCallable, Category = "AccelByte | GameProfile | Api")
	static void GetAllGameProfiles(const FGetAllGameProfilesSuccess& OnSuccess, const FBlueprintErrorHandler& OnError);

	DECLARE_DYNAMIC_DELEGATE_OneParam(FCreateGameProfileSuccess, const FAccelByteModelsGameProfile&, Result);
	UFUNCTION(BlueprintCallable, Category = "AccelByte | GameProfile | Api")
	static void CreateGameProfile(const FAccelByteModelsGameProfileRequest& GameProfileRequest, const FCreateGameProfileSuccess& OnSuccess, const FBlueprintErrorHandler& OnError);

	DECLARE_DYNAMIC_DELEGATE_OneParam(FGetGameProfileSuccess, const FAccelByteModelsGameProfile&, Result);
	UFUNCTION(BlueprintCallable, Category = "AccelByte | GameProfile | Api")
	static void GetGameProfile(const FString& ProfileId, const FGetGameProfileSuccess& OnSuccess, const FBlueprintErrorHandler& OnError);

	DECLARE_DYNAMIC_DELEGATE_OneParam(FUpdateGameProfileSuccess, const FAccelByteModelsGameProfile&, Result);
	UFUNCTION(BlueprintCallable, Category = "AccelByte | GameProfile | Api")
	static void UpdateGameProfile(const FString& ProfileId, const FAccelByteModelsGameProfileRequest& GameProfileRequest, const FUpdateGameProfileSuccess& OnSuccess, const FBlueprintErrorHandler& OnError);

	DECLARE_DYNAMIC_DELEGATE(FDeleteGameProfileSuccess);
	UFUNCTION(BlueprintCallable, Category = "AccelByte | GameProfile | Api")
	static void DeleteGameProfile(const FString& ProfileId, const FDeleteGameProfileSuccess& OnSuccess, const FBlueprintErrorHandler& OnError);

	DECLARE_DYNAMIC_DELEGATE_OneParam(FGetGameProfileAttributeSuccess, const FAccelByteModelsGameProfileAttribute&, Result);
	UFUNCTION(BlueprintCallable, Category = "AccelByte | GameProfile | Api")
	static void GetGameProfileAttribute(const FString& ProfileId, const FString& AttributeName, const FGetGameProfileAttributeSuccess& OnSuccess, const FBlueprintErrorHandler& OnError);

	DECLARE_DYNAMIC_DELEGATE_OneParam(FUpdateGameProfileAttributeSuccess, const FAccelByteModelsGameProfile&, Result);
	UFUNCTION(BlueprintCallable, Category = "AccelByte | GameProfile | Api")
	static void UpdateGameProfileAttribute(const FString& ProfileId, const FAccelByteModelsGameProfileAttribute& Attribute, const FUpdateGameProfileAttributeSuccess& OnSuccess, const FBlueprintErrorHandler& OnError);

};

