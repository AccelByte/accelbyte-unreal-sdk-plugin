// Copyright (c) 2017-2018 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Serialization/JsonSerializerMacros.h"
#include "JusticeBaseModel.h"
#include "UserCreateResponse.generated.h"


struct FUserCreateResponse: public FJsonSerializable
{
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
class UUserCreateResponse : public UObject, public FUserCreateResponse, public FBaseModelJustice<UUserCreateResponse, FUserCreateResponse>
{
	GENERATED_BODY()
	
public:

	UFUNCTION(BlueprintPure, Category = "UserCreateResponse")
		FString GetUserID() { return UserID; };

	UFUNCTION(BlueprintPure, Category = "UserCreateResponse")
		FString GetNamespace() { return Namespace; };

	UFUNCTION(BlueprintPure, Category = "UserCreateResponse")
		FString GetAuthType() { return AuthType; };

	UFUNCTION(BlueprintPure, Category = "UserCreateResponse")
		FString GetDisplayName() { return DisplayName; };

	UFUNCTION(BlueprintPure, Category = "UserCreateResponse")
		FString GetLoginID() { return LoginID; };

};