// Copyright (c) 2017-2018 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "OnlineJsonSerializer.h"
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
	BEGIN_ONLINE_JSON_SERIALIZER
		ONLINE_JSON_SERIALIZE("userId", UserId);
		ONLINE_JSON_SERIALIZE("namespace", Namespace);
		ONLINE_JSON_SERIALIZE("displayName", DisplayName);
		ONLINE_JSON_SERIALIZE("firstName", FirstName);
		ONLINE_JSON_SERIALIZE("lastName", LastName);
		ONLINE_JSON_SERIALIZE("country", Country);		
		ONLINE_JSON_SERIALIZE("avatarSmallUrl", AvatarSmallUrl);
		ONLINE_JSON_SERIALIZE("avatarUrl", AvatarUrl);
		ONLINE_JSON_SERIALIZE("avatarLargeUrl", AvatarLargeUrl);
		ONLINE_JSON_SERIALIZE("email", Email);
		ONLINE_JSON_SERIALIZE("status", Status);
		ONLINE_JSON_SERIALIZE_MAP("customAttributes", CustomAttributes);
	END_ONLINE_JSON_SERIALIZER

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
