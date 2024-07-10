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
	NONE = 0,
	UPDATED_AT,
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
	NONE = 0,
	FIXED,
	RANDOMIZED,
	UNSCHEDULED
};

UENUM(BlueprintType)
enum class EAccelByteModelsChallengeGoalsVisibility : uint8
{
	NONE = 0,
	SHOWALL,
	PERIODONLY,
};

UENUM(BlueprintType)
enum class EAccelByteModelsChallengeMatcher : uint8
{
	NONE = 0,
	EQUAL,
	LESS_THAN,
	GREATER_THAN,
	LESS_THAN_EQUAL,
	GREATER_THAN_EQUAL
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsChallengeResetConfig
{
	GENERATED_BODY()

	// Valid value are 1 to 31, default to 1 for MONTHLY rotation
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | Challenges | ResetConfig")
	int32 ResetDate{};

	// Valid value are 1 (Monday) to 7 (Sunday), default to 1 for WEEKLY rotation
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | Challenges | ResetConfig")
	int32 ResetDay{};

	// Format hh:mm, must be in UTC timezone, default '00:00' for all rotation
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | Challenges | ResetConfig")
	FString ResetTime{};
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
	EAccelByteModelsChallengeAssignmentRule AssignmentRule{EAccelByteModelsChallengeAssignmentRule::NONE};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | Challenges")
	EAccelByteModelsChallengeGoalsVisibility GoalsVisibility{EAccelByteModelsChallengeGoalsVisibility::NONE};

	// Describe number of period challenge will be retired after.
	// The unit depends on rotation type. If rotation is DAILY the unit is in days, MONTHLY the unit is in months etc.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | Challenges")
	int32 EndAfter{0};

	// Describe number of period challenge's goals will be repeated after.
	// The unit depends on rotation type. If rotation is DAILY the unit is in days, MONTHLY the unit is in months etc.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | Challenges")
	int32 RepeatAfter{0};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | Challenges")
	FAccelByteModelsChallengeResetConfig ResetConfig;
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
	NONE = 0,
	STATISTIC,
	ENTITLEMENT,
	ACHIEVEMENT,
	USERACCOUNT,
	STATISTIC_CYCLE,
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsChallengeGoalRequirementPredicate
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | Challenges | Goals")
	EAccelByteModelsChallengeGoalRequirementPredicateParameterType ParameterType{EAccelByteModelsChallengeGoalRequirementPredicateParameterType::NONE};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | Challenges | Goals")
	FString ParameterName{};

	// EQUAL, LESS_THAN, GREATER_THAN, LESS_THAN_EQUAL, GREATER_THAN_EQUAL
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | Challenges | Goals")
	FString Matcher{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | Challenges | Goals")
	float TargetValue{0.0f};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | Challenges | Goals")
	FString StatCycleId{};
};

UENUM(BlueprintType)
enum class EAccelByteModelsChallengeGoalRequirementOperator : uint8
{
	NONE = 0,
	AND,
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsChallengeGoalRequirement
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | Challenges | Goals")
	EAccelByteModelsChallengeGoalRequirementOperator Operator{EAccelByteModelsChallengeGoalRequirementOperator::NONE};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | Challenges | Goals")
	TArray<FAccelByteModelsChallengeGoalRequirementPredicate> Predicates{};
};

UENUM(BlueprintType)
enum class EAccelByteModelsChallengeRewardType : uint8
{
	NONE = 0,
	STATISTIC,
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
	EAccelByteModelsChallengeRewardType Type{EAccelByteModelsChallengeRewardType::NONE};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsChallengeSchedule
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | Challenges | Schedule")
	FDateTime EndTime{0};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | Challenges | Schedule")
	int32 Order{0};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | Challenges | Schedule")
	FDateTime StartTime{0};
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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | Challenges | Goals")
	FAccelByteModelsChallengeSchedule Schedule{};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsChallengeGoalMeta
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | Challenges | GoalMeta")
	int32 ActiveGoalsPerRotation{0};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | Challenges | GoalMeta")
	EAccelByteModelsChallengeAssignmentRule AssignmentRule{EAccelByteModelsChallengeAssignmentRule::NONE};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | Challenges | GoalMeta")
	FString Code{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | Challenges | GoalMeta")
	FDateTime CreatedAt{0};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | Challenges | GoalMeta")
	FString Description{};

	// Describe number of period challenge will be retired after.
	// The unit depends on rotation type. If rotation is DAILY the unit is in days, MONTHLY the unit is in months etc.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | Challenges | GoalMeta")
	int32 EndAfter{0}; 

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | Challenges | GoalMeta")
	FDateTime EndDate{0};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | Challenges | GoalMeta")
	EAccelByteModelsChallengeGoalsVisibility GoalsVisibility{EAccelByteModelsChallengeGoalsVisibility::NONE};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | Challenges | GoalMeta")
	FString Name{};	

	// Describe number of period challenge's goals will be repeated after.
	// The unit depends on rotation type. If rotation is DAILY the unit is in days, MONTHLY the unit is in months etc.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | Challenges | GoalMeta")
	int32 RepeatAfter{0};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | Challenges | GoalMeta")
	FAccelByteModelsChallengeResetConfig ResetConfig{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | Challenges | GoalMeta")
	EAccelByteModelsChallengeRotation Rotation{EAccelByteModelsChallengeRotation::NONE}; 

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | Challenges | GoalMeta")
	FDateTime StartDate{0};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | Challenges | GoalMeta")
	EAccelByteModelsChallengeStatus Status{EAccelByteModelsChallengeStatus::NONE};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | Challenges | GoalMeta")
	FDateTime UpdatedAt{0};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsGetScheduledChallengeGoalsResponse
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | Challenges | Goals")
	TArray<FAccelByteModelsChallengeGoal> Data{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | Challenges | Goals")
	FAccelByteModelsChallengeGoalMeta Meta;

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
	FString ChallengeCode{};
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | Challenges | Rewards")
	FString GoalCode{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | Challenges | Rewards")
	EAccelByteModelsChallengeRewardStatus Status{EAccelByteModelsChallengeRewardStatus::NONE};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | Challenges | Rewards")
	FString UserId{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | Challenges | Rewards")
	float Qty{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | Challenges | Rewards")
	FString ItemId{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | Challenges | Rewards")
	FString ItemName{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | Challenges | Rewards")
	EAccelByteModelsChallengeRewardType Type{EAccelByteModelsChallengeRewardType::NONE};

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
struct ACCELBYTEUE4SDK_API FAccelByteModelsChallengeRewardClaimUserRequest
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | Challenges | Rewards")
	TArray<FString> RewardIDs{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | Challenges | Rewards")
	FString UserID{};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsChallengeError
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | Challenges | Error")
	FJsonObjectWrapper Attributes{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | Challenges | Error")
	int32 ErrorCode{0};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | Challenges | Error")
	FString ErrorMessage{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | Challenges | Error")
	FString Message{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | Challenges | Error")
	FString Name{};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsChallengeRewardClaimUserResponse
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | Challenges | Rewards")
	FAccelByteModelsChallengeError ErrorDetail{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | Challenges | Rewards")
	bool IsSuccess{false};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | Challenges | Rewards")
	FAccelByteModelsChallengeReward Rewards{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | Challenges | Rewards")
	FString UserID{};
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

	// EQUAL, LESS_THAN, GREATER_THAN, LESS_THAN_EQUAL, GREATER_THAN_EQUAL
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | Challenges | Progress")
	FString Matcher{};
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | Challenges | Progress")
	FString ParameterName{};
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | Challenges | Progress")
	FString ParameterType{}; 
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | Challenges | Progress")
	FString StatCycleId{};
};

UENUM(BlueprintType)
enum class EAccelByteModelsChallengeGoalProgressStatus : uint8
{
	NONE = 0,
	ACTIVE,
	COMPLETED,
	RETIRED,
	NOT_STARTED,
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsChallengeGoalProgress
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | Challenges | Progress")
	FString ChallengeCode{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | Challenges | Progress")
	FAccelByteModelsChallengeGoal Goal{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | Challenges | Progress")
	FString GoalCode{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | Challenges | Progress")
	FString GoalProgressionId{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | Challenges | Progress")
	TArray<FAccelByteModelsChallengeGoalProgressRequirement> RequirementProgressions{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | Challenges | Progress")
	EAccelByteModelsChallengeGoalProgressStatus Status{EAccelByteModelsChallengeGoalProgressStatus::NONE};
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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | Challenges | Progress")
	FAccelByteModelsPaging Paging{};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsChallengeServerEvaluateProgressRequest
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | Challenges | Server | Progress")
	TArray<FString> UserIds{};
};
