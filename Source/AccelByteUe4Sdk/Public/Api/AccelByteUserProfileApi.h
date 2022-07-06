// Copyright (c) 2018 - 2020 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Models/AccelByteUserProfileModels.h"
#include "Core/AccelByteError.h"
#include "Core/AccelByteRegistry.h"

namespace AccelByte
{
class Credentials;
class Settings;
namespace Api
{

/**
 * @brief API to create, update, and get user's profile.
 */
class ACCELBYTEUE4SDK_API UserProfile
{
public:
	UserProfile(const Credentials& Credentials, const Settings& Settings, FHttpRetryScheduler& InHttpRef);
	~UserProfile();
private:
	FHttpRetryScheduler& HttpRef;
	Credentials const& CredentialsRef;
	Settings const& SettingsRef;
public:
	/**
	 * @brief Get user's own profile information. If it doesn't exist, that will be an error.
	 * 
	 * @param OnSuccess This will be called when the operation succeeded. The result is FAccelByteModelsUserProfileInfo.
	 * @param OnError This will be called when the operation failed.
	 */
	void GetUserProfile(const THandler<FAccelByteModelsUserProfileInfo>& OnSuccess, const FErrorHandler& OnError);

	/**
	 * @brief Get other user's public profile information. If it doesn't exist, that will be an error.
	 *
	 * @param UserId The id of the user.
	 * @param OnSuccess This will be called when the operation succeeded. The result is FAccelByteModelsPublicUserProfileInfo.
	 * @param OnError This will be called when the operation failed.
	 */
	void GetPublicUserProfileInfo(FString UserId, const THandler<FAccelByteModelsPublicUserProfileInfo>& OnSuccess, const FErrorHandler& OnError);

	/**
	 * @brief Batch get multiple user public profile informations.
	 *
	 * @param UserIds Multiple user ids, separated by comma.
	 * @param OnSuccess This will be called when the operation succeeded.
	 * @param OnError This will be called when the operation failed.
	 */
	void BatchGetPublicUserProfileInfos(FString UserIds, const THandler<TArray<FAccelByteModelsPublicUserProfileInfo>>& OnSuccess, const FErrorHandler& OnError);

	/**
	 * @brief Get user's own custom attribute profile information. If it doesn't exist, that will be an error.
	 *
	 * @param OnSuccess This will be called when the operation succeeded. The result is FJsonObject.
	 * @param OnError This will be called when the operation failed.
	 */
	void GetCustomAttributes(const THandler<FJsonObject>& OnSuccess, const FErrorHandler& OnError);

	/**
	* @brief Get user's own custom attribute profile information. If it doesn't exist, that will be an error.
	*
	* @param UserId The id of the user.
	* @param OnSuccess This will be called when the operation succeeded. The result is FJsonObject.
	* @param OnError This will be called when the operation failed.
	*/
	void GetPublicCustomAttributes(const FString& UserId, const THandler<FJsonObject>& OnSuccess, const FErrorHandler& OnError);

	/**
	 * @brief Update user's current profile information. If it doesn't exist, that will be an error.
	 * 
	 * @param ProfileUpdateRequest Request object.
	 * @param OnSuccess This will be called when the operation succeeded. The result is FAccelByteModelsUserProfileInfo.
	 * @param OnError This will be called when the operation failed.
	 */
	void UpdateUserProfile(const FAccelByteModelsUserProfileUpdateRequest& ProfileUpdateRequest, const THandler<FAccelByteModelsUserProfileInfo>& OnSuccess, const FErrorHandler& OnError);

	/**
	 * @brief Update user's current custom attributes profile information. If it doesn't exist, that will be an error.
	 *
	 * @param CustomAttributesUpdateRequest Request object which consists custom attributes profile information.
	 * @param OnSuccess This will be called when the operation succeeded. The result is FJsonObject.
	 * @param OnError This will be called when the operation failed.
	 */
	void UpdateCustomAttributes(const FJsonObject& CustomAttributesUpdateRequest, const THandler<FJsonObject>& OnSuccess, const FErrorHandler& OnError);

	/**
	 * @brief Create complete player profile. If it already exist, that will be an error.
	 *
	 * @param ProfileCreateRequest Request object.
	 * @param OnSuccess This will be called when the operation succeeded. The result is FAccelByteModelsUserProfileInfo.
	 * @param OnError This will be called when the operation failed.
	 */
	void CreateUserProfile(const FAccelByteModelsUserProfileCreateRequest& ProfileCreateRequest, const THandler<FAccelByteModelsUserProfileInfo>& OnSuccess, const FErrorHandler& OnError);

	/**
	 * @brief Get User Profile Public Info By PublicId.
	 *
	 * @param PublicId The Public Id of user used to get user profile 
	 * @param OnSuccess This will be called when the operation succeeded. The result is FUserProfilePublicInfo.
	 * @param OnError This will be called when the operation failed.
	 */
	void GetUserProfilePublicInfoByPublicId(const FString& PublicId, const THandler<FAccelByteModelsPublicUserProfileInfo>& OnSuccess, const FCustomErrorHandler& OnError);

	/**
	 * @brief Create complete player profile. If it already exist, that will be an error.
	 *
	 *	@param UserId The id of the user.
	 * @param ProfileCreateRequest Request object.
	 * @param OnSuccess This will be called when the operation succeeded. The result is FAccelByteModelsUserProfileInfo.
	 * @param OnError This will be called when the operation failed.
	 */
	void CreateUserProfile(const FString& UserId, FAccelByteModelsUserProfileCreateRequest& ProfileCreateRequest,
	                       const THandler<FAccelByteModelsUserProfileInfo>& OnSuccess, const FErrorHandler& OnError);

	/**
	 * @brief Update user's current profile information. If it doesn't exist, that will be an error.
	 *
	 * @param UserId The id of the user.
	 * @param ProfileUpdateRequest Request object.
	 * @param OnSuccess This will be called when the operation succeeded. The result is FAccelByteModelsUserProfileInfo.
	 * @param OnError This will be called when the operation failed.
	 */
	void UpdateUserProfile(const FString& UserId, const FAccelByteModelsUserProfileUpdateRequest& ProfileUpdateRequest,
	                       const THandler<FAccelByteModelsUserProfileInfo>& OnSuccess, const FErrorHandler& OnError);

	/**
	 * @brief Get user's own profile information. If it doesn't exist, that will be an error.
	 *
	 * @param UserId The id of the user.
	 * @param OnSuccess This will be called when the operation succeeded. The result is FAccelByteModelsUserProfileInfo.
	 * @param OnError This will be called when the operation failed.
	 */
	void GetUserProfile(const FString& UserId, const THandler<FAccelByteModelsUserProfileInfo>& OnSuccess,
	                    const FErrorHandler& OnError);

	/**
	 * @brief Generate an upload URL. It's valid for 10 minutes..
	 *
	 * @param Folder The name of folder where the file will be uploaded, must be between 1-256 characters, all characters allowed no whitespace.
	 * @param FileType One of the these types: jpeg, jpg, png, bmp, gif, mp3, bin, webp.
	 * @param OnSuccess This will be called when the operation succeeded. The result is FAccelByteModelsUserProfileInfo.
	 * @param OnError This will be called when the operation failed.
	 */
	void GenerateUploadURL(const FString& Folder, EAccelByteFileType FileType, THandler<FAccelByteModelsUserProfileUploadURLResult> const& OnSuccess, FErrorHandler const& OnError);

	/**
	 * @brief Generate an upload URL for user content. It's valid for 10 minutes. There are 2 kinds of storage limitation per user : maximum file count and maximum file size.
	 *
	 * @param UserId User's id, should follow UUID version 4 without hyphen.
	 * @param FileType One of the these types: jpeg, jpg, png, bmp, gif, mp3, bin, webp.
	 * @param OnSuccess This will be called when the operation succeeded. The result is FAccelByteModelsUserProfileInfo.
	 * @param OnError This will be called when the operation failed.
	 * @param Category Upload category. Supported categories: default, reporting. Default value : default.
	 */
	void GenerateUploadURLForUserContent(const FString& UserId, EAccelByteFileType FileType, THandler<FAccelByteModelsUserProfileUploadURLResult> const& OnSuccess, FErrorHandler const& OnError,
		EAccelByteUploadCategory Category = EAccelByteUploadCategory::DEFAULT);

private:
	UserProfile() = delete;
	UserProfile(UserProfile const&) = delete;
	UserProfile(UserProfile&&) = delete;	
};

} // Namespace Service
} // Namespace AccelByte
