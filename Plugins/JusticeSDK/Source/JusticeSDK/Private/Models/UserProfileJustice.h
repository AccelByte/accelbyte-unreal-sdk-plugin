// Copyright (c) 2017-2018 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Serialization/JsonSerializerMacros.h"
#include "UserProfileJustice.generated.h"

/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class UUserProfileJustice : public UObject, public FJsonSerializable
{
	GENERATED_BODY()
	
private:
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

	UFUNCTION(BlueprintCallable, Category = "UserProfileJustice")
	FString GetUserId()
	{
		return UserId;
	};

	UFUNCTION(BlueprintCallable, Category = "UserProfileJustice")
	FString GetDisplayName()
	{
		return DisplayName;
	}

	UFUNCTION(BlueprintCallable, Category = "UserProfileJustice")
	FString GetFirstName()
	{
		return FirstName;
	}

	UFUNCTION(BlueprintCallable, Category = "UserProfileJustice")
	FString GetLastName()
	{
		return LastName;
	}

	UFUNCTION(BlueprintCallable, Category = "UserProfileJustice")
	FString GetAvatarSmallUrl()
	{
		return AvatarSmallUrl;
	}

	UFUNCTION(BlueprintCallable, Category = "UserProfileJustice")
	FString GetAvatarLargeUrl()
	{
		return AvatarLargeUrl;
	}

	UFUNCTION(BlueprintCallable, Category = "UserProfileJustice")
	FString GetAvatarUrl()
	{
		return AvatarUrl;
	}

	UFUNCTION(BlueprintCallable, Category = "UserProfileJustice")
	FString GetEmail()
	{
		return Email;
	}

	UFUNCTION(BlueprintCallable, Category = "UserProfileJustice")
	FString GetStatus()
	{
		return Status;
	}

	UFUNCTION(BlueprintCallable, Category = "UserProfileJustice")
	FString GetNamespace()
	{
		return Namespace;
	}
	
	
};
