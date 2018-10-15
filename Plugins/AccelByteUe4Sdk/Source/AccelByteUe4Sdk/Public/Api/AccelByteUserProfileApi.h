// Copyright (c) 2017-2018 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Misc/ConfigCacheIni.h"
#include "Http.h"
#include "AccelByteUserProfileModels.h"
#include "AccelByteError.h"

namespace AccelByte
{
namespace Api
{

/**
 * @brief API to get user's profile.
 * User should log in before using this. See AccelByteServicesUser to see how to log user in.
 */
class ACCELBYTEUE4SDK_API UserProfile
{
public:
	DECLARE_DELEGATE_OneParam(FGetUserProfileSuccess, const FAccelByteModelsUserProfileInfo&);
	/**
	 * @brief Get user's profile information, if it exists.
	 * 
	 * @param AccessToken Access token.
	 * @param Namespace Target.
	 * @param UserId User ID.
	 * @param OnSuccess This will be called when the operation succeeded. The result is FAccelByteModelsUserProfileInfo.
	 */
	static void GetUserProfile(FString ServerBaseUrl, FString AccessToken, FString Namespace, FString UserId, FGetUserProfileSuccess OnSuccess, ErrorDelegate OnError);
	static void GetUserProfileEasy(FGetUserProfileSuccess OnSuccess, ErrorDelegate OnError);

	DECLARE_DELEGATE(FUpdateUserProfileSuccess);
	/**
	 * @brief Update user's current profile information, if it exists.
	 * 
	 * @param AccessToken Access token.
	 * @param Namespace Target.
	 * @param UserId User ID.
	 * @param ProfileUpdateRequest Request object.
	 * @param OnSuccess This will be called when the operation succeeded.
	 * @param OnError This will be called when the operation failed.
	 * @param OnError 
	 */
    static void UpdateUserProfile(FString ServerBaseUrl, FString AccessToken, FString Namespace, FString UserId, const FAccelByteModelsUserProfileUpdateRequest& ProfileUpdateRequest, FUpdateUserProfileSuccess OnSuccess, ErrorDelegate OnError);
	static void UpdateUserProfileEasy(const FAccelByteModelsUserProfileUpdateRequest& ProfileUpdateRequest, FUpdateUserProfileSuccess OnSuccess, ErrorDelegate OnError);

	DECLARE_DELEGATE_OneParam(FCreateUserProfileSuccess, const FAccelByteModelsUserProfileInfo&);
	/**
	 * @brief Create complete player profile.
	 *
	 * @param AccessToken Access token.
	 * @param Namespace Target.
	 * @param UserId User ID.
	 * @param ProfileCreateRequest Request object.
	 * @param OnSuccess This will be called when the operation succeeded. The result is FAccelByteModelsUserCreateResponse.
	 * @param OnError This will be called when the operation failed.
	 */
	static void CreateUserProfile(FString ServerBaseUrl, FString AccessToken, FString Namespace, FString UserId, const FAccelByteModelsUserProfileCreateRequest& ProfileCreateRequest, FCreateUserProfileSuccess OnSuccess, ErrorDelegate OnError);
	static void CreateUserProfileEasy(const FAccelByteModelsUserProfileCreateRequest& ProfileCreateRequest, FCreateUserProfileSuccess OnSuccess, ErrorDelegate OnError);
private:
	UserProfile() = delete; // static class can't have instance
	UserProfile(UserProfile const&) = delete;
	UserProfile(UserProfile&&) = delete;

	static void GetUserProfileResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, FGetUserProfileSuccess OnSuccess, ErrorDelegate OnError);
	static void UpdateUserProfileResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, FUpdateUserProfileSuccess OnSuccess, ErrorDelegate OnError);
	static void CreateUserProfileResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, FCreateUserProfileSuccess OnSuccess, ErrorDelegate OnError);
	
};

} // GameId Service
} // Namespace AccelByte
