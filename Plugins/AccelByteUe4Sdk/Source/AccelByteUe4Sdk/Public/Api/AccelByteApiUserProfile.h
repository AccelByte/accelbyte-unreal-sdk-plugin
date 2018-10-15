// Copyright (c) 2017-2018 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Misc/ConfigCacheIni.h"
#include "Http.h"
#include "AccelByteModelsUserProfile.h"
#include "AccelByteError.h"

namespace AccelByte
{
namespace Api
{
	class User;

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
	 * @param AccessToken User access token.
	 * @param Namespace Required.
	 * @param UserId Required.
	 * @param OnSuccess This will be called when the operation succeeded. The result is FAccelByteModelsUserProfileInfo.
	 */
	static void GetUserProfile(FString ServerBaseUrl, FString AccessToken, FString Namespace, FString UserId, FGetUserProfileSuccess OnSuccess, ErrorDelegate OnError);

	DECLARE_DELEGATE(FUpdateUserProfileSuccess);
	/**
	 * @brief Update user's current profile information, if it exists.
	 * 
	 * @param AccessToken Token User access token.
	 * @param Namespace Required.
	 * @param UserId Required.
	 * @param NewUserProfile UserProfileInfoUpdate object.
	 * @param OnSuccess This will be called when the operation succeeded.
	 * @param OnError This will be called when the operation failed.
	 * @param OnError 
	 */
    static void UpdateUserProfile(FString ServerBaseUrl, FString AccessToken, FString Namespace, FString UserId, const FAccelByteModelsUserProfileInfoUpdate& NewUserProfile, FUpdateUserProfileSuccess OnSuccess, ErrorDelegate OnError);

	DECLARE_DELEGATE_OneParam(FCreateDefaultUserProfileSuccess, const FAccelByteModelsUserProfileInfo&);
	/**
	 * @brief Create empty profile information.
	 * 
	 * @param AccessToken User access token.
	 * @param Namespace Required.
	 * @param UserId Required.
	 * @param DisplayName Required.
	 * @param OnSuccess This will be called when the operation succeeded. The result is FAccelByteModelsUserCreateResponse.
	 * @param OnError This will be called when the operation failed.
	 */
	static void CreateDefaultUserProfile(FString ServerBaseUrl, FString AccessToken, FString Namespace, FString UserId, FString DisplayName, FCreateDefaultUserProfileSuccess OnSuccess, ErrorDelegate OnError);

	DECLARE_DELEGATE_OneParam(FCreateCompleteUserProfileSuccess, const FAccelByteModelsUserProfileInfo&);
	/**
	 * @brief Create complete player profile.
	 *
	 * @param AccessToken User access token.
	 * @param Namespace Required.
	 * @param UserId Required.
	 * @param ProfileRequest Required.
	 * @param OnSuccess This will be called when the operation succeeded. The result is FAccelByteModelsUserCreateResponse.
	 * @param OnError This will be called when the operation failed.
	 */
	static void CreateCompleteUserProfile(FString ServerBaseUrl, FString AccessToken, FString Namespace, FString UserId, const FAccelByteModelsUserProfileRequest& ProfileRequest, FCreateCompleteUserProfileSuccess OnSuccess, ErrorDelegate OnError);

private:
	UserProfile() = delete; // static class can't have instance
	UserProfile(User const&) = delete;
	UserProfile(UserProfile&&) = delete;

	static void GetUserProfileResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, FGetUserProfileSuccess OnSuccess, ErrorDelegate OnError);
	static void UpdateUserProfileResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, FUpdateUserProfileSuccess OnSuccess, ErrorDelegate OnError);
	static void CreateDefaultUserProfileResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, FCreateDefaultUserProfileSuccess OnSuccess, ErrorDelegate OnError);
	static void CreateCompleteUserProfileResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, FCreateCompleteUserProfileSuccess OnSuccess, ErrorDelegate OnError);
};

} // Namespace Service
} // Namespace AccelByte
