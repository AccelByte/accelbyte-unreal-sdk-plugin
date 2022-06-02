// Copyright (c) 2022 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "Dom/JsonObject.h"
#include "Templates/SharedPointer.h"
#include "AccelByteJwtWrapper.generated.h"

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteJwtError
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Models | Error")
	int32 Code = -1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Models | Error")
	FString Message;
};

namespace AccelByte
{	
}
class ACCELBYTEUE4SDK_API AccelByteJwtWrapper
{
public:
	static bool TryDecode(const FString& Token, const FString& PublicKey, TSharedPtr<FJsonObject>& Output, FAccelByteJwtError& Error, const bool VerifySignature = true, const bool VerifyExpiration = true, const FString& VerifySubject = "");
	static void GetExpirationDateTime(const FString& InToken, FDateTime& OutExpiration);
};