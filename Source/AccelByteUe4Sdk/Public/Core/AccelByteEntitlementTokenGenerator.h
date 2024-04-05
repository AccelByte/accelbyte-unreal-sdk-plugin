// Copyright (c) 2022 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "AccelByteApiClient.h"
#include "AccelByteRegistry.h"
#include "IAccelByteTokenGenerator.h"

namespace AccelByte
{
	
struct FTokenGeneratorParams
{
	TArray<FString> ItemIds;
	TArray<FString> AppIds;
	TArray<FString> Skus;
};

const FString ErrorTokenRequestFailed = TEXT("token_request_failed");

class ACCELBYTEUE4SDK_API FAccelByteEntitlementTokenGenerator : public IAccelByteTokenGenerator
{
	DECLARE_DERIVED_EVENT(FAccelByteEntitlementTokenGenerator, IAccelByteTokenGenerator::FTokenReceivedEvent, FTokenReceivedEvent);

public:
	FAccelByteEntitlementTokenGenerator(const FTokenGeneratorParams& Params);
	FAccelByteEntitlementTokenGenerator(const FTokenGeneratorParams& Params, const FApiClientPtr& InApiClient);
	virtual ~FAccelByteEntitlementTokenGenerator() override = default;
	
	virtual void RequestToken() override;
	virtual bool IsTokenValid() const override;
	virtual FTokenReceivedEvent& OnTokenReceived() override;

protected:
	FTokenReceivedEvent TokenReceivedEvent{};

private:
	FDateTime LastSuccessDateTime{};
	FDateTime ExpiredDateTime{};

	TArray<FString> Skus{};
	TArray<FString> ItemIds{};
	TArray<FString> AppIds{};

	FApiClientPtr ApiClient{};

	void OnGetUserEntitlementOwnershipTokenSuccess(const FAccelByteModelsOwnershipToken& Result);
	void OnGetUserEntitlementOwnershipTokenError(int32 ErrorCode, const FString& ErrorMessage);
};
}
