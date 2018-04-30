// Copyright (c) 2017-2018 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "Runtime/Online/HTTP/Public/Http.h"
#include "Private/HTTPJustice.h"
#include "Private/AWSXRayJustice.h"
#include "Private/Models/OrderInfo.h"
#include "Private/Models/OAuthTokenJustice.h"
#include "AsyncLogin.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FLoginOutputPin, const UOAuthTokenJustice*, Token);

UCLASS()
class JUSTICESDK_API UAsyncLogin : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable)
		FLoginOutputPin OnSuccess;

	UPROPERTY(BlueprintAssignable)
		FLoginOutputPin OnFailed;

	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true", Category = "Identity"))
		static UAsyncLogin* Login(FString UserId, FString Password);

	virtual void Activate() override;

private:
	//UFUNCTION()
	//	void ExecuteOnSuccess();

	//UFUNCTION()
	//	void ExecuteOnFailed();

	void OnRequestComplete(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bSuccessful, TSharedRef<FAWSXRayJustice> RequestTrace);

private:
	FString UserId;
	FString Password;
	UOAuthTokenJustice* Token;
};
	
	
	
