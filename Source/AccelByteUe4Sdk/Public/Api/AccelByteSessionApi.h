// Copyright (c) 2022 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.


#pragma once

#include "Core/AccelByteApiBase.h"
#include "Core/AccelByteError.h"
#include "Core/AccelByteHttpRetryScheduler.h"
#include "Models/AccelByteSessionModels.h"
#include "Core/AccelByteUtilities.h"

namespace AccelByte
{
class Credentials;
class Settings;

namespace GameServerApi
{
class ServerSession;
}

namespace Api
{

class ServerSessionApi;

class ACCELBYTEUE4SDK_API Session : public FApiBase
{
public:
	Session(Credentials const& InCredentialsRef, Settings const& InSettingsRef, FHttpRetryScheduler& InHttpRef);
	~Session();

	/**
	 * @brief Create a new game session.
	 *
	 * @param CreateRequest The game session creation request body.
	 * @param OnSuccess This will be called if the operation succeeded.
	 * @param OnError This will be called if the operation failed.
	 */
	void CreateGameSession(FAccelByteModelsV2GameSessionCreateRequest const& CreateRequest, THandler<FAccelByteModelsV2GameSession> const& OnSuccess, FErrorHandler const& OnError);

	/**
	 * @brief Get details for a game session by ID
	 *
	 * @param GameSessionID The ID of the session.
	 * @param OnSuccess This will be called if the operation succeeded.
	 * @param OnError This will be called if the operation failed.
	 */
	void GetGameSessionDetails(FString const& GameSessionID, THandler<FAccelByteModelsV2GameSession> const& OnSuccess, FErrorHandler const& OnError);

	/**
	 * @brief Query game sessions.
	 *
	 * @param QueryObject Query object containing the parameters to query on.
	 * @param OnSuccess This will be called if the operation succeeded.
	 * @param OnError This will be called if the operation failed.
	 * @param Offset Pagination offset.
	 * @param Limit Pagination limit.
	 */
	void QueryGameSessions(FAccelByteModelsV2GameSessionQuery const& QueryObject, THandler<FAccelByteModelsV2PaginatedGameSessionQueryResult> const& OnSuccess, FErrorHandler const& OnError, int32 const& Offset = 0, int32 const& Limit = 20);

	/**
	 * @brief Update a game session by ID.
	 *
	 * @param GameSessionID The ID of the session.
	 * @param UpdateRequest The game session update request body.
	 * @param OnSuccess This will be called if the operation succeeded.
	 * @param OnError This will be called if the operation failed.
	 */
	void UpdateGameSession(FString const& GameSessionID, FAccelByteModelsV2GameSessionUpdateRequest const& UpdateRequest, THandler<FAccelByteModelsV2GameSession> const& OnSuccess, FErrorHandler const& OnError);

	/**
	 * @brief Delete a game session by ID.
	 *
	 * @param GameSessionID The ID of the session.
	 * @param OnSuccess This will be called if the operation succeeded.
	 * @param OnError This will be called if the operation failed.
	 */
	void DeleteGameSession(FString const& GameSessionID, FVoidHandler const& OnSuccess, FErrorHandler const& OnError);

	/**
	 * @brief Send an invite to a game session by ID.
	 *
	 * @param GameSessionID The ID of the session.
	 * @param UserID The ID of the user to invite.
	 * @param OnSuccess This will be called if the operation succeeded.
	 * @param OnError This will be called if the operation failed.
	 */
	void SendGameSessionInvite(FString const& GameSessionID, FString const& UserID, FVoidHandler const& OnSuccess, FErrorHandler const& OnError);

	/**
	 * @brief Reject an invite to a game session by game session ID.
	 *
	 * @param GameSessionID The ID of the session.
	 * @param OnSuccess This will be called if the operation succeeded.
	 * @param OnError This will be called if the operation failed.
	 */
	void RejectGameSessionInvite(FString const& GameSessionID, FVoidHandler const& OnSuccess, FErrorHandler const& OnError);

	/**
	 * @brief Join a game session by ID.
	 *
	 * @param GameSessionID The ID of the session.
	 * @param OnSuccess This will be called if the operation succeeded.
	 * @param OnError This will be called if the operation failed.
	 */
	void JoinGameSession(FString const& GameSessionID, THandler<FAccelByteModelsV2GameSession> const& OnSuccess, FErrorHandler const& OnError);

	/**
	 * @brief Leave a game session by ID.
	 *
	 * @param GameSessionID The ID of the session.
	 * @param OnSuccess This will be called if the operation succeeded.
	 * @param OnError This will be called if the operation failed.
	 */
	void LeaveGameSession(FString const& GameSessionID, FVoidHandler const& OnSuccess, FErrorHandler const& OnError);

	/**
	 * @brief Get a list of the logged in user's game sessions.
	 *
	 * @param OnSuccess This will be called if the operation succeeded.
	 * @param OnError This will be called if the operation failed.
	 * @param Status Optional membership status to query for - either active or invited.
	 */
	void GetMyGameSessions(THandler<FAccelByteModelsV2PaginatedGameSessionQueryResult> const& OnSuccess, FErrorHandler const& OnError, EAccelByteV2SessionMemberStatus Status = EAccelByteV2SessionMemberStatus::EMPTY);

	/**
	 * @brief Create a new party with the calling user as the sole member.
	 *
	 * @param CreateRequest The party creation request with attributes, join type, and members.
	 * @param OnSuccess This will be called if the operation succeeded.
	 * @param OnError This will be called if the operation failed.
	 */
	void CreateParty(FAccelByteModelsV2PartyCreateRequest const& CreateRequest, THandler<FAccelByteModelsV2PartySession> const& OnSuccess, FErrorHandler const& OnError);

	/**
	 * @brief Retrieve party details for the given party ID.
	 *
	 * @param PartyID The ID of the party session to retrieve.
	 * @param OnSuccess This will be called if the operation succeeded.
	 * @param OnError This will be called if the operation failed.
	 */
	void GetPartyDetails(FString const& PartyID, THandler<FAccelByteModelsV2PartySession> const& OnSuccess, FErrorHandler const& OnError);

	/**
	 * @brief Update a party 
	 *
	 * @param PartyID The ID of the party session to update.
	 * @param UpdateRequest The request body to update the party with.
	 * @param OnSuccess This will be called if the operation succeeded.
	 * @param OnError This will be called if the operation failed.
	 */
	void UpdateParty(FString const& PartyID, FAccelByteModelsV2PartyUpdateRequest const& UpdateRequest, THandler<FAccelByteModelsV2PartySession> const& OnSuccess, FErrorHandler const& OnError);

	/**
	 * @brief Promote a new party leader by user ID
	 *
	 * @param PartyID The ID of the party session to update.
	 * @param NewLeaderID The user ID of the new leader to promote.
	 * @param OnSuccess This will be called if the operation succeeded.
	 * @param OnError This will be called if the operation failed.
	 */
	void PromotePartyLeader(FString const& PartyID, FString const& NewLeaderID, THandler<FAccelByteModelsV2PartySession> const& OnSuccess, FErrorHandler const& OnError);

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
	 * @brief Reject an invite to a party 
	 *
	 * @param PartyID The ID of the party session containing the invite to reject.
	 * @param OnSuccess This will be called if the operation succeeded.
	 * @param OnError This will be called if the operation failed.
	 */
	void RejectPartyInvite(FString const& PartyID, FVoidHandler const& OnSuccess, FErrorHandler const& OnError);

	/**
	 * @brief Join a party on behalf of the user.
	 *
	 * @param PartyID The ID of the party session which will be joined.
	 * @param OnSuccess This will be called if the operation succeeded.
	 * @param OnError This will be called if the operation failed.
	 */
	void JoinParty(FString const& PartyID, THandler<FAccelByteModelsV2PartySession> const& OnSuccess, FErrorHandler const& OnError);

	/**
	 * @brief Leave a party on behalf of the user.
	 *
	 * @param PartyID The ID of the party session which will be joined.
	 * @param OnSuccess This will be called if the operation succeeded.
	 * @param OnError This will be called if the operation failed.
	 */
	void LeaveParty(FString const& PartyID, FVoidHandler const& OnSuccess, FErrorHandler const& OnError);

	/**
	 * @brief Kick another user from a party.
	 *
	 * @param PartyID The ID of the party session which the user will be kicked from.
	 * @param UserID The ID of the user to kick 
	 * @param OnSuccess This will be called if the operation succeeded.
	 * @param OnError This will be called if the operation failed.
	 */
	void KickUserFromParty(FString const& PartyID, FString const& UserID, THandler<FAccelByteModelsV2PartySession> const& OnSuccess, FErrorHandler const& OnError);

	/**
	 * @brief Get a list of parties matching the given query.
	 *
	 * @param OnSuccess This will be called if the operation succeeded.
	 * @param OnError This will be called if the operation failed.
	 * @param Status Optional membership status to query for - either active or invited.
	 */
	void GetMyParties(THandler<FAccelByteModelsV2PaginatedPartyQueryResult> const& OnSuccess, FErrorHandler const& OnError, EAccelByteV2SessionMemberStatus Status = EAccelByteV2SessionMemberStatus::EMPTY);

private:
	Session() = delete;
	Session(Session const&) = delete;
	Session(Session&&) = delete;

	// The server session API class needs access to `SerializeAndRemoveEmptyValues`
	friend class GameServerApi::ServerSession;

	static void RemoveEmptyEnumValue(TSharedPtr<FJsonObject> JsonObjectPtr, const FString& EnumFieldName);
	static void RemoveEmptyEnumValuesFromChildren(TSharedPtr<FJsonObject> JsonObjectPtr, const FString& FieldName, const FString& EnumFieldName);

	template <typename DataStruct>
	static void SerializeAndRemoveEmptyValues(const DataStruct& Model, FString& OutputString, bool bIncludeTeams = true)
	{
		TSharedPtr<FJsonObject> JsonObjectPtr;
		SerializeAndRemoveEmptyValues(Model, JsonObjectPtr, bIncludeTeams);

		auto Writer = TJsonWriterFactory<>::Create(&OutputString);
		FJsonSerializer::Serialize(JsonObjectPtr.ToSharedRef(), Writer);
	}

	template <typename DataStruct>
	static void SerializeAndRemoveEmptyValues(const DataStruct& Model, TSharedPtr<FJsonObject>& OutJsonObjectPtr, bool bIncludeTeams = true)
	{
		auto JsonObjectPtr = FJsonObjectConverter::UStructToJsonObject(Model);

		RemoveEmptyEnumValue(JsonObjectPtr, TEXT("joinability"));
		RemoveEmptyEnumValue(JsonObjectPtr, TEXT("type"));
		RemoveEmptyEnumValuesFromChildren(JsonObjectPtr, TEXT("members"), TEXT("status"));

		TArray<FString> ExcludedFieldNamesForRemoval;
		if(bIncludeTeams)
		{
			ExcludedFieldNamesForRemoval.Add(TEXT("teams"));
		}

		FAccelByteUtilities::RemoveEmptyFieldsFromJson(JsonObjectPtr, FAccelByteUtilities::FieldRemovalFlagAll, ExcludedFieldNamesForRemoval);

		OutJsonObjectPtr = JsonObjectPtr;
	}
};
}
}
