#pragma once

#include "CoreMinimal.h"
#include "AccelByteUe4Sdk/Public/Core/AccelByteError.h"
#include "AccelByteUe4Sdk/Public/Models/AccelByteUserModels.h"
#include "Core/AccelByteMultiRegistry.h"
#include "AccelByteBPUser.generated.h"

using namespace AccelByte;
using namespace AccelByte::Api;


DECLARE_DYNAMIC_DELEGATE_OneParam(FDRegisterResponseHandler, FRegisterResponse, Request);


UCLASS(Blueprintable, BlueprintType)
class UUser final : public UObject
{
	GENERATED_BODY()
public:
	void SetApiClient(FApiClientPtr NewApiClientPtr);

	UFUNCTION(BlueprintCallable)
	void Registerv3(
		FRegisterRequestv3 const& RegisterRequest,
		FDRegisterResponseHandler const& OnSuccess,
		FDErrorHandler const& OnError);

	UFUNCTION(BlueprintCallable)
	void LoginWithUsername(
		FString const& Username,
		FString const& Password,
		FDHandler const& OnSuccess,
		FDErrorHandler const& OnError);

	UFUNCTION(BlueprintCallable)
	void LoginWithOtherPlatform(
		EAccelBytePlatformType PlatformType,
		FString const& PlatformToken,
		FDHandler const& OnSuccess,
		FDErrorHandler const& OnError);


private:
	FApiClientPtr ApiClientPtr;
};