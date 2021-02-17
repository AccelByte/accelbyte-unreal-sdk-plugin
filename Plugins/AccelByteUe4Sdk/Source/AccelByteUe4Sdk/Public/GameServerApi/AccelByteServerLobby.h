// Copyright (c) 2020 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "CoreMinimal.h"
#include "Core/AccelByteError.h"

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

	private:
		const ServerCredentials& Credentials;
		const ServerSettings& Settings;

		ServerLobby() = delete;
		ServerLobby(ServerLobby const&) = delete;
		ServerLobby(ServerLobby&&) = delete;
	};
} // namespace GameServerApi
} // namespace AccelByte