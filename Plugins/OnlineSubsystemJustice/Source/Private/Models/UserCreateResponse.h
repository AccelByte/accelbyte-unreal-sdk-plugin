// Copyright (c) 2017-2018 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "OnlineJsonSerializer.h"
#include "UserCreateResponse.generated.h"

UCLASS()
class UUserCreateResponse : public UObject, public FJsonSerializable
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

	BEGIN_ONLINE_JSON_SERIALIZER
		ONLINE_JSON_SERIALIZE("Namespace", Namespace);
		ONLINE_JSON_SERIALIZE("UserId", UserId);
		ONLINE_JSON_SERIALIZE("AuthType", AuthType);
		ONLINE_JSON_SERIALIZE("DisplayName", DisplayName);
		ONLINE_JSON_SERIALIZE("LoginId", LoginId);
	END_ONLINE_JSON_SERIALIZER
	
	
};
