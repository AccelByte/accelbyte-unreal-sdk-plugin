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
 * @brief API to create, update, and get user's profile.
 */
class ACCELBYTEUE4SDK_API UserProfile
{
public:
	/**
	 * @brief Get user's profile information. If it doesn't exist, that will be an error.
	 * 
	 * @param OnSuccess This will be called when the operation succeeded. The result is FAccelByteModelsUserProfileInfo.
	 * @param OnError This will be called when the operation failed.
	 */
	static void GetUserProfile(const THandler<FAccelByteModelsUserProfileInfo>& OnSuccess, const FErrorHandler& OnError);
;
    /**
     * @brief Get user's profile information. If it doesn't exist, that will be an error.
     *
     * @param OnSuccess This will be called when the operation succeeded. The result is FAccelByteModelsUserProfileInfo.
     * @param OnError This will be called when the operation failed.
     */
    static void GetPublicUserProfileInfo(FString UserID, const THandler<FAccelByteModelsPublicUserProfileInfo>& OnSuccess, const FErrorHandler& OnError);

	/**
	 * @brief Update user's current profile information. If it doesn't exist, that will be an error.
	 * 
	 * @param AccessToken Access token.
	 * @param Namespace Target.
	 * @param UserId User ID.
	 * @param ProfileUpdateRequest Request object.
	 * @param OnSuccess This will be called when the operation succeeded.
	 * @param OnError This will be called when the operation failed.
	 */
    static void UpdateUserProfile(const FAccelByteModelsUserProfileUpdateRequest& ProfileUpdateRequest, const FVoidHandler& OnSuccess, const FErrorHandler& OnError);

	/**
	 * @brief Create complete player profile. If it already exist, that will be an error.
	 *
	 * @param AccessToken Access token.
	 * @param Namespace Target.
	 * @param UserId User ID.
	 * @param ProfileCreateRequest Request object.
	 * @param OnSuccess This will be called when the operation succeeded. The result is FAccelByteModelsUserCreateResponse.
	 * @param OnError This will be called when the operation failed.
	 */
	static void CreateUserProfile(const FAccelByteModelsUserProfileCreateRequest& ProfileCreateRequest, const THandler<FAccelByteModelsUserProfileInfo>& OnSuccess, const FErrorHandler& OnError);

private:
	UserProfile() = delete; // static class can't have instance
	UserProfile(UserProfile const&) = delete;
	UserProfile(UserProfile&&) = delete;	
};

} // Namespace Service
} // Namespace AccelByte
