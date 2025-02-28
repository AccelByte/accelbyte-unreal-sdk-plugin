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
	 * @param bIsSearchByUniqueDisplayName Search by unique display name. This is only fulfill if the backend unique_display_name config was true.
	 * @param Limit The limit of the users data result. Default value is 20.
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr SearchUserOtherPlatformDisplayName(FString const& DisplayName
		, EAccelBytePlatformType PlatformType
		, THandler<FPagedUserOtherPlatformInfo> const& OnSuccess
		, FErrorHandler const& OnError
		, int32 Limit = 20
		, int32 Offset = 0
		, bool bIsSearchByUniqueDisplayName = false);

	/**
	 * @brief This function will search user by their third party Platform User ID.
	 *
	 * @param PlatformUserId Targeted user's third party user id.
	 * @param PlatformType The platform type want to use to search user
	 * @param OnSuccess This will be called when the operation succeeded. The result is FUserOtherPlatformInfo.
	 * @param OnError This will be called when the operation failed.
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr SearchUserOtherPlatformUserId(FString const& PlatformUserId
		, EAccelBytePlatformType PlatformType
		, THandler<FUserOtherPlatformInfo> const& OnSuccess
		, FErrorHandler const& OnError);

	/**
	 * @brief This function will ban a user with specific type of ban. Ban types and reason can be queried.
	 * @param UserId Id of user that will be banned.
	 * @param BanUser Information of user that will be banned.
	 * @param OnSuccess This will be called when the operation succeeded. The result is FBanUserInfo.
	 * @param OnError This will be called when the operation failed.
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr BanSingleUser(FString const& UserId
		, FBanUserRequest const& BanUser
		, THandler<FBanUserResponse> const& OnSuccess
		, FErrorHandler const& OnError);

	/**
	 * @brief This function will get user's bans with specific time duration.
	 * @param UserId Id of user that ban's data will be get.
	 * @param Before Only user bans created before the date returned. The date is in ISO-8601
	 * @param After Only user bans created after the date returned. The date is in ISO-8601
	 * @param OnSuccess This will be called when the operation succeeded. The result is FGetUserBansResponse.
	 * @param OnError This will be called when the operation failed.
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr GetUserBans(FString const& UserId
		, FDateTime const& Before
		, FDateTime const& After
		, THandler<FGetUserBansResponse> const& OnSuccess
		, FErrorHandler const& OnError);
	
	/**
	 * @brief This function will get user's ban with specific time duration.
	 * 
	 * @param UserId Id of user that ban's data will be get.
	 * @param OnSuccess This will be called when the operation succeeded. The result is FGetUserBansResponse.
	 * @param OnError This will be called when the operation failed.
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr GetUserBanInfo(FString const& UserId
		, THandler<FGetUserBansResponse> const& OnSuccess
		, FErrorHandler const& OnError);

	/**
	 * @brief This function to List user by user id.
	 * 
	 * @param Request struct request containing user ids.
	 * @param OnSuccess This will be called when the operation succeeded. The result is THandler<FListUserDataResponse>&.
	 * @param OnError This will be called when the operation failed.
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr ListUserByUserId(FListUserDataRequest const& Request
		, THandler<FListUserDataResponse> const& OnSuccess
		, FErrorHandler const& OnError);

	/**
	 * @brief This function will get multiple user(s) information. 
	 *
	 * @param UserIds List UserId(s) to get. SHOULD NOT EXCEED 100
	 * @param OnSuccess This will be called when the operation succeeded. The result is FListBulkUserInfo.
	 * @param OnError This will be called when the operation failed.
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr BulkGetUserInfo(TArray<FString> const& UserIds
		, THandler<FListBulkUserInfo> const& OnSuccess
		, FErrorHandler const& OnError);

	/**
	 * @brief This function will get user basic and public info using users email
	 *
	 * @param Request Request struct containing chosen users emails.
	 * @param OnSuccess This will be called when the operation succeeded. The result is FAccountUsersData.
	 * @param OnError This will be called when the operation failed.
	 *
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr GetUsersInfoByEmails(FUsersEmailsRequest const& Request
		, THandler<FAccountUsersData> const& OnSuccess
		, FErrorHandler const& OnError);
	
private:
	ServerUser() = delete;
	ServerUser(const ServerUser&) = delete;
	ServerUser(ServerUser&&) = delete;
};

} // Namespace GameServerApi
} // Namespace AccelByte
