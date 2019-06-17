// Copyright (c) 2018 - 2019 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "AccelByteCredentials.h"
#include "AccelByteOauth2Api.h"
#include "AccelByteOauth2Models.h"

using namespace AccelByte::Api;

DECLARE_LOG_CATEGORY_EXTERN(LogAccelByteCredentials, Log, All);
DEFINE_LOG_CATEGORY(LogAccelByteCredentials);


namespace AccelByte
{

Credentials::Credentials()
	: ClientAccessToken(TEXT(""))
	, ClientNamespace(TEXT(""))
	, UserAccessToken(TEXT(""))
	, UserRefreshToken(TEXT(""))
	, UserNamespace(TEXT(""))
	, UserId(TEXT(""))
	, UserDisplayName(TEXT(""))
	, UserRefreshTime(0.0)
	, UserExpiredTime(0.0)
	, UserRefreshBackoff(0.0)
	, UserTokenState(ETokenState::Invalid)
{
}

void Credentials::ForgetAll()
{
	UserAccessToken = FString();
	UserRefreshToken = FString();
	UserNamespace = FString();
	UserId = FString();
	UserDisplayName = FString();

	UserRefreshBackoff = 0.0;
	UserRefreshTime = 0.0;
	UserExpiredTime = 0.0;
	UserTokenState = ETokenState::Invalid;
}

void Credentials::SetClientCredentials(const FString& ClientId, const FString& ClientSecret)
{
	this->ClientId = ClientId;
	this->ClientSecret = ClientSecret;
}

void Credentials::SetClientToken(const FString& AccessToken, double ExpiresIn, const FString& Namespace)
{
	ClientAccessToken = AccessToken;
	ClientNamespace = Namespace;
}

void Credentials::SetUserToken(const FString& AccessToken, const FString& RefreshToken, double ExpiredTime, const FString& Id, const FString& DisplayName, const FString& Namespace)
{
	UserAccessToken = AccessToken;
	UserRefreshToken = RefreshToken;
	UserId = Id;
	UserDisplayName = DisplayName;
	UserNamespace = Namespace;
	UserRefreshTime = ExpiredTime;

	UserTokenState = ETokenState::Valid;
}

const FString& Credentials::GetUserAccessToken() const
{
	return UserAccessToken;
}

const FString& Credentials::GetUserRefreshToken() const
{
	return UserRefreshToken;
}

const FString& Credentials::GetUserNamespace() const
{
	return UserNamespace;
}

Credentials::ETokenState Credentials::GetTokenState() const
{
	return UserTokenState;
}

void Credentials::PollRefreshToken(double CurrentTime)
{
	switch (UserTokenState)
	{
	case ETokenState::Expired:
	case ETokenState::Valid:
		if (UserRefreshTime <= CurrentTime)
		{
			Oauth2::GetAccessTokenWithRefreshTokenGrant(
				ClientId, ClientSecret, 
				UserRefreshToken, 
				THandler<FOauth2Token>::CreateLambda([this, CurrentTime](const FOauth2Token& Result)
				{
					SetUserToken(Result.Access_token, Result.Refresh_token, CurrentTime + (Result.Expires_in * FMath::FRandRange(0.7, 0.9)), Result.User_id, Result.Display_name, Result.Namespace);
				}), 
				FErrorHandler::CreateLambda([this, CurrentTime](int32 ErrorCode, const FString& ErrorMessage)
				{
					if (UserRefreshBackoff <= 0.0)
					{
						UserRefreshBackoff = 10.0;
					}

					UserRefreshBackoff *= 2.0;
					UserRefreshBackoff += FMath::FRandRange(1.0, 60.0);
					ScheduleRefreshToken(CurrentTime + UserRefreshBackoff);
					
					UserTokenState = ETokenState::Expired;
				}));

			UserTokenState = ETokenState::Refreshing;
		}

		break;
	case ETokenState::Refreshing:
	case ETokenState::Invalid:
		break;
	}
}

void Credentials::ScheduleRefreshToken(double RefreshTime)
{
	UserRefreshTime = RefreshTime;
}

void Credentials::ForceRefreshToken()
{
	ScheduleRefreshToken(FPlatformTime::Seconds());
}

const FString& Credentials::GetUserId() const
{
	return UserId;
}

const FString& Credentials::GetUserDisplayName() const
{
	return UserDisplayName;
}

const FString& Credentials::GetClientAccessToken() const
{
	return ClientAccessToken;
}

const FString& Credentials::GetClientNamespace() const
{
	return ClientNamespace;
}

} // Namespace AccelByte

#include "AccelByteRegistry.h"

FString UAccelByteBlueprintsCredentials::GetUserAccessToken()
{
	return FRegistry::Credentials.GetUserAccessToken();
}

FString UAccelByteBlueprintsCredentials::GetUserId()
{
	return FRegistry::Credentials.GetUserId();
}

FString UAccelByteBlueprintsCredentials::GetUserDisplayName()
{
	return FRegistry::Credentials.GetUserDisplayName();
}

FString UAccelByteBlueprintsCredentials::GetUserNamespace()
{
	return FRegistry::Credentials.GetUserNamespace();
}
