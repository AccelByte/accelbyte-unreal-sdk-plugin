// Copyright (c) 2021 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "Blueprints/ABEntitlement.h"
#include "Api/AccelByteEntitlementApi.h"


using namespace AccelByte;

void UABEntitlement::SetApiClient(FApiClientPtr const& NewApiClientPtr)
{
	ApiClientPtr = NewApiClientPtr;
}

void UABEntitlement::GetCurrentUserEntitlementHistory(
	FDAccelByteModelsEntitlementHistoryPagingResponse OnSuccess,
	FDErrorHandler OnError,
	EAccelByteEntitlementClass const& EntitlementClass,
	FDateTime StartDate, 
	FDateTime EndDate, 
	int32 Limit, 
	int32 Offset)
{
	const auto EntitlementPtr = ApiClientPtr->GetEntitlementApi().Pin();
	if (EntitlementPtr.IsValid())
	{
		EntitlementPtr->GetCurrentUserEntitlementHistory(
			THandler<FAccelByteModelsUserEntitlementHistoryPagingResult>::CreateLambda(
				[OnSuccess](FAccelByteModelsUserEntitlementHistoryPagingResult const& Response)
				{
					OnSuccess.ExecuteIfBound(Response);
				}),
			FErrorHandler::CreateLambda(
				[OnError](int Code, FString const& Message)
				{
					OnError.ExecuteIfBound(Code, Message);
				}),
			EntitlementClass, StartDate, EndDate, Limit, Offset);
	}
	else
	{
		OnError.ExecuteIfBound(static_cast<int32>(AccelByte::ErrorCodes::InvalidRequest), TEXT("Api already destroyed!"));
	}
}

void UABEntitlement::QueryUserEntitlements(
	FString const& EntitlementName, 
	TArray<FString> const& ItemIds, 
	int32 const& Offset, 
	int32 const& Limit, 
	FDAccelByteModelsEntitlementPagingSlicedResultResponse OnSuccess, 
	FDErrorHandler const& OnError, 
	EAccelByteEntitlementClass EntitlementClass, 
	EAccelByteAppType AppType) 
{
	const auto EntitlementPtr = ApiClientPtr->GetEntitlementApi().Pin();
	if (EntitlementPtr.IsValid())
	{
		EntitlementPtr->QueryUserEntitlements(
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
	else
	{
		OnError.ExecuteIfBound(static_cast<int32>(AccelByte::ErrorCodes::InvalidRequest), TEXT("Api already destroyed!"));
	}
}

void UABEntitlement::GetUserEntitlementById(
	FString const& Entitlementid, 
	FDAccelByteModelsEntitlementInfoResponse OnSuccess, 
	FDErrorHandler OnError) 
{
	const auto EntitlementPtr = ApiClientPtr->GetEntitlementApi().Pin();
	if (EntitlementPtr.IsValid())
	{
		EntitlementPtr->GetUserEntitlementById(
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
	else
	{
		OnError.ExecuteIfBound(static_cast<int32>(AccelByte::ErrorCodes::InvalidRequest), TEXT("Api already destroyed!"));
	}
}

void UABEntitlement::GetUserEntitlementByIds(
	TArray<FString> const& EntitlementIds, 
	FDAccelByteModelsEntitlementInfoResponses OnSuccess, 
	FDErrorHandler OnError, 
	bool bAvailablePlatformOnly)
{
	const auto EntitlementPtr = ApiClientPtr->GetEntitlementApi().Pin();
	if (EntitlementPtr.IsValid())
	{
		EntitlementPtr->GetUserEntitlementByIds(
		EntitlementIds,
		THandler<TArray<FAccelByteModelsEntitlementInfo>>::CreateLambda(
			[OnSuccess](TArray<FAccelByteModelsEntitlementInfo> const& Response)
			{
				OnSuccess.ExecuteIfBound(Response);
			}),
		FErrorHandler::CreateLambda(
			[OnError](int Code, FString const& Message)
			{
				OnError.ExecuteIfBound(Code, Message);
			}),
		bAvailablePlatformOnly);
	}
	else
	{
		OnError.ExecuteIfBound(static_cast<int32>(AccelByte::ErrorCodes::InvalidRequest), TEXT("Api already destroyed!"));
	}
}

void UABEntitlement::GetUserEntitlementOwnershipByAppId(
	FString const& AppId, 
	FDAccelByteModelsEntitlementOwnershipResponse OnSuccess, 
	FDErrorHandler OnError) 
{
	const auto EntitlementPtr = ApiClientPtr->GetEntitlementApi().Pin();
	if (EntitlementPtr.IsValid())
	{
		EntitlementPtr->GetUserEntitlementOwnershipByAppId(
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
	else
	{
		OnError.ExecuteIfBound(static_cast<int32>(AccelByte::ErrorCodes::InvalidRequest), TEXT("Api already destroyed!"));
	}
}

void UABEntitlement::GetUserEntitlementOwnershipBySku(
	FString const& Sku, 
	FDAccelByteModelsEntitlementOwnershipResponse OnSuccess, 
	FDErrorHandler OnError) 
{
	const auto EntitlementPtr = ApiClientPtr->GetEntitlementApi().Pin();
	if (EntitlementPtr.IsValid())
	{
		EntitlementPtr->GetUserEntitlementOwnershipBySku(
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
	else
	{
		OnError.ExecuteIfBound(static_cast<int32>(AccelByte::ErrorCodes::InvalidRequest), TEXT("Api already destroyed!"));
	}
}

void UABEntitlement::GetUserEntitlementOwnershipByItemId(
	FString const& ItemId, 
	FDAccelByteModelsEntitlementOwnershipResponse OnSuccess, 
	FDErrorHandler OnError) 
{
	const auto EntitlementPtr = ApiClientPtr->GetEntitlementApi().Pin();
	if (EntitlementPtr.IsValid())
	{
		EntitlementPtr->GetUserEntitlementOwnershipByItemId(
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
	else
	{
		OnError.ExecuteIfBound(static_cast<int32>(AccelByte::ErrorCodes::InvalidRequest), TEXT("Api already destroyed!"));
	}
}

void UABEntitlement::GetUserEntitlementOwnershipAny(
	TArray<FString> const& ItemIds, 
	TArray<FString> const& AppIds, 
	TArray<FString> const& Skus, 
	FDAccelByteModelsEntitlementOwnershipResponse OnSuccess, 
	FDErrorHandler OnError) 
{
	const auto EntitlementPtr = ApiClientPtr->GetEntitlementApi().Pin();
	if (EntitlementPtr.IsValid())
	{
		EntitlementPtr->GetUserEntitlementOwnershipAny(
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
	else
	{
		OnError.ExecuteIfBound(static_cast<int32>(AccelByte::ErrorCodes::InvalidRequest), TEXT("Api already destroyed!"));
	}
}

void UABEntitlement::ConsumeUserEntitlement(
	FString const& EntitlementId, 
	int32 const& UseCount, 
	FDAccelByteModelsEntitlementInfoResponse OnSuccess, 
	FDErrorHandler OnError) 
{
	const auto EntitlementPtr = ApiClientPtr->GetEntitlementApi().Pin();
	if (EntitlementPtr.IsValid())
	{
		EntitlementPtr->ConsumeUserEntitlement(
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
	else
	{
		OnError.ExecuteIfBound(static_cast<int32>(AccelByte::ErrorCodes::InvalidRequest), TEXT("Api already destroyed!"));
	}
}

void UABEntitlement::CreateDistributionReceiver(
	FString const& ExtUserId, 
	FAccelByteModelsAttributes const& Attributes, 
	FDHandler OnSuccess, 
	FDErrorHandler OnError) 
{
	const auto EntitlementPtr = ApiClientPtr->GetEntitlementApi().Pin();
	if (EntitlementPtr.IsValid())
	{
		EntitlementPtr->CreateDistributionReceiver(
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
	else
	{
		OnError.ExecuteIfBound(static_cast<int32>(AccelByte::ErrorCodes::InvalidRequest), TEXT("Api already destroyed!"));
	}
}

void UABEntitlement::DeleteDistributionReceiver(
	FString const& ExtUserId, 
	FString const& UserId, 
	FDHandler OnSuccess, 
	FDErrorHandler OnError) 
{
	const auto EntitlementPtr = ApiClientPtr->GetEntitlementApi().Pin();
	if (EntitlementPtr.IsValid())
	{
		EntitlementPtr->DeleteDistributionReceiver(
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
	else
	{
		OnError.ExecuteIfBound(static_cast<int32>(AccelByte::ErrorCodes::InvalidRequest), TEXT("Api already destroyed!"));
	}
}

void UABEntitlement::GetDistributionReceiver(
	FString const& PublisherNamespace, 
	FString const& PublisherUserId, 
	FDArrayModelsDistributionReceiverResponse OnSuccess, 
	FDErrorHandler OnError) 

{
	const auto EntitlementPtr = ApiClientPtr->GetEntitlementApi().Pin();
	if (EntitlementPtr.IsValid())
	{
		EntitlementPtr->GetDistributionReceiver(
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
	else
	{
		OnError.ExecuteIfBound(static_cast<int32>(AccelByte::ErrorCodes::InvalidRequest), TEXT("Api already destroyed!"));
	}
}

void UABEntitlement::UpdateDistributionReceiver(
	FString const& ExtUserId, 
	FAccelByteModelsAttributes const& Attributes, 
	FDHandler OnSuccess, 
	FDErrorHandler OnError) 
{
	const auto EntitlementPtr = ApiClientPtr->GetEntitlementApi().Pin();
	if (EntitlementPtr.IsValid())
	{
		EntitlementPtr->UpdateDistributionReceiver(
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
	else
	{
		OnError.ExecuteIfBound(static_cast<int32>(AccelByte::ErrorCodes::InvalidRequest), TEXT("Api already destroyed!"));
	}
}

void UABEntitlement::SyncPlatformPurchase(
	EAccelBytePlatformSync PlatformType, 
	FDHandler OnSuccess, 
	FDErrorHandler OnError) 
{
	const auto EntitlementPtr = ApiClientPtr->GetEntitlementApi().Pin();
	if (EntitlementPtr.IsValid())
	{
		EntitlementPtr->SyncPlatformPurchase(
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
	else
	{
		OnError.ExecuteIfBound(static_cast<int32>(AccelByte::ErrorCodes::InvalidRequest), TEXT("Api already destroyed!"));
	}
}

void UABEntitlement::SyncPlatformPurchaseSingleItem(
	FAccelByteModelsEntitlementSyncBase const& EntitlementSyncBase,
	EAccelBytePlatformSync PlatformType,
	FDHandler OnSuccess,
	FDErrorHandler OnError)
{
	const auto EntitlementPtr = ApiClientPtr->GetEntitlementApi().Pin();
	if (EntitlementPtr.IsValid())
	{
		EntitlementPtr->SyncPlatformPurchase(
		EntitlementSyncBase,
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
	else
	{
		OnError.ExecuteIfBound(static_cast<int32>(AccelByte::ErrorCodes::InvalidRequest), TEXT("Api already destroyed!"));
	}
}

void UABEntitlement::SyncMobilePlatformPurchaseGoogle(
	FAccelByteModelsPlatformSyncMobileGoogle const& SyncRequest, 
	FDHandler OnSuccess, 
	FDErrorHandler OnError) 
{
	const auto EntitlementPtr = ApiClientPtr->GetEntitlementApi().Pin();
	if (EntitlementPtr.IsValid())
	{
		EntitlementPtr->SyncMobilePlatformPurchaseGoogle(
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
	else
	{
		OnError.ExecuteIfBound(static_cast<int32>(AccelByte::ErrorCodes::InvalidRequest), TEXT("Api already destroyed!"));
	}
}

void UABEntitlement::SyncMobilePlatformPurchaseGooglePlay(
	FAccelByteModelsPlatformSyncMobileGoogle const& SyncRequest,
	FDAccelByteModelsPlatformSyncMobileGoogleResponse OnSuccess,
	FDErrorHandler OnError)
{
	const auto EntitlementPtr = ApiClientPtr->GetEntitlementApi().Pin();
	if (EntitlementPtr.IsValid())
	{
		EntitlementPtr->SyncMobilePlatformPurchaseGooglePlay(
		SyncRequest,
		THandler<FAccelByteModelsPlatformSyncMobileGoogleResponse>::CreateLambda(
			[OnSuccess](const FAccelByteModelsPlatformSyncMobileGoogleResponse& Result)
			{
				OnSuccess.ExecuteIfBound(Result);
			}),
		FErrorHandler::CreateLambda(
			[OnError](int Code, FString const& Message)
			{
				OnError.ExecuteIfBound(Code, Message);
			}));
	}
	else
	{
		OnError.ExecuteIfBound(static_cast<int32>(AccelByte::ErrorCodes::InvalidRequest), TEXT("Api already destroyed!"));
	}
}

void UABEntitlement::SyncMobilePlatformPurchaseApple(
	FAccelByteModelsPlatformSyncMobileApple const& SyncRequest, 
	FDHandler OnSuccess, 
	FDErrorHandler OnError)
{
	const auto EntitlementPtr = ApiClientPtr->GetEntitlementApi().Pin();
	if (EntitlementPtr.IsValid())
	{
		EntitlementPtr->SyncMobilePlatformPurchaseApple(
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
	else
	{
		OnError.ExecuteIfBound(static_cast<int32>(AccelByte::ErrorCodes::InvalidRequest), TEXT("Api already destroyed!"));
	}
}

void UABEntitlement::SyncTwitchDropEntitlement(
	FAccelByteModelsTwitchDropEntitlement const& TwitchDropModel, 
	FDHandler OnSuccess, FDErrorHandler OnError) 
{
	const auto EntitlementPtr = ApiClientPtr->GetEntitlementApi().Pin();
	if (EntitlementPtr.IsValid())
	{
		EntitlementPtr->SyncTwitchDropEntitlement(
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
	else
	{
		OnError.ExecuteIfBound(static_cast<int32>(AccelByte::ErrorCodes::InvalidRequest), TEXT("Api already destroyed!"));
	}
}

void UABEntitlement::ValidateUserItemPurchaseCondition(TArray<FString> const& Items,
	FDAccelByteModelsPlatformValidateUserItemPurchaseResponse OnSuccess, FDErrorHandler OnError)
{
	const auto EntitlementPtr = ApiClientPtr->GetEntitlementApi().Pin();
	if (EntitlementPtr.IsValid())
	{
		EntitlementPtr->ValidateUserItemPurchaseCondition(
		Items,
		THandler<TArray<FAccelByteModelsPlatformValidateUserItemPurchaseResponse>>::CreateLambda(
			[OnSuccess](const TArray<FAccelByteModelsPlatformValidateUserItemPurchaseResponse>& Result)
			{
				OnSuccess.ExecuteIfBound(Result);
			}), 
		FErrorHandler::CreateLambda(
			[OnError](int Code, FString const& Message)
			{
				OnError.ExecuteIfBound(Code, Message);
			}));
	}
	else
	{
		OnError.ExecuteIfBound(static_cast<int32>(AccelByte::ErrorCodes::InvalidRequest), TEXT("Api already destroyed!"));
	}
}