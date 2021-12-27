#include "Blueprints/AccelByteBPApiClient.h"

void UABCredentials::SetApiClient(FApiClientPtr const& NewApiClientPtr)
{
	ApiClientPtr = NewApiClientPtr;
}

FString UABCredentials::GetAccessToken() const
{
	return ApiClientPtr->CredentialsRef->GetAccessToken();
}

FString UABCredentials::GetUserId() const
{
	return ApiClientPtr->CredentialsRef->GetUserId();
}

UABApiClient::UABApiClient()
{
	Credentials = NewObject<UABCredentials>();
	User = NewObject<UABUser>();
	Lobby = NewObject<UABLobby>();
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
	Agreement = NewObject<UABAgreement>();
	Reward = NewObject<UABReward>();
	SeasonPass = NewObject<UABSeasonPass>();
	SessionBrowser = NewObject<UABSessionBrowser>();

	SetApiClient(FMultiRegistry::GetApiClient());
}

void UABApiClient::SetApiClient(FApiClientPtr NewApiClientPtr)
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
	Agreement->SetApiClient(ApiClientPtr);
	Reward->SetApiClient(ApiClientPtr);
	SeasonPass->SetApiClient(ApiClientPtr);
	SessionBrowser->SetApiClient(ApiClientPtr);
}

UABApiClient* UABMultiRegistry::GetApiClient(FString const& Key)
{
	UABApiClient* WrapperPtr = NewObject<UABApiClient>();
	WrapperPtr->SetApiClient(FMultiRegistry::GetApiClient(Key));

	return WrapperPtr;
}
