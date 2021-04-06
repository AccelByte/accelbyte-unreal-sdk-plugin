// Copyright (c) 2018 - 2020 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "Core/AccelByteCredentials.h"
#include "Api/AccelByteOauth2Api.h"
#include "Models/AccelByteOauth2Models.h"

using namespace AccelByte::Api;

DECLARE_LOG_CATEGORY_EXTERN(LogAccelByteCredentials, Log, All);
DEFINE_LOG_CATEGORY(LogAccelByteCredentials);


namespace AccelByte
{

Credentials::Credentials()
	: ClientAccessToken(TEXT(""))
	, ClientNamespace(TEXT(""))
	, UserSessionId(TEXT(""))
	, UserSessionExpire(0)
	, UserNamespace(TEXT(""))
	, UserId(TEXT(""))
	, UserDisplayName(TEXT(""))
	, UserSessionState(ESessionState::Invalid)
	, UserRefreshId(TEXT(""))
	, UserRefreshTime(0.0)
	, UserExpiredTime(0.0)
	, UserRefreshBackoff(0.0)
	, PlatformBoundUserId(TEXT(""))
{
}

void Credentials::ForgetAll()
{
	UserSessionId = FString();
	UserSessionExpire = 0;
	UserNamespace = FString();
	UserId = FString();
	UserDisplayName = FString();
	PlatformBoundUserId = FString();

	UserRefreshBackoff = 0.0;
	UserRefreshTime = 0.0;
	UserExpiredTime = 0.0;
	UserSessionState = ESessionState::Invalid;
}

void Credentials::SetClientCredentials(const FString& Id, const FString& Secret)
{
	ClientId = Id;
	ClientSecret = Secret;
}

void Credentials::SetClientToken(const FString& AccessToken, double ExpiresIn, const FString& Namespace)
{
	ClientAccessToken = AccessToken;
	ClientNamespace = Namespace;
}

void Credentials::SetUserSession(const FString& SessionId, double ExpiredTime, const FString& RefreshId)
{
	UserSessionId = SessionId;
	UserSessionExpire = ExpiredTime;
	UserRefreshId = RefreshId;
	UserRefreshTime = ExpiredTime;

	UserSessionState = ESessionState::Valid;
}

void Credentials::SetUserLogin(const FString& Id, const FString& DisplayName, const FString& Namespace)
{
	UserId = Id;
	UserDisplayName = DisplayName;
	UserNamespace = Namespace;
}

void Credentials::SetPlatformInfo(const FString& PlatformUserId)
{
	PlatformBoundUserId = PlatformUserId;
}

void Credentials::SetUserEmailAddress(const FString& EmailAddress)
{
	UserEmailAddress = EmailAddress;
}

const FString& Credentials::GetUserSessionId() const
{
	return UserSessionId;
}

const FString& Credentials::GetUserRefreshId() const
{
	return UserRefreshId;
}

const FString& Credentials::GetUserNamespace() const
{
	return UserNamespace;
}

const FString& Credentials::GetPlatformBoundUserId() const
{
	return PlatformBoundUserId;
}

Credentials::ESessionState Credentials::GetSessionState() const
{
	return UserSessionState;
}

void Credentials::Startup()
{
	PollRefreshTokenHandle = FTicker::GetCoreTicker().AddTicker(
        FTickerDelegate::CreateLambda([this](float DeltaTime)
        {
            PollRefreshToken(FPlatformTime::Seconds());

            return true;
        }),
        0.2f);
}

void Credentials::Shutdown()
{
	if (PollRefreshTokenHandle.IsValid())
	{
		FTicker::GetCoreTicker().RemoveTicker(PollRefreshTokenHandle);
		PollRefreshTokenHandle.Reset();
	}
}

const FString& Credentials::GetUserId() const
{
	return UserId;
}

const FString& Credentials::GetUserDisplayName() const
{
	return UserDisplayName;
}

const FString& Credentials::GetUserEmailAddress() const
{
	return UserEmailAddress;
}

const FString& Credentials::GetClientAccessToken() const
{
	return ClientAccessToken;
}

const FString& Credentials::GetClientNamespace() const
{
	return ClientNamespace;
}

void Credentials::PollRefreshToken(double CurrentTime)
{
	switch (UserSessionState)
	{
	case ESessionState::Expired:
	case ESessionState::Valid:
		if (UserRefreshTime <= CurrentTime)
		{
			Oauth2::GetSessionIdWithRefreshId(
				ClientId, ClientSecret,
				UserRefreshId,
				THandler<FOauth2Session>::CreateLambda([this, CurrentTime](const FOauth2Session& Result)
			{
				SetUserSession(Result.Session_id, CurrentTime + (Result.Expires_in * FMath::FRandRange(0.7, 0.9)), Result.Refresh_id);
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

				UserSessionState = ESessionState::Expired;
			}));

			UserSessionState = ESessionState::Refreshing;
		}

		break;
	case ESessionState::Refreshing:
	case ESessionState::Invalid:
		break;
	}
}

void Credentials::ScheduleRefreshToken(double RefreshTime)
{
	UserRefreshTime = RefreshTime;
}

} // Namespace AccelByte

#include "Core/AccelByteRegistry.h"

FString UAccelByteBlueprintsCredentials::GetUserSessionId()
{
	return FRegistry::Credentials.GetUserSessionId();
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

FString UAccelByteBlueprintsCredentials::GetUserEmailAddress()
{
	return FRegistry::Credentials.GetUserEmailAddress();
}
