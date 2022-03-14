// Copyright (c) 2018 - 2021 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "Core/AccelByteCredentials.h"
#include "Core/AccelByteHttpRetryScheduler.h"
#include "Api/AccelByteOauth2Api.h"
#include "Models/AccelByteOauth2Models.h"

using namespace AccelByte::Api;

DECLARE_LOG_CATEGORY_EXTERN(LogAccelByteCredentials, Log, All);
DEFINE_LOG_CATEGORY(LogAccelByteCredentials);


namespace AccelByte
{

Credentials::Credentials()
	: ClientId()
	, ClientSecret()
	, AuthToken()
	, UserSessionExpire(0)
	, UserEmailAddress()
	, UserSessionState(ESessionState::Invalid)
	, UserRefreshTime(0.0)
	, UserExpiredTime(0.0)
	, UserRefreshBackoff(0.0)
{}

Credentials::~Credentials()
{}

void Credentials::ForgetAll()
{
	AuthToken = {};
	UserSessionExpire = 0;
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

void Credentials::SetAuthToken(const FOauth2Token NewAuthToken, float CurrentTime)
{
	UserSessionExpire = CurrentTime + (NewAuthToken.Expires_in*FMath::FRandRange(0.7, 0.9));
	UserRefreshTime = UserSessionExpire;
	AuthToken = NewAuthToken;
	UserSessionState = ESessionState::Valid;
}

void Credentials::SetUserEmailAddress(const FString& EmailAddress)
{
	UserEmailAddress = EmailAddress;
}

const FString& Credentials::GetAccessToken() const
{
	return AuthToken.Access_token;
}

const FString& Credentials::GetRefreshToken() const
{
	return AuthToken.Refresh_token;
}

const FString& Credentials::GetNamespace() const
{
	return AuthToken.Namespace;
}

const FString& Credentials::GetPlatformUserId() const
{
	return AuthToken.Platform_user_id;
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
	return AuthToken.User_id;
}

const FString& Credentials::GetUserDisplayName() const
{
	return AuthToken.Display_name;
}

const FString& Credentials::GetUserEmailAddress() const
{
	return UserEmailAddress;
}

void Credentials::PollRefreshToken(double CurrentTime)
{
	switch (UserSessionState)
	{
	case ESessionState::Expired:
	case ESessionState::Valid:
		if (UserRefreshTime <= CurrentTime)
		{
			Oauth2::GetTokenWithRefreshToken(
				ClientId, ClientSecret,
				AuthToken.Refresh_token,
				THandler<FOauth2Token>::CreateLambda([this, CurrentTime](const FOauth2Token& Result)
			{
				SetAuthToken(Result, CurrentTime);
				if (RefreshTokenAdditionalActions.IsBound()) 
				{
					RefreshTokenAdditionalActions.Broadcast();
					RefreshTokenAdditionalActions.Clear();
				}
					
				TokenRefreshedEvent.Broadcast();
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

				if (RefreshTokenAdditionalActions.IsBound())
				{
					RefreshTokenAdditionalActions.Broadcast();
					RefreshTokenAdditionalActions.Clear();
				}
					
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

const FOauth2Token& Credentials::GetAuthToken() const
{
	return AuthToken;
}

void AccelByte::Credentials::SetBearerAuthRejectedHandler(FHttpRetryScheduler& HttpRef)
{
	HttpRef.SetBearerAuthRejectedDelegate(
		FHttpRetryScheduler::FBearerAuthRejected::CreateLambda([&]()
			{
				BearerAuthRejectedRefreshToken(HttpRef);
			}));
}

Credentials::FTokenRefreshedEvent& Credentials::OnTokenRefreshed()
{
	return TokenRefreshedEvent;
}

void Credentials::BearerAuthRejectedRefreshToken(FHttpRetryScheduler& HttpRef)
{
	if (GetSessionState() == ESessionState::Refreshing)
	{
		return;
	}
	else if (GetSessionState() == ESessionState::Invalid) 
	{
		HttpRef.ResumeBearerAuthRequest(GetAccessToken());
		return;
	}

	UE_LOG(LogAccelByteCredentials, Verbose, TEXT("BearerAuthRejectedRefreshToken"));
	HttpRef.PauseBearerAuthRequest();

	RefreshTokenAdditionalActions.Add(FVoidHandler::CreateLambda([&]()
		{
			HttpRef.ResumeBearerAuthRequest(GetAccessToken());
		}));

	ScheduleRefreshToken(FPlatformTime::Seconds());
}
} // Namespace AccelByte

#include "Core/AccelByteRegistry.h"

FString UAccelByteBlueprintsCredentials::GetUserSessionId()
{
	return FRegistry::Credentials.GetAccessToken();
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
	return FRegistry::Credentials.GetNamespace();
}

FString UAccelByteBlueprintsCredentials::GetUserEmailAddress()
{
	return FRegistry::Credentials.GetUserEmailAddress();
}
