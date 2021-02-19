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
			* @param UserId The id of the user who will receive achievement.
			* @param OnSuccess This will be called when the operation succeeded.
			* @param OnError This will be called when the operation failed.
		*/
		void GetPartyDataByUserId(const FString& UserId, const THandler<FAccelByteModelsDataPartyResponse> OnSuccess, const FErrorHandler& OnError);

		void WritePartyStorage(const FString& PartyId, TFunction<FJsonObjectWrapper(FJsonObjectWrapper)> PayloadModifier, const THandler<FAccelByteModelsPartyDataNotif>& OnSuccess, const FErrorHandler& OnError, uint32 RetryAttempt = 1);

		void GetPartyStorage(const FString& PartyId, const THandler<FAccelByteModelsPartyDataNotif>& OnSuccess, const FErrorHandler& OnError);

		void GetActiveParties(const int32& Limit, const int32& Offset, const THandler<FAccelByteModelsActivePartiesData>& OnSuccess, const FErrorHandler& OnError);

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