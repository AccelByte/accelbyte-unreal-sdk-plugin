// Copyright (c) 2017-2018 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "OnlineJsonSerializer.h"
#include "OnlineSubsystem.h"
#include "PermissionJustice.h"

class FOAuthTokenJustice : public FOnlineJsonSerializable
{

public:

	FOAuthTokenJustice() :
		ExpiresIn(0),
		LastTokenRefreshUtc(FDateTime::MinValue()),
		NextTokenRefreshUtc(FDateTime::MinValue()),
		TokenRefreshBackoff(FTimespan::Zero())
		
	{ }

	bool ShouldRefresh()
	{
		if (NextTokenRefreshUtc > FDateTime::MinValue() && !RefreshToken.IsEmpty() && TokenRefreshBackoff < FTimespan::FromDays(1))
		{
			return NextTokenRefreshUtc <= FDateTime::UtcNow();
		}
		return false;
	};
	void ScheduleNormalRefresh()
	{
		NextTokenRefreshUtc = LastTokenRefreshUtc + FTimespan::FromSeconds((ExpiresIn + 1) / 2);
		TokenRefreshBackoff = FTimespan::Zero();
		UE_LOG_ONLINE(VeryVerbose, TEXT("FOAuthTokenJustice::ScheduleNormalRefresh(): %s"), *GetRefreshStr());
	};
	void ScheduleBackoffRefresh()
	{
		if (TokenRefreshBackoff.IsZero())
		{
			TokenRefreshBackoff = FTimespan::FromSeconds(10);
		}
		TokenRefreshBackoff *= 2;
		NextTokenRefreshUtc = FDateTime::UtcNow() + TokenRefreshBackoff + FTimespan::FromSeconds(FMath::RandRange(1, 60));
		UE_LOG_ONLINE(VeryVerbose, TEXT("FOAuthTokenJustice::ScheduelBackoffRefresh(): %s"), *GetRefreshStr());
	};

	FDateTime GetExpireTime() { return LastTokenRefreshUtc - FTimespan::FromSeconds(ExpiresIn); };
	FString GetExpireTimeStr() { return GetExpireTime().ToIso8601(); };
	FString GetRefreshStr() { return FString::Printf(TEXT("Expire=%s Refresh=%s Backoff=%.0f"), *GetExpireTimeStr(), *NextTokenRefreshUtc.ToIso8601(), TokenRefreshBackoff.GetTotalSeconds()); };
	void SetLastRefreshTimeToNow() { LastTokenRefreshUtc = FDateTime::UtcNow(); };

	FString AccessToken;
	FString RefreshToken;
	FString TokenType;
	double  ExpiresIn;
	TArray<FPermissionJustice> Permissions;
	TArray<FString> Roles;

	FString UserId;
	FString DisplayName;
	FString Namespace;

	BEGIN_ONLINE_JSON_SERIALIZER
		ONLINE_JSON_SERIALIZE("access_token", AccessToken);
		ONLINE_JSON_SERIALIZE("refresh_token", RefreshToken);
		ONLINE_JSON_SERIALIZE("token_type", TokenType);
		ONLINE_JSON_SERIALIZE("expires_in", ExpiresIn);
		ONLINE_JSON_SERIALIZE("user_id", UserId);
		ONLINE_JSON_SERIALIZE("display_name", DisplayName);
		ONLINE_JSON_SERIALIZE("namespace", Namespace);
		ONLINE_JSON_SERIALIZE_ARRAY("roles", Roles);
	END_ONLINE_JSON_SERIALIZER


	// Tracking when to refresh the token
	FDateTime LastTokenRefreshUtc;
	FDateTime NextTokenRefreshUtc;
	FTimespan TokenRefreshBackoff;

};

