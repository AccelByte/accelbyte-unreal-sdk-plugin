// Copyright (c) 2020 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "CoreMinimal.h"
#include "Core/AccelByteError.h"
#include "Core/AccelByteServerApiBase.h"
#include "Models/AccelByteLobbyModels.h"
#include "Core/AccelByteHttpRetryScheduler.h"

namespace AccelByte
{
class ServerCredentials;
class ServerSettings;

namespace GameServerApi
{

/**
 * @brief Provide APIs to access Achievement service.
 */
class ACCELBYTEUE4SDK_API ServerLobby : public FServerApiBase
{
public:
	ServerLobby(ServerCredentials const& InCredentialsRef
		, ServerSettings const& InSettingsRef
		, FHttpRetryScheduler& InHttpRef
		, TSharedPtr<FServerApiClient, ESPMode::ThreadSafe> InServerApiClient  = nullptr);
	~ServerLobby();

	/**
	 * @brief Get party data by User Id.
	 *
	 * @param UserId The id of the user.
	 * @param OnSuccess This will be called when the operation succeeded.
	 * @param OnError This will be called when the operation failed.
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr GetPartyDataByUserId(FString const& UserId
		, THandler<FAccelByteModelsDataPartyResponse> const& OnSuccess
		, FErrorHandler const& OnError);

	/**
	 * @brief  Write party storage (attributes) data to the targeted party ID.
	 * Beware:
	 * Object will not be write immediately, please take care of the original object until it written.
	 *
	 * @param PartyId Targeted party Id.
	 * @param PayloadModifier Function to modify the latest party data with your customized modifier.
	 * @param OnSuccess This will be called when the operation succeeded. Will return FAccelByteModelsPartyDataNotif model.
	 * @param OnError This will be called when the operation failed.
	 * @param RetryAttempt the number of retry to do when there is an error in writing to party storage (likely due to write conflicts).
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr WritePartyStorage(FString const& PartyId
		, TFunction<FJsonObjectWrapper(FJsonObjectWrapper)> PayloadModifier
		, THandler<FAccelByteModelsPartyDataNotif> const& OnSuccess
		, FErrorHandler const& OnError
		, uint32 RetryAttempt = 1);

	/**
	 * @brief  Get party storage (attributes) by party ID.
	 *
	 * @param PartyId Targeted party Id.
	 * @param OnSuccess This will be called when the operation succeeded. Will return FAccelByteModelsPartyDataNotif model.
	 * @param OnError This will be called when the operation failed.
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr GetPartyStorage(FString const& PartyId
		, THandler<FAccelByteModelsPartyDataNotif> const& OnSuccess
		, FErrorHandler const& OnError);
	
	/**
	 * @brief  Get all session attributes of the user.
	 *
	 * @param UserId The user's user ID.
	 * @param OnSuccess This will be called when the operation succeeded. Will return an FAccelByteModelsGetSessionAttributeAllRequest.
	 * @param OnError This will be called when the operation failed.
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr GetSessionAttributeAll(FString const& UserId
		, THandler<FAccelByteModelsGetSessionAttributeAllResponse> const& OnSuccess
		, FErrorHandler const& OnError);

	/**
	 * @brief  Get session attribute of the user accoding to the Key provided.
	 *
	 * @param UserId The user's user ID.
	 * @param Key The session attribute's key.
	 * @param OnSuccess This will be called when the operation succeeded. Will return an FAccelByteModelsGetSessionAttributeRequest.
	 * @param OnError This will be called when the operation failed.
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr GetSessionAttribute(FString const& UserId
		, FString const& Key
		, THandler<FAccelByteModelsGetSessionAttributeResponse> const& OnSuccess
		, FErrorHandler const& OnError);

	/**
	 * @brief  Set session attribute of the user.
	 *
	 * @param UserId The user's user ID.
	 * @param Attributes The session attributes you want to store.
	 * @param OnSuccess This will be called when the operation succeeded. Will return an FString value.
	 * @param OnError This will be called when the operation failed.
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr SetSessionAttribute(FString const& UserId
		, TMap<FString, FString> const& Attributes
		, FVoidHandler const& OnSuccess
		, FErrorHandler const& OnError);

	/**
	 * @brief  Set session attribute of the user.
	 *
	 * @param UserId The user's user ID.
	 * @param Key The session attributes's Key.
	 * @param Value The session attributes's Value.
	 * @param OnSuccess This will be called when the operation succeeded. Will return an FString value.
	 * @param OnError This will be called when the operation failed.
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr SetSessionAttribute(FString const& UserId
		, FString const& Key
		, FString const& Value
		, FVoidHandler const& OnSuccess
		, FErrorHandler const& OnError);

	/**
	 * @brief Get list of users that is blocking this UserId.
	 *
	 * @param UserId User Id of the Player.
	 * @param OnSuccess This will be called when the operation succeeded. Will return FAccelByteModelsListBlockerResponse model.
	 * @param OnError This will be called when the operation failed.
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr GetListOfBlockers(FString const& UserId
		, THandler<FAccelByteModelsListBlockerResponse> const& OnSuccess
		, FErrorHandler const& OnError);

	/**
	 * @brief  Get list of users that is blocked by this UserId.
	 *
	 * @param UserId User Id of the Player.
	 * @param OnSuccess This will be called when the operation succeeded. Will return FAccelByteModelsListBlockedUserResponse model.
	 * @param OnError This will be called when the operation failed.
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr GetListOfBlockedUsers(FString const& UserId
		, THandler<FAccelByteModelsListBlockedUserResponse> const& OnSuccess
		, FErrorHandler const& OnError);
	
private:

	struct PartyStorageWrapper
	{
		FString PartyId;
		int RemainingAttempt;
		THandler<FAccelByteModelsPartyDataNotif> OnSuccess;
		FErrorHandler OnError;
		TFunction<FJsonObjectWrapper(FJsonObjectWrapper)> PayloadModifier;
	};

	FAccelByteTaskWPtr RequestWritePartyStorage(FString const& PartyId
		, FAccelByteModelsPartyDataUpdateRequest const& Data
		, THandler<FAccelByteModelsPartyDataNotif> const& OnSuccess
		, FErrorHandler const& OnError
		, FSimpleDelegate OnConflicted = NULL);

	void WritePartyStorageRecursive(TSharedPtr<PartyStorageWrapper> DataWrapper);

	ServerLobby() = delete;
	ServerLobby(ServerLobby const&) = delete;
	ServerLobby(ServerLobby&&) = delete;
};
} // namespace GameServerApi
} // namespace AccelByte