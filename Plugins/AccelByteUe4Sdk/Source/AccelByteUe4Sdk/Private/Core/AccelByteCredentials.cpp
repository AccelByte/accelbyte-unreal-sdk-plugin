// Copyright (c) 2018 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "AccelByteCredentials.h"
#include "AccelByteOauth2Models.h"

namespace AccelByte
{

Credentials::Credentials()
{
	OnRefreshSuccess.BindLambda([this]()
	{
		RefreshAttempt = 0;
		UE_LOG(LogTemp, Log, TEXT("OnRefreshSuccess.BindLambda"));
		FTicker::GetCoreTicker().AddTicker(Credentials::Get().GetRefreshTokenTickerDelegate(), Credentials::Get().GetRefreshTokenDuration());
	});

	RefreshTokenTickerDelegate = FTickerDelegate::CreateRaw(this, &AccelByte::Credentials::RefreshTokenTick);
}

Credentials::~Credentials()
{
}

Credentials& Credentials::Get()
{
    // Deferred/lazy initialization
    // Thread-safe in C++11
    static Credentials Instance;
    return Instance;
}

void Credentials::ForgetAll()
{
	UserAccessToken = FString();
	UserRefreshToken = FString();
	UserAccessTokenExpirationUtc = FDateTime();
	UserNamespace = FString();
	UserId = FString();
	UserDisplayName = FString();
	FTicker::GetCoreTicker().RemoveTicker(FTicker::GetCoreTicker().AddTicker(Credentials::Get().GetRefreshTokenTickerDelegate()));
}

void Credentials::SetUserToken(const FString& AccessToken, const FString& RefreshToken, const FDateTime& ExpirationUtc, const FString& Id, const FString& DisplayName, const FString& Namespace)
{
	UserAccessToken = AccessToken;
	UserRefreshToken = RefreshToken;
	UserAccessTokenExpirationUtc = ExpirationUtc;
	UserId = Id;
	UserDisplayName = DisplayName;
	UserNamespace = Namespace;
}

void Credentials::SetClientToken(const FString& AccessToken, const FDateTime& ExpirationUtc, const FString& Namespace)
{
	ClientAccessToken = AccessToken;
	ClientNamespace = Namespace;
}

FString Credentials::GetUserAccessToken() const
{
	return UserAccessToken;
}

FString Credentials::GetUserRefreshToken() const
{
	return UserRefreshToken;
}

FDateTime Credentials::GetUserAccessTokenExpirationUtc() const
{
	return UserAccessTokenExpirationUtc;
}

FString Credentials::GetUserNamespace() const
{
	return UserNamespace;
}

float Credentials::GetRefreshTokenDuration() const
{
	auto Duration = Credentials::Get().GetUserAccessTokenExpirationUtc() - FDateTime::UtcNow();
	float Result = (Duration.GetTotalSeconds() * 0.8f) + (FMath::RandRange(1, 60));	
	return Result;
}

FTickerDelegate & Credentials::GetRefreshTokenTickerDelegate()
{
	return RefreshTokenTickerDelegate;
}

bool Credentials::RefreshTokenTick(float NextTickInSecond)
{
	float NextRefreshIn = FMath::Pow(2, RefreshAttempt);
	if (NextRefreshIn < 60.0f) // next retry is not more than 60 second
	{
		RefreshAttempt += 1;
		UE_LOG(LogTemp, Log, TEXT("Retry, attempt number: %d, retrying in %.4f second"), RefreshAttempt, NextRefreshIn);
		AccelByte::Api::UserAuthentication::RefreshToken(Credentials::GetOnRefreshSuccess(),
			FErrorHandler::CreateLambda([&, NextRefreshIn](int32 Code, FString Message)
		{
			UE_LOG(LogTemp, Log, TEXT("Retrying..."));
			FTicker::GetCoreTicker().AddTicker(Credentials::Get().GetRefreshTokenTickerDelegate(), NextRefreshIn);
		}));
	}
	return false;
}

AccelByte::Api::UserAuthentication::FRefreshTokenSuccess Credentials::GetOnRefreshSuccess() const
{
	return OnRefreshSuccess;
}

FString Credentials::GetUserId() const
{
	return UserId;
}

FString Credentials::GetUserDisplayName() const
{
	return UserDisplayName;
}

FString Credentials::GetClientAccessToken() const
{
	return ClientAccessToken;
}

FString Credentials::GetClientNamespace() const
{
	return ClientNamespace;
}

} // Namespace AccelByte

using AccelByte::Credentials;

FString UAccelByteBlueprintsCredentials::GetUserAccessToken()
{
	return Credentials::Get().GetUserAccessToken();
}

FString UAccelByteBlueprintsCredentials::GetUserRefreshToken()
{
	return Credentials::Get().GetUserRefreshToken();
}

FDateTime UAccelByteBlueprintsCredentials::GetUserAccessTokenExpirationUtc()
{
	return Credentials::Get().GetUserAccessTokenExpirationUtc();
}

FString UAccelByteBlueprintsCredentials::GetUserId()
{
	return Credentials::Get().GetUserId();
}

FString UAccelByteBlueprintsCredentials::GetUserDisplayName()
{
	return Credentials::Get().GetUserDisplayName();
}

FString UAccelByteBlueprintsCredentials::GetUserNamespace()
{
	return Credentials::Get().GetUserNamespace();
}
