#include "Blueprints/AccelByteBPApiClient.h"

void UCredentials::SetApiClient(FApiClientPtr const& NewApiClientPtr)
{
	ApiClientPtr = NewApiClientPtr;
}

FString UCredentials::GetAccessToken() const
{
	return ApiClientPtr->CredentialsRef->GetAccessToken();
}

FString UCredentials::GetUserId() const
{
	return ApiClientPtr->CredentialsRef->GetUserId();
}


UApiClient::UApiClient()
{
	Credentials = NewObject<UCredentials>();
	User = NewObject<UUser>();
	Lobby = NewObject<ULobby>();

	SetApiClient(FMultiRegistry::GetApiClient());
}

void UApiClient::SetApiClient(FApiClientPtr NewApiClientPtr)
{
	ApiClientPtr = NewApiClientPtr;
	Credentials->SetApiClient(ApiClientPtr);
	User->SetApiClient(ApiClientPtr);
	Lobby->SetApiClient(ApiClientPtr);
}

UApiClient* UMultiRegistry::GetApiClient(FString const& Key)
{
	UApiClient* WrapperPtr = NewObject<UApiClient>();
	WrapperPtr->SetApiClient(FMultiRegistry::GetApiClient(Key));

	return WrapperPtr;
}
