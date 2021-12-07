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
	User = NewObject<UABUser>();
	Lobby = NewObject<ULobby>();
	Party = NewObject<UABParty>();
	Matchmaking = NewObject<UABMatchmaking>();
	Friends = NewObject<UABFriends>();
	Catalog = NewObject<UABCatalog>();
	Order = NewObject<UABOrder>();
	Entitlement = NewObject<UABEntitlement>();
	Currency = NewObject<UABCurrency>();
	Wallet = NewObject<UABWallet>();
	Fulfillment = NewObject<UABFulfillment>();
	CloudSave = NewObject<UABCloudSave>();

	SetApiClient(FMultiRegistry::GetApiClient());
}

void UApiClient::SetApiClient(FApiClientPtr NewApiClientPtr)
{
	ApiClientPtr = NewApiClientPtr;
	Credentials->SetApiClient(ApiClientPtr);
	User->SetApiClient(ApiClientPtr);
	Lobby->SetApiClient(ApiClientPtr);
	Party->SetApiClient(ApiClientPtr);
	Matchmaking->SetApiClient(ApiClientPtr);
	Friends->SetApiClient(ApiClientPtr);
	Catalog->SetApiClient(ApiClientPtr);
	Order->SetApiClient(ApiClientPtr);
	Entitlement->SetApiClient(ApiClientPtr);
	Currency->SetApiClient(ApiClientPtr);
	Wallet->SetApiClient(ApiClientPtr);
	Fulfillment->SetApiClient(ApiClientPtr);
	CloudSave->SetApiClient(ApiClientPtr);
}

UApiClient* UMultiRegistry::GetApiClient(FString const& Key)
{
	UApiClient* WrapperPtr = NewObject<UApiClient>();
	WrapperPtr->SetApiClient(FMultiRegistry::GetApiClient(Key));

	return WrapperPtr;
}
