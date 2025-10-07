// Copyright (c) 2023 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "CoreMinimal.h"
#include "Api/AccelByteBaseAnalyticsApi.h"
#include "Models/AccelByteGameStandardEventModels.h"

namespace AccelByte
{
class Credentials;
class Settings;
namespace Api
{

/**
 * @brief Send game standard telemetry data securely and the user should be logged in first.
 */
class ACCELBYTEUE4SDK_API GameStandardEvent : public BaseAnalytics
{
public:
	GameStandardEvent(Credentials& InCredentialsRef
		, Settings const& InSettingsRef
		, FHttpRetrySchedulerBase& InHttpRef
		, TSharedPtr<FApiClient, ESPMode::ThreadSafe> InApiClient = nullptr);

	bool SendMissionStartedEventData(FAccelByteId const& UserId
		, FMissionId const& MissionId
		, FMissionInstanceId const& MissionInstanceId
		, FAccelByteModelsMissionStartedOptPayload const& Optional = FAccelByteModelsMissionStartedOptPayload{});

	bool SendMissionStepEndedEventData(FAccelByteUserId const& UserId
		, FMissionId const& MissionId
		, FMissionInstanceId const& MissionInstanceId
		, FMissionStep const& MissionStep
		, FMissionStepName const& MissionStepName = FMissionStepName{});

	bool SendMissionEndedEventData(FAccelByteUserId const& UserId
		, FMissionId const& MissionId
		, FMissionInstanceId const& MissionInstanceId
		, FMissionSuccess const& MissionSuccess
		, FMissionOutcome const& MissionOutcome = FMissionOutcome{});

	bool SendMatchInfoEventData(FMatchInfoId const& MatchInfoId
		, FAccelByteModelsMatchInfoOptPayload const& Optional = FAccelByteModelsMatchInfoOptPayload{});

	bool SendMatchInfoPlayerEventData(FAccelByteUserId const& UserId
		, FMatchInfoId const& MatchInfoId
		, FAccelByteModelsMatchInfoPlayerOptPayload const& Optional = FAccelByteModelsMatchInfoPlayerOptPayload{});

	bool SendMatchInfoEndedEventData(FMatchInfoId const& MatchInfoId
		, FMatchEndReason const& EndReason
		, FAccelByteModelsMatchInfoEndedOptPayload const& Optional = FAccelByteModelsMatchInfoEndedOptPayload{});

	bool SendPopupAppearEventData(FAccelByteUserId const& UserId
		, FPopupEventId const& PopupId
		, FAccelByteModelsPopupAppearOptPayload const& Optional = FAccelByteModelsPopupAppearOptPayload{});

	bool SendEntityLeveledEventData(FEntityType const& EntityType
		, FEntityId const& EntityId = FEntityId{}
		, FAccelByteUserId const& UserId = FAccelByteUserId{}
		, FAccelByteModelsEntityLeveledOptPayload const& Optional = FAccelByteModelsEntityLeveledOptPayload{});

	bool SendEntityDeadEventData(FEntityType const& EntityType
		, FEntityId const& EntityId = FEntityId{}
		, FAccelByteUserId const& UserId = FAccelByteUserId{}
		, FAccelByteModelsEntityDeadOptPayload const& Optional = FAccelByteModelsEntityDeadOptPayload{});

	bool SendResourceFlowEventData(FAccelByteUserId const& UserId
		, EAccelByteFlowType const& FlowType
		, FAccelByteTransactionId const& TransactionId
		, FTransactionType const& TransactionType
		, FResourceName const& ResourceName
		, FResourceAmount const& Amount
		, FResourceEndBalance const& EndBalance);

	/**
	 * @brief Send/enqueue a single authorized telemetry data of a game standard event.
	 * User should be logged in.
	 *
	 * @param Payload The data to be send, each event expected to have different payload body, see Models/AccelByteGameStandardEventModels.h
	 * @param OnSuccess This will be called when the operation succeeded.
	 * @param OnError This will be called when the operation failed.
	 * @param ClientTimestamp Timestamp when the event is triggered, the default will be FDateTime::UtcNow()
	 */
	template<typename T>
	void SendGameStandardEventData(TSharedRef<T> const& Payload
		, FVoidHandler const& OnSuccess
		, FErrorHandler const& OnError
		, FDateTime const& ClientTimestamp = FDateTime::UtcNow())
	{
		if (!TIsDerivedFrom<T, FAccelByteModelsGameStandardEvent>::IsDerived)
		{
			OnError.ExecuteIfBound((int32)AccelByte::ErrorCodes::InvalidRequest, TEXT("Payload must derive from FAccelByteModelsGameStandardEvent!"));
			return;
		}
		else
		{
			Payload->GameStandardEventName = Payload->GetGameStandardEventName();
			SendEventData(Payload, OnSuccess, OnError, ClientTimestamp);
		}
	}

	void SendGameStandardEventData(TSharedRef<FAccelByteModelsCachedGameStandardEventPayload> const& Payload
		, FVoidHandler const& OnSuccess
		, FErrorHandler const& OnError
		, FDateTime const& ClientTimestamp = FDateTime::UtcNow());

private:
	GameStandardEvent() = delete;
	GameStandardEvent(GameStandardEvent const&) = delete;
	GameStandardEvent(GameStandardEvent&&) = delete;

	FString GetTelemetryKey() override;

};

typedef TSharedRef<GameStandardEvent, ESPMode::ThreadSafe> GameStandardEventRef;
typedef TSharedPtr<GameStandardEvent, ESPMode::ThreadSafe> GameStandardEventPtr;
typedef TWeakPtr<GameStandardEvent, ESPMode::ThreadSafe> GameStandardEventWPtr;

} // Namespace Api
} // Namespace AccelByte
