// Copyright (c) 2018 - 2020 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "CoreMinimal.h"
#include "Core/AccelByteApiBase.h"
#include "UObject/NoExportTypes.h"
#include "Models/AccelByteUserProfileModels.h"
#include "Core/AccelByteError.h"


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
	: public FApiBase
	, public TSharedFromThis<UserProfile, ESPMode::ThreadSafe>
{
public:
	UserProfile(Credentials const& Credentials
		, Settings const& Settings
		, FHttpRetrySchedulerBase& InHttpRef
		, TSharedPtr<AccelByte::FApiClient, ESPMode::ThreadSafe> const& InApiClient = nullptr);

	UserProfile(Credentials const& Credentials
		, Settings const& Settings
		, FHttpRetrySchedulerBase& InHttpRef
		, FAccelBytePlatformPtr const& InAccelBytePlatform);

	virtual ~UserProfile();

	/**
	 * @brief Get user's own profile information. If it doesn't exist, that will be an error.
	 * 
	 * @param OnSuccess This will be called when the operation succeeded. The result is FAccelByteModelsUserProfileInfo.
	 * @param OnError This will be called when the operation failed.
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr GetUserProfile(THandler<FAccelByteModelsUserProfileInfo> const& OnSuccess
		, FErrorHandler const& OnError);

	/**
	 * @brief Get other user's public profile information. If it doesn't exist, that will be an error.
	 *
	 * @param UserId The id of the user.
	 * @param OnSuccess This will be called when the operation succeeded. The result is FAccelByteModelsPublicUserProfileInfo.
	 * @param OnError This will be called when the operation failed.
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr GetPublicUserProfileInfo(FString const& UserId
		, THandler<FAccelByteModelsPublicUserProfileInfo> const& OnSuccess
		, FErrorHandler const& OnError);

	/**
	 * @brief Batch get multiple user public profile information.
	 *
	 * @param UserIds Multiple user ids, separated by comma.
	 * @param OnSuccess This will be called when the operation succeeded.
	 * @param OnError This will be called when the operation failed.
	 * 
	 * @deprecated This method will be removed in the future, so please use BulkGetPublicUserProfileInfos(const TArray<FString>& UserIds
	 *		, const THandler<TArray<FAccelByteModelsPublicUserProfileInfo>>& OnSuccess
	 *		, FErrorHandler const& OnError)
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr BatchGetPublicUserProfileInfos(FString const& UserIds
		, THandler<TArray<FAccelByteModelsPublicUserProfileInfo>> const& OnSuccess
		, FErrorHandler const& OnError);

	/**
	 * @brief Bulk get multiple user public profile information.
	 *
	 * @param UserIds Multiple user ids.
	 * @param OnSuccess This will be called when the operation succeeded.
	 * @param OnError This will be called when the operation failed.
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr BulkGetPublicUserProfileInfos(TArray<FString> const& UserIds
		, THandler<TArray<FAccelByteModelsPublicUserProfileInfo>> const& OnSuccess
		, FErrorHandler const& OnError);

	/**
	 * @brief Bulk get multiple user public profile information.
	 *
	 * @param UserIds Multiple user ids.
	 * @param OnSuccess This will be called when the operation succeeded.
	 * @param OnError This will be called when the operation failed.
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr BulkGetPublicUserProfileInfosV2(TArray<FString> const& UserIds
		, THandler<FAccelByteModelsPublicUserProfileInfoV2> const& OnSuccess
		, FErrorHandler const& OnError);

	/**
	 * @brief Get user's own custom attribute profile information. If it doesn't exist, that will be an error.
	 *
	 * @param OnSuccess This will be called when the operation succeeded. The result is FJsonObject.
	 * @param OnError This will be called when the operation failed.
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr GetCustomAttributes(THandler<FJsonObject> const& OnSuccess
		, FErrorHandler const& OnError);

	/**
	 * @brief Get user's own custom attribute profile information. If it doesn't exist, that will be an error.
	 *
	 * @param UserId The id of the user.
	 * @param OnSuccess This will be called when the operation succeeded. The result is FJsonObject.
	 * @param OnError This will be called when the operation failed.
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr GetPublicCustomAttributes(FString const& UserId
		, THandler<FJsonObject> const& OnSuccess
		, FErrorHandler const& OnError);

	/**
	 * @brief Update user's current profile information. If it doesn't exist, that will be an error.
	 * 
	 * @param ProfileUpdateRequest Request object.
	 * @param OnSuccess This will be called when the operation succeeded. The result is FAccelByteModelsUserProfileInfo.
	 * @param OnError This will be called when the operation failed.
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr UpdateUserProfile(FAccelByteModelsUserProfileUpdateRequest const& ProfileUpdateRequest
		, THandler<FAccelByteModelsUserProfileInfo> const& OnSuccess
		, FErrorHandler const& OnError);

	/**
	 * @brief Update user's current custom attributes profile information. If it doesn't exist, that will be an error.
	 *
	 * @param CustomAttributesUpdateRequest Request object which consists custom attributes profile information.
	 * @param OnSuccess This will be called when the operation succeeded. The result is FJsonObject.
	 * @param OnError This will be called when the operation failed.
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr UpdateCustomAttributes(FJsonObject const& CustomAttributesUpdateRequest
		, THandler<FJsonObject> const& OnSuccess
		, FErrorHandler const& OnError);

	/**
	 * @brief Create complete player profile. If it already exist, that will be an error.
	 *
	 * @param ProfileCreateRequest Request object.
	 * @param OnSuccess This will be called when the operation succeeded. The result is FAccelByteModelsUserProfileInfo.
	 * @param OnError This will be called when the operation failed.
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr CreateUserProfile(FAccelByteModelsUserProfileCreateRequest const& ProfileCreateRequest
		, THandler<FAccelByteModelsUserProfileInfo> const& OnSuccess
		, FErrorHandler const& OnError);

	/**
	 * @brief Get User Profile Public Info By PublicId.
	 *
	 * @param PublicId The Public Id of user used to get user profile 
	 * @param OnSuccess This will be called when the operation succeeded. The result is FUserProfilePublicInfo.
	 * @param OnError This will be called when the operation failed.
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr GetUserProfilePublicInfoByPublicId(FString const& PublicId
		, THandler<FAccelByteModelsPublicUserProfileInfo> const& OnSuccess
		, FCustomErrorHandler const& OnError);

	/**
	 * @brief Create complete player profile. If it already exist, that will be an error.
	 *
	 * @param UserId The id of the user.
	 * @param ProfileCreateRequest Request object.
	 * @param OnSuccess This will be called when the operation succeeded. The result is FAccelByteModelsUserProfileInfo.
	 * @param OnError This will be called when the operation failed.
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr CreateUserProfile(FString const& UserId
		, FAccelByteModelsUserProfileCreateRequest const& ProfileCreateRequest
		, THandler<FAccelByteModelsUserProfileInfo> const& OnSuccess
		, FErrorHandler const& OnError);

	/**
	 * @brief Update user's current profile information. If it doesn't exist, that will be an error.
	 *
	 * @param UserId The id of the user.
	 * @param ProfileUpdateRequest Request object.
	 * @param OnSuccess This will be called when the operation succeeded. The result is FAccelByteModelsUserProfileInfo.
	 * @param OnError This will be called when the operation failed.
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr UpdateUserProfile(FString const& UserId
		, FAccelByteModelsUserProfileUpdateRequest const& ProfileUpdateRequest
		, THandler<FAccelByteModelsUserProfileInfo> const& OnSuccess
		, FErrorHandler const& OnError);

	/**
	 * @brief Get user's own profile information. If it doesn't exist, that will be an error.
	 *
	 * @param UserId The id of the user.
	 * @param OnSuccess This will be called when the operation succeeded. The result is FAccelByteModelsUserProfileInfo.
	 * @param OnError This will be called when the operation failed.
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr GetUserProfile(FString const& UserId
		, THandler<FAccelByteModelsUserProfileInfo> const& OnSuccess
		, FErrorHandler const& OnError);

	/**
	 * @brief Generate an upload URL. It's valid for 10 minutes..
	 *
	 * @param Folder The name of folder where the file will be uploaded, must be between 1-256 characters, all characters allowed no whitespace.
	 * @param FileType One of the these types: jpeg, jpg, png, bmp, gif, mp3, bin, webp.
	 * @param OnSuccess This will be called when the operation succeeded. The result is FAccelByteModelsUserProfileInfo.
	 * @param OnError This will be called when the operation failed.
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr GenerateUploadURL(FString const& Folder
		, EAccelByteFileType FileType
		, THandler<FAccelByteModelsUserProfileUploadURLResult> const& OnSuccess
		, FErrorHandler const& OnError);

	/**
	 * @brief Generate an upload URL for user content. It's valid for 10 minutes. There are 2 kinds of storage limitation per user : maximum file count and maximum file size.
	 *
	 * @param UserId User's id, should follow UUID version 4 without hyphen.
	 * @param FileType One of the these types: jpeg, jpg, png, bmp, gif, mp3, bin, webp.
	 * @param OnSuccess This will be called when the operation succeeded. The result is FAccelByteModelsUserProfileInfo.
	 * @param OnError This will be called when the operation failed.
	 * @param Category Upload category. Supported categories: default, reporting. Default value : default.
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr GenerateUploadURLForUserContent(FString const& UserId
		, EAccelByteFileType FileType
		, THandler<FAccelByteModelsUserProfileUploadURLResult> const& OnSuccess
		, FErrorHandler const& OnError
		, EAccelByteUploadCategory Category = EAccelByteUploadCategory::DEFAULT);

	/**
	 * @brief Get user's own private custom attribute profile information. If it doesn't exist, that will be an error.
	 *
	 * @param OnSuccess This will be called when the operation succeeded. The result is FJsonObject.
	 * @param OnError This will be called when the operation failed.
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr GetPrivateCustomAttributes(THandler<FJsonObjectWrapper> const& OnSuccess
		, FErrorHandler const& OnError);
	
	/**
	 * @brief Update user's own private custom attributes profile information. If it doesn't exist, that will be an error.
	 *
	 * @param PrivateCustomAttributesUpdateRequest Request object which consists private custom attributes profile information.
	 * @param OnSuccess This will be called when the operation succeeded. The result is FJsonObject.
	 * @param OnError This will be called when the operation failed.
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr UpdatePrivateCustomAttributes(FJsonObject const& PrivateCustomAttributesUpdateRequest
		, THandler<FJsonObjectWrapper> const& OnSuccess
		, FErrorHandler const& OnError);

private:
	UserProfile() = delete;
	UserProfile(UserProfile const&) = delete;
	UserProfile(UserProfile&&) = delete;	
};

using UserProfileRef = TSharedRef<UserProfile, ESPMode::ThreadSafe>;
using UserProfilePtr = TSharedPtr<UserProfile, ESPMode::ThreadSafe>;
using UserProfileWPtr = TWeakPtr<UserProfile, ESPMode::ThreadSafe>;

} // Namespace Service
} // Namespace AccelByte
