// Copyright (c) 2017-2018 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Serialization/JsonSerializerMacros.h"
#include "UserProfileInfo.generated.h"

class UserProfileInfo : public FJsonSerializable
{
public:
	UserProfileInfo() {};

	FString UserId;
	FString Namespace;
	FString DisplayName;
	FString FirstName;
	FString LastName;
	FString Country;
	FString AvatarSmallUrl;
	FString AvatarUrl;
	FString AvatarLargeUrl;
	FString Email;
	FString Status;
	TMap<FString, FString> CustomAttributes;

public:
	BEGIN_JSON_SERIALIZER
		JSON_SERIALIZE("userId", UserId);
		JSON_SERIALIZE("namespace", Namespace);
		JSON_SERIALIZE("displayName", DisplayName);
		JSON_SERIALIZE("firstName", FirstName);
		JSON_SERIALIZE("lastName", LastName);
		JSON_SERIALIZE("country", Country);		
		JSON_SERIALIZE("avatarSmallUrl", AvatarSmallUrl);
		JSON_SERIALIZE("avatarUrl", AvatarUrl);
		JSON_SERIALIZE("avatarLargeUrl", AvatarLargeUrl);
		JSON_SERIALIZE("email", Email);
		JSON_SERIALIZE("status", Status);
		JSON_SERIALIZE_MAP("customAttributes", CustomAttributes);
	END_JSON_SERIALIZER
};


UCLASS(Blueprintable, BlueprintType)
class UUserProfileJustice : public UObject, public UserProfileInfo
{
	GENERATED_BODY()
public:
	void FromUserProfileInfo(UserProfileInfo info);

	UFUNCTION(BlueprintPure, Category = "UserProfileJustice")
	FString GetUserId()
	{
		return UserId;
	};

	UFUNCTION(BlueprintPure, Category = "UserProfileJustice")
	FString GetDisplayName()
	{
		return DisplayName;
	}

	UFUNCTION(BlueprintPure, Category = "UserProfileJustice")
	FString GetFirstName()
	{
		return FirstName;
	}

	UFUNCTION(BlueprintPure, Category = "UserProfileJustice")
	FString GetLastName()
	{
		return LastName;
	}

	UFUNCTION(BlueprintPure, Category = "UserProfileJustice")
		FString GetCountry()
	{
		return Country;
	}

	UFUNCTION(BlueprintPure, Category = "UserProfileJustice")
	FString GetAvatarSmallUrl()
	{
		return AvatarSmallUrl;
	}

	UFUNCTION(BlueprintPure, Category = "UserProfileJustice")
	FString GetAvatarLargeUrl()
	{
		return AvatarLargeUrl;
	}

	UFUNCTION(BlueprintPure, Category = "UserProfileJustice")
	FString GetAvatarUrl()
	{
		return AvatarUrl;
	}

	UFUNCTION(BlueprintPure, Category = "UserProfileJustice")
	FString GetEmail()
	{
		return Email;
	}

	UFUNCTION(BlueprintPure, Category = "UserProfileJustice")
	FString GetStatus()
	{
		return Status;
	}

	UFUNCTION(BlueprintPure, Category = "UserProfileJustice")
	FString GetNamespace()
	{
		return Namespace;
	}
	
	
};
