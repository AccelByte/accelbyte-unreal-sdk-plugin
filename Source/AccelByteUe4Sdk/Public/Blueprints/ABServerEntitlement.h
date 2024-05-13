// Copyright (c) 2024 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "CoreMinimal.h"
#include "Core/AccelByteError.h"
#include "Core/AccelByteMultiRegistry.h"
#include "ABServerEntitlement.generated.h"

#pragma region MODEL_AND_DELEGATE_FOR_REQUEST_RESPONSE

DECLARE_DYNAMIC_DELEGATE_OneParam(FDAccelByteModelsEntitlementHistoryResponse, TArray<FAccelByteModelsUserEntitlementHistory>, Response);

#pragma endregion 

UCLASS(Blueprintable, BlueprintType)
class UABServerEntitlement final : public UObject
{
	GENERATED_BODY()
public:
	void SetServerApiClient(AccelByte::FServerApiClientPtr const& NewServerApiClientPtr);

public:
	UFUNCTION(BlueprintCallable, Category = "AccelByte | Server | Entitlement | Api")
	void GetUserEntitlementHistory(FString const& UserId, FString const& EntitlementId, FDAccelByteModelsEntitlementHistoryResponse OnSuccess, FDErrorHandler OnError);

private:
	AccelByte::FServerApiClientPtr ApiClientPtr;
};
