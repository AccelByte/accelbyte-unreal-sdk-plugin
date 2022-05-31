// Copyright (c) 2021 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "CoreMinimal.h"

#include "Core/AccelByteError.h"
#include "Core/AccelByteHttpRetryScheduler.h"
#include "Models/AccelByteGeneralModels.h"

namespace AccelByte
{
class Credentials;
class Settings;
namespace Api
{

class ACCELBYTEUE4SDK_API Miscellaneous
{
public:
	Miscellaneous(const Credentials& CredentialsRef, const Settings& SettingsRef, FHttpRetryScheduler& InHttpRef);
	~Miscellaneous();

	/**
	* @brief Get server current time.
	*
	* @param OnSuccess This will be called when the operation succeeded. The result is FTime.
	* @param OnError This will be called when the operation failed.
	*/
	void GetServerCurrentTime(const THandler<FTime>& OnSuccess, const FErrorHandler& OnError);
private:
	FHttpRetryScheduler& HttpRef;
	Credentials const& CredentialsRef;
	Settings const& SettingsRef;

	Miscellaneous() = delete;
	Miscellaneous(Miscellaneous const&) = delete;
	Miscellaneous(Miscellaneous&&) = delete;
};

} // Namespace Api
} // Namespace AccelByte
