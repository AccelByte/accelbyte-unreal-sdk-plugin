// Copyright (c) 2021 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "Blueprints/AccelByteBPApiClient.h"

using namespace AccelByte;

void UABCredentials::SetApiClient(FApiClientPtr const& NewApiClientPtr)
{
	ApiClientPtr = NewApiClientPtr;
}

void UABCredentials::SetAuthToken(FOauth2Token NewAuthToken, float CurrentTime)
{
	ApiClientPtr->CredentialsRef->SetAuthToken(NewAuthToken, CurrentTime);
}

void UABCredentials::ForgetAll()
{
	ApiClientPtr->CredentialsRef->ForgetAll();
}

void UABCredentials::SetClientCredentials(ESettingsEnvironment Environment)
{
	ApiClientPtr->CredentialsRef->SetClientCredentials(Environment);
}

void UABCredentials::SetUserEmailAddress(FString EmailAddress)
{
	ApiClientPtr->CredentialsRef->SetUserEmailAddress(EmailAddress);
}

void UABCredentials::PollRefreshToken(float CurrentTime)
{
	ApiClientPtr->CredentialsRef->PollRefreshToken(CurrentTime);
}

void UABCredentials::ScheduleRefreshToken(float NextRefreshTime)
{
	ApiClientPtr->CredentialsRef->ScheduleRefreshToken(NextRefreshTime);
}

void UABCredentials::SetErrorOAuth(FErrorOAuthInfo ErrorOAuthInfo)
{
	ApiClientPtr->CredentialsRef->SetErrorOAuth(ErrorOAuthInfo);
}

void UABCredentials::SetAccountUserData(FAccountUserData InAccountUserData)
{
	ApiClientPtr->CredentialsRef->SetAccountUserData(InAccountUserData);
}

FOauth2Token UABCredentials::GetAuthToken()
{
	return ApiClientPtr->CredentialsRef->GetAuthToken();
}

FString UABCredentials::GetRefreshToken()
{
	return ApiClientPtr->CredentialsRef->GetRefreshToken();
}

FString UABCredentials::GetAccessToken()
{
	return ApiClientPtr->CredentialsRef->GetAccessToken();
}

FString UABCredentials::GetUserId()
{
	return ApiClientPtr->CredentialsRef->GetUserId();
}

FString UABCredentials::GetPlatformUserId()
{
	return ApiClientPtr->CredentialsRef->GetPlatformUserId();
}

FString UABCredentials::GetUserDisplayName()
{
	return ApiClientPtr->CredentialsRef->GetUserDisplayName();
}

FString UABCredentials::GetNamespace()
{
	return ApiClientPtr->CredentialsRef->GetNamespace();
}

FString UABCredentials::GetUserEmailAddress()
{
	return ApiClientPtr->CredentialsRef->GetUserEmailAddress();
}

FString UABCredentials::GetLinkingToken()
{
	return ApiClientPtr->CredentialsRef->GetLinkingToken();
}

FAccountUserData UABCredentials::GetAccountUserData()
{
	return ApiClientPtr->CredentialsRef->GetAccountUserData();
}


bool UABCredentials::IsSessionValid()
{
	return ApiClientPtr->CredentialsRef->IsSessionValid();
}

bool UABCredentials::IsComply()
{
	return ApiClientPtr->CredentialsRef->IsComply();
}

UABApiClient::UABApiClient()
{
	Credentials = NewObject<UABCredentials>();
	User = NewObject<UABUser>();
	Lobby = NewObject<UABLobby>();
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
	Statistic = NewObject<UABStatistic>();
	Leaderboard = NewObject<UABLeaderboard>();
	Achievement = NewObject<UABAchievement>();
	UGC = NewObject<UABUGC>();
	Inventory = NewObject<UABInventory>();
	Presence = NewObject<UABPresence>();
	Block = NewObject<UABBlock>();
	Message = NewObject<UABMessage>();
	SessionAttribute = NewObject<UABSessionAttribute>();
	Signaling = NewObject<UABSignaling>();
	Group = NewObject<UABGroup>();
#if 1 // MMv1 Deprecation
	Matchmaking = NewObject<UABMatchmaking>();
	Party = NewObject<UABParty>();
	SessionBrowser = NewObject<UABSessionBrowser>();
#endif
}

void UABApiClient::SetApiClient(FApiClientPtr const& NewApiClientPtr)
{
	ApiClientPtr = NewApiClientPtr;
	Credentials->SetApiClient(ApiClientPtr);
	User->SetApiClient(ApiClientPtr);
	Lobby->SetApiClient(ApiClientPtr);
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
	Statistic->SetApiClient(ApiClientPtr);
	Leaderboard->SetApiClient(ApiClientPtr);
	Achievement->SetApiClient(ApiClientPtr);
	UGC->SetApiClient(ApiClientPtr);
	Inventory->SetApiClient(ApiClientPtr);
	Presence->SetApiClient(ApiClientPtr);
	Block->SetApiClient(ApiClientPtr);
	Message->SetApiClient(ApiClientPtr);
	SessionAttribute->SetApiClient(ApiClientPtr);
	Signaling->SetApiClient(ApiClientPtr);
	Group->SetApiClient(ApiClientPtr);
#if 1 // MMv1 Deprecation
	Matchmaking->SetApiClient(ApiClientPtr);
	Party->SetApiClient(ApiClientPtr);
	SessionBrowser->SetApiClient(ApiClientPtr);
#endif
}