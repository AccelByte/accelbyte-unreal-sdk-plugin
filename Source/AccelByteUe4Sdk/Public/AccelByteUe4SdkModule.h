// Copyright (c) 2018 - 2019 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "CoreMinimal.h"
#include "Core/AccelByteApiClient.h"
#include "Modules/ModuleManager.h"
#include "Core/AGS/AccelBytePlatform.h"
#include "Core/AccelByteError.h"
#include "Core/AccelByteEnvironment.h"
#include "Core/AccelByteServerApiClient.h"
#include "Core/IAccelByteDataStorage.h"
#include "Core/Platform/AccelBytePlatformHandler.h"

class FEnvironmentChangedDelegate
{
public:

	bool IsBound()
	{
		FReadScopeLock Lk{Mtx};
		return ProxyDelegate.IsBound();
	}

	void Broadcast(const ESettingsEnvironment Environment)
	{
		FWriteScopeLock Lk{Mtx};
		ProxyDelegate.Broadcast(Environment);
	}

	template <typename UserClass, typename... VarTypes>
	inline FDelegateHandle AddThreadSafeSP(const TSharedRef<UserClass, ESPMode::ThreadSafe>& InUserObjectRef, typename TMemFunPtrType<false, UserClass, void (const ESettingsEnvironment, std::decay_t<VarTypes>...)>::Type InFunc, VarTypes&&... Vars)
	{
		FWriteScopeLock Lk{Mtx};
		return ProxyDelegate.AddThreadSafeSP(InUserObjectRef, InFunc, Forward<VarTypes>(Vars)...);
	}

	template <typename UserClass, typename... VarTypes>
	inline FDelegateHandle AddThreadSafeSP(const TSharedRef<UserClass, ESPMode::ThreadSafe>& InUserObjectRef, typename TMemFunPtrType<true, UserClass, void (const ESettingsEnvironment, std::decay_t<VarTypes>...)>::Type InFunc, VarTypes&&... Vars)
	{
		FWriteScopeLock Lk{Mtx};
		return ProxyDelegate.AddThreadSafeSP(InUserObjectRef, InFunc, Forward<VarTypes>(Vars)...);
	}

	template<typename FunctorType, typename... VarTypes>
	FDelegateHandle AddLambda(FunctorType&& InFunctor, VarTypes&&... Vars)
	{
		FWriteScopeLock Lk{Mtx};
		return ProxyDelegate.AddLambda(MoveTemp(InFunctor), Forward<VarTypes>(Vars)...);
	}

	bool Remove(FDelegateHandle Handle)
	{
		FWriteScopeLock Lk{Mtx};
		return ProxyDelegate.Remove(Handle);
	}

private:
	TMulticastDelegate<void(const ESettingsEnvironment)> ProxyDelegate;
	FRWLock Mtx;
};

namespace AccelByte
{
	class Settings;
	class ServerSettings;
	class IAccelByteDataStorage;
	class FAccelBytePlatformHandler;
}

// forward declaration
class FAccelByteInstance;

class IAccelByteUe4SdkModuleInterface 
	: public IModuleInterface
{
	friend class FAccelByteModuleUtilities;

public:
	static IAccelByteUe4SdkModuleInterface& Get()
	{
		return FModuleManager::LoadModuleChecked<IAccelByteUe4SdkModuleInterface>("AccelByteUe4Sdk");
	}

	static bool IsAvailable()
	{
		return FModuleManager::Get().IsModuleLoaded("AccelByteUe4Sdk");
	}

	virtual void SetEnvironment(ESettingsEnvironment const Environment) = 0;
	virtual AccelByte::Settings const& GetClientSettings() const = 0;
	virtual AccelByte::ServerSettings const& GetServerSettings() const = 0;
	virtual ESettingsEnvironment const& GetSettingsEnvironment() const = 0;
	virtual FEnvironmentChangedDelegate& OnEnvironmentChanged() = 0;
	virtual AccelByte::IAccelByteDataStorage* GetLocalDataStorage() = 0;
	virtual AccelByte::FAccelBytePlatformHandler& GetPlatformHandler() = 0;
	virtual AccelByte::FAccelBytePlatformPtr GetAccelBytePlatform(AccelByte::BaseSettingsPtr const& InSettings) = 0;

	virtual TSharedPtr<FAccelByteInstance, ESPMode::ThreadSafe> CreateAccelByteInstance() = 0;
	virtual TSharedPtr<FAccelByteInstance, ESPMode::ThreadSafe> CreateAccelByteInstance(AccelByte::Settings & InSettings
		, AccelByte::ServerSettings & InServerSettings) = 0;
};