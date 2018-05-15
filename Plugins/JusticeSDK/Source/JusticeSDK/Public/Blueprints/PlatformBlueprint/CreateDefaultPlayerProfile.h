// Copyright (c) 2017-2018 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "Models/OAuthTokenJustice.h"
#include "CreateDefaultPlayerProfile.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FCreateDefaultPlayerProfileOutputPin);

UCLASS()
class JUSTICESDK_API UAsyncCreateDefaultPlayerProfile : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable)
		FCreateDefaultPlayerProfileOutputPin OnSuccess;

	UPROPERTY(BlueprintAssignable)
		FCreateDefaultPlayerProfileOutputPin OnFailed;

	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true", Category = "Platform"))
		static UAsyncCreateDefaultPlayerProfile* CreateDefaultPlayerProfile(FString Email, FString DisplayName);

	virtual void Activate() override;
private:
    FString Email;
    FString DisplayName;
};