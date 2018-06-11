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
#include "OAuthTokenJustice.generated.h"

class OAuthTokenJustice: public FJsonSerializable
{
public:
	FString AccessToken;
	FString UserRefreshToken;
	FString TokenType;
	double  ExpiresIn;
	TArray<FPermissionJustice> Permissions;
	TArray<FString> Roles;
	TArray<FString> Bans;
	FString UserId;
	FString DisplayName;
	FString Namespace;
	FDateTime LastTokenRefreshUtc;
	FDateTime NextTokenRefreshUtc;
	FTimespan TokenRefreshBackoff;
	int JFlags;
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
		//DEBUG
		//NextTokenRefreshUtc = FDateTime::UtcNow() + FTimespan::FromSeconds(30);
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
	FString GetUserId() { return UserId; };
	FString GetDisplayName() { return DisplayName; };
	FString GetNamespace() { return Namespace; };
	FString GetRoles(int Index) { return Roles[Index]; };

	BEGIN_JSON_SERIALIZER
		JSON_SERIALIZE("access_token", AccessToken);
		JSON_SERIALIZE("refresh_token", UserRefreshToken);
		JSON_SERIALIZE("expires_in", ExpiresIn);
		JSON_SERIALIZE("token_type", TokenType);
		JSON_SERIALIZE_ARRAY("roles", Roles);
		//JSON_SERIALIZE_ARRAY("permissions", Roles);
		JSON_SERIALIZE_ARRAY("bans", Bans);
		JSON_SERIALIZE("user_id", UserId);
		JSON_SERIALIZE("display_name", DisplayName);
		JSON_SERIALIZE("namespace", Namespace);
		
	END_JSON_SERIALIZER
};


UCLASS()
class UOAuthTokenJustice : public UObject, public OAuthTokenJustice
{
	GENERATED_BODY()
public:

	UOAuthTokenJustice(const class FObjectInitializer& ObjectInitializer):
		Super(ObjectInitializer)		
	{ }

	void FromParent(OAuthTokenJustice* parent)
	{
		AccessToken = parent->AccessToken;
		UserRefreshToken = parent->UserRefreshToken;
		TokenType = parent->TokenType;
		ExpiresIn = parent->ExpiresIn;
		Permissions = parent->Permissions;
		Roles = parent->Roles;
		UserId = parent->UserId;
		DisplayName = parent->DisplayName;
		Namespace = parent->Namespace;
		LastTokenRefreshUtc = parent->LastTokenRefreshUtc;
		NextTokenRefreshUtc = parent->NextTokenRefreshUtc;		
	}

	static UOAuthTokenJustice* Deserialize(FString json)
	{
		UOAuthTokenJustice* newToken = NewObject<UOAuthTokenJustice>();
		if (newToken->FromJson(json))
		{
			return newToken;
		}
		return nullptr;

	}



	// Function Getter
	UFUNCTION(BlueprintCallable, Category = "OAuthTokenJustice")
		FString GetAccessToken() { return AccessToken; };

	UFUNCTION(BlueprintCallable, Category = "OAuthTokenJustice")
		FString GetRefreshToken() { return UserRefreshToken; };

	UFUNCTION(BlueprintCallable, Category = "OAuthTokenJustice")
		FString GetTokenType() { return TokenType; };

	//UFUNCTION(BlueprintCallable, Category = "OAuthTokenJustice")
	//	double GetExpiresIn() { return ExpiresIn; };

	UFUNCTION(BlueprintCallable, Category = "OAuthTokenJustice")
		FString GetUserId() { return UserId; };

	UFUNCTION(BlueprintCallable, Category = "OAuthTokenJustice")
		FString GetDisplayName() { return DisplayName; };

	UFUNCTION(BlueprintCallable, Category = "OAuthTokenJustice")
		FString GetNamespace() { return Namespace; };

	UFUNCTION(BlueprintCallable, Category = "OAuthTokenJustice")
		FString GetRoles(int Index) { return Roles[Index]; };
};

