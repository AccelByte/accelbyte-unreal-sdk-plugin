// Copyright (c) 2021 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "CoreMinimal.h"
#include "JsonObjectWrapper.h"
#include "Models/AccelByteEcommerceModels.h"

#include "AccelByteSeasonPassModels.generated.h"

UENUM(BlueprintType)
enum class EAccelByteSeasonPassStrategyMethod : uint8
{
	NONE = 0,
	CURRENCY
};

UENUM(BlueprintType)
enum class EAccelByteSeasonPassStatus : uint8
{
	DRAFT = 0,
	PUBLISHED,
	RETIRED
};

UENUM(BlueprintType)
enum class EAccelByteSeasonPassRewardType : uint8
{
	ITEM // currently only support this type
};

UENUM(BlueprintType)
enum class EAccelByteSeasonPassSource : uint8
{
	SWEAT = 0,
	PAID_FOR
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteSeasonPassExcessStrategy
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | SeasonPass | Models | ExcessStrategy")
	EAccelByteSeasonPassStrategyMethod Method{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | SeasonPass | Models | ExcessStrategy")
	FString Currency{};
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | SeasonPass | Models | ExcessStrategy")
	int32 PercentPerExp{};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsSeasonPass
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | SeasonPass | Models | Passes")
	FString Title{};
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | SeasonPass | Models | Passes")
	FString Description{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | SeasonPass | Models | Passes")
	FString SeasonId{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | SeasonPass | Models | Passes")
	FString Code{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | SeasonPass | Models | Passes")
	FString Namespace{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | SeasonPass | Models | Passes")
	FString DisplayOrder{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | SeasonPass | Models | Passes")
	bool AutoEnroll{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | SeasonPass | Models | Passes")
	FString PassItemId{};
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | SeasonPass | Models | Passes")
	TArray<FAccelByteModelsItemImage> Images{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | SeasonPass | Models | Passes")
	FString Language{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | SeasonPass | Models | Passes")
	FDateTime CreatedAt{0};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | SeasonPass | Models | Passes")
	FDateTime UpdatedAt{0};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsSeasonPassTierJsonObject
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | SeasonPass | Models | Tier")
	FString Id{};
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | SeasonPass | Models | Tier")
	int32 RequiredExp{};

	/**
	 * @brief PassCode as key, RewardCode as Values. This should be TMap<FString, TArray<FString>>, but UHT not supported it.
	 *	See AccelByteSeasonPassApi::GetCurrentSeason for more details how to process it
	 **/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | SeasonPass | Models | Tier")
	FJsonObjectWrapper Rewards{};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsSeasonPassTier
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | SeasonPass | Models | Tier")
	FString Id{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | SeasonPass | Models | Tier")
	int32 RequiredExp{};

	/** @brief PassCode as key, RewardCode as Values. **/
	TMap<FString, TArray<FString>> Rewards{};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsSeasonPassRewardCurrency
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | SeasonPass | Models | RewardCurrency")
	FString Namespace{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | SeasonPass | Models | RewardCurrency")
	FString CurrencyCode{};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsSeasonPassReward
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | SeasonPass | Models | Reward")
	FString Namespace{};
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | SeasonPass | Models | Reward")
	FString SeasonId{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | SeasonPass | Models | Reward")
	FString Code{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | SeasonPass | Models | Reward")
	EAccelByteSeasonPassRewardType Type{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | SeasonPass | Models | Reward")
	FString ItemId{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | SeasonPass | Models | Reward")
	FString ItemName{};
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | SeasonPass | Models | Reward")
	FString ItemSku{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | SeasonPass | Models | Reward")
	TArray<FAccelByteModelsSeasonPassRewardCurrency> Currency{};
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | SeasonPass | Models | Reward")
	int32 Quantity{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | SeasonPass | Models | Reward")
	FAccelByteModelsItemImage Image{};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsSeasonTierRewardAsJsonObject
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | SeasonPass | Models | Season")
	FString Title{};
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | SeasonPass | Models | Season")
	FString Description{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | SeasonPass | Models | Season")
	FString Id{};
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | SeasonPass | Models | Season")
	FString Namespace{};
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | SeasonPass | Models | Season")
	FString Name{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | SeasonPass | Models | Season")
	FDateTime Start{0};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | SeasonPass | Models | Season")
	FDateTime End{0};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | SeasonPass | Models | Season")
	FString TierItemId{};
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | SeasonPass | Models | Season")
	bool AutoClaim{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | SeasonPass | Models | Season")
	TArray<FAccelByteModelsItemImage> Images{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | SeasonPass | Models | Season")
	TArray<FString> PassCodes{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | SeasonPass | Models | Season")
	EAccelByteSeasonPassStatus Status{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | SeasonPass | Models | Season")
	FDateTime PublishedAt{0};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | SeasonPass | Models | Season")
	FString Language{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | SeasonPass | Models | Season")
	FDateTime CreatedAt{0};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | SeasonPass | Models | Season")
	FDateTime UpdatedAt{0};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | SeasonPass | Models | Season")
	TArray<FAccelByteModelsSeasonPass> Passes{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | SeasonPass | Models | Season")
	TMap<FString, FAccelByteModelsSeasonPassReward> Rewards{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | SeasonPass | Models | Season")
	TArray<FAccelByteModelsSeasonPassTierJsonObject> Tiers{};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsSeason
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | SeasonPass | Models | Season")
	FString Id{};
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | SeasonPass | Models | Season")
	FString Namespace{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | SeasonPass | Models | Season")
	FString Name{};
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | SeasonPass | Models | Season")
	FDateTime Start{0};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | SeasonPass | Models | Season")
	FDateTime End{0};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | SeasonPass | Models | Season")
	EAccelByteSeasonPassStatus Status{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | SeasonPass | Models | Season")
	FDateTime PublishedAt{0};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsSeasonInfo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | SeasonPass | Models | Season")
	FString Title{};
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | SeasonPass | Models | Season")
	FString Description{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | SeasonPass | Models | Season")
	FString Id{};
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | SeasonPass | Models | Season")
	FString Namespace{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | SeasonPass | Models | Season")
	FString Name{};
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | SeasonPass | Models | Season")
	FDateTime Start{0};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | SeasonPass | Models | Season")
	FDateTime End{0};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | SeasonPass | Models | Season")
	FString TierItemId{};
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | SeasonPass | Models | Season")
	bool AutoClaim{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | SeasonPass | Models | Season")
	TArray<FAccelByteModelsItemImage> Images{};
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | SeasonPass | Models | Season")
	TArray<FString> PassCodes{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | SeasonPass | Models | Season")
	EAccelByteSeasonPassStatus Status{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | SeasonPass | Models | Season")
	FDateTime PublishedAt{0};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | SeasonPass | Models | Season")
	FString Language{};
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | SeasonPass | Models | Season")
	FDateTime CreatedAt{0};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | SeasonPass | Models | Season")
	FDateTime UpdatedAt{0};
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | SeasonPass | Models | Season")
	TArray<FAccelByteModelsSeasonPass> Passes{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | SeasonPass | Models | Season")
	TMap<FString, FAccelByteModelsSeasonPassReward> Rewards{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | SeasonPass | Models | Season")
	TArray<FAccelByteModelsSeasonPassTier> Tiers{};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsUserSeasonInfo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | SeasonPass | Models | Season")
	FString Id{};
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | SeasonPass | Models | Season")
	FString Namespace{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | SeasonPass | Models | Season")
	FString UserId{};
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | SeasonPass | Models | Season")
	FString SeasonId{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | SeasonPass | Models | Season")
	FDateTime EnrolledAt{0};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | SeasonPass | Models | Season")
	TArray<FString> EnrolledPasses{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | SeasonPass | Models | Season")
	int32 CurrentTierIndex{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | SeasonPass | Models | Season")
	int32 LastTierIndex{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | SeasonPass | Models | Season")
	int32 RequiredExp{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | SeasonPass | Models | Season")
	int32 CurrentExp{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | SeasonPass | Models | Season")
	bool Cleared{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | SeasonPass | Models | Season")
	FAccelByteModelsSeason Season{};

	/**
	 * @brief Tier Index as key, Passes (include rewards) as Values.
	 * Values is also a TMap, PassCode as key, RewardCodes as values
	 ***/
	TMap<int32, TMap<FString, TArray<FString>>> ToClaimRewards{};

	/**
	 * @brief Tier Index as key, Passes (include rewards) as Values.
	 * Values is also a TMap, PassCode as key, RewardCodes as values
	 **/
	TMap<int32, TMap<FString, TArray<FString>>> ClaimingRewards{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | SeasonPass | Models | Season")
	FDateTime CreatedAt{0};
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | SeasonPass | Models | Season")
	FDateTime UpdatedAt{0};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | SeasonPass | Models | Season")
	int32 TotalPaidForExp{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | SeasonPass | Models | Season")
	int32 TotalSweatExp{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | SeasonPass | Models | Season")
	int32 TotalExp{};
};


USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsUserSeasonInfoWithoutReward
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | SeasonPass | Models | Season")
	FString Id{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | SeasonPass | Models | Season")
	FString Namespace{};
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | SeasonPass | Models | Season")
	FString UserId{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | SeasonPass | Models | Season")
	FString SeasonId{};
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | SeasonPass | Models | Season")
	FDateTime EnrolledAt{0};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | SeasonPass | Models | Season")
	TArray<FString> EnrolledPasses{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | SeasonPass | Models | Season")
	int32 CurrentTierIndex{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | SeasonPass | Models | Season")
	int32 LastTierIndex{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | SeasonPass | Models | Season")
	int32 RequiredExp{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | SeasonPass | Models | Season")
	int32 CurrentExp{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | SeasonPass | Models | Season")
	bool Cleared{};
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | SeasonPass | Models | Season")
	FAccelByteModelsSeason Season{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | SeasonPass | Models | Season")
	FDateTime CreatedAt{0};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | SeasonPass | Models | Season")
	FDateTime UpdatedAt{0};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | SeasonPass | Models | Season")
	int32 TotalPaidForExp{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | SeasonPass | Models | Season")
	int32 TotalSweatExp{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | SeasonPass | Models | Season")
	int32 TotalExp{};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsUserSeasonInfoClaimRewardAsJsonObject
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | SeasonPass | Models | Season")
	FString Id{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | SeasonPass | Models | Season")
	FString Namespace{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | SeasonPass | Models | Season")
	FString UserId{};
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | SeasonPass | Models | Season")
	FString SeasonId{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | SeasonPass | Models | Season")
	FDateTime EnrolledAt{0};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | SeasonPass | Models | Season")
	TArray<FString> EnrolledPasses{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | SeasonPass | Models | Season")
	int32 CurrentTierIndex{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | SeasonPass | Models | Season")
	int32 LastTierIndex{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | SeasonPass | Models | Season")
	int32 RequiredExp{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | SeasonPass | Models | Season")
	int32 CurrentExp{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | SeasonPass | Models | Season")
	bool Cleared{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | SeasonPass | Models | Season")
	FAccelByteModelsSeason Season{};

	/**
	 * @brief Tier Index as key, Passes (include rewards) as Values. This should be TMap<int32, TMap<FString, TArray<FString>>>
	 * Values is also a TMap, PassCode as key, RewardCodes as values
	 **/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | SeasonPass | Models | Season")
	TMap<int32, FJsonObjectWrapper> ToClaimRewards{};

	/**
	 * @brief Tier Index as key, Passes (include rewards) as Values. This should be TMap<int32, TMap<FString, TArray<FString>>>
	 * Values is also a TMap, PassCode as key, RewardCodes as values
	 **/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | SeasonPass | Models | Season")
	TMap<int32, FJsonObjectWrapper> ClaimingRewards{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | SeasonPass | Models | Season")
	FDateTime CreatedAt{0};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | SeasonPass | Models | Season")
	FDateTime UpdatedAt{0};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsSeasonClaimRewardRequest
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | SeasonPass | Models | Season | Reward")
	FString PassCode{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | SeasonPass | Models | Season | Reward")
	int32 TierIndex{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | SeasonPass | Models | Season | Reward")
	FString RewardCode{};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsSeasonClaimRewardResponseJsonWrapper
{
	GENERATED_BODY()

	/**
	 * @brief Tier Index as key, Passes (include rewards) as Values. This should be TMap<int32, TMap<TArray<FString>>>, but UHT not supported it.
	 * Values is also a TMap, PassCode as key, RewardCodes as values
	 **/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | SeasonPass | Models | Season | Reward")
	TMap<int32, FJsonObjectWrapper> ToClaimRewards{};

	/**
	 * @brief Tier Index as key, Passes (include rewards) as Values. This should be TMap<int32, TMap<FString, TArray<FString>>>
	 * Values is also a TMap, PassCode as key, RewardCodes as values
	 **/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | SeasonPass | Models | Season | Reward")
	TMap<int32, FJsonObjectWrapper> ClaimingRewards{};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsSeasonClaimRewardResponse
{
	GENERATED_BODY()

	/**
	 * @brief Tier Index as key, Passes (include rewards) as Values.
	 * Values is also a TMap, PassCode as key, RewardCodes as values
	 **/
	TMap<int32, TMap<FString, TArray<FString>>> ToClaimRewards{};

	/**
	 * @brief Tier Index as key, Passes (include rewards) as Values.
	 * Values is also a TMap, PassCode as key, RewardCodes as values
	 **/
	TMap<int32, TMap<FString, TArray<FString>>> ClaimingRewards{};
};


USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsUserSeasonData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | SeasonPass | Models | Data")
	FString Id{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | SeasonPass | Models | Data")
	FString Namespace{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | SeasonPass | Models | Data")
	FString SeasonId{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | SeasonPass | Models | Data")
	FString UserId{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | SeasonPass | Models | Data")
	int32 GrantExp{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | SeasonPass | Models | Data")
	TArray<FString> Tags{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | SeasonPass | Models | Data")
	FDateTime CreatedAt{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | SeasonPass | Models | Data")
	EAccelByteSeasonPassSource Source{};

};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsUserSeasonPaging
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | SeasonPass | Models | Paging")
	FString Previous{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | SeasonPass | Models | Paging")
	FString Next{}; 
};


USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsUserSeasonExpHistory
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | SeasonPass | Models | History")
	TArray<FAccelByteModelsUserSeasonData> Data{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | SeasonPass | Models | History")
	FAccelByteModelsUserSeasonPaging Paging{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | SeasonPass | Models | History")
	int32 Total{};
 
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsQueryUserSeasonExp
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | SeasonPass | Models | History")
	TArray<FString> Tags{};
};