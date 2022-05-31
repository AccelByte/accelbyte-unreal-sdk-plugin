// Copyright (c) 2022 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "Core/AccelByteEntitlementTokenGenerator.h"
#include "Api/AccelByteEntitlementApi.h"
#include "Models/AccelByteEcommerceModels.h"
#include "Core/AccelByteJwtWrapper.h"

DECLARE_LOG_CATEGORY_EXTERN(LogAccelByteEntitlementTokenGenerator, Log, All);
DEFINE_LOG_CATEGORY(LogAccelByteEntitlementTokenGenerator);

FAccelByteEntitlementTokenGenerator::FAccelByteEntitlementTokenGenerator(const FTokenGeneratorParams& Params) :
	Skus(Params.Skus),
	ItemIds(Params.ItemIds),
	AppIds(Params.AppIds)
{}

FAccelByteEntitlementTokenGenerator::~FAccelByteEntitlementTokenGenerator()
{
	IAccelByteTokenGenerator::~IAccelByteTokenGenerator();
}

void FAccelByteEntitlementTokenGenerator::RequestToken()
{
	if(!Token.IsEmpty() && ExpiredDateTime > FDateTime::UtcNow())
	{
		TokenReceivedEvent.Broadcast(Token);
		return;
	}

	FRegistry::Entitlement.GetUserEntitlementOwnershipTokenOnly(ItemIds, AppIds, Skus,
		THandler<FAccelByteModelsOwnershipToken>::CreateLambda([&](const FAccelByteModelsOwnershipToken& result)
		{
			Token = result.OwnershipToken;
			FDateTime TokenExpiration;
			AccelByteJwtWrapper::GetExpirationDateTime(Token, TokenExpiration);

			ExpiredDateTime = FDateTime::UtcNow() + (TokenExpiration - FDateTime::UtcNow() - FTimespan::FromSeconds(30));
			TokenReceivedEvent.Broadcast(Token);
		}), FErrorHandler::CreateLambda([](const int32 Code, const FString& Message)
		{
			UE_LOG(LogAccelByteEntitlementTokenGenerator, Warning, TEXT("Get user entitlement ownership token failed with code %d, %s"), Code, *Message);
		}));
}

bool FAccelByteEntitlementTokenGenerator::IsTokenValid() const
{
	return !Token.IsEmpty() && ExpiredDateTime >= FDateTime::UtcNow();
}

FAccelByteEntitlementTokenGenerator::FTokenReceivedEvent& FAccelByteEntitlementTokenGenerator::OnTokenReceived()
{
	return TokenReceivedEvent;
}
