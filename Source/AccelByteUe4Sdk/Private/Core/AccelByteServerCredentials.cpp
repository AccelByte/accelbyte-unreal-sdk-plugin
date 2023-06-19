// Copyright (c) 2019 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "Core/AccelByteServerCredentials.h"
#include "Core/AccelByteRegistry.h"
#include "Core/AccelByteOauth2Api.h"
#include "Models/AccelByteOauth2Models.h"
#include "AccelByteUe4SdkModule.h"
#include "Core/AccelByteRegistry.h"

using namespace AccelByte;
using namespace AccelByte::Api;

DECLARE_LOG_CATEGORY_EXTERN(LogAccelByteServerCredentials, Log, All);
DEFINE_LOG_CATEGORY(LogAccelByteServerCredentials);


namespace AccelByte
{

ServerCredentials::ServerCredentials()
	: AccessToken()
	, Namespace()
	, UserId()
{
}

const FString ServerCredentials::DefaultSection = TEXT("/Script/AccelByteUe4Sdk.AccelByteServerSettings");

void ServerCredentials::ForgetAll()
{
	BaseCredentials::ForgetAll();
	AccessToken = FString();
}

void ServerCredentials::SetClientCredentials(const ESettingsEnvironment Environment)
{
	FString SectionPath;
	switch (Environment)
	{
	case ESettingsEnvironment::Development:
		SectionPath = TEXT("/Script/AccelByteUe4Sdk.AccelByteServerSettingsDev");
		break;
	case ESettingsEnvironment::Certification:
		SectionPath = TEXT("/Script/AccelByteUe4Sdk.AccelByteServerSettingsCert");
		break;
	case ESettingsEnvironment::Production:
		SectionPath = TEXT("/Script/AccelByteUe4Sdk.AccelByteServerSettingsProd");
		break;
	case ESettingsEnvironment::Default:
	default:
		SectionPath = TEXT("/Script/AccelByteUe4Sdk.AccelByteServerSettings");
		break;
	}

	if (GConfig->GetString(*SectionPath, TEXT("ClientId"), ClientId, GEngineIni))
	{
		GConfig->GetString(*SectionPath, TEXT("ClientSecret"), ClientSecret, GEngineIni);
	}
	else
	{
		GConfig->GetString(*DefaultSection, TEXT("ClientId"), ClientId, GEngineIni);
		GConfig->GetString(*DefaultSection, TEXT("ClientSecret"), ClientSecret, GEngineIni);
	}
}

void ServerCredentials::SetClientToken(const FString& InAccessToken, double ExpiresIn, const FString& InNamespace)
{
	AccessToken = InAccessToken;
	ExpireTime = ExpiresIn;
	if (ExpireTime > MaxBackoffTime)
	{
		ScheduleRefreshToken(ExpireTime - MaxBackoffTime);
	}
	else
	{
		ScheduleRefreshToken(ExpireTime*BackoffRatio);
	}
	Namespace = InNamespace;
	BackoffCount = 0;
	SessionState = ESessionState::Valid;

	if (!PollRefreshTokenHandle.IsValid()) {
		PollRefreshTokenHandle = FTickerAlias::GetCoreTicker().AddTicker(
			FTickerDelegate::CreateLambda([this](float DeltaTime)
				{
					PollRefreshToken(FPlatformTime::Seconds());
					return true;
				}),
			0.2f);
	}
}

void ServerCredentials::Startup()
{
	IAccelByteUe4SdkModuleInterface& ABSDKModule = IAccelByteUe4SdkModuleInterface::Get();
	SetClientCredentials(ABSDKModule.GetSettingsEnvironment());
}

void ServerCredentials::Shutdown()
{
	RemoveFromTicker(PollRefreshTokenHandle);
}

void ServerCredentials::RemoveFromTicker(FDelegateHandleAlias& handle)
{
	if (handle.IsValid())
	{
		FTickerAlias::GetCoreTicker().RemoveTicker(handle);
		handle.Reset();
	}
}
void ServerCredentials::PollRefreshToken(double CurrentTime)
{
	switch (SessionState)
	{
		case ESessionState::Expired:
		case ESessionState::Valid:
			if (GetRefreshTime() <= CurrentTime)
			{
				Oauth2::GetTokenWithClientCredentials(ClientId
					, ClientSecret
					, THandler<FOauth2Token>::CreateLambda(
						[this, CurrentTime](const FOauth2Token& Result)
						{
							SessionState = ESessionState::Valid;
							SetClientToken(Result.Access_token, Result.Expires_in, Result.Namespace);
							TokenRefreshedEvent.Broadcast(true);
						})
					, FErrorHandler::CreateLambda(
						[&](int32 Code, const FString& Message) 
						{ 
							BackoffCount++;
							if (BackoffCount < MaxBackoffCount)
							{
								if (ExpireTime > MaxBackoffTime)
								{
									ExpireTime = MaxBackoffTime;
								}
								else if (ExpireTime <= MinBackoffTime)
								{
									ExpireTime = MinBackoffTime;
								}
								else
								{
									ExpireTime = ExpireTime * BackoffRatio;
								}

								RefreshBackoff = ExpireTime * BackoffRatio;
								ScheduleRefreshToken(RefreshBackoff);
								SessionState = ESessionState::Expired;
							}
							else
							{
								SessionState = ESessionState::Invalid;
							}
							TokenRefreshedEvent.Broadcast(false);
						})
					, FRegistry::ServerSettings.IamServerUrl);

				SessionState = ESessionState::Refreshing;
			}

			break;
		case ESessionState::Refreshing:
		case ESessionState::Invalid:
			break;
	}
}

void ServerCredentials::ScheduleRefreshToken(double NextRefreshTime)
{
	RefreshTime = FPlatformTime::Seconds() + NextRefreshTime;
}

void ServerCredentials::SetMatchId(const FString& GivenMatchId)
{
	MatchId = GivenMatchId;
}


const FString& ServerCredentials::GetClientAccessToken() const
{
	return GetAccessToken();
}

const FString& ServerCredentials::GetAccessToken() const
{
	return AccessToken;
}

const FString& ServerCredentials::GetClientNamespace() const
{
	return GetNamespace();
}

const FString& ServerCredentials::GetNamespace() const
{
	return Namespace;
}

const double ServerCredentials::GetExpireTime() const
{
	return ExpireTime;
}

const double ServerCredentials::GetRefreshTime() const
{
	return RefreshTime;
}

const FString& ServerCredentials::GetMatchId() const
{
	return MatchId;
}

const FString& ServerCredentials::GetUserId() const
{
	return UserId;
}

} // Namespace AccelByte

#include "Core/AccelByteRegistry.h"

FString UAccelByteBlueprintsServerCredentials::GetClientAccessToken()
{
	return FRegistry::ServerCredentials.GetClientAccessToken();
}

FString UAccelByteBlueprintsServerCredentials::GetClientNamespace()
{
	return FRegistry::ServerCredentials.GetClientNamespace();
}

FString UAccelByteBlueprintsServerCredentials::GetMatchId()
{
	return FRegistry::ServerCredentials.GetMatchId();
}