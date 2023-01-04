// Copyright (c) 2022 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "Api/AccelByteStoreDisplayApi.h"
#include "Core/AccelByteError.h"
#include "JsonUtilities.h"
#include "Core/AccelByteRegistry.h"
#include "Core/AccelByteReport.h"
#include "Core/AccelByteHttpRetryScheduler.h"
#include "Core/AccelByteSettings.h"
#include "Core/AccelByteUtilities.h"

namespace AccelByte
{
	namespace Api
	{
		StoreDisplay::StoreDisplay(Credentials const& InCredentialsRef
				, Settings const& InSettingsRef
				, FHttpRetryScheduler& InHttpRef)
				: FApiBase(InCredentialsRef, InSettingsRef, InHttpRef)
		{
		}
		StoreDisplay::~StoreDisplay(){}

		void StoreDisplay::GetAllViews(FString const& StoreId
			, FString const& Language
			, THandler<TArray<FAccelByteModelsViewInfo>> const& OnSuccess
			, FErrorHandler const& OnError)
		{
			FReport::Log(FString(__FUNCTION__));

			FString Verb = TEXT("GET");
			FString Url = FString::Printf(TEXT("%s/public/namespaces/%s/users/%s/views"),
				*SettingsRef.PlatformServerUrl,
				*CredentialsRef.GetNamespace(),
				*CredentialsRef.GetUserId());

			const TMap<FString, FString> Params = FAccelByteUtilities::CreateQueryParamsAndSkipIfValueEmpty({
				{ TEXT("storeId"), StoreId },
				{ TEXT("language"), Language },
			});
	
			HttpClient.ApiRequest(Verb, Url, Params, OnSuccess, OnError);
		}

		void StoreDisplay::ListActiveSectionContents(FString const& StoreId
			, FString const& ViewId
			, FString const& Region
			, FString const& Language
			, THandler<TArray<FAccelByteModelsSectionInfo>> const& OnSuccess
			, FErrorHandler const& OnError)
		{
			FReport::Log(FString(__FUNCTION__));

			FString Verb = TEXT("GET");
			FString Url = FString::Printf(TEXT("%s/public/namespaces/%s/users/%s/sections"),
				*SettingsRef.PlatformServerUrl,
				*CredentialsRef.GetNamespace(),
				*CredentialsRef.GetUserId());

			const TMap<FString, FString> Params = FAccelByteUtilities::CreateQueryParamsAndSkipIfValueEmpty({
				{ TEXT("storeId"), StoreId },
				{ TEXT("viewId"), ViewId },
				{ TEXT("region"), Region },
				{ TEXT("language"), Language },
			});
	
			HttpClient.ApiRequest(Verb, Url, Params, OnSuccess, OnError);
		}
	
	} // Namespace Api
} // Namespace AccelByte
