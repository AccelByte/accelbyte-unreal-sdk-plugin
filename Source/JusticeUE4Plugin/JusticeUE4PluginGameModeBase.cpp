// Copyright (c) 2017-2018 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "JusticeUE4PluginGameModeBase.h"
#include "Misc/ConfigCacheIni.h"

#define DO_CHECK 1

AJusticeUE4PluginGameModeBase::AJusticeUE4PluginGameModeBase()
{
}

void AJusticeUE4PluginGameModeBase::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);

	// Load Identity Interface
	OnlineSub = IOnlineSubsystem::Get();
	check(OnlineSub);

	Identity = OnlineSub->GetIdentityInterface();
	check(Identity.IsValid());

	// Setup Login delegates
	LoginCompleteHandle = Identity->AddOnLoginCompleteDelegate_Handle(
		DefaultLocalUserNum,
		FOnLoginCompleteDelegate::CreateUObject(this, &AJusticeUE4PluginGameModeBase::OnLoginCompleteDelegate));

	// Setup Logout delegates
	LogoutCompleteHandle = Identity->AddOnLogoutCompleteDelegate_Handle(
		DefaultLocalUserNum,
		FOnLogoutCompleteDelegate::CreateUObject(this, &AJusticeUE4PluginGameModeBase::OnLogoutCompleteDelegate));

}

void AJusticeUE4PluginGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	// User login credentials
	FOnlineAccountCredentials user;
	user.Id = TEXT("test@example.com");
	user.Token = TEXT("123456");
	user.Type = TEXT("PasswordGrant");
	Identity->Login(DefaultLocalUserNum, user);
}

void AJusticeUE4PluginGameModeBase::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	UE_LOG(LogOnline, Log, TEXT("EndPlay triggered."));
	Identity->Logout(DefaultLocalUserNum);
}

void AJusticeUE4PluginGameModeBase::OnLoginCompleteDelegate(int32 LocalUserNum, bool bSuccessful, const FUniqueNetId& UserId, const FString& ErrorStr)
{
	Identity->ClearOnLoginCompleteDelegate_Handle(LocalUserNum, LoginCompleteHandle);
	UE_LOG(LogOnline, Log, TEXT("Game received login %s. %s"), bSuccessful ? TEXT("successful") : TEXT("failed"), *UserId.ToString());
}

void AJusticeUE4PluginGameModeBase::OnLogoutCompleteDelegate(int32 LocalUserNum, bool bSuccessful)
{
	Identity->ClearOnLogoutCompleteDelegate_Handle(LocalUserNum, LogoutCompleteHandle);
	UE_LOG(LogOnline, Log, TEXT("Game received logout. status: %s"), bSuccessful ? TEXT("successful") : TEXT("failed"));
}
