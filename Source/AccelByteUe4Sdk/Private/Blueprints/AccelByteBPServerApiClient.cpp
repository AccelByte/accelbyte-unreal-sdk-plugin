// Copyright (c) 2021 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "Blueprints/AccelByteBPServerApiClient.h"
#include "Core/AccelByteRegistry.h"
#include "Core/AccelByteServerCredentials.h"

FString UABServerCredentials::GetAccessToken() const
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
	ServerMatchmaking->SetServerApiClient(ApiClientPtr);
	ServerSessionBrowser->SetServerApiClient(ApiClientPtr);
	ServerStatistic->SetServerApiClient(ApiClientPtr);
}

UServerApiClient::UServerApiClient()
{
	Credentials = NewObject<UABServerCredentials>();
	ServerOauth2 = NewObject<UABServerOauth2>();
	ServerDSM = NewObject<UABServerDSM>();
	ServerLobby = NewObject<UABServerLobby>();
	ServerMatchmaking = NewObject<UABServerMatchmaking>();
	ServerSessionBrowser = NewObject<UABServerSessionBrowser>();
	ServerStatistic = NewObject<UABServerStatistic>();


	SetServerApiClient(FMultiRegistry::GetServerApiClient());
}

UServerApiClient* UServerMultiRegistry::GetServerApiClient(FString const& Key)
{
	UServerApiClient* WrapperPtr = NewObject<UServerApiClient>();
	WrapperPtr->SetServerApiClient(FMultiRegistry::GetServerApiClient(Key));
	return WrapperPtr;
}
