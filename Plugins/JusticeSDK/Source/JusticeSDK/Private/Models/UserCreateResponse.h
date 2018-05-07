// Copyright (c) 2017-2018 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "FUserCreateResponse.h"
#include "UserCreateResponse.generated.h"

UCLASS()
class UUserCreateResponse : public UObject
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UserCreateResponse")
		FString Namespace;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UserCreateResponse")
		FString UserId;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UserCreateResponse")
		FString AuthType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UserCreateResponse")
		FString DisplayName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UserCreateResponse")
		FString LoginId;

	void LoadFromStruct(FUserCreateResponse item) 
	{
		Namespace = item.Namespace;
		UserId = item.UserId;
		AuthType = item.AuthType;
		DisplayName = item.DisplayName;
		LoginId = item.LoginId;
	}

};