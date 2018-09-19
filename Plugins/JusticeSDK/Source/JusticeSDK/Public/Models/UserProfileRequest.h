// Copyright (c) 2017-2018 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Serialization/JsonSerializerMacros.h"
#include "JusticeBaseModel.h"

struct FUserProfileRequest : public FJsonSerializable
{
private:
	FString                 Country;

public:
	FUserProfileRequest() {
		FString DefaultLocale = FString::Printf(TEXT(""));
	#if PLATFORM_IOS
		DefaultLocale = FIOSPlatformMisc::GetDefaultLocale();
	#elif PLATFORM_ANDROID
		DefaultLocale = FAndroidMisc::GetDefaultLocale();
	#else
		DefaultLocale = FGenericPlatformMisc::GetDefaultLocale();
	#endif
		TArray<FString> RegionID;
		DefaultLocale.ParseIntoArray(RegionID, TEXT("_"), true);
		Country = RegionID[1];
	};

	FString                 DisplayName;
	FString                 FirstName;
	FString                 LastName;
	FString                 Language;
	FString                 AvatarSmallURL;
	FString                 AvatarURL;
	FString                 AvatarLargeURL;
	FString                 Email;
	FString                 Timezone;
	FString                 DateOfBirth;
	TMap<FString, FString>  CustomAttributes;
public:
	BEGIN_JSON_SERIALIZER
		JSON_SERIALIZE("displayName", DisplayName);
		JSON_SERIALIZE("firstName", FirstName);
		JSON_SERIALIZE("lastName", LastName);
		JSON_SERIALIZE("country", Country);		
		JSON_SERIALIZE("language", Language);
		JSON_SERIALIZE("avatarSmallUrl", AvatarSmallURL);
		JSON_SERIALIZE("avatarUrl", AvatarURL);
		JSON_SERIALIZE("avatarLargeUrl", AvatarLargeURL);
		JSON_SERIALIZE("email", Email);
		JSON_SERIALIZE("timeZone", Timezone);
		JSON_SERIALIZE("dateOfBirth", DateOfBirth);
		JSON_SERIALIZE_MAP("customAttributes", CustomAttributes);
	END_JSON_SERIALIZER
};