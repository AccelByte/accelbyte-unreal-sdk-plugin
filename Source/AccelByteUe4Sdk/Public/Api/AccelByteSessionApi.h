// Copyright (c) 2022 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.


#pragma once

#include "Core/AccelByteError.h"
#include "Core/AccelByteHttpRetryScheduler.h"
#include "Models/AccelByteCloudSaveModels.h"
#include "Models/AccelByteSessionModels.h"

namespace AccelByte
{
class Credentials;
class Settings;

namespace Api
{

class ACCELBYTEUE4SDK_API Session
{
public:
	Session(Credentials const& CredentialsRef, Settings const& SettingsRef, FHttpRetryScheduler& HttpRef);
	~Session();

	/**
	 * @brief Create a new party with the calling user as the sole member.
	 *
	 * @param CreateRequest The party creation request with attributes, join type, and members.
	 * @param OnSuccess This will be called if the operation succeeded.
	 * @param OnError This will be called if the operation failed.
	 */
	void CreateParty(FAccelByteModelsPartyCreateRequest const& CreateRequest, THandler<FAccelByteModelsBaseSession> const& OnSuccess, FErrorHandler const& OnError);
	
	/**
	 * @brief Retrieve party details for the given party ID.
	 *
	 * @param PartyID The ID of the party session to retrieve.
	 * @param OnSuccess This will be called if the operation succeeded.
	 * @param OnError This will be called if the operation failed.
	 */
	void GetPartyDetails(FString const& PartyID, THandler<FAccelByteModelsBaseSession> const& OnSuccess, FErrorHandler const& OnError);

	/**
     * @brief Send a party invite to the given user. 
     *
     * @param PartyID The ID of the party session for which the invite will be created.
     * @param UserID The ID of the user to invite.
     * @param OnSuccess This will be called if the operation succeeded.
     * @param OnError This will be called if the operation failed.
     */
	void SendPartyInvite(FString const& PartyID, FString const& UserID, FVoidHandler const& OnSuccess, FErrorHandler const& OnError);

	/**
	 * @brief Join a party on behalf of the user. 
	 *
	 * @param PartyID The ID of the party session which will be joined.
	 * @param OnSuccess This will be called if the operation succeeded.
	 * @param OnError This will be called if the operation failed.
	 */
	void JoinParty(FString const& PartyID, THandler<FAccelByteModelsBaseSession> const& OnSuccess, FErrorHandler const& OnError);
	
private:
	FHttpRetryScheduler& HttpRef;
	Credentials const& Credentials;
	Settings const& Settings;

	Session() = delete;
	Session(Session const&) = delete;
	Session(Session&&) = delete;
};
}
}
