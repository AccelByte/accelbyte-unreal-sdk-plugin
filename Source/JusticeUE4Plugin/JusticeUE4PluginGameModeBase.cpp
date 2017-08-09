// Fill out your copyright notice in the Description page of Project Settings.

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
}

void AJusticeUE4PluginGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	// User login credentials
	FOnlineAccountCredentials user;
	user.Id    = TEXT("test@example.com");
	user.Token = TEXT("123456");
	user.Type  = TEXT("PasswordGrant");
	
	Identity->Login(DefaultLocalUserNum, user);
}

void AJusticeUE4PluginGameModeBase::OnLoginCompleteDelegate(int32 LocalUserNum, bool bSuccessful, const FUniqueNetId& UserId, const FString& ErrorStr)
{
	Identity->ClearOnLoginCompleteDelegate_Handle(LocalUserNum, LoginCompleteHandle);
	UE_LOG(LogOnline, Log, TEXT("Game received login %s. %s"), bSuccessful ? TEXT("successful") : TEXT("failed"), *UserId.ToString());
	
	//TEST CASES
	TArray<FString> CheckedResourceArray;
	CheckedResourceArray.Add("{namespace}_{userId}_ORDER_*");
	CheckedResourceArray.Add("{namespace}_ORDER_*");
	CheckedResourceArray.Add("{namespace}_ORDER_orderID90");
	CheckedResourceArray.Add("{namespace}_{userId}_GODMODE_*");
	CheckedResourceArray.Add("{namespace}_{userId}_GODMODE_Ammo");
	CheckedResourceArray.Add("{namespace}_{userId}_GODMODE_Health");
	CheckedResourceArray.Add("{namespace}_PAYMENT_paymentID23");
	CheckedResourceArray.Add("{namespace}_CANCONNECT_*");
	CheckedResourceArray.Add("{namespace}_FULFILLMENT_*");
	CheckedResourceArray.Add("{namespace}_{userId}_FULFILLMENT_fulfillmentID1231");
	FString CheckedAction = "*"; //c:create r:read u:update d:delete *:all

	for (FString CheckedResource : CheckedResourceArray)
	{
		if (Identity->GetUserAccount(UserId)->GetUserAttribute(CheckedResource, CheckedAction))
		{
			//granting permissions
			UE_LOG(LogTemp, Warning, TEXT("Granting permission of %s"), *CheckedResource);
		}
		else
		{
			//denying permissions
			UE_LOG(LogTemp, Warning, TEXT("Denying permission of %s"), *CheckedResource);
		}
	}
}

