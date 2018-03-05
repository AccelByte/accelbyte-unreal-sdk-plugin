// Copyright (c) 2017-2018 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "OnlineIdentityJustice.h"
#include "Misc/CommandLine.h"
#include "Misc/Guid.h"
#include "Misc/OutputDeviceRedirector.h"
#include "OnlineSubsystem.h"
#include "IPAddress.h"
#include "SocketSubsystem.h"
#include "AWSXRayJustice.h"

FOnlineIdentityJustice::FOnlineIdentityJustice(class FOnlineSubsystemJustice* InSubsystem)
{
	if (!GConfig->GetString(TEXT("OnlineSubsystemJustice"), TEXT("BaseURL"), BaseURL, GEngineIni))
	{
		UE_LOG_ONLINE(Error, TEXT("Missing BaseURL= in [OnlineSubsystemJustice] of DefaultEngine.ini"));
	}
	
	if (!GConfig->GetString(TEXT("OnlineSubsystemJustice"), TEXT("ClientId"), Client.Id, GEngineIni))
	{
		UE_LOG_ONLINE(Error, TEXT("Missing ClientId= in [OnlineSubsystemJustice] of DefaultEngine.ini"));
	}
	
	if (!GConfig->GetString(TEXT("OnlineSubsystemJustice"), TEXT("ClientSecret"), Client.Token, GEngineIni))
	{
		UE_LOG_ONLINE(Error, TEXT("Missing ClientSecret= in [OnlineSubsystemJustice] of DefaultEngine.ini"));
	}
	OnlineAsyncTaskManagerJustice = InSubsystem->GetOnlineAsyncTaskManager();
}

FOnlineIdentityJustice::FOnlineIdentityJustice()
{
}

void FOnlineIdentityJustice::OnRefreshToken(FDateTime time, int32 nextTick, FOnlineSubsystemJustice * onlineSubsytemJustice)
{
	for (TSharedPtr<FUserOnlineAccount> UserAccountPtr : GetAllUserAccounts())
	{				
		FOnlineAccountCredentials Credentials;
		Credentials.Id = UserAccountPtr->GetUserId()->ToString();
		int32 LocalUserNum = ((FUserOnlineAccountJustice*)UserAccountPtr.Get())->LocalUserNum;
		if (!Login(LocalUserNum, Credentials))
		{
			UE_LOG_ONLINE(Warning, TEXT("Failed to refresh token. User=%s"), *UserAccountPtr->GetUserId()->ToString());
		}
	}
}
FOnlineIdentityJustice::~FOnlineIdentityJustice()
{
}

bool FUserOnlineAccountJustice::GetAuthAttribute(const FString& AttrName, FString& OutAttrValue) const
{
	const FString* FoundAttr = AdditionalAuthData.Find(AttrName);
	if (FoundAttr != NULL)
	{
		OutAttrValue = *FoundAttr;
		return true;
	}
	return false;
}

bool FUserOnlineAccountJustice::GetUserAttribute(const FString& AttrName, FString& OutAttrValue) const
{
	//instant return attribute value for other usage
	TArray<FString> FullQuery;
	const FString* FoundAttr;

	AttrName.ParseIntoArray(FullQuery, TEXT(":"), true);
	if (FullQuery.Num() < 2)
	{
		FoundAttr = UserAttributes.Find(AttrName);
		if (FoundAttr != NULL)
		{
			OutAttrValue = *FoundAttr;
			return true;
		}
	}

	//check the action bit and make clean copy of query
	int32 CheckedActionBit = -1;
	TArray<FString> Query;
	TArray<FString> KeyArray;
	if (FullQuery[FullQuery.Num() - 2] == TEXT("action"))
	{
		CheckedActionBit = FCString::Atoi(*FullQuery.Last());
		for (int32 i = 0; i < FullQuery.Num() - 2; i++)
		{
			Query.Add(FullQuery[i]);
		}
	}
	else
	{
		Query.Append(FullQuery);
	}

	//permission query without action specified will be denied instantly
	if (!AttrName.Contains(TEXT("ROLE"), ESearchCase::CaseSensitive, ESearchDir::FromStart) && CheckedActionBit==-1)
	{
		OutAttrValue = TEXT("Action must be specified for permission query");
		return false;
	}

	//find perms and action bit to compare
	FString CheckedTrails;
	UserAttributes.GetKeys(KeyArray);
	bool bMatchFound = false;
	for (FString Key : KeyArray)
	{
		//check for match possibility by the element number
		TArray<FString> Permission;
		Key.ParseIntoArray(Permission, TEXT(":"), true);
		if (Permission.Num() != Query.Num())
		{
			continue;
		}

		//check the element of each query and permission position
		bMatchFound = true;
		for (int i = 0; i < Permission.Num(); i++)
		{
			//check if any match or wild card
			if (Permission[i].Equals(Query[i], ESearchCase::CaseSensitive)
				|| Permission[i].Equals(*FString(TEXT("*")), ESearchCase::CaseSensitive)
				|| Query[i].Equals(*FString(TEXT("*")), ESearchCase::CaseSensitive))
			{
				continue;
			}

			//if nothing match check the string trails
			CheckedTrails = Query[i];
			if (!(
				(CheckedTrails.Len() > 1)
				&& CheckedTrails.RemoveFromEnd(TEXT("*"), ESearchCase::CaseSensitive)
				&& Permission[i].Contains(CheckedTrails, ESearchCase::CaseSensitive, ESearchDir::FromStart)
				))
			{
				bMatchFound = false;
				break;
			}
		}

		//check if bMatchFond has been invalidated
		if (bMatchFound)
		{
			FoundAttr = UserAttributes.Find(Key);
			if (FoundAttr != NULL)
			{
				OutAttrValue = *FoundAttr;
				int Verify = (CheckedActionBit & ~FCString::Atoi(**UserAttributes.Find(Key)));
				if (CheckedActionBit == -1 || Verify == 0)
				{
					OutAttrValue = Key +":action:"+ *FoundAttr;
					return true;
				}
				OutAttrValue = "Match found for "+Key+" but action permitted is "+*UserAttributes.Find(Key);
				return false;
			}
		}
	}
	OutAttrValue = TEXT("No match found");
	return false;
}

bool FUserOnlineAccountJustice::SetUserAttribute(const FString& AttrName, const FString& AttrValue)
{
	const FString* FoundAttr = UserAttributes.Find(AttrName);
	if (FoundAttr == NULL || *FoundAttr != AttrValue)
	{
		UserAttributes.Add(AttrName, AttrValue);
		return true;
	}
	return false;
}

inline FString GenerateRandomUserId(int32 LocalUserNum)
{
	FString HostName;
	if (!ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->GetHostName(HostName))
	{
		// could not get hostname, use address
		bool bCanBindAll;
		TSharedPtr<class FInternetAddr> Addr = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->GetLocalHostAddr(*GLog, bCanBindAll);
		HostName = Addr->ToString(false);
	}

	const bool bForceUniqueId = FParse::Param( FCommandLine::Get(), TEXT( "StableJusticeID" ) );
	
	if ( ( GIsFirstInstance || bForceUniqueId ) && !GIsEditor )
	{
		// When possible, return a stable user id
		return FString::Printf( TEXT( "%s-%s" ), *HostName, *FPlatformMisc::GetLoginId().ToUpper() );
	}

	// If we're not the first instance (or in the editor), return truly random id
	return FString::Printf( TEXT( "%s-%s" ), *HostName, *FGuid::NewGuid().ToString() );
}

bool FOnlineIdentityJustice::Login(int32 LocalUserNum, const FOnlineAccountCredentials& AccountCredentials)
{
	FString ErrorStr;
	TSharedPtr<FUserOnlineAccountJustice> UserAccountPtr;
	TSharedRef<IHttpRequest> Request = FHttpModule::Get().CreateRequest();
	TSharedRef<FAWSXRayJustice> RequestTrace = MakeShareable(new FAWSXRayJustice());

	if (LocalUserNum < 0 || LocalUserNum >= MAX_LOCAL_PLAYERS)
	{
		ErrorStr = FString::Printf(TEXT("Invalid LocalUserNum=%d"), LocalUserNum);
	}
	else if (AccountCredentials.Id.IsEmpty())
	{
		ErrorStr = TEXT("Invalid account id, string empty");
	}
	else
	{
		TSharedPtr<FUserOnlineAccountJustice>* UserAccountSearchPtr = UserAccounts.Find(LocalUserNum);
		if (UserAccountSearchPtr == nullptr)
		{
			FUniqueNetIdString NewUserId(AccountCredentials.Id);
			UserAccountPtr = MakeShareable(new FUserOnlineAccountJustice(AccountCredentials.Id, LocalUserNum));			
			UserAccountPtr->SetUserAttribute(TEXT("id"), AccountCredentials.Id);
			UserAccounts.Add(LocalUserNum, UserAccountPtr);		
		}
		else
		{
			UserAccountPtr = *UserAccountSearchPtr;
		}


		Request->SetURL(BaseURL + TEXT("/iam/oauth/token"));
		Request->SetHeader(TEXT("Authorization"), FHTTPJustice::BasicAuth(Client.Id, Client.Token));
		Request->SetVerb(TEXT("POST"));
		Request->SetHeader(TEXT("Content-Type"),  TEXT("application/x-www-form-urlencoded; charset=utf-8"));
		Request->SetHeader(TEXT("Accept"),        TEXT("application/json"));
		
		Request->SetHeader(TEXT("X-Amzn-TraceId"), RequestTrace->XRayTraceID());

		// Refresh grant
		if (UserAccountPtr->Token.ShouldRefresh())
		{
			FString Grant = FString::Printf(TEXT("grant_type=refresh_token&refresh_token=%s"), *FGenericPlatformHttp::UrlEncode(UserAccountPtr->Token.RefreshToken));
			Request->SetContentAsString(Grant);
			Request->OnProcessRequestComplete().BindRaw(this, &FOnlineIdentityJustice::TokenRefreshGrantComplete, UserAccountPtr, LocalUserNum, RequestTrace);
			if (!Request->ProcessRequest())
			{
				ErrorStr = FString::Printf(TEXT("request failed. URL=%s"), *Request->GetURL());
			}
			UE_LOG_ONLINE(VeryVerbose, TEXT("FOnlineIdentityJustice::Login(): refresh grant User=%s %s"), *AccountCredentials.Id, *RequestTrace->ToString());
		}
		// Password grant
		else if (!AccountCredentials.Token.IsEmpty())
		{
			FString Grant = FString::Printf(TEXT("grant_type=password&username=%s&password=%s"),
				*FGenericPlatformHttp::UrlEncode(AccountCredentials.Id), *FGenericPlatformHttp::UrlEncode(AccountCredentials.Token));

			Request->SetContentAsString(Grant);
			Request->OnProcessRequestComplete().BindRaw(this, &FOnlineIdentityJustice::TokenPasswordGrantComplete, UserAccountPtr, LocalUserNum, RequestTrace);						
			if (!Request->ProcessRequest())
			{
				ErrorStr = FString::Printf(TEXT("request failed. URL=%s"), *Request->GetURL());
			}
	
			UE_LOG_ONLINE(VeryVerbose, TEXT("FOnlineIdentityJustice::Login(): password grant User=%s %s"), *AccountCredentials.Id, *RequestTrace->ToString());
		}
	}

	if (!ErrorStr.IsEmpty())
	{
		UE_LOG_ONLINE(Warning, TEXT("Login failed. User=%s Error=%s %s ReqTime=%.3f"), *AccountCredentials.Id, *ErrorStr, *RequestTrace->ToString(), Request->GetElapsedTime());
		TriggerOnLoginCompleteDelegates(LocalUserNum, false, *UserAccountPtr->GetUserId(), ErrorStr);
		return false;
	}

	UE_LOG_ONLINE(VeryVerbose, TEXT("FOnlineIdentityJustice::Login(): request dispatched. User=%s %s"), *AccountCredentials.Id, *RequestTrace->ToString());

	return true;
}

void FOnlineIdentityJustice::TokenRefreshGrantComplete(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bSuccessful, TSharedPtr<FUserOnlineAccountJustice> UserAccountPtr, int32 LocalUserNum, TSharedRef<FAWSXRayJustice> RequestTrace)
{
	FString ErrorStr;

	if (!bSuccessful || !Response.IsValid())
	{
		ErrorStr = TEXT("request failed");
		UserAccountPtr->Token.ScheduleBackoffRefresh();
	}
	else
	{
		switch (Response->GetResponseCode())
		{
		case EHttpResponseCodes::Ok:
			{
				FString ResponseStr = Response->GetContentAsString();
				TSharedPtr<FJsonObject> JsonObject;
				TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(ResponseStr);
				if (FJsonSerializer::Deserialize(Reader, JsonObject) && JsonObject.IsValid())
				{
					if (UserAccountPtr->Token.FromJson(JsonObject))
					{
						UserAccountPtr->Token.SetLastRefreshTimeToNow();
						UserAccountPtr->Token.ScheduleNormalRefresh();
						UserAccountPtr->SetUserId(UserAccountPtr->Token.UserId);

						OnRefreshTokenLogDelegate.BindRaw(this, &FOnlineIdentityJustice::OnRefreshToken);
						OnlineAsyncTaskManagerJustice->UpdateDelegateSchedule(FTaskTypeJustice::IdentityRefresh,
																			  FTimespan::FromSeconds((UserAccountPtr->Token.ExpiresIn + 1) * 0.8),
																		      UserAccountPtr->Token.NextTokenRefreshUtc, 
																			  OnRefreshTokenLogDelegate);
					}
					else
					{
						ErrorStr = TEXT("unable to deserlize response from json object");
					}
				}
				else
				{
					ErrorStr = TEXT("unable to deserlize response from server");
				}
				
				if (!ErrorStr.IsEmpty())
				{
					UserAccountPtr->Token.ScheduleBackoffRefresh();
				}
			}
			break;

		case EHttpResponseCodes::Forbidden:
		case EHttpResponseCodes::Denied:
			ErrorStr = FString::Printf(TEXT("request denied Code=%d"), Response->GetResponseCode());
			UserAccountPtr->Token = FOAuthTokenJustice();
			TriggerOnLoginCompleteDelegates(LocalUserNum, false, *UserAccountPtr->GetUserId(), *ErrorStr);
			TriggerOnLoginChangedDelegates(LocalUserNum);
			break;

		default:
			UserAccountPtr->Token.ScheduleBackoffRefresh();
			ErrorStr = FString::Printf(TEXT("unexpected reponse Code=%d"), Response->GetResponseCode());
			break;
		}
	}

	if (!ErrorStr.IsEmpty())
	{
		UE_LOG_ONLINE(Warning, TEXT("Token refresh failed. User=%s Error=%s %s %s ReqTime=%.3f"),
					  *UserAccountPtr->GetUserIdStr(), *ErrorStr, *UserAccountPtr->Token.GetRefreshStr(), *RequestTrace->ToString(), Request->GetElapsedTime());
		UserAccountPtr->Token = FOAuthTokenJustice();
		return;
	}

	UE_LOG_ONLINE(Log, TEXT("Token refresh successful. User=%s %s %s ReqTime=%.3f"),
				  *UserAccountPtr->GetUserIdStr(), *UserAccountPtr->Token.GetRefreshStr(), *RequestTrace->ToString(), Request->GetElapsedTime());
	
}

void FOnlineIdentityJustice::TokenLogoutComplete(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bSuccessful, TSharedPtr<FUserOnlineAccountJustice> UserAccountPtr, int32 LocalUserNum, TSharedRef<FAWSXRayJustice> RequestTrace)
{
	FString ErrorStr;

	if (!bSuccessful || !Response.IsValid())
	{
		ErrorStr = TEXT("request failed");
	}
	else
	{
		if (Response->GetResponseCode() == EHttpResponseCodes::Ok)
		{
			UE_LOG_ONLINE(Log, TEXT("Token logout receive success response "));
		}
		else
		{
			ErrorStr = FString::Printf(TEXT("unexpcted response Code=%d"), Response->GetResponseCode());
		}
	}

	if (!ErrorStr.IsEmpty())
	{
		UE_LOG_ONLINE(Error, TEXT("Token logout. User=%s Error=%s %s %s ReqTime=%.3f"),
			*UserAccountPtr->GetUserIdStr(), *ErrorStr, *UserAccountPtr->Token.GetRefreshStr(), *RequestTrace->ToString(), Request->GetElapsedTime());
		UserAccountPtr->Token = FOAuthTokenJustice();
		TriggerOnLogoutCompleteDelegates(LocalUserNum, false);
		return;
	}

	UserAccounts.Remove(LocalUserNum);
	TriggerOnLogoutCompleteDelegates(LocalUserNum, true);
}

void FOnlineIdentityJustice::TokenPasswordGrantComplete(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bSuccessful, TSharedPtr<FUserOnlineAccountJustice> UserAccountPtr, int32 LocalUserNum, TSharedRef<FAWSXRayJustice> RequestTrace)
{
	FString ErrorStr;

	if (!bSuccessful || !Response.IsValid())
	{
		ErrorStr = TEXT("request failed");
	}
	else
	{
		switch (Response->GetResponseCode())
		{
			case EHttpResponseCodes::Ok:
			{
				FString ResponseStr = Response->GetContentAsString();

				TSharedPtr<FJsonObject> JsonObject;
				TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(ResponseStr);
				if (FJsonSerializer::Deserialize(Reader, JsonObject) && JsonObject.IsValid())
				{
					if (UserAccountPtr->Token.FromJson(JsonObject))
					{
						TArray<TSharedPtr<FJsonValue>> PermissionArray = JsonObject->GetArrayField(TEXT("permissions"));						
						for (TSharedPtr<FJsonValue> Permission : PermissionArray)
						{
							TSharedPtr<FJsonObject> JsonPermissionObject = Permission->AsObject();
							FString Resource = JsonPermissionObject->GetStringField(TEXT("Resource"));
							int32 Action = JsonPermissionObject->GetIntegerField(TEXT("Action"));
							FPermissionJustice PermissionObject = FPermissionJustice(Resource, Action);
							UserAccountPtr->Token.Permissions.Add(PermissionObject);
							UserAccountPtr->SetUserAttribute(Resource, FString::FromInt(Action));
						}
						UserAccountPtr->Token.SetLastRefreshTimeToNow();
						UserAccountPtr->Token.ScheduleNormalRefresh();		
						UserAccountPtr->SetUserId(UserAccountPtr->Token.UserId);

						OnRefreshTokenLogDelegate.BindRaw(this, &FOnlineIdentityJustice::OnRefreshToken);
						OnlineAsyncTaskManagerJustice->UpdateDelegateSchedule(FTaskTypeJustice::IdentityRefresh,
							FTimespan::FromSeconds((UserAccountPtr->Token.ExpiresIn + 1) * 0.8),
							UserAccountPtr->Token.NextTokenRefreshUtc,
							OnRefreshTokenLogDelegate);

					}
					else
					{
						ErrorStr = TEXT("unable to deserlize response from json object");
					}
				}
				else
				{
					ErrorStr = TEXT("unable to deserlize response from server");
				}
			}
			break;
				
			default:
				ErrorStr = FString::Printf(TEXT("unexpcted response Code=%d"), Response->GetResponseCode());
			break;
		}
	}
	
	if (!ErrorStr.IsEmpty())
	{
		UE_LOG_ONLINE(Warning, TEXT("Token password grant failed. User=%s Error=%s %s %s ReqTime=%.3f"),
					  *UserAccountPtr->GetUserIdStr(), *ErrorStr, *UserAccountPtr->Token.GetRefreshStr(), *RequestTrace->ToString(), Request->GetElapsedTime());
		UserAccountPtr->Token = FOAuthTokenJustice();
		TriggerOnLoginCompleteDelegates(LocalUserNum, false, *UserAccountPtr->GetUserId(), *ErrorStr);
		TriggerOnLoginChangedDelegates(LocalUserNum);
		return;
	}
	
	UE_LOG_ONLINE(Log, TEXT("Token password grant successful. UserId=%s %s %s ReqTime=%.3f"),
				  *UserAccountPtr->GetUserIdStr(), *UserAccountPtr->Token.GetRefreshStr(), *RequestTrace->ToString(), Request->GetElapsedTime());
	TriggerOnLoginCompleteDelegates(LocalUserNum, true, *UserAccountPtr->GetUserId(), *ErrorStr);
	TriggerOnLoginChangedDelegates(LocalUserNum);
}

bool FOnlineIdentityJustice::Logout(int32 LocalUserNum)
{
	FString ErrorStr;
	TSharedPtr<FUserOnlineAccountJustice> UserAccountPtr;
	TSharedPtr<FUserOnlineAccountJustice>* UserAccountSearch = UserAccounts.Find(LocalUserNum);
	if (UserAccountSearch != nullptr)
	{
		UserAccountPtr = *UserAccountSearch;
		TSharedRef<IHttpRequest> Request = FHttpModule::Get().CreateRequest();
		TSharedRef<FAWSXRayJustice> RequestTrace = MakeShareable(new FAWSXRayJustice());

		Request->SetURL(BaseURL + TEXT("/iam/oauth/revoke"));
		Request->SetHeader(TEXT("Authorization"), FHTTPJustice::BasicAuth(Client.Id, Client.Token));
		Request->SetVerb(TEXT("POST"));
		Request->SetHeader(TEXT("Content-Type"), TEXT("application/x-www-form-urlencoded; charset=utf-8"));
		Request->SetHeader(TEXT("Accept"), TEXT("application/json"));

		Request->SetHeader(TEXT("X-Amzn-TraceId"), RequestTrace->XRayTraceID());

		FString Grant = FString::Printf(TEXT("token=%s"), *FGenericPlatformHttp::UrlEncode(UserAccountPtr->Token.AccessToken));
		Request->SetContentAsString(Grant);
		Request->OnProcessRequestComplete().BindRaw(this, &FOnlineIdentityJustice::TokenLogoutComplete, UserAccountPtr, LocalUserNum, RequestTrace);
		if (!Request->ProcessRequest())
		{
			ErrorStr = FString::Printf(TEXT("request failed. URL=%s"), *Request->GetURL());
		}
		UE_LOG_ONLINE(VeryVerbose, TEXT("FOnlineIdentityJustice::Logout() UserId=%s"), *UserAccountPtr->Token.UserId);
		return true;
	}
	else
	{
		UE_LOG_ONLINE(Warning, TEXT("No logged in user found for LocalUserNum=%d."),
			LocalUserNum);
		TriggerOnLogoutCompleteDelegates(LocalUserNum, false);
	}
	return false;
}

bool FOnlineIdentityJustice::AutoLogin(int32 LocalUserNum)
{
	FString LoginStr;
	FString PasswordStr;
	FString TypeStr;

	FParse::Value(FCommandLine::Get(), TEXT("AUTH_LOGIN="), LoginStr);
	FParse::Value(FCommandLine::Get(), TEXT("AUTH_PASSWORD="), PasswordStr);
	FParse::Value(FCommandLine::Get(), TEXT("AUTH_TYPE="), TypeStr);
	
	if (!LoginStr.IsEmpty())
	{
		if (!PasswordStr.IsEmpty())
		{
			if (!TypeStr.IsEmpty())
			{
				return Login(0, FOnlineAccountCredentials(TypeStr, LoginStr, PasswordStr));
			}
			else
			{
				UE_LOG_ONLINE(Warning, TEXT("AutoLogin missing AUTH_TYPE=<type>."));
			}
		}
		else
		{
			UE_LOG_ONLINE(Warning, TEXT("AutoLogin missing AUTH_PASSWORD=<password>."));
		}
	}
	else
	{
		UE_LOG_ONLINE(Warning, TEXT("AutoLogin missing AUTH_LOGIN=<login id>."));
	}
	return false;
}

TSharedPtr<FUserOnlineAccount> FOnlineIdentityJustice::GetUserAccount(const FUniqueNetId& UserId) const
{
	for (TMap<int32, TSharedPtr<FUserOnlineAccountJustice>>::TConstIterator It(UserAccounts); It; ++It)
	{
		if (It.Value()->GetUserId().Get() == UserId) 
		{
			return It.Value();
		}
	}
	return NULL;
}

TArray<TSharedPtr<FUserOnlineAccount> > FOnlineIdentityJustice::GetAllUserAccounts() const
{
	TArray<TSharedPtr<FUserOnlineAccount> > Result;
	
	for (TMap<int32, TSharedPtr<FUserOnlineAccountJustice>>::TConstIterator It(UserAccounts); It; ++It)
	{
		Result.Add(It.Value());
	}

	return Result;
}

TSharedPtr<const FUniqueNetId> FOnlineIdentityJustice::GetUniquePlayerId(int32 LocalUserNum) const
{
	const TSharedPtr<FUserOnlineAccountJustice>* FoundId = UserAccounts.Find(LocalUserNum);
	if (FoundId != NULL)
	{
		return FoundId->Get()->GetUserId();
	}
	return NULL;
}

TSharedPtr<const FUniqueNetId> FOnlineIdentityJustice::CreateUniquePlayerId(uint8* Bytes, int32 Size)
{
	if (Bytes != NULL && Size > 0)
	{
		FString StrId(Size, (TCHAR*)Bytes);
		return MakeShareable(new FUniqueNetIdString(StrId));
	}
	return NULL;
}

TSharedPtr<const FUniqueNetId> FOnlineIdentityJustice::CreateUniquePlayerId(const FString& Str)
{
	return MakeShareable(new FUniqueNetIdString(Str));
}

ELoginStatus::Type FOnlineIdentityJustice::GetLoginStatus(int32 LocalUserNum) const
{
	TSharedPtr<const FUniqueNetId> UserId = GetUniquePlayerId(LocalUserNum);
	if (UserId.IsValid())
	{
		return GetLoginStatus(*UserId);
	}
	return ELoginStatus::NotLoggedIn;
}

ELoginStatus::Type FOnlineIdentityJustice::GetLoginStatus(const FUniqueNetId& UserId) const 
{
	TSharedPtr<FUserOnlineAccount> UserAccount = GetUserAccount(UserId);
	if (UserAccount.IsValid() &&
		UserAccount->GetUserId()->IsValid())
	{
		return ELoginStatus::LoggedIn;
	}
	return ELoginStatus::NotLoggedIn;
}

FString FOnlineIdentityJustice::GetPlayerNickname(int32 LocalUserNum) const
{
	TSharedPtr<const FUniqueNetId> UniqueId = GetUniquePlayerId(LocalUserNum);
	if (UniqueId.IsValid())
	{
		return UniqueId->ToString();
	}

	return TEXT("JusticeUser");
}

FString FOnlineIdentityJustice::GetPlayerNickname(const FUniqueNetId& UserId) const
{
	return UserId.ToString();
}

FString FOnlineIdentityJustice::GetAuthToken(int32 LocalUserNum) const
{
	TSharedPtr<const FUniqueNetId> UserId = GetUniquePlayerId(LocalUserNum);
	if (UserId.IsValid())
	{
		TSharedPtr<FUserOnlineAccount> UserAccount = GetUserAccount(*UserId);
		if (UserAccount.IsValid())
		{
			return UserAccount->GetAccessToken();
		}
	}
	return FString();
}

void FOnlineIdentityJustice::GetUserPrivilege(const FUniqueNetId& UserId, EUserPrivileges::Type Privilege, const FOnGetUserPrivilegeCompleteDelegate& Delegate)
{
	Delegate.ExecuteIfBound(UserId, Privilege, (uint32)EPrivilegeResults::NoFailures);
}

FPlatformUserId FOnlineIdentityJustice::GetPlatformUserIdFromUniqueNetId(const FUniqueNetId& UniqueNetId) const
{
	for (int i = 0; i < MAX_LOCAL_PLAYERS; ++i)
	{
		auto CurrentUniqueId = GetUniquePlayerId(i);
		if (CurrentUniqueId.IsValid() && (*CurrentUniqueId == UniqueNetId))
		{
			return i;
		}
	}

	return PLATFORMUSERID_NONE;
}

void FOnlineIdentityJustice::RevokeAuthToken(const FUniqueNetId & UserId, const FOnRevokeAuthTokenCompleteDelegate & Delegate)
{
	unimplemented();
}

FString FOnlineIdentityJustice::GetAuthType() const
{
	return TEXT("Justice OAuth");
}