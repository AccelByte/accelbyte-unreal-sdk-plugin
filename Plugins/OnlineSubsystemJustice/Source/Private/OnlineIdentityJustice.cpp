// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.
#pragma clang optimize off

#include "OnlineIdentityJustice.h"
#include "Misc/CommandLine.h"
#include "Misc/Guid.h"
#include "Misc/OutputDeviceRedirector.h"
#include "OnlineSubsystem.h"
#include "IPAddress.h"
#include "SocketSubsystem.h"

FOnlineIdentityJustice::FOnlineIdentityJustice(class FOnlineSubsystemJustice* InSubsystem)
{
	if (!GConfig->GetString(TEXT("OnlineSubsystemJustice"), TEXT("BaseURL"), BaseURL, GEngineIni))
	{
		UE_LOG_ONLINE(Error, TEXT("Missing BaseURL= in [OnlineSubsystemJustice] of DefaultEngine.ini"));
	}
	BaseURL += TEXT("/iam");
	
	if (!GConfig->GetString(TEXT("OnlineSubsystemJustice"), TEXT("ClientId"), Client.Id, GEngineIni))
	{
		UE_LOG_ONLINE(Error, TEXT("Missing ClientId= in [OnlineSubsystemJustice] of DefaultEngine.ini"));
	}
	
	if (!GConfig->GetString(TEXT("OnlineSubsystemJustice"), TEXT("ClientSecret"), Client.Token, GEngineIni))
	{
		UE_LOG_ONLINE(Error, TEXT("Missing ClientSecret= in [OnlineSubsystemJustice] of DefaultEngine.ini"));
	}
}

FOnlineIdentityJustice::FOnlineIdentityJustice()
{
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
	const FString* FoundAttr = UserAttributes.Find(AttrName);
	if (FoundAttr != NULL)
	{
		OutAttrValue = *FoundAttr;
		return true;
	}
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

/*
 * Process the response to an OAuth password token grant
 */
void FOnlineIdentityJustice::LoginComplete(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bSuccessful,
										   TSharedPtr<FUserOnlineAccountJustice> UserAccountPtr, int32 LocalUserNum)
{
	FString ErrorStr;
	
	if (!bSuccessful || !Response.IsValid())
	{
		ErrorStr = FString::Printf(TEXT("Bad response. url=%s"), *Request->GetURL());
	}
	else if (!EHttpResponseCodes::IsOk(Response->GetResponseCode()))
	{
		ErrorStr = FString::Printf(TEXT("Server declined request. url=%s code=%d response=%s"),
					*Request->GetURL(), Response->GetResponseCode(), *Response->GetContentAsString());
	}
	else
	{
		FString ResponseStr = Response->GetContentAsString();
		TSharedPtr<FJsonObject> JsonObject;
		TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(ResponseStr);
		if (!FJsonSerializer::Deserialize(Reader, JsonObject) || !JsonObject.IsValid())
		{
			ErrorStr = FString::Printf(TEXT("Bad json response. url=%s code=%d response=%s"),
						*Request->GetURL(),Response->GetResponseCode(), *ResponseStr);
		}
		else if (!UserAccountPtr->Token.SetFromJsonObject(JsonObject))
		{
			ErrorStr = FString::Printf(TEXT("Missing fields in response. url=%s code=%d response=%s"),
						*Request->GetURL(),Response->GetResponseCode(), *ResponseStr);
		}
	}

	if (!ErrorStr.IsEmpty())
	{
		UE_LOG_ONLINE(Warning, TEXT("Login request failed. %s"), *ErrorStr);
		TriggerOnLoginCompleteDelegates(LocalUserNum, false, *UserAccountPtr->GetUserId(), ErrorStr);
		return;
	}

	UE_LOG_ONLINE(Verbose, TEXT("Login succeeded. %s"), *UserAccountPtr->GetUserIdStr());
	TriggerOnLoginCompleteDelegates(LocalUserNum, true, *UserAccountPtr->GetUserId(), ErrorStr);
}

bool FOnlineIdentityJustice::Login(int32 LocalUserNum, const FOnlineAccountCredentials& AccountCredentials)
{
	FString ErrorStr;
	TSharedPtr<FUserOnlineAccountJustice> UserAccountPtr;
	
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
		TSharedPtr<const FUniqueNetId>* UserId = UserIds.Find(LocalUserNum);
		if (UserId == NULL)
		{
			FUniqueNetIdString NewUserId(AccountCredentials.Id);
			UserAccountPtr = MakeShareable(new FUserOnlineAccountJustice(AccountCredentials.Id));
			UserAccountPtr->UserAttributes.Add(TEXT("id"), AccountCredentials.Id);
			
			// update/add cached entry for user
			UserAccounts.Add(NewUserId, UserAccountPtr.ToSharedRef());
			
			// keep track of user ids for local users
			UserIds.Add(LocalUserNum, UserAccountPtr->GetUserId());
		}
		else
		{
			UserAccountPtr = *UserAccounts.Find(FUniqueNetIdString(AccountCredentials.Id));
		}
	}
	
	if (!ErrorStr.IsEmpty())
	{
		UE_LOG_ONLINE(Warning, TEXT("Login request failed. %s"), *ErrorStr);
		TriggerOnLoginCompleteDelegates(LocalUserNum, false, FUniqueNetIdString(), ErrorStr);
		return false;
	}

	// Do online auth
	TSharedRef<IHttpRequest> Request = FJusticeHTTP::Get().CreateRequest();
	Request->SetURL(BaseURL + TEXT("/oauth/token"));
	Request->SetVerb(TEXT("POST"));
	Request->SetHeader(TEXT("Authorization"), FJusticeHTTP::BasicAuth(Client.Id, Client.Token));
	Request->SetHeader(TEXT("Content-Type"),  TEXT("application/x-www-form-urlencoded"));
	Request->SetHeader(TEXT("Accept"),        TEXT("application/json"));
	
	FString PasswordGrant = FString::Printf(TEXT("grant_type=password&username=%s&password=%s"),
										 *FGenericPlatformHttp::UrlEncode(AccountCredentials.Id),
										 *FGenericPlatformHttp::UrlEncode(AccountCredentials.Token));

	Request->SetContentAsString(PasswordGrant);
	Request->OnProcessRequestComplete().BindRaw(this, &FOnlineIdentityJustice::LoginComplete, UserAccountPtr, LocalUserNum);
	
	if (!Request->ProcessRequest())
	{
		UE_LOG_ONLINE(Error, TEXT("HTTP request failed for user password grant. %s"), *AccountCredentials.Id);
		return false;
	}

	return true;
}

bool FOnlineIdentityJustice::Logout(int32 LocalUserNum)
{
	TSharedPtr<const FUniqueNetId> UserId = GetUniquePlayerId(LocalUserNum);
	if (UserId.IsValid())
	{
		// remove cached user account
		UserAccounts.Remove(FUniqueNetIdString(*UserId));
		// remove cached user id
		UserIds.Remove(LocalUserNum);
		// not async but should call completion delegate anyway
		TriggerOnLogoutCompleteDelegates(LocalUserNum, true);

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
	TSharedPtr<FUserOnlineAccount> Result;

	FUniqueNetIdString StringUserId(UserId);
	const TSharedRef<FUserOnlineAccountJustice>* FoundUserAccount = UserAccounts.Find(StringUserId);
	if (FoundUserAccount != NULL)
	{
		Result = *FoundUserAccount;
	}

	return Result;
}

TArray<TSharedPtr<FUserOnlineAccount> > FOnlineIdentityJustice::GetAllUserAccounts() const
{
	TArray<TSharedPtr<FUserOnlineAccount> > Result;
	
	for (TMap<FUniqueNetIdString, TSharedRef<FUserOnlineAccountJustice>>::TConstIterator It(UserAccounts); It; ++It)
	{
		Result.Add(It.Value());
	}

	return Result;
}

TSharedPtr<const FUniqueNetId> FOnlineIdentityJustice::GetUniquePlayerId(int32 LocalUserNum) const
{
	const TSharedPtr<const FUniqueNetId>* FoundId = UserIds.Find(LocalUserNum);
	if (FoundId != NULL)
	{
		return *FoundId;
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

FPlatformUserId FOnlineIdentityJustice::GetPlatformUserIdFromUniqueNetId(const FUniqueNetId& UniqueNetId)
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

FString FOnlineIdentityJustice::GetAuthType() const
{
	return TEXT("Justice OAuth");
}

bool FJusticeOAuthToken::SetFromJsonObject(const TSharedPtr<FJsonObject> GrantResponse)
{
	// User token (only user tokens have refresh tokens)
	if (GrantResponse->HasField(TEXT("refresh_token")))
	{
		if (GrantResponse->TryGetStringField(TEXT("access_token"),  AccessToken)  &&
			GrantResponse->TryGetStringField(TEXT("refresh_token"), RefreshToken) &&
			GrantResponse->TryGetStringField(TEXT("token_type"),    TokenType)    &&
			GrantResponse->TryGetNumberField(TEXT("expires_in"),    ExpiresIn))
		{
			return true;
		}
	}
	else
	{
		// Client token (no refresh token)
		if (GrantResponse->TryGetStringField(TEXT("access_token"), AccessToken) &&
			GrantResponse->TryGetStringField(TEXT("token_type"),   TokenType)   &&
			GrantResponse->TryGetNumberField(TEXT("expires_in"),   ExpiresIn))
		{
			return true;
		}
	}
	return false;
}

