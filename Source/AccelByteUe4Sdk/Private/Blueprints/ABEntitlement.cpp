// Copyright (c) 2021 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "Blueprints/ABEntitlement.h"
#include "Api/AccelByteEntitlementApi.h"
#include "Core/AccelByteRegistry.h"

void UABEntitlement::SetApiClient(FApiClientPtr const& NewApiClientPtr)
{
	ApiClientPtr = NewApiClientPtr;
}

void UABEntitlement::QueryUserEntitlements(
	FString const& EntitlementName, 
	TArray<FString> const& ItemIds, 
	int32 const& Offset, 
	int32 const& Limit, 
	FDAccelByteModelsEntitlementPagingSlicedResultResponse OnSuccess, 
	FDErrorHandler const& OnError, 
	EAccelByteEntitlementClass EntitlementClass, 
	EAccelByteAppType AppType) const
{
	ApiClientPtr->Entitlement.QueryUserEntitlements(
		EntitlementName,
		ItemIds,
		Offset,
		Limit,
		THandler<FAccelByteModelsEntitlementPagingSlicedResult>::CreateLambda(
			[OnSuccess](FAccelByteModelsEntitlementPagingSlicedResult const& Response)
			{
				OnSuccess.ExecuteIfBound(Response);
			}),
		FErrorHandler::CreateLambda(
			[OnError](int Code, FString const& Message)
			{
				OnError.ExecuteIfBound(Code, Message);
			}),
		EntitlementClass,
		AppType);
}

void UABEntitlement::GetUserEntitlementById(
	FString const& Entitlementid, 
	FDAccelByteModelsEntitlementInfoResponse OnSuccess, 
	FDErrorHandler OnError) const
{
	ApiClientPtr->Entitlement.GetUserEntitlementById(
		Entitlementid,
		THandler<FAccelByteModelsEntitlementInfo>::CreateLambda(
			[OnSuccess](FAccelByteModelsEntitlementInfo const& Response)
			{
				OnSuccess.ExecuteIfBound(Response);
			}),
		FErrorHandler::CreateLambda(
			[OnError](int Code, FString const& Message)
			{
				OnError.ExecuteIfBound(Code, Message);
			}));
}

void UABEntitlement::GetUserEntitlementOwnershipByAppId(
	FString const& AppId, 
	FDAccelByteModelsEntitlementOwnershipResponse OnSuccess, 
	FDErrorHandler OnError) const
{
	ApiClientPtr->Entitlement.GetUserEntitlementOwnershipByAppId(
		AppId,
		THandler<FAccelByteModelsEntitlementOwnership>::CreateLambda(
			[OnSuccess](FAccelByteModelsEntitlementOwnership const& Response)
			{
				OnSuccess.ExecuteIfBound(Response);
			}),
		FErrorHandler::CreateLambda(
			[OnError](int Code, FString const& Message)
			{
				OnError.ExecuteIfBound(Code, Message);
			}));
}

void UABEntitlement::GetUserEntitlementOwnershipBySku(
	FString const& Sku, 
	FDAccelByteModelsEntitlementOwnershipResponse OnSuccess, 
	FDErrorHandler OnError) const
{
	ApiClientPtr->Entitlement.GetUserEntitlementOwnershipBySku(
		Sku,
		THandler<FAccelByteModelsEntitlementOwnership>::CreateLambda(
			[OnSuccess](FAccelByteModelsEntitlementOwnership const& Response)
			{
				OnSuccess.ExecuteIfBound(Response);
			}),
		FErrorHandler::CreateLambda(
			[OnError](int Code, FString const& Message)
			{
				OnError.ExecuteIfBound(Code, Message);
			}));
}

void UABEntitlement::GetUserEntitlementOwnershipByItemId(
	FString const& ItemId, 
	FDAccelByteModelsEntitlementOwnershipResponse OnSuccess, 
	FDErrorHandler OnError) const
{
	ApiClientPtr->Entitlement.GetUserEntitlementOwnershipByItemId(
		ItemId,
		THandler<FAccelByteModelsEntitlementOwnership>::CreateLambda(
			[OnSuccess](FAccelByteModelsEntitlementOwnership const& Response)
			{
				OnSuccess.ExecuteIfBound(Response);
			}),
		FErrorHandler::CreateLambda(
			[OnError](int Code, FString const& Message)
			{
				OnError.ExecuteIfBound(Code, Message);
			}));
}

void UABEntitlement::GetUserEntitlementOwnershipAny(
	TArray<FString> const& ItemIds, 
	TArray<FString> const& AppIds, 
	TArray<FString> const& Skus, 
	FDAccelByteModelsEntitlementOwnershipResponse OnSuccess, 
	FDErrorHandler OnError) const
{
	ApiClientPtr->Entitlement.GetUserEntitlementOwnershipAny(
		ItemIds,
		AppIds,
		Skus,
		THandler<FAccelByteModelsEntitlementOwnership>::CreateLambda(
			[OnSuccess](FAccelByteModelsEntitlementOwnership const& Response)
			{
				OnSuccess.ExecuteIfBound(Response);
			}),
		FErrorHandler::CreateLambda(
			[OnError](int Code, FString const& Message)
			{
				OnError.ExecuteIfBound(Code, Message);
			}));
}

void UABEntitlement::ConsumeUserEntitlement(
	FString const& EntitlementId, 
	int32 const& UseCount, 
	FDAccelByteModelsEntitlementInfoResponse OnSuccess, 
	FDErrorHandler OnError) const
{
	ApiClientPtr->Entitlement.ConsumeUserEntitlement(
		EntitlementId,
		UseCount,
		THandler<FAccelByteModelsEntitlementInfo>::CreateLambda(
			[OnSuccess](FAccelByteModelsEntitlementInfo const& Response)
			{
				OnSuccess.ExecuteIfBound(Response);
			}),
		FErrorHandler::CreateLambda(
			[OnError](int Code, FString const& Message)
			{
				OnError.ExecuteIfBound(Code, Message);
			}));
}

void UABEntitlement::CreateDistributionReceiver(
	FString const& ExtUserId, 
	FAccelByteModelsAttributes const& Attributes, 
	FDHandler OnSuccess, 
	FDErrorHandler OnError) const
{
	ApiClientPtr->Entitlement.CreateDistributionReceiver(
		ExtUserId,
		Attributes,
		FVoidHandler::CreateLambda(
			[OnSuccess]()
			{
				OnSuccess.ExecuteIfBound();
			}),
		FErrorHandler::CreateLambda(
			[OnError](int Code, FString const& Message)
			{
				OnError.ExecuteIfBound(Code, Message);
			}));
}

void UABEntitlement::DeleteDistributionReceiver(
	FString const& ExtUserId, 
	FString const& UserId, 
	FDHandler OnSuccess, 
	FDErrorHandler OnError) const
{
	ApiClientPtr->Entitlement.DeleteDistributionReceiver(
		ExtUserId,
		UserId,
		FVoidHandler::CreateLambda(
			[OnSuccess]()
			{
				OnSuccess.ExecuteIfBound();
			}),
		FErrorHandler::CreateLambda(
			[OnError](int Code, FString const& Message)
			{
				OnError.ExecuteIfBound(Code, Message);
			}));
}

void UABEntitlement::GetDistributionReceiver(
	FString const& PublisherNamespace, 
	FString const& PublisherUserId, 
	FDArrayModelsDistributionReceiverResponse OnSuccess, 
	FDErrorHandler OnError) const

{
	ApiClientPtr->Entitlement.GetDistributionReceiver(
		PublisherNamespace,
		PublisherUserId,
		THandler<TArray<FAccelByteModelsDistributionReceiver>>::CreateLambda(
			[OnSuccess](TArray<FAccelByteModelsDistributionReceiver> const& Response)
			{
				FArrayModelsDistributionReceiver Result;
				Result.Content = Response;
				OnSuccess.ExecuteIfBound(Result);
			}),
		FErrorHandler::CreateLambda(
			[OnError](int Code, FString const& Message)
			{
				OnError.ExecuteIfBound(Code, Message);
			}));
}

void UABEntitlement::UpdateDistributionReceiver(
	FString const& ExtUserId, 
	FAccelByteModelsAttributes const& Attributes, 
	FDHandler OnSuccess, 
	FDErrorHandler OnError) const
{
	ApiClientPtr->Entitlement.UpdateDistributionReceiver(
		ExtUserId,
		Attributes,
		FVoidHandler::CreateLambda(
			[OnSuccess]()
			{
				OnSuccess.ExecuteIfBound();
			}),
		FErrorHandler::CreateLambda(
			[OnError](int Code, FString const& Message)
			{
				OnError.ExecuteIfBound(Code, Message);
			}));
}

void UABEntitlement::SyncPlatformPurchase(
	EAccelBytePlatformSync PlatformType, 
	FDHandler OnSuccess, 
	FDErrorHandler OnError) const
{
	ApiClientPtr->Entitlement.SyncPlatformPurchase(
		PlatformType,
		FVoidHandler::CreateLambda(
			[OnSuccess]()
			{
				OnSuccess.ExecuteIfBound();
			}),
		FErrorHandler::CreateLambda(
			[OnError](int Code, FString const& Message)
			{
				OnError.ExecuteIfBound(Code, Message);
			}));
}

void UABEntitlement::SyncMobilePlatformPurchaseGoogle(
	FAccelByteModelsPlatformSyncMobileGoogle const& SyncRequest, 
	FDHandler OnSuccess, 
	FDErrorHandler OnError) const
{
	ApiClientPtr->Entitlement.SyncMobilePlatformPurchaseGoogle(
		SyncRequest,
		FVoidHandler::CreateLambda(
			[OnSuccess]()
			{
				OnSuccess.ExecuteIfBound();
			}),
		FErrorHandler::CreateLambda(
			[OnError](int Code, FString const& Message)
			{
				OnError.ExecuteIfBound(Code, Message);
			}));
}

void UABEntitlement::SyncMobilePlatformPurchaseApple(
	FAccelByteModelsPlatformSyncMobileApple const& SyncRequest, 
	FDHandler OnSuccess, 
	FDErrorHandler OnError) const
{
	ApiClientPtr->Entitlement.SyncMobilePlatformPurchaseApple(
		SyncRequest,
		FVoidHandler::CreateLambda(
			[OnSuccess]()
			{
				OnSuccess.ExecuteIfBound();
			}),
		FErrorHandler::CreateLambda(
			[OnError](int Code, FString const& Message)
			{
				OnError.ExecuteIfBound(Code, Message);
			}));
}

void UABEntitlement::SyncTwitchDropEntitlement(
	FAccelByteModelsTwitchDropEntitlement const& TwitchDropModel, 
	FDHandler OnSuccess, FDErrorHandler OnError) const 
{
	ApiClientPtr->Entitlement.SyncTwitchDropEntitlement(
		TwitchDropModel,
		FVoidHandler::CreateLambda(
			[OnSuccess]()
	{
		OnSuccess.ExecuteIfBound();
	}),
		FErrorHandler::CreateLambda(
			[OnError](int Code, FString const& Message)
	{
		OnError.ExecuteIfBound(Code, Message);
	}));
}
