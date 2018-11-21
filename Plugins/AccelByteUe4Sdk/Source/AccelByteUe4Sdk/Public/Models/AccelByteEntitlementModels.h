// Copyright (c) 2018 AccelByte Inc. All Rights Reserved.
// This is licensed software from Accelbyte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "CoreMinimal.h"
#include "AccelByteItemModels.h"
#include "AccelByteEntitlementModels.generated.h"

#pragma region EntitlementEnumField

UENUM(BlueprintType)
enum class EAccelByteEntitlementClass : uint8
{
	NONE = 0 ,
	APP,
	ENTITLEMENT,
	DISTRIBUTION
};

UENUM(BlueprintType)
enum class EAccelByteEntitlementType : uint8
{
	NONE = 0,
	DURABLE,
	CONSUMABLE
};

UENUM(BlueprintType)
enum class EAccelByteEntitlementStatus : uint8
{
	NONE = 0,
	ACTIVE,
	INACTIVE,
	DISTRIBUTED,
	REVOKED,
	DELETED
};

UENUM(BlueprintType)
enum class EAccelByteAppType : uint8
{
	NONE = 0,
	GAME,
	SOFTWARE,
	DLC,
	DEMO
};

#pragma endregion EntitlementEnumField

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsEntitlementItemSnapshot
{
	GENERATED_BODY()
		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Entitlement | Models | EntitlementPagingSlicedResult | EntitlementInfo | ItemSnapshot ")
		FString ItemId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Entitlement | Models | EntitlementPagingSlicedResult | EntitlementInfo | ItemSnapshot ")
		FString AppId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Entitlement | Models | EntitlementPagingSlicedResult | EntitlementInfo | ItemSnapshot ")
		EAccelByteAppType AppType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Entitlement | Models | EntitlementPagingSlicedResult | EntitlementInfo | ItemSnapshot ")
		FString Sku;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Entitlement | Models | EntitlementPagingSlicedResult | EntitlementInfo | ItemSnapshot ")
		FString Namespace;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Entitlement | Models | EntitlementPagingSlicedResult | EntitlementInfo | ItemSnapshot ")
		FString Name;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Entitlement | Models | EntitlementPagingSlicedResult | EntitlementInfo | ItemSnapshot ")
		int32 UseCount;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Entitlement | Models | EntitlementPagingSlicedResult | EntitlementInfo | ItemSnapshot ")
		EAccelByteItemType ItemType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Entitlement | Models | EntitlementPagingSlicedResult | EntitlementInfo | ItemSnapshot ")
		FString TargetCurrencyCode;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Entitlement | Models | EntitlementPagingSlicedResult | EntitlementInfo | ItemSnapshot ")
		FString TargetNamespace;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Entitlement | Models | EntitlementPagingSlicedResult | EntitlementInfo | ItemSnapshot ")
		FString Title;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Entitlement | Models | EntitlementPagingSlicedResult | EntitlementInfo | ItemSnapshot ")
		FAccelByteModelsItemInfoImage ThumbnailImage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Entitlement | Models | EntitlementPagingSlicedResult | EntitlementInfo | ItemSnapshot ")
		FAccelByteModelsItemInfoRegionData RegionDataItem;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Entitlement | Models | EntitlementPagingSlicedResult | EntitlementInfo | ItemSnapshot ")
		TArray<FString> ItemIds;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Entitlement | Models | EntitlementPagingSlicedResult | EntitlementInfo | ItemSnapshot ")
		int32 MaxCountPerUser;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Entitlement | Models | EntitlementPagingSlicedResult | EntitlementInfo | ItemSnapshot ")
		FString Region;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Entitlement | Models | EntitlementPagingSlicedResult | EntitlementInfo | ItemSnapshot ")
		FString Language;
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsEntitlementInfo
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Entitlement | Models | EntitlementPagingSlicedResult | EntitlementInfo ")
		FString Id;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Entitlement | Models | EntitlementPagingSlicedResult | EntitlementInfo ")
		FString Namespace;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Entitlement | Models | EntitlementPagingSlicedResult | EntitlementInfo ")
		EAccelByteEntitlementClass Clazz;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Entitlement | Models | EntitlementPagingSlicedResult | EntitlementInfo ")
		EAccelByteEntitlementType Type;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Entitlement | Models | EntitlementPagingSlicedResult | EntitlementInfo ")
		EAccelByteEntitlementStatus Status;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Entitlement | Models | EntitlementPagingSlicedResult | EntitlementInfo ")
		FString AppId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Entitlement | Models | EntitlementPagingSlicedResult | EntitlementInfo ")
		EAccelByteAppType AppType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Entitlement | Models | EntitlementPagingSlicedResult | EntitlementInfo ")
		FString Sku;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Entitlement | Models | EntitlementPagingSlicedResult | EntitlementInfo ")
		FString UserId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Entitlement | Models | EntitlementPagingSlicedResult | EntitlementInfo ")
		FString ItemId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Entitlement | Models | EntitlementPagingSlicedResult | EntitlementInfo ")
		FString BundleItemId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Entitlement | Models | EntitlementPagingSlicedResult | EntitlementInfo ")
		FString ItemNamespace;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Entitlement | Models | EntitlementPagingSlicedResult | EntitlementInfo ")
		FString Name;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Entitlement | Models | EntitlementPagingSlicedResult | EntitlementInfo ")
		int32 UseCount;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Entitlement | Models | EntitlementPagingSlicedResult | EntitlementInfo ")
		int32 Quantity;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Entitlement | Models | EntitlementPagingSlicedResult | EntitlementInfo ")
		int32 DistributedQuantity;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Entitlement | Models | EntitlementPagingSlicedResult | EntitlementInfo ")
		FString TargetNamespace;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Entitlement | Models | EntitlementPagingSlicedResult | EntitlementInfo ")
		FAccelByteModelsEntitlementItemSnapshot ItemSnapshot;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Entitlement | Models | EntitlementPagingSlicedResult | EntitlementInfo ")
		FString StartDate;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Entitlement | Models | EntitlementPagingSlicedResult | EntitlementInfo ")
		FString EndDate;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Entitlement | Models | EntitlementPagingSlicedResult | EntitlementInfo ")
		FString GrantedAt;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Entitlement | Models | EntitlementPagingSlicedResult | EntitlementInfo ")
		FString CreatedAt;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Entitlement | Models | EntitlementPagingSlicedResult | EntitlementInfo ")
		FString UpdatedAt;
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsEntitlementPaging
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Entitlement | Models | EntitlementPagingSlicedResult | Paging")
		FString Previous;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Entitlement | Models | EntitlementPagingSlicedResult | Paging")
		FString Next;
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsEntitlementPagingSlicedResult
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Entitlement | Models | EntitlementPagingSlicedResult ")
		TArray<FAccelByteModelsEntitlementInfo> Data;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Entitlement | Models | EntitlementPagingSlicedResult ")
		FAccelByteModelsEntitlementPaging Paging;
};
