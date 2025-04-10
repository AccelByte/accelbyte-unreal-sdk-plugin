// Copyright (c) 2019 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "Models/AccelByteOauth2Models.h"
#include "Core/AccelByteError.h"
#include "Core/AccelByteHttpRetryScheduler.h"
#include "Core/AccelByteServerApiBase.h"
#include "Core/AccelByteOauth2Api.h"

using AccelByte::THandler;
using AccelByte::FVoidHandler;
using AccelByte::FErrorHandler;

namespace AccelByte
{
class ServerCredentials;
class ServerSettings;

/**
 * @brief Contains interface to C++ functions.
 */
namespace GameServerApi
{

/**
 * @brief The API for getting OAuth2 access token. The tokens are stored in memory for convenience.
 */
class ACCELBYTEUE4SDK_API ServerOauth2 : public FServerApiBase
{
public:
	ServerOauth2(ServerCredentials& InCredentialsRef
		, ServerSettings& InSettingsRef
		, FHttpRetryScheduler& InHttpRef
		, TSharedPtr<FServerApiClient, ESPMode::ThreadSafe> InServerApiClient = nullptr);
	~ServerOauth2();

	/**
	 * @brief This is to get access token from `client_credentials` grant, then store the access token in memory.
	 * Server need to be logged in use `client_credentials` grant so that it can register and shutdown DS.
	 *
	 * @param OnSuccess This will be called when the operation succeeded.
	 * @param OnError This will be called when the operation failed.
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr LoginWithClientCredentials(FVoidHandler const& OnSuccess
		, FErrorHandler const& OnError);

	FAccelByteTaskWPtr GetJwks(THandler<FJwkSet> const& OnSuccess
		, FErrorHandler const& OnError);

	/**
	 * @brief Remove access tokens, user ID, and other credentials from memory.
	 */
	void ForgetAllCredentials();

private:
	void OnLoginSuccess(FVoidHandler const& OnSuccess
		, FOauth2Token const& Response) const;

	/**
	* @brief Get client token.
	* he result is FAccelByteModelsOauth2Token.
	*
	* @param ClientId The issued OAuth2 client credentials.
	* @param ClientSecret The issued OAuth2 client credentials.
	* @param OnSuccess This will be called when the operation succeeded.
	* @param OnError This will be called when the operation failed.
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr GetAccessTokenWithClientCredentialsGrant(FString const& ClientId
		, FString const& ClientSecret
		, THandler<FOauth2Token> const& OnSuccess
		, FErrorHandler const& OnError);

	TSharedRef<ServerCredentials, ESPMode::ThreadSafe> ServerCredentialsRef;
	AccelByte::Api::Oauth2 Oauth;
	ServerOauth2() = delete; // static class can't have instance
	ServerOauth2(ServerOauth2 const&) = delete;
	ServerOauth2(ServerOauth2&&) = delete;
};

} // Namespace Api
} // Namespace AccelByte