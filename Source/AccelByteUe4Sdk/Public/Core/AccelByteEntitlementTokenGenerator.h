#pragma once

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
	
class ACCELBYTEUE4SDK_API FAccelByteEntitlementTokenGenerator : public IAccelByteTokenGenerator
{
	DECLARE_DERIVED_EVENT(FAccelByteEntitlementTokenGenerator, IAccelByteTokenGenerator::FTokenReceivedEvent, FTokenReceivedEvent);

public:
	FAccelByteEntitlementTokenGenerator(const FTokenGeneratorParams& Params);
	virtual ~FAccelByteEntitlementTokenGenerator() override;
	
	virtual void RequestToken() override;
	virtual bool IsTokenValid() const override;
	virtual FTokenReceivedEvent& OnTokenReceived() override;

protected:
	FTokenReceivedEvent TokenReceivedEvent;

private:
	FDateTime LastSuccessDateTime;
	FDateTime ExpiredDateTime;

	TArray<FString> Skus;
	TArray<FString> ItemIds;
	TArray<FString> AppIds;
};	
}
