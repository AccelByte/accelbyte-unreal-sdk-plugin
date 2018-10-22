// Copyright (c) 2018 AccelByte Inc. All Rights Reserved.
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
	static void GetUserProfile(const FString& AccessToken, const FString& Namespace, const FString& UserId, const FGetUserProfileSuccess& OnSuccess, const FErrorHandler& OnError);
	static void GetUserProfileEasy(const FGetUserProfileSuccess& OnSuccess, const FErrorHandler& OnError);

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
    static void UpdateUserProfile(const FString& AccessToken, const FString& Namespace, const FString& UserId, const FAccelByteModelsUserProfileUpdateRequest& ProfileUpdateRequest, const FUpdateUserProfileSuccess& OnSuccess, const FErrorHandler& OnError);
	static void UpdateUserProfileEasy(const FAccelByteModelsUserProfileUpdateRequest& ProfileUpdateRequest, const FUpdateUserProfileSuccess& OnSuccess, const FErrorHandler& OnError);

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
	static void CreateUserProfile(const FString& AccessToken, const FString& Namespace, const FString& UserId, const FAccelByteModelsUserProfileCreateRequest& ProfileCreateRequest, const FCreateUserProfileSuccess& OnSuccess, const FErrorHandler& OnError);
	static void CreateUserProfileEasy(const FAccelByteModelsUserProfileCreateRequest& ProfileCreateRequest, const FCreateUserProfileSuccess& OnSuccess, const FErrorHandler& OnError);
private:
	UserProfile() = delete; // static class can't have instance
	UserProfile(UserProfile const&) = delete;
	UserProfile(UserProfile&&) = delete;

	static void GetUserProfileResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, bool Successful, FGetUserProfileSuccess OnSuccess, FErrorHandler OnError);
	static void UpdateUserProfileResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, bool Successful, FUpdateUserProfileSuccess OnSuccess, FErrorHandler OnError);
	static void CreateUserProfileResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, bool Successful, FCreateUserProfileSuccess OnSuccess, FErrorHandler OnError);
	
};

} // GameId Service
} // Namespace AccelByte
