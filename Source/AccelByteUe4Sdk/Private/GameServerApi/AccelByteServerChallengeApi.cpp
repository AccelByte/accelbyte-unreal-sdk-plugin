// Copyright (c) 2021 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.


#include "GameServerApi/AccelByteServerChallengeApi.h"
#include "Core/AccelByteHttpRetryScheduler.h"
#include "Core/AccelByteRegistry.h"
#include "Core/AccelByteReport.h"
#include "Core/AccelByteUtilities.h"
#include "Core/AccelByteServerCredentials.h"
#include "Core/AccelByteServerSettings.h"

namespace AccelByte
{
namespace GameServerApi
{

ServerChallenge::ServerChallenge(ServerCredentials const& InCredentialsRef
	, ServerSettings const& InSettingsRef
	, FHttpRetryScheduler& InHttpRef)
	: FServerApiBase(InCredentialsRef, InSettingsRef, InHttpRef)
{
}

ServerChallenge::~ServerChallenge()
{
}

FAccelByteTaskWPtr ServerChallenge::EvaluateChallengeProgress(FAccelByteModelsChallengeServerEvaluateProgressRequest const& Request
	, FVoidHandler const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	const FString Url = FString::Printf(TEXT("%s/v1/admin/namespaces/%s/progress/evaluate")
		, *ServerSettingsRef.ChallengeServerUrl
		, *ServerCredentialsRef->GetClientNamespace());

	return HttpClient.ApiRequest(TEXT("POST"), Url, {}, Request, OnSuccess, OnError);
}

}
}
