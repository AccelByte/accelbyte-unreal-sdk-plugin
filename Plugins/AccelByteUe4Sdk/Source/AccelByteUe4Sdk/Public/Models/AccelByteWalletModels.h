// Copyright (c) 2018 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "CoreMinimal.h"
#include "AccelByteWalletModels.generated.h"

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsWalletInfo
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Wallet | Models | WalletInfo")
		FString Id;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Wallet | Models | WalletInfo")
		FString Namespace;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Wallet | Models | WalletInfo")
		FString UserId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Wallet | Models | WalletInfo")
		FString CurrencyCode;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Wallet | Models | WalletInfo")
		FString CurrencySymbol;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Wallet | Models | WalletInfo")
		int32 Balance;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Wallet | Models | WalletInfo")
		FString CreatedAt;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Wallet | Models | WalletInfo")
		FString UpdatedAt;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Wallet | Models | WalletInfo")
		FString Status;
};

