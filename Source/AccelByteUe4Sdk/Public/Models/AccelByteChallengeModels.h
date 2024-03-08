// Copyright (c) 2024 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "CoreMinimal.h"
#include "JsonUtilities.h"
#include "Models/AccelByteGeneralModels.h"
#include "AccelByteChallengeModels.generated.h"

UENUM(BlueprintType)
enum class EAccelByteModelsChallengeSortBy : uint8
{
	UPDATED_AT = 0,
	UPDATED_AT_ASC,
	UPDATED_AT_DESC,
	CREATED_AT,
	CREATED_AT_ASC,
	CREATED_AT_DESC,
};

UENUM(BlueprintType)
enum class EAccelByteModelsChallengeStatus : uint8
{
	NONE = 0,
	INIT,
	RETIRED,
	TIED,
};

UENUM(BlueprintType)
enum class EAccelByteModelsChallengeRotation : uint8
{
	NONE = 0,
	DAILY,
	WEEKLY,
	MONTHLY,
	CUSTOM,
};

UENUM(BlueprintType)
enum class EAccelByteModelsChallengeAssignmentRule : uint8
{
	FIXED = 0,
	RANDOMIZED,
	UNSCHEDULED
};

UENUM(BlueprintType)
enum class EAccelByteModelsChallengeGoalsVisibility : uint8
{
	SHOWALL = 0,
	PERIODONLY,
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsChallenge
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | Challenges")
	FString Code{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | Challenges")
	FString Name{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | Challenges")
	FString Description{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | Challenges")
	EAccelByteModelsChallengeStatus Status{EAccelByteModelsChallengeStatus::NONE};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | Challenges")
	FString CreatedAt{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | Challenges")
	FString UpdatedAt{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | Challenges")
	FString StartDate{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | Challenges")
	FString EndDate{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | Challenges")
	int64 ActiveGoalsPerRotation{0};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | Challenges")
	EAccelByteModelsChallengeRotation Rotation{EAccelByteModelsChallengeRotation::NONE};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | Challenges")
	EAccelByteModelsChallengeAssignmentRule AssignmentRule{EAccelByteModelsChallengeAssignmentRule::FIXED};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | Challenges")
	EAccelByteModelsChallengeGoalsVisibility GoalsVisibility{EAccelByteModelsChallengeGoalsVisibility::SHOWALL};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsGetChallengesResponse
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | Challenges")
	TArray<FAccelByteModelsChallenge> Data{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | Challenges")
	FAccelByteModelsPaging Paging{};
};

UENUM(BlueprintType)
enum class EAccelByteModelsChallengeGoalRequirementPredicateParameterType : uint8
{
	STATISTIC = 0,
	ENTITLEMENT,
	ACHIEVEMENT,
	USER_ACCOUNT,
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsChallengeGoalRequirementPredicate
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | Challenges | Goals")
	EAccelByteModelsChallengeGoalRequirementPredicateParameterType ParameterType{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | Challenges | Goals")
	FString ParameterName{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | Challenges | Goals")
	FString Matcher{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | Challenges | Goals")
	float TargetValue{0.0f};
};

UENUM(BlueprintType)
enum class EAccelByteModelsChallengeGoalRequirementOperator : uint8
{
	AND = 0,
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsChallengeGoalRequirement
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | Challenges | Goals")
	EAccelByteModelsChallengeGoalRequirementOperator Operator{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | Challenges | Goals")
	TArray<FAccelByteModelsChallengeGoalRequirementPredicate> Predicates{};
};

UENUM(BlueprintType)
enum class EAccelByteModelsChallengeRewardType : uint8
{
	STATISTIC = 0,
	ENTITLEMENT,
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsChallengeGoalReward
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | Challenges | Rewards")
	float Qty{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | Challenges | Rewards")
	FString ItemId{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | Challenges | Rewards")
	FString ItemName{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | Challenges | Rewards")
	EAccelByteModelsChallengeRewardType Type{EAccelByteModelsChallengeRewardType::STATISTIC};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsChallengeGoal
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | Challenges | Goals")
	FString Code{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | Challenges | Goals")
	FString ChallengeCode{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | Challenges | Goals")
	FString Namespace{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | Challenges | Goals")
	FString Name{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | Challenges | Goals")
	FString Description{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | Challenges | Goals")
	FString CreatedAt{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | Challenges | Goals")
	FString UpdatedAt{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | Challenges | Goals")
	bool IsActive{false};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | Challenges | Goals")
	TArray<FString> Tags{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | Challenges | Goals")
	TArray<FAccelByteModelsChallengeGoalRequirement> RequirementGroups{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | Challenges | Goals")
	TArray<FAccelByteModelsChallengeGoalReward> Rewards{};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsGetScheduledChallengeGoalsResponse
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | Challenges | Goals")
	TArray<FAccelByteModelsChallengeGoal> Data{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | Challenges | Goals")
	FAccelByteModelsPaging Paging{};
};

UENUM(BlueprintType)
enum class EAccelByteModelsChallengeRewardStatus : uint8
{
	NONE = 0,
	UNCLAIMED,
	CLAIMED,
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsChallengeReward
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | Challenges | Rewards")
	FString Id{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | Challenges | Rewards")
	FString GoalCode{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | Challenges | Rewards")
	EAccelByteModelsChallengeRewardStatus Status{EAccelByteModelsChallengeRewardStatus::UNCLAIMED};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | Challenges | Rewards")
	FString UserId{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | Challenges | Rewards")
	float Qty{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | Challenges | Rewards")
	FString ItemId{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | Challenges | Rewards")
	FString ItemName{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | Challenges | Rewards")
	EAccelByteModelsChallengeRewardType Type{EAccelByteModelsChallengeRewardType::STATISTIC};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | Challenges | Rewards")
	FString CreatedAt{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | Challenges | Rewards")
	FString UpdatedAt{};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsChallengeGetRewardStatusResponse
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | Challenges | Rewards")
	TArray<FAccelByteModelsChallengeReward> Data{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | Challenges | Rewards")
	FAccelByteModelsPaging Paging{};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsChallengeRewardClaimRequest
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | Challenges | Rewards")
	TArray<FString> RewardIDs{};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsChallengeGoalProgressRequirement
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | Challenges | Progress")
	FString Id{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | Challenges | Progress")
	FString CompletedAt{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | Challenges | Progress")
	float TargetValue{0.0f};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | Challenges | Progress")
	float CurrentValue{0.0f};
};

UENUM(BlueprintType)
enum class EAccelByteModelsChallengeGoalProgressStatus : uint8
{
	ACTIVE = 0,
	COMPLETED,
	RETIRED,
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsChallengeGoalProgress
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | Challenges | Progress")
	FString GoalCode{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | Challenges | Progress")
	FString GoalProgressionId{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | Challenges | Progress")
	TArray<FAccelByteModelsChallengeGoalProgressRequirement> RequirementProgressions{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | Challenges | Progress")
	EAccelByteModelsChallengeGoalProgressStatus Status{};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsChallengeProgressMeta
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | Challenges | Progress")
	FString Code{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | Challenges | Progress")
	FString Name{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | Challenges | Progress")
	FString Description{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | Challenges | Progress")
	FString StartDate{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | Challenges | Progress")
	FString EndDate{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | Challenges | Progress")
	FString UserId{};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsChallengeProgressResponse
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | Challenges | Progress")
	TArray<FAccelByteModelsChallengeGoalProgress> Data{};
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | Challenges | Progress")
	FAccelByteModelsChallengeProgressMeta Meta{};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsChallengeServerEvaluateProgressRequest
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | Challenges | Server | Progress")
	TArray<FString> UserIds{};

};
