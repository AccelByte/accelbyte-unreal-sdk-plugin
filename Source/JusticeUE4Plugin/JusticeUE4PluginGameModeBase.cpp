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
	user.Id = TEXT("test@example.com");
	user.Token = TEXT("123456");
	user.Type = TEXT("PasswordGrant");

	Identity->Login(DefaultLocalUserNum, user);
}

void AJusticeUE4PluginGameModeBase::OnLoginCompleteDelegate(int32 LocalUserNum, bool bSuccessful, const FUniqueNetId& UserId, const FString& ErrorStr)
{
	Identity->ClearOnLoginCompleteDelegate_Handle(LocalUserNum, LoginCompleteHandle);
	UE_LOG(LogOnline, Log, TEXT("Game received login %s. %s"), bSuccessful ? TEXT("successful") : TEXT("failed"), *UserId.ToString());

	///*
	////Query Example
	//FString CheckedResource = TEXT("NAMESPACE:justice:USER:t9sk283j21i8:ORDER:rec231:action:2");  //check permission of certain user to read certain order record	
	//FString CheckedResource = TEXT("NAMESPACE:evil:USER:*:ORDER:*:action:15");                    //Check any user's permission to do anything on any order record
	//FString CheckedResource = TEXT("NAMESPACE:justice:ROLEID:GODMODE");                           //check if a client/user have godmode role	
	//FString CheckedResource = TEXT("NAMESPACE:justice:ROLEID:GODMODE*");                          //check if a client/user have role prefixed with GODMODE
	//*/
	//FString CheckedResource = TEXT("NAMESPACE:justice:PAYMENT:action:8");
	//FString Result;	

	//if (Identity->GetUserAccount(UserId)->GetUserAttribute(CheckedResource, Result))
	//{
	//	//granting permissions
	//	UE_LOG(LogTemp, Warning, TEXT("Granting permission of %s"), *CheckedResource);
	//}
	//else
	//{
	//	//denying permissions
	//	UE_LOG(LogTemp, Warning, TEXT("Denying permission of %s"), *CheckedResource);
	//}	

	TArray<FString> CheckedResourceArray;

	CheckedResourceArray.Add("NAMESPACE:justice:USER:t9sk283j421i8:ORDER:rec231:action:15");//check exact query 			(exp granted)
	CheckedResourceArray.Add("NAMESPACE:justice:USER:342t2345yw:ORDER:rec231:action:15");	//check 1 wc		 			(exp granted)	
	CheckedResourceArray.Add("NAMESPACE:evil:USER:3k123l4k1:ACHIEVEMENT:ach342w:action:15");//check incorrct resource		(exp deny)
	CheckedResourceArray.Add("NAMESPACE:evil:USER:*:ORDER:*:action:15");					//check mixed passing wildcard 	(exp granted)
	CheckedResourceArray.Add("NAMESPACE:justice:ROLEID:GODMODE:action:8");					//check role with sub 			(exp granted)
	CheckedResourceArray.Add("NAMESPACE:justice:FILEUPLOAD:action:8");						//check perm wo sub 			(exp granted)
	CheckedResourceArray.Add("NAMESPACE:justice:FILEUPLOAD:action:2");						//check sub action permissio 	(exp granted)
	CheckedResourceArray.Add("NAMESPACE:justice:FILEUPLOAD:action:1");						//check insufficient permissio 	(exp deny)
	CheckedResourceArray.Add("NAMESPACE:*:FILEUPLOAD:action:10");							//wc2wc namespace			 	(exp granted)
	CheckedResourceArray.Add("NAMESPACE:*:PAYMENT:*:action:8");								//wc2 2 missmatch namespace		(exp deny)	
	CheckedResourceArray.Add("NAMESPACE:justice:ACHIEVEMENT:*:action:15");					//check missing query element 	(exp deny)
	CheckedResourceArray.Add("NAMESPACE:justice:ROLEID:GODMODE");							//check fully match flag 		(exp granted)
	CheckedResourceArray.Add("NAMESPACE:justice:ROLEID:GODMODE*");

	for (FString CheckedResource : CheckedResourceArray)
	{
		//Queriying Permissions
		//FString CheckedResource= "NAMESPACE:AnyNamespace:Resource:SubResource";	
		FString Result; //bitmask
		if (Identity->GetUserAccount(UserId)->GetUserAttribute(CheckedResource, Result))
		{
			//granting permissions
			UE_LOG(LogTemp, Warning, TEXT("Granting permission of %s, result %s"), *CheckedResource, *Result);
		}
		else
		{
			//denying permissions
			UE_LOG(LogTemp, Warning, TEXT("Denying permission of %s, result %s"), *CheckedResource, *Result);
		}
	}
}

