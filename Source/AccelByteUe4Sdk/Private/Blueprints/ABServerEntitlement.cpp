// Copyright (c) 2024 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "Blueprints/ABServerEntitlement.h"

using namespace AccelByte;

void UABServerEntitlement::SetServerApiClient(FServerApiClientPtr const& NewServerApiClientPtr)
{
	ApiClientPtr = NewServerApiClientPtr;
}

void UABServerEntitlement::GetUserEntitlementHistory(
	FString const& UserId, 
	FString const& EntitlementId, 
	FDAccelByteModelsEntitlementHistoryResponse OnSuccess, 
	FDErrorHandler OnError)
{
	ApiClientPtr->ServerEcommerce.GetUserEntitlementHistory(
		UserId,
		EntitlementId,
		THandler<TArray<FAccelByteModelsUserEntitlementHistory>>::CreateLambda(
			[OnSuccess](TArray<FAccelByteModelsUserEntitlementHistory> const& Response)
			{
				OnSuccess.ExecuteIfBound(Response);
			}),
		FErrorHandler::CreateLambda(
			[OnError](int Code, FString const& Message)
			{
				OnError.ExecuteIfBound(Code, Message);
			}));
}