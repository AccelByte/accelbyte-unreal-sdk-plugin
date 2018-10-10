// Copyright (c) 2017-2018 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "AccelByteBlueprintsIdentity.h"
#include "AccelByteApiIdentity.h"

using AccelByte::Api::Identity;
using AccelByte::ErrorDelegate;

void UAccelByteBlueprintsIdentity::GetAccessTokenWithDeviceGrant(FString ServerBaseUrl, FString ClientId, FString ClientSecret, FString Namespace, FGetAccessTokenWithDeviceGrantSuccess OnSuccess, FBlueprintError OnError)
{
	Identity::GetAccessTokenWithDeviceGrant(ServerBaseUrl, ClientId, ClientSecret, Namespace, Identity::FGetAccessTokenWithDeviceGrantSuccess::CreateLambda([OnSuccess](const FAccelByteModelsOAuthToken& Result)
	{
		OnSuccess.ExecuteIfBound(Result);
	}),
		ErrorDelegate::CreateLambda([OnError](int32 ErrorCode, FString ErrorString)
	{
		OnError.ExecuteIfBound(ErrorCode, ErrorString);
	}));
}

void UAccelByteBlueprintsIdentity::GetUserAccessTokenWithPasswordGrant(FString ServerBaseUrl, FString ClientId,
	FString ClientSecret, FString Namespace, FString LoginId, FString Password,
	FLoginWithDeviceIdGetUserAccessTokenWithPasswordGrantSuccess OnSuccess, FBlueprintError OnError)
{
	Identity::GetUserAccessTokenWithPasswordGrant(ServerBaseUrl, ClientId, ClientSecret, Namespace, LoginId, Password, Identity::FGetUserAccessTokenWithPasswordGrantSuccess::CreateLambda([OnSuccess](const FAccelByteModelsOAuthToken& Result)
	{
		OnSuccess.ExecuteIfBound(Result);
	}),
		ErrorDelegate::CreateLambda([OnError](int32 ErrorCode, FString ErrorString)
	{
		OnError.ExecuteIfBound(ErrorCode, ErrorString);
	}));
}
