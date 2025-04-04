// Copyright (c) 2021 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "CoreMinimal.h"
#include "Core/AccelByteError.h"
#include "Models/AccelByteOauth2Models.h"
#include "Core/AccelByteServerApiClient.h"
#include "ABServerOauth2.generated.h"

DECLARE_DYNAMIC_DELEGATE_OneParam(FDJwkSetDelegate, FJwkSet, Response);

UCLASS(Blueprintable, BlueprintType)
class UABServerOauth2 final : public UObject
{
	GENERATED_BODY()
public:
	void SetServerApiClient(AccelByte::FServerApiClientPtr const& NewServerApiClientPtr);

public:
	UFUNCTION(BlueprintCallable, Category = "AccelByte | Server | OAuth2 | Api")
	void LoginWithClientCredentials(FDHandler OnSuccess, FDErrorHandler OnError);

	UFUNCTION(BlueprintCallable, Category = "AccelByte | Server | OAuth2 | Api")
	void GetJwks(FDJwkSetDelegate OnSuccess, FDErrorHandler OnError);

	UFUNCTION(BlueprintCallable, Category = "AccelByte | Server | OAuth2 | Api")
	void ForgetAllCredentials();

private:
	AccelByte::FServerApiClientPtr ApiClientPtr;
};
