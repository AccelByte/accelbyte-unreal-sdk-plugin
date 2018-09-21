// Copyright (c) 2017-2018 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "CoreMinimal.h"
#include "AccelByteModelsWallet.generated.h"

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsWalletInfo
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Models | Wallet | WalletInfo")
		FString Id;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Models | Wallet | WalletInfo")
		FString Namespace;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Models | Wallet | WalletInfo")
		FString UserId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Models | Wallet | WalletInfo")
		FString CurrencyCode;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Models | Wallet | WalletInfo")
		FString CurrencySymbol;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Models | Wallet | WalletInfo")
		int32 Balance;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Models | Wallet | WalletInfo")
		FString CreatedAt;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Models | Wallet | WalletInfo")
		FString UpdatedAt;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Models | Wallet | WalletInfo")
		FString Status;
};

