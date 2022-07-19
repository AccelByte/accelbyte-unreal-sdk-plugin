// Copyright (c) 2021 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "CoreMinimal.h"
#include "Core/AccelByteError.h"
#include "Models/AccelByteEcommerceModels.h"
#include "Core/AccelByteMultiRegistry.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "ABEntitlement.generated.h"

using namespace AccelByte;

#pragma region MODEL_AND_DELEGATE_FOR_REQUEST_RESPONSE

USTRUCT(BlueprintType)
struct FArrayModelsDistributionReceiver
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Entitlement | Models | Distribution Receiver")
	TArray<FAccelByteModelsDistributionReceiver> Content;
};

DECLARE_DYNAMIC_DELEGATE_OneParam(FDAccelByteModelsEntitlementPagingSlicedResultResponse, FAccelByteModelsEntitlementPagingSlicedResult, Response);

DECLARE_DYNAMIC_DELEGATE_OneParam(FDAccelByteModelsEntitlementInfoResponse, FAccelByteModelsEntitlementInfo, Response);

DECLARE_DYNAMIC_DELEGATE_OneParam(FDAccelByteModelsEntitlementOwnershipResponse, FAccelByteModelsEntitlementOwnership, Response);

DECLARE_DYNAMIC_DELEGATE_OneParam(FDArrayModelsDistributionReceiverResponse, FArrayModelsDistributionReceiver, Response);

DECLARE_DYNAMIC_DELEGATE_OneParam(FDAccelByteModelsPlatformSyncMobileGoogleResponse, FAccelByteModelsPlatformSyncMobileGoogleResponse, Response);

DECLARE_DYNAMIC_DELEGATE_OneParam(FDAccelByteModelsPlatformValidateUserItemPurchaseResponse, TArray<FAccelByteModelsPlatformValidateUserItemPurchaseResponse>, Response);

#pragma endregion 

UCLASS(Blueprintable, BlueprintType)
class UABEntitlement : public UObject
{
	GENERATED_BODY()
public:
	void SetApiClient(FApiClientPtr const& NewApiClientPtr);

public:
	UFUNCTION(BlueprintCallable, Category = "AccelByte | Entitlement | Api")
	void QueryUserEntitlements(FString const& EntitlementName, TArray<FString> const& ItemIds, int32 const& Offset, int32 const& Limit, FDAccelByteModelsEntitlementPagingSlicedResultResponse OnSuccess, FDErrorHandler const& OnError, EAccelByteEntitlementClass EntitlementClass, EAccelByteAppType AppType);

	UFUNCTION(BlueprintCallable, Category = "AccelByte | Entitlement | Api")
	void GetUserEntitlementById(FString const& Entitlementid, FDAccelByteModelsEntitlementInfoResponse OnSuccess, FDErrorHandler OnError);

	UFUNCTION(BlueprintCallable, Category = "AccelByte | Entitlement | Api")
	void GetUserEntitlementOwnershipByAppId(FString const& AppId, FDAccelByteModelsEntitlementOwnershipResponse OnSuccess, FDErrorHandler OnError);

	UFUNCTION(BlueprintCallable, Category = "AccelByte | Entitlement | Api")
	void GetUserEntitlementOwnershipBySku(FString const& Sku, FDAccelByteModelsEntitlementOwnershipResponse OnSuccess, FDErrorHandler OnError);

	UFUNCTION(BlueprintCallable, Category = "AccelByte | Entitlement | Api")
	void GetUserEntitlementOwnershipByItemId(FString const& ItemId, FDAccelByteModelsEntitlementOwnershipResponse OnSuccess, FDErrorHandler OnError);

	UFUNCTION(BlueprintCallable, Category = "AccelByte | Entitlement | Api")
	void GetUserEntitlementOwnershipAny(TArray<FString> const& ItemIds, TArray<FString> const& AppIds, TArray<FString> const& Skus, FDAccelByteModelsEntitlementOwnershipResponse OnSuccess, FDErrorHandler OnError);
	
	UFUNCTION(BlueprintCallable, Category = "AccelByte | Entitlement | Api")
	void ConsumeUserEntitlement(FString const& EntitlementId, int32 const& UseCount, FDAccelByteModelsEntitlementInfoResponse OnSuccess, FDErrorHandler OnError);
	
	UFUNCTION(BlueprintCallable, Category = "AccelByte | Entitlement | Api")
	void CreateDistributionReceiver(FString const& ExtUserId, FAccelByteModelsAttributes const& Attributes, FDHandler OnSuccess, FDErrorHandler OnError);
	
	UFUNCTION(BlueprintCallable, Category = "AccelByte | Entitlement | Api")
	void DeleteDistributionReceiver(FString const& ExtUserId, FString const& UserId, FDHandler OnSuccess, FDErrorHandler OnError);

	UFUNCTION(BlueprintCallable, Category = "AccelByte | Entitlement | Api")
	void GetDistributionReceiver(FString const& PublisherNamespace, FString const& PublisherUserId, FDArrayModelsDistributionReceiverResponse OnSuccess, FDErrorHandler OnError);

	UFUNCTION(BlueprintCallable, Category = "AccelByte | Entitlement | Api")
	void UpdateDistributionReceiver(FString const& ExtUserId, FAccelByteModelsAttributes const& Attributes, FDHandler OnSuccess, FDErrorHandler OnError);

	UFUNCTION(BlueprintCallable, Category = "AccelByte | Entitlement | Api")
	void SyncPlatformPurchase(EAccelBytePlatformSync PlatformType, FDHandler OnSuccess, FDErrorHandler OnError);

	UFUNCTION(BlueprintCallable, Category = "AccelByte | Entitlement | Api")
	void SyncMobilePlatformPurchaseGoogle(const FAccelByteModelsPlatformSyncMobileGoogle& SyncRequest, FDHandler OnSuccess, FDErrorHandler OnError);

	UFUNCTION(BlueprintCallable, Category = "AccelByte | Entitlement | Api")
	void SyncMobilePlatformPurchaseGooglePlay(const FAccelByteModelsPlatformSyncMobileGoogle& SyncRequest, FDAccelByteModelsPlatformSyncMobileGoogleResponse OnSuccess, FDErrorHandler OnError);
	
	UFUNCTION(BlueprintCallable, Category = "AccelByte | Entitlement | Api")
	void SyncMobilePlatformPurchaseApple(FAccelByteModelsPlatformSyncMobileApple const& SyncRequest, FDHandler OnSuccess, FDErrorHandler OnError);
 
	UFUNCTION(BlueprintCallable, Category = "AccelByte | Entitlement | Api")
	void SyncTwitchDropEntitlement(FAccelByteModelsTwitchDropEntitlement const& TwitchDropModel, FDHandler OnSuccess, FDErrorHandler OnError);

	UFUNCTION(BlueprintCallable, Category = "AccelByte | Entitlement")
	void ValidateUserItemPurchaseCondition(TArray<FString> const& Items, FDAccelByteModelsPlatformValidateUserItemPurchaseResponse OnSuccess, FDErrorHandler OnError);
 
private:
	FApiClientPtr ApiClientPtr;
};
