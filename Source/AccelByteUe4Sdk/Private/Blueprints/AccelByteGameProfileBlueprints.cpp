// Copyright (c) 2019 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "Blueprints/AccelByteGameProfileBlueprints.h"
#include "Api/AccelByteGameProfileApi.h"
#include "Core/AccelByteRegistry.h"

using AccelByte::FRegistry;
using AccelByte::FErrorHandler;

void UAccelByteBlueprintsGameProfile::BatchGetPublicGameProfiles(TArray<FString> UserIds, const FBatchGetPublicGameProfilesSuccess& OnSuccess, const FBlueprintErrorHandler& OnError)
{
	FRegistry::GameProfile.BatchGetPublicGameProfiles(UserIds, THandler<TArray<FAccelByteModelsPublicGameProfile>>::CreateLambda([OnSuccess](const TArray<FAccelByteModelsPublicGameProfile>& Result)
	{
		OnSuccess.ExecuteIfBound(Result);
	}),
	FErrorHandler::CreateLambda([OnError](int32 Code, FString Message)
	{
		OnError.ExecuteIfBound(Code, Message);
	}));
}

void UAccelByteBlueprintsGameProfile::GetAllGameProfiles(const FGetAllGameProfilesSuccess & OnSuccess, const FBlueprintErrorHandler & OnError)
{
	FRegistry::GameProfile.GetAllGameProfiles(THandler< TArray<FAccelByteModelsGameProfile>>::CreateLambda([OnSuccess](const TArray<FAccelByteModelsGameProfile>& Result)
	{
		OnSuccess.ExecuteIfBound(Result);
	}),
	FErrorHandler::CreateLambda([OnError](int32 Code, FString Message)
	{
		OnError.ExecuteIfBound(Code, Message);
	}));
}

void UAccelByteBlueprintsGameProfile::CreateGameProfile(const FAccelByteModelsGameProfileRequest & GameProfileRequest, const FCreateGameProfileSuccess & OnSuccess, const FBlueprintErrorHandler & OnError)
{
	FRegistry::GameProfile.CreateGameProfile(GameProfileRequest, THandler<FAccelByteModelsGameProfile>::CreateLambda([OnSuccess](const FAccelByteModelsGameProfile& Result)
	{
		OnSuccess.ExecuteIfBound(Result);
	}),
	FErrorHandler::CreateLambda([OnError](int32 Code, FString Message)
	{
		OnError.ExecuteIfBound(Code, Message);
	}));
}

void UAccelByteBlueprintsGameProfile::GetGameProfile(const FString & ProfileId, const FGetGameProfileSuccess & OnSuccess, const FBlueprintErrorHandler & OnError)
{
	FRegistry::GameProfile.GetGameProfile(ProfileId, THandler<FAccelByteModelsGameProfile>::CreateLambda([OnSuccess](const FAccelByteModelsGameProfile& Result)
	{
		OnSuccess.ExecuteIfBound(Result);
	}),
		FErrorHandler::CreateLambda([OnError](int32 Code, FString Message)
	{
		OnError.ExecuteIfBound(Code, Message);
	}));
}

void UAccelByteBlueprintsGameProfile::UpdateGameProfile(const FString & ProfileId, const FAccelByteModelsGameProfileRequest & GameProfileRequest, const FUpdateGameProfileSuccess & OnSuccess, const FBlueprintErrorHandler & OnError)
{
	FRegistry::GameProfile.UpdateGameProfile(ProfileId, GameProfileRequest, THandler<FAccelByteModelsGameProfile>::CreateLambda([OnSuccess](const FAccelByteModelsGameProfile& Result)
	{
		OnSuccess.ExecuteIfBound(Result);
	}),
		FErrorHandler::CreateLambda([OnError](int32 Code, FString Message)
	{
		OnError.ExecuteIfBound(Code, Message);
	}));
}

void UAccelByteBlueprintsGameProfile::DeleteGameProfile(const FString & ProfileId, const FDeleteGameProfileSuccess & OnSuccess, const FBlueprintErrorHandler & OnError)
{
	FRegistry::GameProfile.DeleteGameProfile(ProfileId, FVoidHandler::CreateLambda([OnSuccess]()
	{
		OnSuccess.ExecuteIfBound();
	}),
		FErrorHandler::CreateLambda([OnError](int32 Code, FString Message)
	{
		OnError.ExecuteIfBound(Code, Message);
	}));
}

void UAccelByteBlueprintsGameProfile::GetGameProfileAttribute(const FString & ProfileId, const FString & AttributeName, const FGetGameProfileAttributeSuccess & OnSuccess, const FBlueprintErrorHandler & OnError)
{
	FRegistry::GameProfile.GetGameProfileAttribute(ProfileId, AttributeName, THandler<FAccelByteModelsGameProfileAttribute>::CreateLambda([OnSuccess](const FAccelByteModelsGameProfileAttribute& Result)
	{
		OnSuccess.ExecuteIfBound(Result);
	}),
		FErrorHandler::CreateLambda([OnError](int32 Code, FString Message)
	{
		OnError.ExecuteIfBound(Code, Message);
	}));
}

void UAccelByteBlueprintsGameProfile::UpdateGameProfileAttribute(const FString & ProfileId, const FAccelByteModelsGameProfileAttribute& Attribute, const FUpdateGameProfileAttributeSuccess & OnSuccess, const FBlueprintErrorHandler & OnError)
{
	FRegistry::GameProfile.UpdateGameProfileAttribute(ProfileId, Attribute, THandler<FAccelByteModelsGameProfile>::CreateLambda([OnSuccess](const FAccelByteModelsGameProfile& Result)
	{
		OnSuccess.ExecuteIfBound(Result);
	}),
		FErrorHandler::CreateLambda([OnError](int32 Code, FString Message)
	{
		OnError.ExecuteIfBound(Code, Message);
	}));
}
