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
#include "OAuthTokenJustice.generated.h"

class OAuthTokenJustice: public FJsonSerializable
{
public:
	FString AccessToken;
	FString UserRefreshToken;
	FString TokenType;
	double  ExpiresIn;
	TArray<PermissionJustice> Permissions;
	TArray<FString> Roles;
	TArray<FString> Bans;
	FString UserID;
	FString DisplayName;
	FString Namespace;
	FDateTime LastTokenRefreshUtc;
	FDateTime NextTokenRefreshUtc;
	FTimespan TokenRefreshBackoff;
	int32 JFlags;
public:
	OAuthTokenJustice():
		ExpiresIn(0),
		LastTokenRefreshUtc(FDateTime::MinValue()),
		NextTokenRefreshUtc(FDateTime::MinValue()),
		TokenRefreshBackoff(FTimespan::Zero())
	{ }

	bool ShouldRefresh()
	{
		if (NextTokenRefreshUtc > FDateTime::MinValue() && !UserRefreshToken.IsEmpty() && TokenRefreshBackoff < FTimespan::FromDays(1))
		{
			return NextTokenRefreshUtc <= FDateTime::UtcNow();
		}
		return false;
	};
	void ScheduleNormalRefresh()
	{
		NextTokenRefreshUtc = LastTokenRefreshUtc + FTimespan::FromSeconds((ExpiresIn + 1) * 0.8);
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
		NextTokenRefreshUtc = FDateTime::UtcNow() + TokenRefreshBackoff + FTimespan::FromSeconds(FMath::RandRange(1, 60));
		UE_LOG(LogJustice, Log, TEXT("FOAuthTokenJustice::ScheduelBackoffRefresh(): %s"), *GetRefreshStr());
	};

	FDateTime GetExpireTime() { return LastTokenRefreshUtc - FTimespan::FromSeconds(ExpiresIn); };
	FString GetExpireTimeStr() { return GetExpireTime().ToIso8601(); };
	FString GetRefreshStr() { return FString::Printf(TEXT("Expire=%s Refresh=%s Backoff=%.0f"), *GetExpireTimeStr(), *NextTokenRefreshUtc.ToIso8601(), TokenRefreshBackoff.GetTotalSeconds()); };
	void SetLastRefreshTimeToNow() { LastTokenRefreshUtc = FDateTime::UtcNow(); };
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
		JSON_SERIALIZE_ARRAY_SERIALIZABLE("permissions", Permissions, PermissionJustice);
		JSON_SERIALIZE_ARRAY("bans", Bans);
		JSON_SERIALIZE("user_id", UserID);
		JSON_SERIALIZE("display_name", DisplayName);
		JSON_SERIALIZE("namespace", Namespace);
	END_JSON_SERIALIZER
};

UCLASS()
class UOAuthTokenJustice : public UObject, public OAuthTokenJustice, public JusticeBaseModel<UOAuthTokenJustice, OAuthTokenJustice>
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable, Category = "OAuthTokenJustice")
		FString GetAccessToken() { return AccessToken; };

	UFUNCTION(BlueprintCallable, Category = "OAuthTokenJustice")
		FString GetRefreshToken() { return UserRefreshToken; };

	UFUNCTION(BlueprintCallable, Category = "OAuthTokenJustice")
		FString GetTokenType() { return TokenType; };

	UFUNCTION(BlueprintCallable, Category = "OAuthTokenJustice")
		FString GetUserId() { return UserID; };

	UFUNCTION(BlueprintCallable, Category = "OAuthTokenJustice")
		FString GetDisplayName() { return DisplayName; };

	UFUNCTION(BlueprintCallable, Category = "OAuthTokenJustice")
		FString GetNamespace() { return Namespace; };

	UFUNCTION(BlueprintCallable, Category = "OAuthTokenJustice")
		FString GetRoles(int32 Index) { return Roles[Index]; };
};