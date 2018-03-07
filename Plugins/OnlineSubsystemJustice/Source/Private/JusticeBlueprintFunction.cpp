// Copyright (c) 2017-2018 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "JusticeBlueprintFunction.h"
#include "FJusticeComponent.h"

const int32 DefaultLocalUserNum = 0;  

bool UJusticeBlueprintFunction::Logout()
{
	const IOnlineSubsystem* OnlineSub;
	IOnlineIdentityPtr Identity;

	OnlineSub = IOnlineSubsystem::Get();
	check(OnlineSub);
	Identity = OnlineSub->GetIdentityInterface();

	FDelegateHandle LogoutCompleteHandle;

	// Setup Logout delegates
	LogoutCompleteHandle = Identity->AddOnLogoutCompleteDelegate_Handle(
		DefaultLocalUserNum,
		FOnLogoutCompleteDelegate::CreateLambda([Identity, &LogoutCompleteHandle](int32 LocalUserNum, bool bSuccessful) {
		if (LocalUserNum == DefaultLocalUserNum)
		{
			Identity->ClearOnLogoutCompleteDelegate_Handle(LocalUserNum, LogoutCompleteHandle);
			UE_LOG(LogOnline, Log, TEXT("Game received logout via Blueprint. status: %s"), bSuccessful ? TEXT("successful") : TEXT("failed"));
		}
	}));

	Identity->Logout(DefaultLocalUserNum);
	return true;
}

bool UJusticeBlueprintFunction::Login(FString UserId, FString Password, FLoginCompleteMulticastDelegate onComplete)
{
	UE_LOG_ONLINE(VeryVerbose, TEXT("Bluprint Login Called User=%s  Pass=%s"), *UserId, *Password);

	const IOnlineSubsystem* OnlineSub;
	IOnlineIdentityPtr Identity;

	OnlineSub = IOnlineSubsystem::Get();
	check(OnlineSub);
	Identity = OnlineSub->GetIdentityInterface();

	

	FDelegateHandle LoginCompleteHandle;
	LoginCompleteHandle = Identity->AddOnLoginCompleteDelegate_Handle(
		DefaultLocalUserNum,
		FOnLoginCompleteDelegate::CreateLambda([onComplete, Identity, &LoginCompleteHandle](int32 LocalUserNum, bool bSuccessful, const FUniqueNetId& UserId, const FString& ErrorStr) {			
		if (LocalUserNum == DefaultLocalUserNum)
		{
			Identity->ClearOnLoginCompleteDelegate_Handle(LocalUserNum, LoginCompleteHandle);
			UE_LOG(LogOnline, VeryVerbose, TEXT("Game received login via Blueprint. Status: %s.User ID: %s"), bSuccessful ? TEXT("successful") : TEXT("failed"), *UserId.ToString());
			FString token = Identity->GetAuthToken(LocalUserNum);			
			TSharedPtr<const FUniqueNetId> playerID =  Identity->GetUniquePlayerId(LocalUserNum);
			UE_LOG(LogOnline, VeryVerbose, TEXT("Justice UserID %s"),  *playerID->ToString());
			onComplete.Execute(bSuccessful, token, ErrorStr);
		}
		})
	);

	FOnlineAccountCredentials user;
	user.Id = UserId;
	user.Token = Password;
	user.Type = TEXT("password");
	Identity->Login(DefaultLocalUserNum, user);

	return true;
}

UFJusticeComponent*  UJusticeBlueprintFunction::GetJusticeComponent()
{
	return UFJusticeComponent::GetInstance();
}

