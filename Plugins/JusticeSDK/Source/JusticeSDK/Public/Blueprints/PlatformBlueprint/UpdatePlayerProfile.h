// Copyright (c) 2017-2018 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "Models/OAuthTokenJustice.h"
#include "UpdatePlayerProfile.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FUpdatePlayerProfileOutputPin, FString, ErrorStr);

UCLASS()
class JUSTICESDK_API UAsyncUpdatePlayerProfile : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable)
		FUpdatePlayerProfileOutputPin OnSuccess;

	UPROPERTY(BlueprintAssignable)
		FUpdatePlayerProfileOutputPin OnFailed;

	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true", Category = "Platform"))
		static UAsyncUpdatePlayerProfile* UpdatePlayerProfile(FString DisplayName, FString FirstName, FString LastName, FString Country, FString AvatarUrl, FString Language, FString Timezone, FString DateOfBirth, TMap<FString, FString> CustomAttributes);

	virtual void Activate() override;

private:
    FString DisplayName;
    FString FirstName;
    FString LastName;
    FString Country;
    FString AvatarUrl;
	FString Language;
	FString Timezone;
	FString DateOfBirth;
	TMap<FString, FString> CustomAttributes;
};