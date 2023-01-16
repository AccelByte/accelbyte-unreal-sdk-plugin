// Copyright (c) 2021 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "Api/AccelByteMiscellaneousApi.h"
#include "Core/AccelByteRegistry.h"
#include "Core/AccelByteReport.h"
#include "Core/AccelByteHttpRetryScheduler.h"
#include "Core/AccelByteSettings.h"

namespace AccelByte
{
namespace Api
{

Miscellaneous::Miscellaneous(Credentials const& InCredentialsRef
	, Settings const& InSettingsRef
	, FHttpRetryScheduler& InHttpRef)
	: FApiBase(InCredentialsRef, InSettingsRef, InHttpRef)
{}

Miscellaneous::~Miscellaneous()
{}

void Miscellaneous::GetServerCurrentTime(const THandler<FTime>& OnSuccess, const FErrorHandler& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	const FString Url = FString::Printf(TEXT("%s/v1/public/misc/time"), *SettingsRef.BasicServerUrl);

	const FString Accept = TEXT("application/json");

	const TMap<FString, FString> Headers = {
		{TEXT("Accept"), Accept}
	};

	HttpClient.Request(TEXT("GET"), Url, TEXT(""), Headers, OnSuccess, OnError);
}

} // Namespace Api
} // Namespace AccelByte
