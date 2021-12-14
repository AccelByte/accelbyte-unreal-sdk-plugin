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
	ServerDSM->SetServerApiClient(ApiClientPtr);
	ServerLobby->SetServerApiClient(ApiClientPtr);
}

UServerApiClient::UServerApiClient()
{
	Credentials = NewObject<UABServerCredentials>();
	ServerDSM = NewObject<UABServerDSM>();
	ServerLobby = NewObject<UABServerLobby>();

	SetServerApiClient(FMultiRegistry::GetServerApiClient());
}

UServerApiClient* UServerMultiRegistry::GetServerApiClient(FString const& Key)
{
	UServerApiClient* WrapperPtr = NewObject<UServerApiClient>();
	WrapperPtr->SetServerApiClient(FMultiRegistry::GetServerApiClient(Key));
	return WrapperPtr;
}
