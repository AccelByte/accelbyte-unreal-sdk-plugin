// Copyright (c) 2019 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "CoreMinimal.h"
#include "Models/AccelByteGeneralModels.h"
#include "Models/AccelByteEcommerceModels.h"
#include "AccelByteStatisticModels.generated.h"

UENUM(BlueprintType)
enum class EAccelByteStatisticSetBy : uint8
{
	CLIENT = 0,
	SERVER
};

UENUM(BlueprintType)
enum class EAccelByteStatisticStatus : uint8
{
	INIT = 0,
	TIED
};

UENUM(BlueprintType)
enum class EAccelByteStatisticUpdateStrategy : uint8
{
	OVERRIDE = 0,
	INCREMENT,
	MAX,
	MIN
};

UENUM(BlueprintType)
enum class EAccelByteStatisticSortBy : uint8
{
	NONE = 0,
	STAT_CODE,
	STAT_CODE_ASC,
	STAT_CODE_DESC,
	CREATED_AT,
	CREATED_AT_ASC,
	CREATED_AT_DESC,
	UPDATED_AT,
	UPDATED_AT_ASC,
	UPDATED_AT_DESC
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsStatInfo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Statistic | Models | StatInfo")
	FString CreatedAt{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Statistic | Models | StatInfo")
	float DefaultValue{};
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Statistic | Models | StatInfo")
	FString Description{};
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Statistic | Models | StatInfo")
	bool IncrementOnly{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Statistic | Models | StatInfo")
	float Maximum{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Statistic | Models | StatInfo")
	float Minimum{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Statistic | Models | StatInfo")
	FString Name{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Statistic | Models | StatInfo")
	FString Namespace{}; // lower-case namespace keyword is reserved 

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Statistic | Models | StatInfo")
	bool SetAsGlobal{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Statistic | Models | StatInfo")
	EAccelByteStatisticSetBy SetBy{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Statistic | Models | StatInfo")
	FString StatCode{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Statistic | Models | StatInfo")
	EAccelByteStatisticStatus Status{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Statistic | Models | StatInfo")
	FString UpdatedAt{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Statistic | Models | StatInfo")
	TArray<FString> CycleIds{};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsUserStatItemInfo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Statistic | Models | UserStatItemInfo")
	FString CreatedAt{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Statistic | Models | UserStatItemInfo")
	FString Namespace{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Statistic | Models | UserStatItemInfo")
	FString userId{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Statistic | Models | UserStatItemInfo")
	FString StatCode{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Statistic | Models | UserStatItemInfo")
	FString StatName{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Statistic | Models | UserStatItemInfo")
	TArray<FString> Tags{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Statistic | Models | UserStatItemInfo")
	FString UpdatedAt{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Statistic | Models | UserStatItemInfo")
	float Value{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Statistic | Models | UserStatItemInfo")
	FJsonObjectWrapper AdditionalData{};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsStatItemIncResult
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Statistic | Models | StatItemIncResult")
	float currentValue{};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsBulkStatItemOperationResult
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Statistic | Models | BulkStatItemIncResult")
	TMap<FString, FString> Details{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Statistic | Models | BulkStatItemIncResult")
	FString StatCode{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Statistic | Models | BulkStatItemIncResult")
	bool Success{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Statistic | Models | BulkStatItemIncResult")
	FString UserId{};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsUserStatItemPagingSlicedResult
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Statistic | Models | StatItemPagingSlicedResult")
	TArray<FAccelByteModelsUserStatItemInfo> Data{};
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Statistic | Models | StatItemPagingSlicedResult")
	FAccelByteModelsPaging Paging{};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsBulkUserStatItemInc
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Statistic | Models | BulkUserStatItemInc")
	float inc{};
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Statistic | Models | BulkUserStatItemInc")
	FString userId{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Statistic | Models | BulkUserStatItemInc")
	FString statCode{};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsBulkStatItemInc
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Statistic | Models | BulkStatItemInc")
	float inc{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Statistic | Models | BulkStatItemInc")
	FString statCode{};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsBulkStatItemCreate
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Statistic | Models | BulkStatItemCreate")
	FString StatCode{};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsFetchUser
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Statistic | Models | BulkFetchUser")
	FString ProfileId{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Statistic | Models | BulkFetchUser")
	FString StatCode{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Statistic | Models | BulkFetchUser")
	float Value{};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsUpdateUserStatItem
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Statistic | Models | BulkUpdateMultipleUser")
	FString UserId{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Statistic | Models | BulkUpdateMultipleUser")
	FString AdditionalKey{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Statistic | Models | BulkUpdateMultipleUser")
	FString StatCode{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Statistic | Models | BulkUpdateMultipleUser")
	EAccelByteStatisticUpdateStrategy UpdateStrategy{EAccelByteStatisticUpdateStrategy::OVERRIDE};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Statistic | Models | BulkUpdateMultipleUser")
	float Value{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Statistic | Models | BulkUpdateMultipleUser")
	FJsonObjectWrapper AdditionalData{};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsUpdateUserStatItemsResponse
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Statistic | Models | BulkUpdateMultipleUserResponse")
	bool Success{}; 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Statistic | Models | BulkUpdateMultipleUserResponse")
	FString StatCode{}; 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Statistic | Models | BulkUpdateMultipleUserResponse")
	FJsonObjectWrapper Details{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Statistic | Models | BulkUpdateMultipleUserResponse")
	FString UserId{}; 

};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsUserStatItem
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Statistic | Models | BulkUserStatItem")
	FString StatCode{}; 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Statistic | Models | BulkUserStatItem")
	FJsonObjectWrapper AdditionalData{};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsUpdateUserStatItemWithStatCode
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Statistic | Models | UpdateUserStatItemWithStatCode")
	FString StatCode{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Statistic | Models | UpdateUserStatItemWithStatCode")
	EAccelByteStatisticUpdateStrategy UpdateStrategy{EAccelByteStatisticUpdateStrategy::OVERRIDE};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Statistic | Models | UpdateUserStatItemWithStatCode")
	float Value{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Statistic | Models | UpdateUserStatItemWithStatCode")
	FJsonObjectWrapper AdditionalData{};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsPublicUpdateUserStatItem
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Statistic | Models | BulkUpdateUserStatItem")
	EAccelByteStatisticUpdateStrategy UpdateStrategy { EAccelByteStatisticUpdateStrategy::OVERRIDE };
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Statistic | Models | BulkUpdateUserStatItem")
	float Value{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Statistic | Models | BulkUpdateUserStatItem")
	FJsonObjectWrapper AdditionalData {};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsUpdateUserStatItemValueResponse
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Statistic | Models | UpdateUserStatItemValueResponse")
	float CurrentValue{};  
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsGlobalStatItemValueResponse
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Statistic | Models | StatItemValueResponse")
	FString StatCode {};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Statistic | Models | StatItemValueResponse")
	FString StatName {};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Statistic | Models | StatItemValueResponse")
	FString Namespace {};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Statistic | Models | StatItemValueResponse")
	int32 Value {};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Statistic | Models | StatItemValueResponse")
	TArray<FString> Tags{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Statistic | Models | StatItemValueResponse")
	FDateTime CreatedAt {0};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Statistic | Models | StatItemValueResponse")
	FDateTime UpdatedAt {0};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsStatItemValueResponse
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Statistic | Models | StatItemValueResponse")
	FString StatCode{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Statistic | Models | StatItemValueResponse")
	FString statName{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Statistic | Models | StatItemValueResponse")
	FString Namespace{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Statistic | Models | StatItemValueResponse")
	int32 Value{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Statistic | Models | StatItemValueResponse")
	TArray<FString> Tags{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Statistic | Models | StatItemValueResponse")
	FDateTime CreatedAt {0};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Statistic | Models | StatItemValueResponse")
	FDateTime UpdatedAt {0};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Statistic | Models | StatItemValueResponse")
	FString UserId{};	 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Statistic | Models | StatItemValueResponse")
	FJsonObjectWrapper AdditionalData{};	 
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsResetUserStatItemValue
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Statistic | Models | ResetUserStatItemValue")
	FString UserId{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Statistic | Models | ResetUserStatItemValue")
	FString StatCode{};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsUserStatCycleItemInfo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Statistic | Models | StatCycleItemValueResponse")
	FString StatCode{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Statistic | Models | StatCycleItemValueResponse")
	FString StatName{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Statistic | Models | StatCycleItemValueResponse")
	FString CycleId{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Statistic | Models | StatCycleItemValueResponse")
	FString CycleName{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Statistic | Models | StatCycleItemValueResponse")
	FString Namespace{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Statistic | Models | StatCycleItemValueResponse")
	int32 Value{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Statistic | Models | StatCycleItemValueResponse")
	TArray<FString> Tags{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Statistic | Models | StatCycleItemValueResponse")
	FDateTime CreatedAt {0};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Statistic | Models | StatCycleItemValueResponse")
	FDateTime UpdatedAt {0};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Statistic | Models | StatCycleItemValueResponse")
	FString UserId{};	
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsUserStatCycleItemPagingSlicedResult
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Statistic | Models | StatItemPagingSlicedResult")
	TArray<FAccelByteModelsUserStatCycleItemInfo> Data{};
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Statistic | Models | StatItemPagingSlicedResult")
	FAccelByteModelsPaging Paging{};
};

USTRUCT(BlueprintType)
struct FAccelByteModelsStatCycleConfig
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Statistic | Models | FAccelByteModelsStatCycleConfig")
	FString Id{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Statistic | Models | FAccelByteModelsStatCycleConfig")
	FString Namespace{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Statistic | Models | FAccelByteModelsStatCycleConfig")
	EAccelByteCycle CycleType{EAccelByteCycle::DAILY};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Statistic | Models | FAccelByteModelsStatCycleConfig")
	FString Name{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Statistic | Models | FAccelByteModelsStatCycleConfig")
	FString Description{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Statistic | Models | FAccelByteModelsStatCycleConfig")
	FString ResetTime{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Statistic | Models | FAccelByteModelsStatCycleConfig")
	int32 ResetDay{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Statistic | Models | FAccelByteModelsStatCycleConfig")
	int32 ResetDate{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Statistic | Models | FAccelByteModelsStatCycleConfig")
	int32 ResetMonth{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Statistic | Models | FAccelByteModelsStatCycleConfig")
	int32 SeasonPeriod{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Statistic | Models | FAccelByteModelsStatCycleConfig")
	int32 CurrentVersion{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Statistic | Models | FAccelByteModelsStatCycleConfig")
	FString Status{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Statistic | Models | FAccelByteModelsStatCycleConfig")
	FDateTime NextReset{0};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Statistic | Models | FAccelByteModelsStatCycleConfig")
	FDateTime Start{0};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Statistic | Models | FAccelByteModelsStatCycleConfig")
	FDateTime End{0};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Statistic | Models | FAccelByteModelsStatCycleConfig")
	FDateTime CreatedAt{0};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Statistic | Models | FAccelByteModelsStatCycleConfig")
	FDateTime UpdatedAt{0};
};

USTRUCT(BlueprintType)
struct FAccelByteModelsStatCycleConfigPagingResult
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Statistic | Models | FAccelByteModelsStatCycleConfigPaging")
	TArray<FAccelByteModelsStatCycleConfig> Data{};
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Statistic | Models | FAccelByteModelsStatCycleConfigPaging")
	FAccelByteModelsPaging Paging{};
};


USTRUCT(BlueprintType)
struct FAccelByteModelsMultipleStatSortBy
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Statistic | Models | StatSortBy")
	TArray<EAccelByteStatisticSortBy> SortBy { EAccelByteStatisticSortBy::UPDATED_AT_ASC };
};
