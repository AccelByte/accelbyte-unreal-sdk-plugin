// Copyright (c) 2021 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.


#include "GameServerApi/AccelByteServerUserApi.h"

#include "Core/AccelByteHttpRetryScheduler.h"
#include "Core/AccelByteRegistry.h"
#include "Core/AccelByteReport.h"
#include "Core/AccelByteServerCredentials.h"
#include "Core/AccelByteServerSettings.h"
#include "Core/AccelByteUtilities.h"

namespace AccelByte
{
	namespace GameServerApi
	{
		ServerUser::ServerUser(ServerCredentials const& InCredentialsRef
			, ServerSettings const& InSettingsRef
			, FHttpRetryScheduler& InHttpRef)
			: CredentialsRef{InCredentialsRef}
			, SettingsRef{InSettingsRef}
			, HttpRef{InHttpRef}
		{
		}

		ServerUser::~ServerUser()
		{
		}

		void ServerUser::SearchUserOtherPlatformDisplayName(const FString& DisplayName, EAccelBytePlatformType PlatformType, const THandler<FPagedUserOtherPlatformInfo>& OnSuccess,
			const FErrorHandler& OnError, const int32& Limit, const int32& Offset)
		{
			FReport::Log(FString(__FUNCTION__));

			if (PlatformType == EAccelBytePlatformType::Nintendo || PlatformType == EAccelBytePlatformType::Oculus || PlatformType == EAccelBytePlatformType::Apple)
			{
				OnError.ExecuteIfBound(400, TEXT("Cannot search user using this function. Use SearchUserOtherPlatformUserId instead."));
				return;
			}
			
			FString PlatformId      = FAccelByteUtilities::GetPlatformString(PlatformType);
			
			FString Authorization   = FString::Printf(TEXT("Bearer %s"), *CredentialsRef.GetClientAccessToken());
			FString Url				= FString::Printf(TEXT("%s/v3/admin/namespaces/%s/users/search?by=thirdPartyPlatform&limit=%d&offset=%d&platformBy=platformDisplayName&platformId=%s&query=%s"),
					*SettingsRef.IamServerUrl, *CredentialsRef.GetClientNamespace(), Limit, Offset, *PlatformId, *DisplayName);
			FString Verb            = TEXT("GET");
			FString ContentType     = TEXT("application/json");
			FString Accept          = TEXT("application/json");

			FHttpRequestPtr Request = FHttpModule::Get().CreateRequest();
			Request->SetURL(Url);
			Request->SetHeader(TEXT("Authorization"), Authorization);
			Request->SetVerb(Verb);
			Request->SetHeader(TEXT("Content-Type"), ContentType);
			Request->SetHeader(TEXT("Accept"), Accept);

			HttpRef.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
		}

		void ServerUser::SearchUserOtherPlatformUserId(const FString& PlatformUserId, EAccelBytePlatformType PlatformType, const THandler<FUserOtherPlatformInfo>& OnSuccess,
				const FErrorHandler& OnError)
		{
			FReport::Log(FString(__FUNCTION__));

			FString PlatformId      = FAccelByteUtilities::GetPlatformString(PlatformType);
			FString Authorization   = FString::Printf(TEXT("Bearer %s"), *CredentialsRef.GetClientAccessToken());
			FString Url				= FString::Printf(TEXT("%s/v3/admin/namespaces/%s/platforms/%s/users/%s"), *SettingsRef.IamServerUrl, *CredentialsRef.GetClientNamespace(), *PlatformId, *PlatformUserId);
			FString Verb            = TEXT("GET");
			FString ContentType     = TEXT("application/json");
			FString Accept          = TEXT("application/json");

			FHttpRequestPtr Request = FHttpModule::Get().CreateRequest();
			Request->SetURL(Url);
			Request->SetHeader(TEXT("Authorization"), Authorization);
			Request->SetVerb(Verb);
			Request->SetHeader(TEXT("Content-Type"), ContentType);
			Request->SetHeader(TEXT("Accept"), Accept);

			HttpRef.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
		}

		void ServerUser::BanSingleUser(const FString& UserId, const FBanUserRequest& BanUser,
			const THandler<FBanUserResponse>& OnSuccess, const FErrorHandler& OnError)
		{
			FReport::Log(FString(__FUNCTION__));
		
			FString Authorization   = FString::Printf(TEXT("Bearer %s"), *CredentialsRef.GetClientAccessToken());
			FString Url				= FString::Printf(TEXT("%s/v3/admin/namespaces/%s/users/%s/bans"), *SettingsRef.IamServerUrl, *CredentialsRef.GetClientNamespace(), *UserId);
			FString Verb            = TEXT("POST");
			FString ContentType     = TEXT("application/json");
			FString Accept          = TEXT("application/json");
		
			FHttpRequestPtr Request = FHttpModule::Get().CreateRequest();
			Request->SetURL(Url);
			Request->SetHeader(TEXT("Authorization"), Authorization);
			Request->SetVerb(Verb);
			Request->SetHeader(TEXT("Content-Type"), ContentType);
			Request->SetHeader(TEXT("Accept"), Accept);
		
			FString Content{}; 
			FJsonObjectConverter::UStructToJsonObjectString(BanUser, Content);
			Request->SetContentAsString(Content);
		
			HttpRef.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
		}
		
	}
}
