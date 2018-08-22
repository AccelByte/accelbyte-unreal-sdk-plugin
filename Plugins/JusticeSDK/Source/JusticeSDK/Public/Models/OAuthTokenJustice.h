// Copyright (c) 2017-2018 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Serialization/JsonSerializerMacros.h"
#include "PermissionJustice.h"
#include "Misc/DateTime.h"
#include "JusticeLog.h"
#include "JusticeBaseModel.h"

struct FOAuthTokenJustice: public FJsonSerializable
{
	FString						AccessToken;
	FString						UserRefreshToken;
	FString						TokenType;
	double						ExpiresIn;
	TArray<FPermissionJustice>	Permissions;
	TArray<FString>				Roles;
	TArray<FString>				Bans;
	FString						UserID;
	FString						DisplayName;
	FString						Namespace;
	FDateTime					LastTokenRefreshUTC;
	FDateTime					NextTokenRefreshUTC;
	FTimespan					TokenRefreshBackoff;
	int32						JFlags;

    FOAuthTokenJustice():
		ExpiresIn(0),
		LastTokenRefreshUTC(FDateTime::MinValue()),
		NextTokenRefreshUTC(FDateTime::MinValue()),
		TokenRefreshBackoff(FTimespan::Zero())
	{ }

	bool ShouldRefresh()
	{
		if (NextTokenRefreshUTC > FDateTime::MinValue() && !UserRefreshToken.IsEmpty() && TokenRefreshBackoff < FTimespan::FromDays(1))
		{
			return NextTokenRefreshUTC <= FDateTime::UtcNow();
		}
		return false;
	};
	void ScheduleNormalRefresh()
	{
		NextTokenRefreshUTC = LastTokenRefreshUTC + FTimespan::FromSeconds((ExpiresIn + 1) * 0.8);
		TokenRefreshBackoff = FTimespan::Zero();
		UE_LOG(LogJustice, Log, TEXT("FOAuthTokenJustice::ScheduleNormalRefresh(): %s"), *GetRefreshStr());
	};
	void ScheduleBackoffRefresh()
	{
		if (TokenRefreshBackoff.IsZero())
		{
			TokenRefreshBackoff = FTimespan::FromSeconds(10);
		}
		TokenRefreshBackoff *= 2;
		NextTokenRefreshUTC = FDateTime::UtcNow() + TokenRefreshBackoff + FTimespan::FromSeconds(FMath::RandRange(1, 60));
		UE_LOG(LogJustice, Log, TEXT("FOAuthTokenJustice::ScheduelBackoffRefresh(): %s"), *GetRefreshStr());
	};

	FDateTime GetExpireTime() { return LastTokenRefreshUTC - FTimespan::FromSeconds(ExpiresIn); };
	FString GetExpireTimeStr() { return GetExpireTime().ToIso8601(); };
	FString GetRefreshStr() { return FString::Printf(TEXT("Expire=%s Refresh=%s Backoff=%.0f"), *GetExpireTimeStr(), *NextTokenRefreshUTC.ToIso8601(), TokenRefreshBackoff.GetTotalSeconds()); };
	void SetLastRefreshTimeToNow() { LastTokenRefreshUTC = FDateTime::UtcNow(); };
	FString GetAccessToken() { return AccessToken; };
	FString GetRefreshToken() { return UserRefreshToken; };
	FString GetTokenType() { return TokenType; };
	FString GetUserId() { return UserID; };
	FString GetDisplayName() { return DisplayName; };
	FString GetNamespace() { return Namespace; };
	FString GetRoles(int32 Index) { return Roles[Index]; };

	BEGIN_JSON_SERIALIZER
		JSON_SERIALIZE("access_token", AccessToken);
		JSON_SERIALIZE("refresh_token", UserRefreshToken);
		JSON_SERIALIZE("expires_in", ExpiresIn);
		JSON_SERIALIZE("token_type", TokenType);
		JSON_SERIALIZE_ARRAY("roles", Roles);		
		JSON_SERIALIZE_ARRAY_SERIALIZABLE("permissions", Permissions, FPermissionJustice);
		JSON_SERIALIZE_ARRAY("bans", Bans);
		JSON_SERIALIZE("user_id", UserID);
		JSON_SERIALIZE("display_name", DisplayName);
		JSON_SERIALIZE("namespace", Namespace);
	END_JSON_SERIALIZER
};