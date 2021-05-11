// Copyright (c) 2020 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "CoreMinimal.h"
#include "Core/AccelByteError.h"
#include "Models/AccelByteGeneralModels.h"
#include "Models/AccelByteLobbyModels.h"

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
		ServerLobby(const ServerCredentials& Credentials, const ServerSettings& Settings);
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
		* @brief  Get active parties.
		*
		* @param Limit The amount of returned item per page. Set is to 0 to retrieve by maximum limit.
		* @param Offset Offset of the item (active party). First item is = 0.
		* @param OnSuccess This will be called when the operation succeeded. Will return an array of FAccelByteModelsActivePartiesData model.
		* @param OnError This will be called when the operation failed.
		*/
		void GetActiveParties(const int32& Limit, const int32& Offset, const THandler<FAccelByteModelsActivePartiesData>& OnSuccess, const FErrorHandler& OnError);

		/**
		* @brief  Get active parties.
		*
		* @param Paging The processed paging response.
		* @param PaginationType Which page that will be opened
		* @param OnSuccess This will be called when the operation succeeded. Will return an array of FAccelByteModelsActivePartiesData model.
		* @param OnError This will be called when the operation failed.
		*/
		void GetActiveParties(const FAccelByteModelsPaging& Paging, const EAccelBytePaginationType& PaginationType, const THandler<FAccelByteModelsActivePartiesData>& OnSuccess, const FErrorHandler& OnError);

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

		const ServerCredentials& Credentials;
		const ServerSettings& Settings;

		ServerLobby() = delete;
		ServerLobby(ServerLobby const&) = delete;
		ServerLobby(ServerLobby&&) = delete;
	};
} // namespace GameServerApi
} // namespace AccelByte