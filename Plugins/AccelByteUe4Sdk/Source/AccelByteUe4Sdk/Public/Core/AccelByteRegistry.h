// Copyright (c) 2018 - 2019 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "AccelByteSettings.h"
#include "AccelByteCredentials.h"

using namespace AccelByte;

namespace AccelByte
{

class FHttpRetryScheduler;

namespace Api
{
	class User;
	class UserProfile;
	class Category;
	class Entitlement;
	class Order;
	class Item;
	class Wallet;
	class CloudStorage;
	class Lobby;
	class GameProfile;
	class Statistic;
}

class ACCELBYTEUE4SDK_API FRegistry
{
public:
	static Settings Settings;
	static Credentials Credentials;
	static FHttpRetryScheduler HttpRetryScheduler;
	static Api::User User;
	static Api::UserProfile UserProfile;
	static Api::Category Category;
	static Api::Entitlement Entitlement;
	static Api::Order Order;
	static Api::Item Item;
	static Api::Wallet Wallet;
	static Api::CloudStorage CloudStorage;
	static Api::Lobby Lobby;
	static Api::GameProfile GameProfile;
	static Api::Statistic Statistic;

	//Static class doesn't have constructors or destructor
	FRegistry() = delete;
	FRegistry(const FRegistry& other) = delete;
	FRegistry& operator=(const FRegistry& other) = delete;
	FRegistry(FRegistry&& other) = delete;
	FRegistry& operator=(FRegistry&& other) = delete;
	~FRegistry() = delete;
};

}

