// Copyright (c) 2021 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "Api/AccelByteFulfillmentApi.h"
#include "Core/AccelByteError.h"

#include "Core/AccelByteReport.h"
#include "Core/AccelByteHttpRetryScheduler.h"
#include "Core/AccelByteSettings.h"

namespace AccelByte
{
namespace Api
{
Fulfillment::Fulfillment(Credentials const& InCredentialsRef
	, Settings const& InSettingsRef
	, FHttpRetrySchedulerBase& InHttpRef
	, TSharedPtr<FApiClient, ESPMode::ThreadSafe> InApiClient)
	: FApiBase(InCredentialsRef, InSettingsRef, InHttpRef, InApiClient)
{}

Fulfillment::~Fulfillment()
{}

FAccelByteTaskWPtr Fulfillment::RedeemCode(FString const& Code
	, FString const& Region
	, FString const& Language
	, THandler<FAccelByteModelsFulfillmentResult> const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	const FString Url = FString::Printf(TEXT("%s/public/namespaces/%s/users/%s/fulfillment/code")
		, *SettingsRef.PlatformServerUrl
		, *FGenericPlatformHttp::UrlEncode(CredentialsRef->GetNamespace())
		, *FGenericPlatformHttp::UrlEncode(CredentialsRef->GetUserId()));

	FString Content;
	FAccelByteModelsFulFillCodeRequest CodeRequest;
	CodeRequest.Code = Code;
	CodeRequest.Region = Region;
	CodeRequest.Language = Language;
	
	TSharedPtr<FJsonObject> Json = FJsonObjectConverter::UStructToJsonObject(CodeRequest);
	FAccelByteUtilities::RemoveEmptyStrings(Json);
	TSharedRef<TJsonWriter<>> const Writer = TJsonWriterFactory<>::Create(&Content);
	FJsonSerializer::Serialize(Json.ToSharedRef(), Writer);

	return HttpClient.ApiRequest(TEXT("POST"), Url, {}, Content, OnSuccess, OnError);
}

} // Namespace Api
} // Namespace AccelByte
