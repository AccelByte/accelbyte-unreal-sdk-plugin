// Copyright (c) 2021 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.


#include "GameServerApi/AccelByteServerSeasonPassApi.h"

#include "Core/AccelByteHttpRetryScheduler.h"
#include "Core/AccelByteRegistry.h"
#include "Core/AccelByteReport.h"
#include "Core/AccelByteServerCredentials.h"
#include "Core/AccelByteServerSettings.h"

namespace AccelByte
{
	namespace GameServerApi
	{
		ServerSeasonPass::ServerSeasonPass(const ServerCredentials& Credentials, const ServerSettings& Setting, FHttpRetryScheduler& InHttpRef) : Credentials(Credentials), Settings(Setting), HttpRef(InHttpRef)
		{
		}

		ServerSeasonPass::~ServerSeasonPass()
		{
		}

		void ServerSeasonPass::GrantExpToUser(const FString& UserId, int32 Exp, const THandler<FAccelByteModelsUserSeasonInfoWithoutReward>& OnSuccess, const FErrorHandler& OnError)
		{
			FReport::Log(FString(__FUNCTION__));

			FString Authorization = FString::Printf(TEXT("Bearer %s"), *Credentials.GetClientAccessToken());
			FString Url = FString::Printf(TEXT("%s/admin/namespaces/%s/users/%s/seasons/current/exp"), *Settings.SeasonPassServerUrl, *Credentials.GetClientNamespace(), *UserId);
			FString Verb = TEXT("POST");
			FString ContentType = TEXT("application/json");
			FString Accept = TEXT("application/json");
			FString Content = FString::Printf(TEXT("{\"exp\": %d}"), Exp);

			FHttpRequestPtr Request = FHttpModule::Get().CreateRequest();
			Request->SetURL(Url);
			Request->SetHeader(TEXT("Authorization"), Authorization);
			Request->SetVerb(Verb);
			Request->SetHeader(TEXT("Content-Type"), ContentType);
			Request->SetHeader(TEXT("Accept"), Accept);
			Request->SetContentAsString(Content);

			HttpRef.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
		}

		void ServerSeasonPass::GetCurrentUserSeasonProgression(const FString& UserId, const THandler<FAccelByteModelsUserSeasonInfoWithoutReward>& OnSuccess, const FErrorHandler& OnError)
		{
			FReport::Log(FString(__FUNCTION__));
			
			FString Authorization = FString::Printf(TEXT("Bearer %s"), *Credentials.GetClientAccessToken());
			FString Url = FString::Printf(TEXT("%s/admin/namespaces/%s/users/%s/seasons/current/progression"), *Settings.SeasonPassServerUrl, *Credentials.GetClientNamespace(), *UserId);
			FString Verb = TEXT("GET");
			FString ContentType = TEXT("application/json");
			FString Accept = TEXT("application/json");
			
			FHttpRequestPtr Request = FHttpModule::Get().CreateRequest();
			Request->SetURL(Url);
			Request->SetHeader(TEXT("Authorization"), Authorization);
			Request->SetVerb(Verb);
			Request->SetHeader(TEXT("Content-Type"), ContentType);
			Request->SetHeader(TEXT("Accept"), Accept);

			HttpRef.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
		}
		
	}
}
