// Copyright (c) 2017-2018 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Serialization/JsonSerializerMacros.h"
#include "JusticeBaseModel.h"
#include "UserProfileInfo.generated.h"

struct FUserProfileInfo : public FJsonSerializable
{
public:
	FUserProfileInfo() {};

	FString					UserID;
	FString					Namespace;
	FString					DisplayName;
	FString					FirstName;
	FString					LastName;
	FString					Country;
	FString					AvatarSmallURL;
	FString					AvatarURL;
	FString					AvatarLargeURL;
	FString					Email;
	FString					Status;
	FString					Language;
	FString					Timezone;
	FString					DateOfBirth;
	TMap<FString, FString>	CustomAttributes;
public:
	BEGIN_JSON_SERIALIZER
		JSON_SERIALIZE("userId", UserID);
		JSON_SERIALIZE("namespace", Namespace);
		JSON_SERIALIZE("displayName", DisplayName);
		JSON_SERIALIZE("firstName", FirstName);
		JSON_SERIALIZE("lastName", LastName);
		JSON_SERIALIZE("country", Country);		
		JSON_SERIALIZE("avatarSmallUrl", AvatarSmallURL);
		JSON_SERIALIZE("avatarUrl", AvatarURL);
		JSON_SERIALIZE("avatarLargeUrl", AvatarLargeURL);
		JSON_SERIALIZE("email", Email);
		JSON_SERIALIZE("status", Status);
		JSON_SERIALIZE("language", Language);
		JSON_SERIALIZE("timeZone", Timezone);
		JSON_SERIALIZE("dateOfBirth", DateOfBirth);
		JSON_SERIALIZE_MAP("customAttributes", CustomAttributes);
	END_JSON_SERIALIZER
};



class UserProfileInfoUpdate : public FJsonSerializable
{
public:
	UserProfileInfoUpdate() {};

	FString					DisplayName;
	FString					FirstName;
	FString					LastName;
	FString					AvatarSmallURL;
	FString					AvatarURL;
	FString					AvatarLargeURL;
	FString					Email;
	FString					Language;
	FString					Timezone;
	FString					DateOfBirth;
	TMap<FString, FString>	CustomAttributes;
	FString					Country;
	FString					Status;

public:
	BEGIN_JSON_SERIALIZER
		JSON_SERIALIZE("displayName", DisplayName);
		JSON_SERIALIZE("firstName", FirstName);
		JSON_SERIALIZE("lastName", LastName);		
		JSON_SERIALIZE("avatarSmallUrl", AvatarSmallURL);
		JSON_SERIALIZE("avatarUrl", AvatarURL);
		JSON_SERIALIZE("avatarLargeUrl", AvatarLargeURL);
		JSON_SERIALIZE("email", Email);
		JSON_SERIALIZE("language", Language);
		JSON_SERIALIZE("timeZone", Timezone);
		JSON_SERIALIZE("dateOfBirth", DateOfBirth);
		JSON_SERIALIZE_MAP("customAttributes", CustomAttributes);
		JSON_SERIALIZE("country", Country);
		JSON_SERIALIZE("status", Status);
	END_JSON_SERIALIZER
};

UCLASS(Blueprintable, BlueprintType)
class UUserProfileJustice : public UObject, public FUserProfileInfo, public FBaseModelJustice<UUserProfileJustice, FUserProfileInfo>
{
	GENERATED_BODY()
public:

	UFUNCTION(BlueprintPure, Category = "UserProfileJustice")
		FString GetUserId()	{ return UserID; }

	UFUNCTION(BlueprintPure, Category = "UserProfileJustice")
		FString GetDisplayName() { return DisplayName; }

	UFUNCTION(BlueprintPure, Category = "UserProfileJustice")
		FString GetFirstName() { return FirstName; }

	UFUNCTION(BlueprintPure, Category = "UserProfileJustice")
		FString GetLastName() {	return LastName; }

	UFUNCTION(BlueprintPure, Category = "UserProfileJustice")
		FString GetCountry() {	return Country;	}

	UFUNCTION(BlueprintPure, Category = "UserProfileJustice")
		FString GetAvatarSmallUrl()	{ return AvatarSmallURL; }

	UFUNCTION(BlueprintPure, Category = "UserProfileJustice")
		FString GetAvatarLargeUrl()	{ return AvatarLargeURL; }

	UFUNCTION(BlueprintPure, Category = "UserProfileJustice")
		FString GetAvatarUrl() { return AvatarURL; }

	UFUNCTION(BlueprintPure, Category = "UserProfileJustice")
		FString GetEmail() { return Email; }

	UFUNCTION(BlueprintPure, Category = "UserProfileJustice")
		FString GetStatus()	{ return Status; }

	UFUNCTION(BlueprintPure, Category = "UserProfileJustice")
		FString GetNamespace() { return Namespace; }

	UFUNCTION(BlueprintPure, Category = "UserProfileJustice")
		FString GetLanguage() {	return Language; }

	UFUNCTION(BlueprintPure, Category = "UserProfileJustice")
		FString GetTimezone() {	return Timezone; }

	UFUNCTION(BlueprintPure, Category = "UserProfileJustice")
		FString GetDateOfBirth() { return DateOfBirth; }
};
