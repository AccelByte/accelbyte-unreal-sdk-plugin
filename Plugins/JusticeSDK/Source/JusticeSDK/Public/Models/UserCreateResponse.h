// Copyright (c) 2017-2018 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Serialization/JsonSerializerMacros.h"
#include "JusticeBaseModel.h"
#include "UserCreateResponse.generated.h"


class UserCreateResponse: public FJsonSerializable
{
public:
	FString Namespace;
	FString UserID;
	FString AuthType;
	FString DisplayName;
	FString LoginID;

	BEGIN_JSON_SERIALIZER
		JSON_SERIALIZE("Namespace", Namespace);
		JSON_SERIALIZE("UserId", UserID);
		JSON_SERIALIZE("AuthType", AuthType);
		JSON_SERIALIZE("DisplayName", DisplayName);
		JSON_SERIALIZE("LoginId", LoginID);
	END_JSON_SERIALIZER
};


UCLASS()
class UUserCreateResponse : public UObject, public UserCreateResponse, public JusticeBaseModel<UUserCreateResponse, UserCreateResponse>
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UserCreateResponse")
		FString Namespace;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UserCreateResponse")
		FString UserID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UserCreateResponse")
		FString AuthType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UserCreateResponse")
		FString DisplayName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UserCreateResponse")
		FString LoginID;
};