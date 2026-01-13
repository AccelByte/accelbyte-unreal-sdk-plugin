// Copyright (c) 2022 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "Core/AccelByteEntitlementTokenGenerator.h"
#include "Api/AccelByteEntitlementApi.h"
#include "Models/AccelByteEcommerceModels.h"
#include "Core/AccelByteJwtWrapper.h"

using namespace AccelByte;
DECLARE_LOG_CATEGORY_EXTERN(LogAccelByteEntitlementTokenGenerator, Log, All);
DEFINE_LOG_CATEGORY(LogAccelByteEntitlementTokenGenerator);

FAccelByteEntitlementTokenGenerator::FAccelByteEntitlementTokenGenerator(const FTokenGeneratorParams& Params)
	: FAccelByteEntitlementTokenGenerator(Params, nullptr)
{
}

FAccelByteEntitlementTokenGenerator::FAccelByteEntitlementTokenGenerator(const FTokenGeneratorParams& Params, const FApiClientPtr& InApiClient)
	: Skus(Params.Skus)
	, ItemIds(Params.ItemIds)
	, AppIds(Params.AppIds)
	, ApiClient(InApiClient)
{
}

void FAccelByteEntitlementTokenGenerator::RequestToken()
{
	if (!Token.IsEmpty() && ExpiredDateTime > FDateTime::UtcNow())
	{
		TokenReceivedEvent.Broadcast(Token);
		return;
	}

	const THandler<FAccelByteModelsOwnershipToken> OnGetUserEntitlementOwnershipTokenSuccessDelegate =
		THandler<FAccelByteModelsOwnershipToken>::CreateSP(SharedThis(this), &FAccelByteEntitlementTokenGenerator::OnGetUserEntitlementOwnershipTokenSuccess);

	const FErrorHandler OnGetUserEntitlementOwnershipTokenErrorDelegate =
		FErrorHandler::CreateSP(SharedThis(this), &FAccelByteEntitlementTokenGenerator::OnGetUserEntitlementOwnershipTokenError);

	if (ApiClient.IsValid())
	{
		auto Entitlement = ApiClient->GetEntitlementApi().Pin();
		if (Entitlement.IsValid())
		{
			Entitlement->GetUserEntitlementOwnershipTokenOnly(ItemIds
				, AppIds
				, Skus
				, OnGetUserEntitlementOwnershipTokenSuccessDelegate
				, OnGetUserEntitlementOwnershipTokenErrorDelegate);
		}
		else
		{
			UE_LOG(LogAccelByteEntitlementTokenGenerator, Warning, TEXT("Invalid Entitlement API from API Client"));
			OnGetUserEntitlementOwnershipTokenError(static_cast<int32>(ErrorCodes::InvalidRequest), TEXT("Invalid Entitlement API from API Client"));
		}
	}
	else
	{
		OnGetUserEntitlementOwnershipTokenError(static_cast<int32>(ErrorCodes::InvalidRequest), TEXT("ApiClient is invalid"));
	}
}

void FAccelByteEntitlementTokenGenerator::OnGetUserEntitlementOwnershipTokenSuccess(const FAccelByteModelsOwnershipToken& Result)
{
	Token = Result.OwnershipToken;

	FDateTime TokenExpiration{};
	AccelByteJwtWrapper::GetExpirationDateTime(Token, TokenExpiration);
	ExpiredDateTime = FDateTime::UtcNow() + (TokenExpiration - FDateTime::UtcNow() - FTimespan::FromSeconds(30));

	TokenReceivedEvent.Broadcast(Token);
}

bool FAccelByteEntitlementTokenGenerator::IsTokenValid() const
{
	return !Token.IsEmpty() && ExpiredDateTime >= FDateTime::UtcNow();
}

FAccelByteEntitlementTokenGenerator::FTokenReceivedEvent& FAccelByteEntitlementTokenGenerator::OnTokenReceived()
{
	return TokenReceivedEvent;
}

void FAccelByteEntitlementTokenGenerator::OnGetUserEntitlementOwnershipTokenError(int32 ErrorCode, const FString& ErrorMessage)
{
	UE_LOG(LogAccelByteEntitlementTokenGenerator, Warning, TEXT("Get user entitlement ownership token failed with code %d, %s"), ErrorCode, *ErrorMessage);

	// Even though token generation failed, we want to broadcast our 'TokenReceivedEvent' with an invalid token string.
	// This way, lobby can properly raise a connection error for the failed attempt.
	TokenReceivedEvent.Broadcast(ErrorTokenRequestFailed);
}
