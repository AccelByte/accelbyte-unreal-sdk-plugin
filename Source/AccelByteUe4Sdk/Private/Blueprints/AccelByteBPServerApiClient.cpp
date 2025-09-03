// Copyright (c) 2021 - 2022 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "Blueprints/AccelByteBPServerApiClient.h"
#include "Core/AccelByteServerCredentials.h"

using namespace AccelByte;

FString UABServerCredentials::GetAccessToken() 
{
	return ApiClientPtr->ServerCredentialsRef->GetClientAccessToken();
}

void UABServerCredentials::SetServerApiClient(FServerApiClientPtr const& NewApiClientPtr)
{
	ApiClientPtr = NewApiClientPtr;
}

void UServerApiClient::SetServerApiClient(FServerApiClientPtr const& NewApiClientPtr)
{
	ApiClientPtr = NewApiClientPtr;
	Credentials->SetServerApiClient(ApiClientPtr);
	ServerOauth2->SetServerApiClient(ApiClientPtr);
	ServerDSM->SetServerApiClient(ApiClientPtr);
	ServerLobby->SetServerApiClient(ApiClientPtr);
#if 1 // MMv1 Deprecation
	ServerMatchmaking->SetServerApiClient(ApiClientPtr);
	ServerSessionBrowser->SetServerApiClient(ApiClientPtr);
#endif
	ServerAchievement->SetServerApiClient(ApiClientPtr);
	ServerStatistic->SetServerApiClient(ApiClientPtr);
}

UServerApiClient::UServerApiClient()
{
	Credentials = NewObject<UABServerCredentials>();
	ServerOauth2 = NewObject<UABServerOauth2>();
	ServerDSM = NewObject<UABServerDSM>();
	ServerLobby = NewObject<UABServerLobby>();
#if 1 // MMv1 Deprecation
	ServerMatchmaking = NewObject<UABServerMatchmaking>();
	ServerSessionBrowser = NewObject<UABServerSessionBrowser>();
#endif
	ServerAchievement = NewObject<UABServerAchievement>();
	ServerStatistic = NewObject<UABServerStatistic>();
}