// Copyright (c) 2023 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "CoreMinimal.h"
#include "Dom/JsonObject.h"
#include "Core/AccelByteUtilities.h"
#include "Core/AccelByteIdValidator.h"
#include "Models/AccelByteGameTelemetryModels.h"
#include "AccelByteGameStandardEventModels.generated.h"

#define INVALID_ACCELBYTE_ID TEXT("INVALID");

/** @brief Game Standard Event Name list enum. */
UENUM(BlueprintType)
enum class EAccelByteGameStandardEventName : uint8
{
	//Old event name
	quest_Started,
	quest_Ended,
	player_Leveled,
	player_Dead,
	reward_Collected,

	resource_Sourced,
	resource_Sinked,
	resource_Upgraded,
	resource_Actioned,
	
	//New event name
	mission_Started,
	mission_Step_Ended,
	mission_Ended,
	matchinfo,
	matchinfo_Player,
	matchinfo_Ended,
	popup_Appear,
	entity_Leveled,
	entity_Dead,
	resource_Flow
};

USTRUCT()
struct ACCELBYTEUE4SDK_API FAccelByteModelsGameStandardEvent
{
	GENERATED_BODY()

	UPROPERTY()
		FString GameStandardEventName{};

	virtual const FString GetGameStandardEventName()
	{
		return TEXT("GameStandardEvent");
	};

	FAccelByteModelsGameStandardEvent() = default;
	virtual ~FAccelByteModelsGameStandardEvent() = default;
};

#pragma region AccelByteId
USTRUCT()
struct ACCELBYTEUE4SDK_API FAccelByteId
{
	GENERATED_BODY()

	FAccelByteId()
	{
		AccelByteId = INVALID_ACCELBYTE_ID;
	};

	FAccelByteId(const FString& InAccelByteId)
	{
		if (!SetValue(InAccelByteId))
		{
			AccelByteId = INVALID_ACCELBYTE_ID;
		}
	};

	virtual ~FAccelByteId() {};

	FString ToString() const
	{
		return AccelByteId;
	}

	bool IsValid() const
	{
		return !AccelByteId.IsEmpty() && AccelByteId != INVALID_ACCELBYTE_ID;
	}

protected:
	virtual bool SetValue(const FString& InAccelByteId)
	{
		if (AccelByte::FAccelByteIdValidator::IsAccelByteIdValid(InAccelByteId, EAccelByteIdHypensRule::NO_HYPENS))
		{
			AccelByteId = InAccelByteId;
			return true;
		}	
		return false;
	}

private:
	FString AccelByteId{};
};

USTRUCT()
struct ACCELBYTEUE4SDK_API FAccelByteUserId : public FAccelByteId 
{
	GENERATED_BODY()

	FAccelByteUserId(const FString& InData)
		: FAccelByteId(InData) {};
	FAccelByteUserId()
		: FAccelByteId() {};
};

USTRUCT()
struct ACCELBYTEUE4SDK_API FAccelByteTransactionId : public FAccelByteId 
{
	GENERATED_BODY()

	FAccelByteTransactionId(const FString& InData)
		: FAccelByteId(InData) {};
	FAccelByteTransactionId()
		: FAccelByteId() {};
};

USTRUCT()
struct ACCELBYTEUE4SDK_API FMissionInstanceId : public FAccelByteId 
{
	GENERATED_BODY()

	FMissionInstanceId(const FString& InData)
		: FAccelByteId(InData) {};
	FMissionInstanceId()
		: FAccelByteId() {};
};

USTRUCT()
struct ACCELBYTEUE4SDK_API FEntityId : public FAccelByteId 
{
	GENERATED_BODY()

	FEntityId(const FString& InData)
		: FAccelByteId(InData) {};
	FEntityId()
		: FAccelByteId() {};
};
#pragma endregion AccelByteId

#pragma region FreeformString
USTRUCT()
struct ACCELBYTEUE4SDK_API FBaseFreeformData
{
	GENERATED_BODY()
	FBaseFreeformData(const FString& InData)
		:Data(InData)
	{};
	FBaseFreeformData() {};
	~FBaseFreeformData() {};

	FString ToString() const
	{
		return Data;
	};

	bool IsEmpty() const
	{
		return Data.IsEmpty();
	}

private:
	FString Data{};
};

USTRUCT()
struct ACCELBYTEUE4SDK_API FMissionId : public FBaseFreeformData
{
	GENERATED_BODY()

	FMissionId(const FString& InData)
		: FBaseFreeformData(InData) {};
	FMissionId()
		: FBaseFreeformData() {};
};

USTRUCT()
struct ACCELBYTEUE4SDK_API FTransactionType : public FBaseFreeformData
{
	GENERATED_BODY()

	FTransactionType(const FString& InData)
		: FBaseFreeformData(InData) {};
	FTransactionType()
		: FBaseFreeformData() {};
};

USTRUCT()
struct ACCELBYTEUE4SDK_API FResourceName : public FBaseFreeformData
{
	GENERATED_BODY()

	FResourceName(const FString& InData)
		: FBaseFreeformData(InData) {};
	FResourceName()
		: FBaseFreeformData() {};
};

USTRUCT()
struct ACCELBYTEUE4SDK_API FEntityType : public FBaseFreeformData
{
	GENERATED_BODY()

	FEntityType(const FString& InData)
		: FBaseFreeformData(InData) {};
	FEntityType()
		: FBaseFreeformData() {};
};

USTRUCT()
struct ACCELBYTEUE4SDK_API FMissionStepName : public FBaseFreeformData
{
	GENERATED_BODY()

	FMissionStepName(const FString& InData)
		: FBaseFreeformData(InData) {};
	FMissionStepName()
		: FBaseFreeformData() {};
};

USTRUCT()
struct ACCELBYTEUE4SDK_API FMissionOutcome : public FBaseFreeformData
{
	GENERATED_BODY()

	FMissionOutcome(const FString& InData)
		: FBaseFreeformData(InData) {};
	FMissionOutcome()
		: FBaseFreeformData() {};
};

USTRUCT()
struct ACCELBYTEUE4SDK_API FMatchInfoId : public FBaseFreeformData
{
	GENERATED_BODY()

	FMatchInfoId(const FString& InData)
		: FBaseFreeformData(InData) {};
	FMatchInfoId()
		: FBaseFreeformData() {};
};

USTRUCT()
struct ACCELBYTEUE4SDK_API FMatchEndReason : public FBaseFreeformData
{
	GENERATED_BODY()

	FMatchEndReason(const FString& InData)
		: FBaseFreeformData(InData) {};
	FMatchEndReason()
		: FBaseFreeformData() {};
};

USTRUCT()
struct ACCELBYTEUE4SDK_API FMatchGameMode : public FBaseFreeformData
{
	GENERATED_BODY()

	FMatchGameMode(const FString& InData)
		: FBaseFreeformData(InData) {};
	FMatchGameMode()
		: FBaseFreeformData() {};
};

USTRUCT()
struct ACCELBYTEUE4SDK_API FMatchDifficulty : public FBaseFreeformData
{
	GENERATED_BODY()

	FMatchDifficulty(const FString& InData)
		: FBaseFreeformData(InData) {};
	FMatchDifficulty()
		: FBaseFreeformData() {};
};

USTRUCT()
struct ACCELBYTEUE4SDK_API FMatchTeam : public FBaseFreeformData
{
	GENERATED_BODY()

	FMatchTeam(const FString& InData)
		: FBaseFreeformData(InData) {};
	FMatchTeam()
		: FBaseFreeformData() {};
};

USTRUCT()
struct ACCELBYTEUE4SDK_API FMatchClass : public FBaseFreeformData
{
	GENERATED_BODY()

	FMatchClass(const FString& InData)
		: FBaseFreeformData(InData) {};
	FMatchClass()
		: FBaseFreeformData() {};
};

USTRUCT()
struct ACCELBYTEUE4SDK_API FMatchRank : public FBaseFreeformData
{
	GENERATED_BODY()

	FMatchRank(const FString& InData)
		: FBaseFreeformData(InData) {};
	FMatchRank()
		: FBaseFreeformData() {};
};

USTRUCT()
struct ACCELBYTEUE4SDK_API FMatchWinner : public FBaseFreeformData
{
	GENERATED_BODY()

	FMatchWinner(const FString& InData)
		: FBaseFreeformData(InData) {};
	FMatchWinner()
		: FBaseFreeformData() {};
};

USTRUCT()
struct ACCELBYTEUE4SDK_API FPopupEventId : public FBaseFreeformData
{
	GENERATED_BODY()

	FPopupEventId(const FString& InData)
		: FBaseFreeformData(InData) {};
	FPopupEventId()
		: FBaseFreeformData() {};
};
#pragma endregion FreeformString

#pragma region NumbersData
USTRUCT()
struct ACCELBYTEUE4SDK_API FBaseNumberData
{
	GENERATED_BODY()
	FBaseNumberData(const FString& InData)
	{
		if (FCString::IsNumeric(*InData))
		{
			Data = InData;
		}
	};
	FBaseNumberData(float InData)
	{
		Data = FString::SanitizeFloat(InData);
	}
	FBaseNumberData(int32 InData)
	{
		Data = FString::FromInt(InData);
	}
	FBaseNumberData() {};
	virtual ~FBaseNumberData() {};

	FString ToString() const
	{
		return Data;
	}

	bool IsValid() const
	{
		return !Data.IsEmpty();
	}

private:
	FString Data{};
};

USTRUCT()
struct ACCELBYTEUE4SDK_API FResourceAmount : public FBaseNumberData
{
	GENERATED_BODY()

	FResourceAmount(const FString& InData)
		: FBaseNumberData(InData) {};
	FResourceAmount(int32 InData)
		: FBaseNumberData(InData) {};
	FResourceAmount(float InData)
		: FBaseNumberData(InData) {};
	FResourceAmount()
		: FBaseNumberData() {};
};

USTRUCT()
struct ACCELBYTEUE4SDK_API FResourceEndBalance : public FBaseNumberData
{
	GENERATED_BODY()

	FResourceEndBalance(const FString& InData)
		: FBaseNumberData(InData) {};
	FResourceEndBalance(int32 InData)
		: FBaseNumberData(InData) {};
	FResourceEndBalance(float InData)
		: FBaseNumberData(InData) {};
	FResourceEndBalance()
		: FBaseNumberData() {};
};
#pragma endregion NumbersData

USTRUCT()
struct ACCELBYTEUE4SDK_API FMissionStep
{
	GENERATED_BODY()
	FMissionStep(int32 Step)
	{
		if (Step > 0)
		{
			Number = Step;
		}
	}

	FMissionStep() {};
	~FMissionStep() {};

	int32 GetValue() const
	{
		return Number;
	}

private:
	int32 Number{0};
};

USTRUCT()
struct ACCELBYTEUE4SDK_API FMissionSuccess
{
	GENERATED_BODY()
	FMissionSuccess(bool bInSuccess)
		: bSuccess(bInSuccess)
	{}

	FMissionSuccess() {};
	~FMissionSuccess() {};

	bool GetValue() const
	{
		return bSuccess;
	}

private:
	bool bSuccess{false};
};

struct ACCELBYTEUE4SDK_API FAccelByteModelsCachedGameStandardEventPayload : public FAccelByteModelsGameStandardEvent
{
	FAccelByteModelsTelemetryBody Payload{};

	virtual const FString GetGameStandardEventName() override
	{
		return GameStandardEventName;
	};
};

#pragma region Resource Events
USTRUCT()
struct ACCELBYTEUE4SDK_API FAccelByteModelsResourceSourcedPayload : public FAccelByteModelsGameStandardEvent
{
	GENERATED_BODY()

	UPROPERTY()
		FString ResourceName{};
	UPROPERTY()
		FString ResourceID{};
	UPROPERTY()
		FString ResourceCategory{};
	UPROPERTY()
		FString ResourceRating{};
	UPROPERTY()
		FString ResourceSource{};
	UPROPERTY()
		FString ResourceLevelRequirement{};
	UPROPERTY()
		FString ResourceRarity{};
	UPROPERTY()
		FString SourcedAmount{};

	virtual const FString GetGameStandardEventName()
	{
		return FAccelByteUtilities::GetUEnumValueAsString(EAccelByteGameStandardEventName::resource_Sourced);
	};
};

USTRUCT()
struct ACCELBYTEUE4SDK_API FAccelByteModelsResourceSinkedPayload : public FAccelByteModelsGameStandardEvent
{
	GENERATED_BODY()

	UPROPERTY()
		FString ResourceName{};
	UPROPERTY()
		FString ResourceID{};
	UPROPERTY()
		FString ResourceCategory{};
	UPROPERTY()
		FString ResourceRating{};
	UPROPERTY()
		FString ResourceSource{};
	UPROPERTY()
		FString ResourceLevelRequirement{};
	UPROPERTY()
		FString ResourceRarity{};
	UPROPERTY()
		FString SinkAmount{};
	UPROPERTY()
		FString SinkedReason{};

	virtual const FString GetGameStandardEventName()
	{
		return FAccelByteUtilities::GetUEnumValueAsString(EAccelByteGameStandardEventName::resource_Sinked);
	};
};

USTRUCT()
struct ACCELBYTEUE4SDK_API FAccelByteModelsResourceUpgradedPayload : public FAccelByteModelsGameStandardEvent
{
	GENERATED_BODY()

	UPROPERTY()
		FString ResourceName{};
	UPROPERTY()
		FString ResourceID{};
	UPROPERTY()
		FString ResourceCategory{};
	UPROPERTY()
		FString ResourceRating{};
	UPROPERTY()
		FString ResourceSource{};
	UPROPERTY()
		FString ResourceLevelRequirement{};
	UPROPERTY()
		FString ResourceRarity{};
	UPROPERTY()
		FString UpgradeType{};
	UPROPERTY()
		FString UpgradeSource{};
	UPROPERTY()
		FString UpgradeLevel{};

	virtual const FString GetGameStandardEventName()
	{
		return FAccelByteUtilities::GetUEnumValueAsString(EAccelByteGameStandardEventName::resource_Upgraded);
	};
};

USTRUCT()
struct ACCELBYTEUE4SDK_API FAccelByteModelsResourceActionedPayload : public FAccelByteModelsGameStandardEvent
{
	GENERATED_BODY()

	UPROPERTY()
		FString ResourceName{};
	UPROPERTY()
		FString ResourceID{};
	UPROPERTY()
		FString ResourceCategory{};
	UPROPERTY()
		FString ResourceRating{};
	UPROPERTY()
		FString ResourceSource{};
	UPROPERTY()
		FString ResourceLevelRequirement{};
	UPROPERTY()
		FString ResourceRarity{};
	UPROPERTY()
		FString EquipLocation{};
	UPROPERTY()
		FString ActionName{};
	UPROPERTY()
		FString ActionTarget{};

	virtual const FString GetGameStandardEventName()
	{
		return FAccelByteUtilities::GetUEnumValueAsString(EAccelByteGameStandardEventName::resource_Actioned);
	};
};
#pragma endregion

#pragma region ProgressionEvents
USTRUCT()
struct ACCELBYTEUE4SDK_API FAccelByteModelsQuestStartedPayload : public FAccelByteModelsGameStandardEvent
{
	GENERATED_BODY()

	UPROPERTY()
		FString QuestName{};
	UPROPERTY()
		FString QuestID{};
	UPROPERTY()
		FString QuestType{};
	UPROPERTY()
		FString QuestDifficulty{};

	virtual const FString GetGameStandardEventName()
	{
		return FAccelByteUtilities::GetUEnumValueAsString(EAccelByteGameStandardEventName::quest_Started);
	};
};

USTRUCT()
struct ACCELBYTEUE4SDK_API FAccelByteModelsQuestEndedPayload : public FAccelByteModelsGameStandardEvent
{
	GENERATED_BODY()

	UPROPERTY()
		FString QuestName{};
	UPROPERTY()
		FString QuestID{};
	UPROPERTY()
		FString QuestType{};
	UPROPERTY()
		FString QuestOutcome{};
	UPROPERTY()
		FString QuestDifficulty{};

	virtual const FString GetGameStandardEventName()
	{
		return FAccelByteUtilities::GetUEnumValueAsString(EAccelByteGameStandardEventName::quest_Ended);
	};
};

USTRUCT()
struct ACCELBYTEUE4SDK_API FAccelByteModelsPlayerLeveledPayload : public FAccelByteModelsGameStandardEvent
{
	GENERATED_BODY()

	UPROPERTY()
		FString LevelStatus{};
	UPROPERTY()
		FString LevelID{};
	UPROPERTY()
		FString LevelName{};
	UPROPERTY()
		FString LevelDifficulty{};
	UPROPERTY()
		FString LevelSubject{};

	virtual const FString GetGameStandardEventName()
	{
		return FAccelByteUtilities::GetUEnumValueAsString(EAccelByteGameStandardEventName::player_Leveled);
	};
};

USTRUCT()
struct ACCELBYTEUE4SDK_API FAccelByteModelsPlayerDeadPayload : public FAccelByteModelsGameStandardEvent
{
	GENERATED_BODY()

	UPROPERTY()
		FString DeathTimeStamp{};
	UPROPERTY()
		FString DeathType{};
	UPROPERTY()
		FString DeathCause{};
	UPROPERTY()
		FString DeathLocation{};

	virtual const FString GetGameStandardEventName()
	{
		return FAccelByteUtilities::GetUEnumValueAsString(EAccelByteGameStandardEventName::player_Dead);
	};
};
#pragma endregion

#pragma region RewardEvents
USTRUCT()
struct ACCELBYTEUE4SDK_API FAccelByteModelsRewardCollectedPayload : public FAccelByteModelsGameStandardEvent
{
	GENERATED_BODY()

	UPROPERTY()
		FString RewardName{};
	UPROPERTY()
		FString RewardType{};
	UPROPERTY()
		FString RewardAmount{};

	virtual const FString GetGameStandardEventName()
	{
		return FAccelByteUtilities::GetUEnumValueAsString(EAccelByteGameStandardEventName::reward_Collected);
	};
};

#pragma endregion

#pragma region MissionEvents

USTRUCT()
struct ACCELBYTEUE4SDK_API FAccelByteModelsMissionStartedOptPayload : public FAccelByteModelsGameStandardEvent
{
	GENERATED_BODY()

	UPROPERTY()
	FString MissionType{};
	UPROPERTY()
	FString MissionName{};
	UPROPERTY()
	FString MissionLocation{};
	UPROPERTY()
	FString MissionDifficulty{};

	virtual const FString GetGameStandardEventName()
	{
		return FAccelByteUtilities::GetUEnumValueAsString(EAccelByteGameStandardEventName::mission_Started);
	};
};

USTRUCT()
struct ACCELBYTEUE4SDK_API FAccelByteModelsMissionStartedPayload : public FAccelByteModelsMissionStartedOptPayload
{
	GENERATED_BODY()

	FAccelByteModelsMissionStartedPayload(const FAccelByteModelsMissionStartedOptPayload& Payload)
		: FAccelByteModelsMissionStartedOptPayload()
	{
		MissionType = Payload.MissionType;
		MissionName = Payload.MissionName;
		MissionLocation = Payload.MissionLocation;
		MissionDifficulty = Payload.MissionDifficulty;
	}
	FAccelByteModelsMissionStartedPayload() {};
	~FAccelByteModelsMissionStartedPayload() {};

	UPROPERTY()
	FString UserID{};
	UPROPERTY()
	FString MissionID{};
	UPROPERTY()
	FString MissionInstanceID{};
};

USTRUCT()
struct ACCELBYTEUE4SDK_API FAccelByteModelsMissionStepEndedPayload : public FAccelByteModelsGameStandardEvent 
{
	GENERATED_BODY()

	UPROPERTY()
	FString UserID{};
	UPROPERTY()
	FString MissionID{};
	UPROPERTY()
	FString MissionInstanceID{};
	UPROPERTY()
	int32 MissionStep{ 0 };
	UPROPERTY()
	FString MissionStepName{};

	virtual const FString GetGameStandardEventName()
	{
		return FAccelByteUtilities::GetUEnumValueAsString(EAccelByteGameStandardEventName::mission_Step_Ended);
	};
};

USTRUCT()
struct ACCELBYTEUE4SDK_API FAccelByteModelsMissionEndedPayload : public FAccelByteModelsGameStandardEvent 
{
	GENERATED_BODY()

	UPROPERTY()
	FString UserID{};
	UPROPERTY()
	FString MissionID{};
	UPROPERTY()
	FString MissionInstanceID{};
	UPROPERTY()
	bool MissionSuccess{false};
	UPROPERTY()
	FString MissionOutcome{};

	virtual const FString GetGameStandardEventName()
	{
		return FAccelByteUtilities::GetUEnumValueAsString(EAccelByteGameStandardEventName::mission_Ended);
	};
};
#pragma endregion MissionEvents

#pragma region MatchInfoEvents
USTRUCT()
struct ACCELBYTEUE4SDK_API FAccelByteModelsMatchInfoOptPayload : public FAccelByteModelsGameStandardEvent
{
	GENERATED_BODY()

	UPROPERTY()
	FString MatchID{};
	UPROPERTY()
	FString GameMode{};
	UPROPERTY()
	FString MatchDifficulty{};

	virtual const FString GetGameStandardEventName()
	{
		return FAccelByteUtilities::GetUEnumValueAsString(EAccelByteGameStandardEventName::matchinfo);
	};
};

USTRUCT()
struct ACCELBYTEUE4SDK_API FAccelByteModelsMatchInfoPayload : public FAccelByteModelsMatchInfoOptPayload
{
	GENERATED_BODY()

	FAccelByteModelsMatchInfoPayload(const FAccelByteModelsMatchInfoOptPayload& Payload)
		: FAccelByteModelsMatchInfoOptPayload()
	{
		MatchID = Payload.MatchID;
		GameMode = Payload.GameMode;
		MatchDifficulty = Payload.MatchDifficulty;
	}
	FAccelByteModelsMatchInfoPayload() {};
	~FAccelByteModelsMatchInfoPayload() {};

	UPROPERTY()
	FString MatchinfoID{};
};

USTRUCT()
struct ACCELBYTEUE4SDK_API FAccelByteModelsMatchInfoPlayerOptPayload : public FAccelByteModelsGameStandardEvent
{
	GENERATED_BODY()

	UPROPERTY()
	FString MatchID{};
	UPROPERTY()
	FString Team{};
	UPROPERTY()
	FString Class{};
	UPROPERTY()
	FString Rank{};

	virtual const FString GetGameStandardEventName()
	{
		return FAccelByteUtilities::GetUEnumValueAsString(EAccelByteGameStandardEventName::matchinfo_Player);
	};
};

USTRUCT()
struct ACCELBYTEUE4SDK_API FAccelByteModelsMatchInfoPlayerPayload : public FAccelByteModelsMatchInfoPlayerOptPayload
{
	GENERATED_BODY()

	FAccelByteModelsMatchInfoPlayerPayload(const FAccelByteModelsMatchInfoPlayerOptPayload& Payload)
		: FAccelByteModelsMatchInfoPlayerOptPayload()
	{
		MatchID = Payload.MatchID;
		Team = Payload.Team;
		Class = Payload.Class;
		Rank = Payload.Rank;
	}
	FAccelByteModelsMatchInfoPlayerPayload() {};
	~FAccelByteModelsMatchInfoPlayerPayload() {};

	UPROPERTY()
	FString UserID{};
	UPROPERTY()
	FString MatchinfoID{};
};

USTRUCT()
struct ACCELBYTEUE4SDK_API FAccelByteModelsMatchInfoEndedOptPayload : public FAccelByteModelsGameStandardEvent
{
	GENERATED_BODY()

	UPROPERTY()
	FString MatchID{};
	UPROPERTY()
	FString Winner{};

	virtual const FString GetGameStandardEventName()
	{
		return FAccelByteUtilities::GetUEnumValueAsString(EAccelByteGameStandardEventName::matchinfo_Ended);
	};
};

USTRUCT()
struct ACCELBYTEUE4SDK_API FAccelByteModelsMatchInfoEndedPayload : public FAccelByteModelsMatchInfoEndedOptPayload
{
	GENERATED_BODY()

	FAccelByteModelsMatchInfoEndedPayload(const FAccelByteModelsMatchInfoEndedOptPayload& Payload)
		: FAccelByteModelsMatchInfoEndedOptPayload()
	{
		MatchID = Payload.MatchID;
		Winner = Payload.Winner;
	}
	FAccelByteModelsMatchInfoEndedPayload() {};
	~FAccelByteModelsMatchInfoEndedPayload() {};

	UPROPERTY()
	FString MatchinfoID{};
	UPROPERTY()
	FString EndReason{};
};
#pragma endregion MatchInfoEvents

#pragma region PopupEvents
USTRUCT()
struct ACCELBYTEUE4SDK_API FAccelByteModelsPopupAppearOptPayload : public FAccelByteModelsGameStandardEvent
{
	GENERATED_BODY()

	UPROPERTY()
	FString PopupType{};
	UPROPERTY()
	FString PopupName{};

	virtual const FString GetGameStandardEventName()
	{
		return FAccelByteUtilities::GetUEnumValueAsString(EAccelByteGameStandardEventName::popup_Appear);
	};
};

USTRUCT()
struct ACCELBYTEUE4SDK_API FAccelByteModelsPopupAppearPayload : public FAccelByteModelsPopupAppearOptPayload
{
	GENERATED_BODY()

	FAccelByteModelsPopupAppearPayload(const FAccelByteModelsPopupAppearOptPayload& Payload)
		: FAccelByteModelsPopupAppearOptPayload()
	{
		PopupType = Payload.PopupType;
		PopupName = Payload.PopupName;
	}
	FAccelByteModelsPopupAppearPayload() {};
	~FAccelByteModelsPopupAppearPayload() {};

	UPROPERTY()
	FString UserID{};
	UPROPERTY()
	FString PopupID{};
};
#pragma endregion PopupEvents

#pragma region EntityEvents
USTRUCT()
struct ACCELBYTEUE4SDK_API FAccelByteModelsEntityLeveledOptPayload : public FAccelByteModelsGameStandardEvent
{
	GENERATED_BODY()

	UPROPERTY()
	FString LevelStat{};
	UPROPERTY()
	int32 LevelChange{0};
	UPROPERTY()
	int32 LevelCurrent{0};

	virtual const FString GetGameStandardEventName()
	{
		return FAccelByteUtilities::GetUEnumValueAsString(EAccelByteGameStandardEventName::entity_Leveled);
	};
};

USTRUCT()
struct ACCELBYTEUE4SDK_API FAccelByteModelsEntityLeveledPayload : public FAccelByteModelsEntityLeveledOptPayload
{
	GENERATED_BODY()

	FAccelByteModelsEntityLeveledPayload(const FAccelByteModelsEntityLeveledOptPayload& Payload)
		: FAccelByteModelsEntityLeveledOptPayload()
	{
		LevelStat = Payload.LevelStat;
		LevelChange = Payload.LevelChange;
		LevelCurrent = Payload.LevelCurrent;
	};
	FAccelByteModelsEntityLeveledPayload() {};
	~FAccelByteModelsEntityLeveledPayload() {};

	UPROPERTY()
	FString EntityType{};
	UPROPERTY()
	FString UserID{};
	UPROPERTY()
	FString EntityID{};
};

USTRUCT()
struct ACCELBYTEUE4SDK_API FAccelByteModelsEntityDeadOptPayload : public FAccelByteModelsGameStandardEvent
{
	GENERATED_BODY()

	UPROPERTY()
	int32 DeathDay{0};
	UPROPERTY()
	FString DeathLocation{};
	UPROPERTY()
	FString DeathType{};
	UPROPERTY()
	FString DeathSource{};

	virtual const FString GetGameStandardEventName()
	{
		return FAccelByteUtilities::GetUEnumValueAsString(EAccelByteGameStandardEventName::entity_Dead);
	};
};

USTRUCT()
struct ACCELBYTEUE4SDK_API FAccelByteModelsEntityDeadPayload : public FAccelByteModelsEntityDeadOptPayload
{
	GENERATED_BODY()

	FAccelByteModelsEntityDeadPayload(const FAccelByteModelsEntityDeadOptPayload& Payload)
		: FAccelByteModelsEntityDeadOptPayload()
	{
		DeathDay = Payload.DeathDay;
		DeathLocation = Payload.DeathLocation;
		DeathType = Payload.DeathType;
		DeathSource = Payload.DeathSource;
	}
	FAccelByteModelsEntityDeadPayload() {};
	~FAccelByteModelsEntityDeadPayload() {};

	UPROPERTY()
	FString EntityType{};
	UPROPERTY()
	FString UserID{};
	UPROPERTY()
	FString EntityID{};
};
#pragma endregion EntityEvents

#pragma region ResourceFlowEvents
UENUM(BlueprintType)
enum class EAccelByteFlowType : uint8 
{
	sink,
	source
};

USTRUCT()
struct ACCELBYTEUE4SDK_API FAccelByteModelsResourceFlowPayload : public FAccelByteModelsGameStandardEvent
{
	GENERATED_BODY()

	UPROPERTY()
	FString UserID{};
	UPROPERTY()
	FString FlowType{};
	UPROPERTY()
	FString TransactionID{};
	UPROPERTY()
	FString TransactionType{};
	UPROPERTY()
	FString ResourceName{};
	UPROPERTY()
	FString Amount{};
	UPROPERTY()
	FString EndBalance{};

	virtual const FString GetGameStandardEventName()
	{
		return FAccelByteUtilities::GetUEnumValueAsString(EAccelByteGameStandardEventName::resource_Flow);
	};
};
#pragma endregion ResourceFlowEvents