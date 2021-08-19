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
	FString purchasingItemTitle;
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

void AdminCreateEcommerceCurrency(FCurrencyCreateRequest Currency, const FSimpleDelegate& OnSuccess, const FErrorHandler& OnError);
void AdminGetEcommerceCurrency(FString CurrencyCode, const FSimpleDelegate& OnSuccess, const FErrorHandler& OnError);
void AdminDeleteEcommerceCurrency(FString CurrencyCode, const FSimpleDelegate& OnSuccess, const FErrorHandler& OnError);

void AdminGetEcommercePublishedStore(const FString& Namespace, const THandler<FStoreInfo>& OnSuccess, const FErrorHandler& OnError);
void AdminGetEcommercePublishedStore(const THandler<FStoreInfo>& OnSuccess, const FErrorHandler& OnError);
void AdminDeleteEcommercePublishedStore(const FSimpleDelegate& OnSuccess, const FErrorHandler& OnError);

void AdminCreateEcommerceStore(FStoreCreateRequest Store, const THandler<FStoreInfo>& OnSuccess, const FErrorHandler& OnError);
void AdminCreateEcommerceStore(const FString& Namespace, FStoreCreateRequest Store, const THandler<FStoreInfo>& OnSuccess, const FErrorHandler& OnError);
void AdminGetEcommerceStoreAll(const FString& Namespace, const THandler<TArray<FStoreInfo>>& OnSuccess, const FErrorHandler& OnError);
void AdminGetEcommerceStoreAll(const THandler<TArray<FStoreInfo>>& OnSuccess, const FErrorHandler& OnError);
void AdminDeleteEcommerceStore(FString StoreId, const FSimpleDelegate& OnSuccess, const FErrorHandler& OnError);
void AdminCloneEcommerceStore(FString Source, FString Target, const FSimpleDelegate& OnSuccess, const FErrorHandler& OnError);
void AdminCloneEcommerceStore(const FString& Namespace, FString Source, FString Target, const FSimpleDelegate& OnSuccess, const FErrorHandler& OnError);

void AdminCreateEcommerceCategory(FCategoryCreateRequest Category, FString StoreId, const THandler<FCategoryInfo>& OnSuccess, const FErrorHandler& OnError);

void AdminCreateEcommerceItem(FItemCreateRequest Item, FString StoreId, const THandler<FItemFullInfo>& OnSuccess, const FErrorHandler& OnError);
void AdminCreateEcommerceItem(const FString& Namespace, FItemCreateRequest Item, FString StoreId, const THandler<FItemFullInfo>& OnSuccess, const FErrorHandler& OnError);
void AdminGetEcommerceItemBySKU(const FString& Namespace, const FString& StoreId, const FString& SKU, const bool& ActiveOnly, const THandler<FItemFullInfo>& OnSuccess, const FErrorHandler& OnError);
void AdminGrantEcommerceUserEntitlements(const FString& Namespace, const FString& UserId, const TArray<FAccelByteModelsEntitlementGrant>& EntitlementGrant, const THandler<TArray<FAccelByteModelsStackableEntitlementInfo>>& OnSuccess, const FErrorHandler& OnError);

void AdminCreateEcommerceCampaign(FCampaignCreateModel body, const THandler<FCampaignInfo>& OnSuccess, const FErrorHandler& OnError);
void AdminUpdateEcommerceCampaign(FString campaignId, FCampaignUpdateModel body, const THandler<FCampaignInfo>& OnSuccess, const FErrorHandler& OnError);
void AdminGetEcommerceCampaignByName(FString name, const THandler<FCampaignPagingSlicedResult>& OnSuccess, const FErrorHandler& OnError);
void AdminCreateEcommerceCampaignCodes(FString campaignId, FCampaignCodeCreateModel body, const THandler<FCampaignCodeCreateResult>& OnSuccess, const FErrorHandler& OnError);
void AdminGetEcommerceCampaignCodes(FString campaignId, const THandler<FCodeInfoPagingSlicedResult>& OnSuccess, const FErrorHandler& OnError);
void AdminDisableEcommerceCampaignCode(FString code, const THandler<FCodeInfo>& OnSuccess, const FErrorHandler& OnError);