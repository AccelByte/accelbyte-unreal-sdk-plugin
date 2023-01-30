// Copyright (c) 2021 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "CoreMinimal.h"

#include "Core/AccelByteError.h"
#include "Models/AccelByteUserModels.h"
#include "Core/AccelByteHttpRetryScheduler.h"
#include "Core/AccelByteServerApiBase.h"

namespace AccelByte
{
class ServerCredentials;
class ServerSettings;

namespace GameServerApi
{
	
/**
 * @brief Server User APIs to manage user related like search user by third party platform.
 */
class ACCELBYTEUE4SDK_API ServerUser : public FServerApiBase
{
public:
	ServerUser(ServerCredentials const& InCredentialsRef, ServerSettings const& InSettingsRef, FHttpRetryScheduler& InHttpRef);
	~ServerUser();

	/**
	 * @brief This function will search user by their third party Display Name. The query will be used to find the user with the most approximate display name.
	 *
	 * @param DisplayName Targeted user's third party by Display Name.
	 * @param PlatformType The platform type want to use to search user
	 * @param OnSuccess This will be called when the operation succeeded. The result is FPagedPublicUsersInfo.
	 * @param OnError This will be called when the operation failed.
	 * @param Offset The offset of the users data result. Default value is 0.
	 * @param Limit The limit of the users data result. Default value is 20.
	 */
	void SearchUserOtherPlatformDisplayName(const FString& DisplayName
		, EAccelBytePlatformType PlatformType
		, const THandler<FPagedUserOtherPlatformInfo>& OnSuccess
		, const FErrorHandler& OnError
		, const int32& Limit = 20
		, const int32& Offset = 0 );

	/**
	 * @brief This function will search user by their third party Platform User ID.
	 *
	 * @param PlatformUserId Targeted user's third party user id.
	 * @param PlatformType The platform type want to use to search user
	 * @param OnSuccess This will be called when the operation succeeded. The result is FUserOtherPlatformInfo.
	 * @param OnError This will be called when the operation failed.
	 */
	void SearchUserOtherPlatformUserId(const FString& PlatformUserId
		, EAccelBytePlatformType PlatformType
		, const THandler<FUserOtherPlatformInfo>& OnSuccess
		, const FErrorHandler& OnError);

	/**
	 * @brief This function will ban a user with specific type of ban. Ban types and reason can be queried.
	 * @param UserId Id of user that will be banned.
	 * @param BanUser Information of user that will be banned.
	 * @param OnSuccess This will be called when the operation succeeded. The result is FBanUserInfo.
	 * @param OnError This will be called when the operation failed.
	 */
	void BanSingleUser(const FString& UserId
		, const FBanUserRequest& BanUser
		, const THandler<FBanUserResponse>& OnSuccess
		, const FErrorHandler& OnError);

	/**
	 * @brief This function will get user's bans with specific time duration.
	 * @param UserId Id of user that ban's data will be get.
	 * @param Before Only user bans created before the date returned. The date is in ISO-8601
	 * @param After Only user bans created after the date returned. The date is in ISO-8601
	 * @param OnSuccess This will be called when the operation succeeded. The result is FGetUserBansResponse.
	 * @param OnError This will be called when the operation failed.
	 */
	void GetUserBans(const FString& UserId
		, const FDateTime& Before
		, const FDateTime& After
		, const THandler<FGetUserBansResponse>& OnSuccess
		, const FErrorHandler& OnError);


	/**
	 * @brief This function will get user's ban with specific time duration.
	 * 
	 * @param UserId Id of user that ban's data will be get.
	 * @param OnSuccess This will be called when the operation succeeded. The result is FGetUserBansResponse.
	 * @param OnError This will be called when the operation failed.
	 */
	void GetUserBanInfo(const FString& UserId
		, const THandler<FGetUserBansResponse>& OnSuccess
		, const FErrorHandler& OnError);

private:
	ServerUser() = delete;
	ServerUser(const ServerUser&) = delete;
	ServerUser(ServerUser&&) = delete;
};

}
}
