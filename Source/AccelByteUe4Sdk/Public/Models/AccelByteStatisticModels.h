// Copyright (c) 2019 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "CoreMinimal.h"
#include "JsonUtilities.h"
#include "Models/AccelByteGeneralModels.h"
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

