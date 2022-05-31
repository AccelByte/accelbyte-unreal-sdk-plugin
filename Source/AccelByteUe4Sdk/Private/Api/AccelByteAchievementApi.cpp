// Copyright (c) 2020 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "Api/AccelByteAchievementApi.h"
#include "Core/AccelByteError.h"
#include "Core/AccelByteRegistry.h"
#include "Core/AccelByteReport.h"
#include "Core/AccelByteHttpRetryScheduler.h"
#include "JsonUtilities.h"
#include "Core/AccelByteSettings.h"

namespace AccelByte
{
namespace Api
{

Achievement::Achievement(Credentials const& InCredentialsRef
	, Settings const& InSettingsRef
	, FHttpRetryScheduler& InHttpRef)
	: HttpRef{InHttpRef}
	, CredentialsRef{InCredentialsRef}
	, SettingsRef{InSettingsRef}
{}

Achievement::~Achievement()
{}

FString Achievement::ConvertAchievementSortByToString(const EAccelByteAchievementListSortBy& SortBy)
{
	switch (SortBy)
	{
	case EAccelByteAchievementListSortBy::LISTORDER:
		return TEXT("listOrder");
	case EAccelByteAchievementListSortBy::LISTORDER_ASC:
		return TEXT("listOrder%3Aasc");
	case EAccelByteAchievementListSortBy::LISTORDER_DESC:
		return TEXT("listOrder%3Adesc");
	case EAccelByteAchievementListSortBy::CREATED_AT:
		return TEXT("createdAt");
	case EAccelByteAchievementListSortBy::CREATED_AT_ASC:
		return TEXT("createdAt%3Aasc");
	case EAccelByteAchievementListSortBy::CREATED_AT_DESC:
		return TEXT("createdAt%3Adesc");
	case EAccelByteAchievementListSortBy::UPDATED_AT:
		return TEXT("updatedAt");
	case EAccelByteAchievementListSortBy::UPDATED_AT_ASC:
		return TEXT("updatedAt%3Aasc");
	case EAccelByteAchievementListSortBy::UPDATED_AT_DESC:
		return TEXT("updatedAt%3Adesc");
	}
	return TEXT("");
}

void Achievement::QueryAchievements(const FString& Language, const EAccelByteAchievementListSortBy& SortBy, const THandler<FAccelByteModelsPaginatedPublicAchievement>& OnSuccess, const FErrorHandler& OnError,
	const int32& Offset, const int32& Limit)
{
	FReport::Log(FString(__FUNCTION__));
	
	FString Authorization   = FString::Printf(TEXT("Bearer %s"), *CredentialsRef.GetAccessToken());
	FString Url             = FString::Printf(TEXT("%s/v1/public/namespaces/%s/achievements"), *SettingsRef.AchievementServerUrl, *CredentialsRef.GetNamespace());
	FString Verb            = TEXT("GET");
	FString ContentType     = TEXT("application/json");
	FString Accept          = TEXT("application/json");
	FString Query			= TEXT("");

	if (!Language.IsEmpty())
	{
		Query.Append(Query.IsEmpty() ? TEXT("") : TEXT("&"));
		Query.Append(FString::Printf(TEXT("language=%s"), *Language));
	}
	if (SortBy != EAccelByteAchievementListSortBy::NONE)
	{
		Query.Append(Query.IsEmpty() ? TEXT("") : TEXT("&"));
		Query.Append(FString::Printf(TEXT("sortBy=%s"), *ConvertAchievementSortByToString(SortBy)));
	}
	if (Offset >= 0)
	{
		Query.Append(Query.IsEmpty() ? TEXT("") : TEXT("&"));
		Query.Append(FString::Printf(TEXT("offset=%d"), Offset));
	}
	if (Limit >= 0)
	{
		Query.Append(Query.IsEmpty() ? TEXT("") : TEXT("&"));
		Query.Append(FString::Printf(TEXT("limit=%d"), Limit));
	}
	Url.Append(Query.IsEmpty() ? TEXT("") : FString::Printf(TEXT("?%s"), *Query));

	FHttpRequestPtr Request = FHttpModule::Get().CreateRequest();
	Request->SetURL(Url);
	Request->SetHeader(TEXT("Authorization"), Authorization);
	Request->SetVerb(Verb);
	Request->SetHeader(TEXT("Content-Type"), ContentType);
	Request->SetHeader(TEXT("Accept"), Accept);

	HttpRef.ProcessRequest(Request,  CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
}

void Achievement::GetAchievement(const FString& AchievementCode, const THandler<FAccelByteModelsMultiLanguageAchievement>& OnSuccess, const FErrorHandler& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	if (AchievementCode.IsEmpty())
	{
		OnError.ExecuteIfBound(404, TEXT("Url is invalid. Achievement Code is empty."));
		return;
	}

	FString Authorization	= FString::Printf(TEXT("Bearer %s"), *CredentialsRef.GetAccessToken());
	FString Url				= FString::Printf(TEXT("%s/v1/public/namespaces/%s/achievements/%s"), *SettingsRef.AchievementServerUrl, *CredentialsRef.GetNamespace(), *AchievementCode);
	FString Verb			= TEXT("GET");
	FString ContentType		= TEXT("application/json");
	FString Accept			= TEXT("application/json");

	FHttpRequestPtr Request = FHttpModule::Get().CreateRequest();
	Request->SetURL(Url);
	Request->SetHeader(TEXT("Authorization"), Authorization);
	Request->SetVerb(Verb);
	Request->SetHeader(TEXT("Content-Type"), ContentType);
	Request->SetHeader(TEXT("Accept"), Accept);

	HttpRef.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
}

void Achievement::QueryUserAchievements(const EAccelByteAchievementListSortBy& SortBy, const THandler<FAccelByteModelsPaginatedUserAchievement>& OnSuccess, const FErrorHandler& OnError,
	const int32& Offset, const int32& Limit, bool PreferUnlocked)
{
	FReport::Log(FString(__FUNCTION__));

	FString Authorization = FString::Printf(TEXT("Bearer %s"), *CredentialsRef.GetAccessToken());
	FString Url = FString::Printf(TEXT("%s/v1/public/namespaces/%s/users/%s/achievements"), *SettingsRef.AchievementServerUrl, *CredentialsRef.GetNamespace(), *CredentialsRef.GetUserId());
	FString Verb = TEXT("GET");
	FString ContentType = TEXT("application/json");
	FString Accept = TEXT("application/json");
	FString Query = TEXT("");

	if (SortBy != EAccelByteAchievementListSortBy::NONE)
	{
		Query.Append(Query.IsEmpty() ? TEXT("") : TEXT("&"));
		Query.Append(FString::Printf(TEXT("sortBy=%s"), *ConvertAchievementSortByToString(SortBy)));
	}
	if (Offset >= 0)
	{
		Query.Append(Query.IsEmpty() ? TEXT("") : TEXT("&"));
		Query.Append(FString::Printf(TEXT("offset=%d"), Offset));
	}
	if (Limit >= 0)
	{
		Query.Append(Query.IsEmpty() ? TEXT("") : TEXT("&"));
		Query.Append(FString::Printf(TEXT("limit=%d"), Limit));
	}
	{
		Query.Append(Query.IsEmpty() ? TEXT("") : TEXT("&"));
		Query.Append(FString::Printf(TEXT("preferUnlocked=%s"), PreferUnlocked ? TEXT("true"):TEXT("false")));
	}
	Url.Append(Query.IsEmpty() ? TEXT("") : FString::Printf(TEXT("?%s"), *Query));

	FHttpRequestPtr Request = FHttpModule::Get().CreateRequest();
	Request->SetURL(Url);
	Request->SetHeader(TEXT("Authorization"), Authorization);
	Request->SetVerb(Verb);
	Request->SetHeader(TEXT("Content-Type"), ContentType);
	Request->SetHeader(TEXT("Accept"), Accept);

	HttpRef.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
}

void Achievement::UnlockAchievement(const FString& AchievementCode, const FVoidHandler OnSuccess, const FErrorHandler& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	if (AchievementCode.IsEmpty())
	{
		OnError.ExecuteIfBound(404, TEXT("Url is invalid. Achievement Code is empty."));
		return;
	}

	FString Authorization = FString::Printf(TEXT("Bearer %s"), *CredentialsRef.GetAccessToken());
	FString Url = FString::Printf(TEXT("%s/v1/public/namespaces/%s/users/%s/achievements/%s/unlock"), *SettingsRef.AchievementServerUrl, *CredentialsRef.GetNamespace(), *CredentialsRef.GetUserId(), *AchievementCode);
	FString Verb = TEXT("PUT");
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

} // Namespace Api
} // Namespace AccelByte
