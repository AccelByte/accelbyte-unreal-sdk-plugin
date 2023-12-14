// Copyright (c) 2023 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "CoreMinimal.h"
#include "Dom/JsonObject.h"
#include "Core/AccelByteUtilities.h"
#include "Models/AccelByteGameTelemetryModels.h"
#include "AccelByteGameStandardEventModels.generated.h"

/** @brief Game Standard Event Name list enum. */
UENUM(BlueprintType)
enum class EAccelByteGameStandardEventName : uint8
{
	resource_Sourced,
	resource_Sinked,
	resource_Upgraded,
	resource_Actioned,
	quest_Started,
	quest_Ended,
	player_Leveled,
	player_Dead,
	reward_Collected
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