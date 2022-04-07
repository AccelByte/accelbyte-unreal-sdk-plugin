// Copyright (c) 2019 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "Core/AccelByteServerCredentials.h"
#include "Core/AccelByteRegistry.h"
#include "Api/AccelByteOauth2Api.h"
#include "GameServerApi/AccelByteServerOauth2Api.h"
#include "Models/AccelByteOauth2Models.h"

using namespace AccelByte::Api;

DECLARE_LOG_CATEGORY_EXTERN(LogAccelByteServerCredentials, Log, All);
DEFINE_LOG_CATEGORY(LogAccelByteServerCredentials);


namespace AccelByte
{

ServerCredentials::ServerCredentials()
	: ClientAccessToken(TEXT(""))
	, ClientNamespace(TEXT(""))
	, ClientSessionState(ESessionState::Invalid)
{
}

const FString ServerCredentials::DefaultSection = TEXT("/Script/AccelByteUe4Sdk.AccelByteServerSettings");

void ServerCredentials::ForgetAll()
{
	ClientAccessToken = FString();

	ClientSessionState = ESessionState::Invalid;
}

void ServerCredentials::SetClientCredentials(const FString& ClientId_, const FString& ClientSecret_)
{
	ClientId = ClientId_;
	ClientSecret = ClientSecret_;
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

void ServerCredentials::SetClientToken(const FString& AccessToken, double ExpiresIn, const FString& Namespace)
{
	ClientAccessToken = AccessToken;
	ClientExpireTime = ExpiresIn;
	ScheduleRefreshToken(ClientExpireTime);
	ClientNamespace = Namespace;

	ClientSessionState = ESessionState::Valid;

	if (!PollRefreshTokenHandle.IsValid()) {
		PollRefreshTokenHandle = FTicker::GetCoreTicker().AddTicker(
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
}

void ServerCredentials::Shutdown()
{
	RemoveFromTicker(PollRefreshTokenHandle);
}

void ServerCredentials::RemoveFromTicker(FDelegateHandle& handle)
{
	if (handle.IsValid())
	{
		FTicker::GetCoreTicker().RemoveTicker(handle);
		handle.Reset();
	}
}
void ServerCredentials::PollRefreshToken(double CurrentTime)
{
	switch (ClientSessionState)
	{
		case ESessionState::Expired:
		case ESessionState::Valid:
			if (GetRefreshTime() <= CurrentTime)
			{
				FRegistry::ServerOauth2.LoginWithClientCredentials(FVoidHandler::CreateLambda([&]() 
					{ 
						ClientSessionState = ESessionState::Valid; 
					}),
					FErrorHandler::CreateLambda([&](int32 Code, const FString& Message) 
						{ 
							if (ClientRefreshBackoff <= 0.0)
							{
								ClientRefreshBackoff = 10.0;
							}

							ClientRefreshBackoff *= 2.0;
							ClientRefreshBackoff += FMath::FRandRange(1.0, 60.0);
							ScheduleRefreshToken(CurrentTime + ClientRefreshBackoff);
							ClientSessionState = ESessionState::Expired; 
						}));

				ClientSessionState = ESessionState::Refreshing;
			}

			break;
		case ESessionState::Refreshing:
		case ESessionState::Invalid:
			break;
	}
}

void ServerCredentials::ScheduleRefreshToken(double RefreshTime)
{
	ClientRefreshTime = RefreshTime;
}

void ServerCredentials::SetMatchId(const FString& GivenMatchId)
{
	MatchId = GivenMatchId;
}

ServerCredentials::ESessionState ServerCredentials::GetSessionState() const
{
	return ClientSessionState;
}

const FString& ServerCredentials::GetClientAccessToken() const
{
	return ClientAccessToken;
}

const FString& ServerCredentials::GetClientNamespace() const
{
	return ClientNamespace;
}

const double ServerCredentials::GetExpireTime() const
{
	return ClientExpireTime;
}

const double ServerCredentials::GetRefreshTime() const
{
	return ClientRefreshTime;
}

const FString& ServerCredentials::GetMatchId() const
{
	return MatchId;
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