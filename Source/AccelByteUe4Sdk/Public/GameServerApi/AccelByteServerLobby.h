// Copyright (c) 2020 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "CoreMinimal.h"
#include "Core/AccelByteError.h"
#include "Models/AccelByteGeneralModels.h"
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
	class ACCELBYTEUE4SDK_API ServerLobby
	{
	public:
		ServerLobby(ServerCredentials const& InCredentialsRef, ServerSettings const& InSettingsRef, FHttpRetryScheduler& InHttpRef);
		~ServerLobby();

		/**
			* @brief Get party data by User Id.
			*
			* @param UserId The id of the user.
			* @param OnSuccess This will be called when the operation succeeded.
			* @param OnError This will be called when the operation failed.
		*/
		void GetPartyDataByUserId(const FString& UserId, const THandler<FAccelByteModelsDataPartyResponse> OnSuccess, const FErrorHandler& OnError);

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
		*/
		void WritePartyStorage(const FString& PartyId, TFunction<FJsonObjectWrapper(FJsonObjectWrapper)> PayloadModifier, const THandler<FAccelByteModelsPartyDataNotif>& OnSuccess, const FErrorHandler& OnError, uint32 RetryAttempt = 1);

		/**
		* @brief  Get party storage (attributes) by party ID.
		*
		* @param PartyId Targeted party Id.
		* @param OnSuccess This will be called when the operation succeeded. Will return FAccelByteModelsPartyDataNotif model.
		* @param OnError This will be called when the operation failed.
		*/
		void GetPartyStorage(const FString& PartyId, const THandler<FAccelByteModelsPartyDataNotif>& OnSuccess, const FErrorHandler& OnError);
		
		/**
		* @brief  Get all session attributes of the user.
		*
		* @param UserId The user's user ID.
		* @param OnSuccess This will be called when the operation succeeded. Will return an FAccelByteModelsGetSessionAttributeAllRequest.
		* @param OnError This will be called when the operation failed.
		*/
		void GetSessionAttributeAll(const FString& UserId, const THandler<FAccelByteModelsGetSessionAttributeAllResponse>& OnSuccess, const FErrorHandler& OnError);

		/**
		* @brief  Get session attribute of the user accoding to the Key provided.
		*
		* @param UserId The user's user ID.
		* @param Key The session attribute's key.
		* @param OnSuccess This will be called when the operation succeeded. Will return an FAccelByteModelsGetSessionAttributeRequest.
		* @param OnError This will be called when the operation failed.
		*/
		void GetSessionAttribute(const FString& UserId, const FString& Key, const THandler<FAccelByteModelsGetSessionAttributeResponse>& OnSuccess, const FErrorHandler& OnError);

		/**
		* @brief  Set session attribute of the user.
		*
		* @param UserId The user's user ID.
		* @param Attributes The session attributes you want to store.
		* @param OnSuccess This will be called when the operation succeeded. Will return an FString value.
		* @param OnError This will be called when the operation failed.
		*/
		void SetSessionAttribute(const FString& UserId, const TMap<FString, FString>& Attributes, const FVoidHandler& OnSuccess, const FErrorHandler& OnError);

		/**
		* @brief  Set session attribute of the user.
		*
		* @param UserId The user's user ID.
		* @param Key The session attributes's Key.
		* @param Value The session attributes's Value.
		* @param OnSuccess This will be called when the operation succeeded. Will return an FString value.
		* @param OnError This will be called when the operation failed.
		*/
		void SetSessionAttribute(const FString& UserId, const FString& Key, const FString& Value, const FVoidHandler& OnSuccess, const FErrorHandler& OnError);

		/**
		* @brief Get list of users that is blocking this UserId.
		*
		* @param UserId User Id of the Player.
		* @param OnSuccess This will be called when the operation succeeded. Will return FAccelByteModelsListBlockerResponse model.
		* @param OnError This will be called when the operation failed.
		*/
		void GetListOfBlockers(const FString& UserId, const THandler<FAccelByteModelsListBlockerResponse> OnSuccess, const FErrorHandler& OnError);

		/**
		* @brief  Get list of users that is blocked by this UserId.
		*
		* @param UserId User Id of the Player.
		* @param OnSuccess This will be called when the operation succeeded. Will return FAccelByteModelsListBlockedUserResponse model.
		* @param OnError This will be called when the operation failed.
		*/
		void GetListOfBlockedUsers(const FString& UserId, const THandler<FAccelByteModelsListBlockedUserResponse> OnSuccess, const FErrorHandler& OnError);
		
	private:

		struct PartyStorageWrapper
		{
			FString PartyId;
			int RemainingAttempt;
			THandler<FAccelByteModelsPartyDataNotif> OnSuccess;
			FErrorHandler OnError;
			TFunction<FJsonObjectWrapper(FJsonObjectWrapper)> PayloadModifier;
		};

		void RequestWritePartyStorage(const FString& PartyId, const FAccelByteModelsPartyDataUpdateRequest& Data, const THandler<FAccelByteModelsPartyDataNotif>& OnSuccess, const FErrorHandler& OnError, FSimpleDelegate OnConflicted = NULL);

		void WritePartyStorageRecursive(TSharedPtr<PartyStorageWrapper> DataWrapper);

		ServerCredentials const& CredentialsRef;
		ServerSettings const& SettingsRef;
		FHttpRetryScheduler& HttpRef;

		ServerLobby() = delete;
		ServerLobby(ServerLobby const&) = delete;
		ServerLobby(ServerLobby&&) = delete;
	};
} // namespace GameServerApi
} // namespace AccelByte