// Copyright (c) 2017-2018 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "Models/OAuthTokenJustice.h"
#include "RequestCurrentPlayerProfile.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FRequestProfileOutputPin, const UUserProfileJustice*, UserProfile);

UCLASS()
class JUSTICESDK_API UAsyncRequestCurrentPlayerProfile : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable)
		FRequestProfileOutputPin OnSuccess;

	UPROPERTY(BlueprintAssignable)
		FRequestProfileOutputPin OnFailed;

	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true", Category = "Platform"))
		static UAsyncRequestCurrentPlayerProfile* RequestCurrentPlayerProfile();

	virtual void Activate() override;
};