// Copyright (c) 2019 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Models/AccelByteEcommerceModels.h"
#include "AccelByteEntitlementBlueprints.generated.h"

UCLASS(Blueprintable, BlueprintType)
class UAccelByteBlueprintsEntitlement : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	DECLARE_DYNAMIC_DELEGATE_TwoParams(FBlueprintErrorHandler, int32, ErrorCode, const FString&, ErrorMessage);

	DECLARE_DYNAMIC_DELEGATE_OneParam(FQueryUserEntitlementSuccess, const FAccelByteModelsEntitlementPagingSlicedResult&, Result);
	UFUNCTION(BlueprintCallable, Category = "AccelByte | Entitlement | Api ")
		static void QueryUserEntitlement(const FString & EntitlementName, const FString & ItemId, int32 Page, int32 Size, const FQueryUserEntitlementSuccess& OnSuccess, const FBlueprintErrorHandler& OnError, EAccelByteEntitlementClass EntitlementClass, EAccelByteAppType AppType);

};