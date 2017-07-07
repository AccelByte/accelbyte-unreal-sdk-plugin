// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#pragma once
 
#include "CoreMinimal.h"
#include "UObject/CoreOnline.h"
#include "OnlineSubsystemTypes.h"
#include "Interfaces/OnlineIdentityInterface.h"

/**
 * Info associated with an user account generated by this online service
 */
class FUserOnlineAccountJustice : 
	public FUserOnlineAccount
{

public:

	// FOnlineUser
	
	virtual TSharedRef<const FUniqueNetId> GetUserId() const override { return UserIdPtr; }
	virtual FString GetRealName() const override { return TEXT("DummyRealName"); }
	virtual FString GetDisplayName(const FString& Platform = FString()) const override  { return TEXT("DummyDisplayName"); }
	virtual bool GetUserAttribute(const FString& AttrName, FString& OutAttrValue) const override;
	virtual bool SetUserAttribute(const FString& AttrName, const FString& AttrValue) override;

	// FUserOnlineAccount

	virtual FString GetAccessToken() const override { return TEXT("DummyAuthTicket"); }
	virtual bool GetAuthAttribute(const FString& AttrName, FString& OutAttrValue) const override;

	// FUserOnlineAccountJustice

	FUserOnlineAccountJustice(const FString& InUserId=TEXT("")) 
		: UserIdPtr(new FUniqueNetIdString(InUserId))
	{ }

	virtual ~FUserOnlineAccountJustice()
	{
	}

	/** User Id represented as a FUniqueNetId */
	TSharedRef<const FUniqueNetId> UserIdPtr;

		/** Additional key/value pair data related to auth */
	TMap<FString, FString> AdditionalAuthData;
		/** Additional key/value pair data related to user attribution */
	TMap<FString, FString> UserAttributes;
};

/**
 * Justice service implementation of the online identity interface
 */
class FOnlineIdentityJustice : public IOnlineIdentity
{
public:

	// IOnlineIdentity

	virtual bool Login(int32 LocalUserNum, const FOnlineAccountCredentials& AccountCredentials) override;
	virtual bool Logout(int32 LocalUserNum) override;
	virtual bool AutoLogin(int32 LocalUserNum) override;
	virtual TSharedPtr<FUserOnlineAccount> GetUserAccount(const FUniqueNetId& UserId) const override;
	virtual TArray<TSharedPtr<FUserOnlineAccount> > GetAllUserAccounts() const override;
	virtual TSharedPtr<const FUniqueNetId> GetUniquePlayerId(int32 LocalUserNum) const override;
	virtual TSharedPtr<const FUniqueNetId> CreateUniquePlayerId(uint8* Bytes, int32 Size) override;
	virtual TSharedPtr<const FUniqueNetId> CreateUniquePlayerId(const FString& Str) override;
	virtual ELoginStatus::Type GetLoginStatus(int32 LocalUserNum) const override;
	virtual ELoginStatus::Type GetLoginStatus(const FUniqueNetId& UserId) const override;
	virtual FString GetPlayerNickname(int32 LocalUserNum) const override;
	virtual FString GetPlayerNickname(const FUniqueNetId& UserId) const override;
	virtual FString GetAuthToken(int32 LocalUserNum) const override;
	virtual void GetUserPrivilege(const FUniqueNetId& UserId, EUserPrivileges::Type Privilege, const FOnGetUserPrivilegeCompleteDelegate& Delegate) override;
	virtual FPlatformUserId GetPlatformUserIdFromUniqueNetId(const FUniqueNetId& UniqueNetId) override;
	virtual FString GetAuthType() const override;

	// FOnlineIdentityJustice

	/**
	 * Constructor
	 *
	 * @param InSubsystem online subsystem being used
	 */
	FOnlineIdentityJustice(class FOnlineSubsystemJustice* InSubsystem);

	/**
	 * Destructor
	 */
	virtual ~FOnlineIdentityJustice();


private:

	void LoginComplete(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);

	/**
	 * Should use the initialization constructor instead
	 */
	FOnlineIdentityJustice();

	/** Ids mapped to locally registered users */
	TMap<int32, TSharedPtr<const FUniqueNetId>> UserIds;

	/** Ids mapped to locally registered users */
	TMap<FUniqueNetIdString, TSharedRef<FUserOnlineAccountJustice>> UserAccounts;

	/** IAM Base URL Route */
	FString IamBaseURL;

};

typedef TSharedPtr<FOnlineIdentityJustice, ESPMode::ThreadSafe> FOnlineIdentityJusticePtr;