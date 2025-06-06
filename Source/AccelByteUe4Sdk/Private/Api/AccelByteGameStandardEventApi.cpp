// Copyright (c) 2023 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "Api/AccelByteGameStandardEventApi.h"

namespace AccelByte
{
namespace Api
{
	
GameStandardEvent::GameStandardEvent(Credentials& InCredentialsRef
	, Settings const& InSettingsRef
	, FHttpRetryScheduler& InHttpRef
	, TSharedPtr<FApiClient, ESPMode::ThreadSafe> InApiClient)
	: BaseAnalytics(InCredentialsRef, InSettingsRef, InHttpRef, InApiClient, TEXT("GameStandardEvent"), true, true)
{}

FString GameStandardEvent::GetTelemetryKey()
{
	FString Environment = *SettingsRef.SettingsEnvironment;
	FString UserId = *CredentialsRef->GetUserId();
	if (UserId.IsEmpty())
	{
		return UserId;
	}
	FString TelemetryKey = FString::Printf(TEXT("%sGameStandardClientEvent_%s"), *Environment, *UserId);
	return TelemetryKey;
}

bool GameStandardEvent::SendMissionStartedEventData(FAccelByteId const& UserId
	, FMissionId const& MissionId
	, FMissionInstanceId const& MissionInstanceId
	, FAccelByteModelsMissionStartedOptPayload const& Optional)
{
	if (!UserId.IsValid()
		|| MissionId.IsEmpty()
		|| !MissionInstanceId.IsValid())
	{
		return false;
	}
	TSharedRef<FAccelByteModelsMissionStartedPayload> Payload = MakeShared<FAccelByteModelsMissionStartedPayload>( Optional );
	Payload->UserID = UserId.ToString();
	Payload->MissionID = MissionId.ToString();
	Payload->MissionInstanceID = MissionInstanceId.ToString();
	SendGameStandardEventData(Payload, FVoidHandler(), FErrorHandler());
	return true;
}

bool GameStandardEvent::SendMissionStepEndedEventData(FAccelByteUserId const& UserId
	, FMissionId const& MissionId
	, FMissionInstanceId const& MissionInstanceId
	, FMissionStep const& MissionStep
	, FMissionStepName const& MissionStepName)
{
	if (!UserId.IsValid()
		|| MissionId.IsEmpty()
		|| !MissionInstanceId.IsValid())
	{
		return false;
	}
	TSharedRef<FAccelByteModelsMissionStepEndedPayload> Payload = MakeShared<FAccelByteModelsMissionStepEndedPayload>();
	Payload->UserID = UserId.ToString();
	Payload->MissionID = MissionId.ToString();
	Payload->MissionInstanceID = MissionInstanceId.ToString();
	Payload->MissionStep = MissionStep.GetValue();
	Payload->MissionStepName = MissionStepName.ToString();
	SendGameStandardEventData(Payload, FVoidHandler(), FErrorHandler());
	return true;
}

bool GameStandardEvent::SendMissionEndedEventData(FAccelByteUserId const& UserId
	, FMissionId const& MissionId
	, FMissionInstanceId const& MissionInstanceId
	, FMissionSuccess const& MissionSuccess
	, FMissionOutcome const& MissionOutcome)
{
	if (!UserId.IsValid()
		|| MissionId.IsEmpty()
		|| !MissionInstanceId.IsValid())
	{
		return false;
	}
	TSharedRef<FAccelByteModelsMissionEndedPayload> Payload = MakeShared<FAccelByteModelsMissionEndedPayload>();
	Payload->UserID = UserId.ToString();
	Payload->MissionID = MissionId.ToString();
	Payload->MissionInstanceID = MissionInstanceId.ToString();
	Payload->MissionSuccess = MissionSuccess.GetValue();
	Payload->MissionOutcome = MissionOutcome.ToString();
	SendGameStandardEventData(Payload, FVoidHandler(), FErrorHandler());
	return true;
}

bool GameStandardEvent::SendMatchInfoEventData(FMatchInfoId const& MatchInfoId
	, FAccelByteModelsMatchInfoOptPayload const& Optional)
{
	if (MatchInfoId.IsEmpty())
	{
		return false;
	}
	TSharedRef<FAccelByteModelsMatchInfoPayload> Payload = MakeShared<FAccelByteModelsMatchInfoPayload>(Optional);
	Payload->MatchinfoID = MatchInfoId.ToString();
	SendGameStandardEventData(Payload, FVoidHandler(), FErrorHandler());
	return true;
}

bool GameStandardEvent::SendMatchInfoPlayerEventData(FAccelByteUserId const& UserId
	, FMatchInfoId const& MatchInfoId
	, FAccelByteModelsMatchInfoPlayerOptPayload const& Optional)
{
	if (!UserId.IsValid()
		|| MatchInfoId.IsEmpty())
	{
		return false;
	}
	TSharedRef<FAccelByteModelsMatchInfoPlayerPayload> Payload = MakeShared<FAccelByteModelsMatchInfoPlayerPayload>(Optional);
	Payload->UserID = UserId.ToString();
	Payload->MatchinfoID = MatchInfoId.ToString();
	SendGameStandardEventData(Payload, FVoidHandler(), FErrorHandler());
	return true;
}

bool GameStandardEvent::SendMatchInfoEndedEventData(FMatchInfoId const& MatchInfoId
	, FMatchEndReason const& EndReason
	, FAccelByteModelsMatchInfoEndedOptPayload const& Optional)
{
	if (MatchInfoId.IsEmpty()
		|| EndReason.IsEmpty())
	{
		return false;
	}
	TSharedRef<FAccelByteModelsMatchInfoEndedPayload> Payload = MakeShared<FAccelByteModelsMatchInfoEndedPayload>(Optional);
	Payload->MatchinfoID = MatchInfoId.ToString();
	Payload->EndReason = EndReason.ToString();
	SendGameStandardEventData(Payload, FVoidHandler(), FErrorHandler());
	return true;
}

bool GameStandardEvent::SendPopupAppearEventData(FAccelByteUserId const& UserId
	, FPopupEventId const& PopupId
	, FAccelByteModelsPopupAppearOptPayload const& Optional)
{
	if (!UserId.IsValid()
		|| PopupId.IsEmpty())
	{
		return false;
	}
	TSharedRef<FAccelByteModelsPopupAppearPayload> Payload = MakeShared<FAccelByteModelsPopupAppearPayload>(Optional);
	Payload->UserID = UserId.ToString();
	Payload->PopupID = PopupId.ToString();
	SendGameStandardEventData(Payload, FVoidHandler(), FErrorHandler());
	return true;
}

bool GameStandardEvent::SendEntityLeveledEventData(FEntityType const& EntityType
	, FEntityId const& EntityId
	, FAccelByteUserId const& UserId
	, FAccelByteModelsEntityLeveledOptPayload const& Optional)
{
	if (EntityType.IsEmpty())
	{
		return false;
	}
	TSharedRef<FAccelByteModelsEntityLeveledPayload> Payload = MakeShared<FAccelByteModelsEntityLeveledPayload>(Optional);
	Payload->EntityType = EntityType.ToString();
	Payload->EntityID = EntityId.ToString();
	if (UserId.IsValid())
	{
		Payload->UserID = UserId.ToString();
	}
	SendGameStandardEventData(Payload, FVoidHandler(), FErrorHandler());
	return true;
}

bool GameStandardEvent::SendEntityDeadEventData(FEntityType const& EntityType
	, FEntityId const& EntityId
	, FAccelByteUserId const& UserId
	, FAccelByteModelsEntityDeadOptPayload const& Optional)
{
	if (EntityType.IsEmpty())
	{
		return false;
	}
	TSharedRef<FAccelByteModelsEntityDeadPayload> Payload = MakeShared<FAccelByteModelsEntityDeadPayload>(Optional);
	Payload->EntityType = EntityType.ToString();
	Payload->EntityID = EntityId.ToString();
	if (UserId.IsValid())
	{
		Payload->UserID = UserId.ToString();
	}
	SendGameStandardEventData(Payload, FVoidHandler(), FErrorHandler());
	return true;
}

bool GameStandardEvent::SendResourceFlowEventData(FAccelByteUserId const& UserId
	, EAccelByteFlowType const& FlowType
	, FAccelByteTransactionId const& TransactionId
	, FTransactionType const& TransactionType
	, FResourceName const& ResourceName
	, FResourceAmount const& Amount
	, FResourceEndBalance const& EndBalance)
{
	if (!UserId.IsValid()
		|| !TransactionId.IsValid()
		|| TransactionType.IsEmpty()
		|| ResourceName.IsEmpty()
		|| !Amount.IsValid()
		|| !EndBalance.IsValid())
	{
		return false;
	}
	TSharedRef<FAccelByteModelsResourceFlowPayload> Payload = MakeShared<FAccelByteModelsResourceFlowPayload>();
	Payload->UserID = UserId.ToString();
	Payload->FlowType = FAccelByteUtilities::GetUEnumValueAsString(FlowType);
	Payload->TransactionID = TransactionId.ToString();
	Payload->TransactionType = TransactionType.ToString();
	Payload->ResourceName = ResourceName.ToString();
	Payload->Amount = Amount.ToString();
	Payload->EndBalance = EndBalance.ToString();
	SendGameStandardEventData(Payload, FVoidHandler(), FErrorHandler());
	return true;
}

void GameStandardEvent::SendGameStandardEventData(TSharedRef<FAccelByteModelsCachedGameStandardEventPayload> const& Payload
	, FVoidHandler const& OnSuccess
	, FErrorHandler const& OnError
	, FDateTime const& ClientTimestamp)
{

	UEnum* EnumPtr = StaticEnum<EAccelByteGameStandardEventName>();
	int64 ValueInt = 0;

	if (EnumPtr != nullptr)
	{
		ValueInt = EnumPtr->GetValueByNameString(Payload->GetGameStandardEventName());

		if (ValueInt == INDEX_NONE)
		{
			OnError.ExecuteIfBound((int32)AccelByte::ErrorCodes::InvalidRequest, TEXT("EventName is not listed on EAccelByteGameStandardEventName!"));
			return;
		}
	}
	else
	{
		OnError.ExecuteIfBound((int32)AccelByte::ErrorCodes::UnknownError, TEXT("Failed to process the request, Unknown Error!"));
		return;
	}

	SendEventData(Payload->Payload, OnSuccess, OnError, ClientTimestamp);
}

} 
}