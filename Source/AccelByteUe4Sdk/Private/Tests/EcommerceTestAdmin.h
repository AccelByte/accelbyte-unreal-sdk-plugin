// Copyright (c) 2018 - 2021 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "TestUtilities.h"

struct EcommerceExpectedVariable
{
	FString ExpectedRootCategoryPath;
	FString ExpectedChildCategoryPath;
	FString ExpectedGrandChildCategoryPath;
	FCurrencyCreateRequest ExpectedCurrency;
	FString campaignCode;
	FString campaignName;
	FString expiredCampaignName;
	FString notStartedCampaignName;
	FString redeemableItemTitle;
	FString ExpectedRootItemTitle;
	FString ExpectedChildItemTitle;
	FStoreCreateRequest ExpectedStoreArchive;
	FStoreCreateRequest ExpectedStoreTemporary;
	int32 LootCoinQuantity;
	FItemFullInfo redeemableItem;
	FItemFullInfo LootItem;
	FItemFullInfo LootCoin;
	FItemFullInfo LootResult;
	FItemFullInfo BundleResult;
	FCampaignInfo campaignResult;
	FCampaignInfo expiredCampaignResult;
	FCampaignInfo notStartedCampaignResult;
	FCodeInfo codeInfo;
	FCodeInfo expiredCodeInfo;
	FCodeInfo notStartedCodeInfo;
};

void SetupEcommerce(EcommerceExpectedVariable& Variables, const FSimpleDelegate& OnSuccess, const FErrorHandler& OnError);
void TearDownEcommerce(EcommerceExpectedVariable& Variables, const FSimpleDelegate& OnSuccess, const FErrorHandler& OnError);

void Ecommerce_Currency_Create(FCurrencyCreateRequest Currency, const FSimpleDelegate& OnSuccess, const FErrorHandler& OnError);
void Ecommerce_Currency_Get(FString CurrencyCode, const FSimpleDelegate& OnSuccess, const FErrorHandler& OnError);
void Ecommerce_Currency_Delete(FString CurrencyCode, const FSimpleDelegate& OnSuccess, const FErrorHandler& OnError);

void Ecommerce_PublishedStore_Get(const FString& Namespace, const THandler<FStoreInfo>& OnSuccess, const FErrorHandler& OnError);
void Ecommerce_PublishedStore_Get(const THandler<FStoreInfo>& OnSuccess, const FErrorHandler& OnError);
void Ecommerce_PublishedStore_Delete(const FSimpleDelegate& OnSuccess, const FErrorHandler& OnError);

void Ecommerce_Store_Create(FStoreCreateRequest Store, const THandler<FStoreInfo>& OnSuccess, const FErrorHandler& OnError);
void Ecommerce_Store_Create(const FString& Namespace, FStoreCreateRequest Store, const THandler<FStoreInfo>& OnSuccess, const FErrorHandler& OnError);
void Ecommerce_Store_Get_All(const FString& Namespace, const THandler<TArray<FStoreInfo>>& OnSuccess, const FErrorHandler& OnError);
void Ecommerce_Store_Get_All(const THandler<TArray<FStoreInfo>>& OnSuccess, const FErrorHandler& OnError);
void Ecommerce_Store_Delete(FString StoreId, const FSimpleDelegate& OnSuccess, const FErrorHandler& OnError);
void Ecommerce_Store_Clone(FString Source, FString Target, const FSimpleDelegate& OnSuccess, const FErrorHandler& OnError);
void Ecommerce_Store_Clone(const FString& Namespace, FString Source, FString Target, const FSimpleDelegate& OnSuccess, const FErrorHandler& OnError);

void Ecommerce_Category_Create(FCategoryCreateRequest Category, FString StoreId, const THandler<FCategoryInfo>& OnSuccess, const FErrorHandler& OnError);

void Ecommerce_Item_Create(FItemCreateRequest Item, FString StoreId, const THandler<FItemFullInfo>& OnSuccess, const FErrorHandler& OnError);
void Ecommerce_Item_Create(const FString& Namespace, FItemCreateRequest Item, FString StoreId, const THandler<FItemFullInfo>& OnSuccess, const FErrorHandler& OnError);
void Ecommerce_GetItem_BySKU(const FString& Namespace, const FString& StoreId, const FString& SKU, const bool& ActiveOnly, const THandler<FItemFullInfo>& OnSuccess, const FErrorHandler& OnError);
void Ecommerce_GrantUserEntitlements(const FString& Namespace, const FString& UserId, const TArray<FAccelByteModelsEntitlementGrant>& EntitlementGrant, const THandler<TArray<FAccelByteModelsStackableEntitlementInfo>>& OnSuccess, const FErrorHandler& OnError);

void Ecommerce_Campaign_Create(FCampaignCreateModel body, const THandler<FCampaignInfo>& OnSuccess, const FErrorHandler& OnError);
void Ecommerce_Campaign_Update(FString campaignId, FCampaignUpdateModel body, const THandler<FCampaignInfo>& OnSuccess, const FErrorHandler& OnError);
void Ecommerce_CampaignCodes_Create(FString campaignId, FCampaignCodeCreateModel body, const THandler<FCampaignCodeCreateResult>& OnSuccess, const FErrorHandler& OnError);
void Ecommerce_Campaign_Get_ByName(FString name, const THandler<FCampaignPagingSlicedResult>& OnSuccess, const FErrorHandler& OnError);
void Ecommerce_CampaignCodes_Get(FString campaignId, const THandler<FCodeInfoPagingSlicedResult>& OnSuccess, const FErrorHandler& OnError);
void Ecommerce_CampaignCode_Disable(FString code, const THandler<FCodeInfo>& OnSuccess, const FErrorHandler& OnError);