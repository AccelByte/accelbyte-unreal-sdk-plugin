// Copyright (c) 2021 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "CoreMinimal.h"
#include "Http.h"
#include "Models/AccelByteEcommerceModels.h"
#include "Core/AccelByteError.h"
#include "Core/AccelByteHttpRetryScheduler.h" 

namespace AccelByte
{
class Credentials;
class Settings;
namespace Api
{

/**
 * @brief Currency API for getting information from Currency
 */
class ACCELBYTEUE4SDK_API Currency
{
public:
	Currency(Credentials const& InCredentialsRef, Settings const& InSettingsRef, FHttpRetryScheduler& InHttpRef);
	~Currency();
private:
	FHttpRetryScheduler& HttpRef;
	Credentials const& CredentialsRef;
	Settings const& SettingsRef;
public:
	/**
	 * @brief Get currency list information for a specific namespace.
	 *
	 * @param Namespace The Namespace.
	 * @param OnSuccess This will be called when operation succeeded. The result is const FAccelByteModelsCurrencyList&.
	 * @param OnError This will be called when the operation failed.
	 * @param CurrencyType The Currency type.
	 */
	void GetCurrencyList(FString const& Namespace, THandler<TArray<FAccelByteModelsCurrencyList>> const& OnSuccess, FErrorHandler const& OnError,
		EAccelByteCurrencyType CurrencyType = EAccelByteCurrencyType::NONE);

private:
	Currency() = delete;
	Currency(Currency const&) = delete;
	Currency(Currency&&) = delete;
};

} // Namespace Api
} // Namespace AccelByte
