// Copyright (c) 2021 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "Blueprints/ABFulfillment.h"
#include "Api/AccelByteFulfillmentApi.h"
#include "Core/AccelByteRegistry.h"

void UABFulfillment::SetApiClient(FApiClientPtr const& NewApiClientPtr)
{
	ApiClientPtr = NewApiClientPtr;
}

void UABFulfillment::RedeemCode(
	FString const& Code,
	FString const& Region,
	FString const& Language,
	FDAccelByteModelsFulfillmentResponse OnSuccess,
	FDErrorHandler OnError) 
{
	ApiClientPtr->Fulfillment.RedeemCode(
		Code,
		Region,
		Language,
		THandler<FAccelByteModelsFulfillmentResult>::CreateLambda(
			[OnSuccess](FAccelByteModelsFulfillmentResult const& Response)
			{
				OnSuccess.ExecuteIfBound(Response);
			}),
		FErrorHandler::CreateLambda(
			[OnError](int Code, FString const& Message)
			{
				OnError.ExecuteIfBound(Code, Message);
			}));
}
