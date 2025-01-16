// Copyright (c) 2019 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "CoreMinimal.h"
#include "Core/AccelByteApiBase.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Core/AccelByteError.h"
#include "Core/AccelByteHttpRetryScheduler.h"
#include "Models/AccelByteGameProfileModels.h"

// Forward declarations
class IWebSocket;

namespace AccelByte
{
class Credentials;
class Settings;
namespace Api
{

/**
 * @brief GameProfile API to manage user's in-game profiles (character).
 */
class ACCELBYTEUE4SDK_API GameProfile : public FApiBase, public TSharedFromThis<GameProfile, ESPMode::ThreadSafe>
{
public:
	GameProfile(Credentials const& InCredentialsRef, Settings const& InSettingsRef, FHttpRetryScheduler& InHttpRef);
	~GameProfile();

	/**
	 * @brief Get public game profiles from the specified user id(s).
	 *
	 * @param UserIds Specify user id(s) those will be requested. One UserId is required.
	 * @param OnSuccess This will be called when the operation succeeded. The result is an array of FAccelByteModelsPublicGameProfile.
	 * @param OnError This will be called when the operation failed.
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr BatchGetPublicGameProfiles(TArray<FString> const& UserIds
		, THandler<TArray<FAccelByteModelsPublicGameProfile>> const& OnSuccess
		, FErrorHandler const& OnError);

	/**
	 * @brief Get all user's game profiles.
	 *
	 * @param OnSuccess This will be called when the operation succeeded. The result is an array FAccelByteModelsGameProfile.
	 * @param OnError This will be called when the operation failed.
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr GetAllGameProfiles(THandler<TArray<FAccelByteModelsGameProfile>> const& OnSuccess
		, FErrorHandler const& OnError);

	/**
	 * @brief Create a game profile.
	 *
	 * @param GameProfileRequest Game profile that will be created.
	 * @param OnSuccess This will be called when the operation succeeded. The result is a FAccelByteModelsGameProfile that created.
	 * @param OnError This will be called when the operation failed.
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr CreateGameProfile(FAccelByteModelsGameProfileRequest const& GameProfileRequest
		, THandler<FAccelByteModelsGameProfile> const& OnSuccess
		, FErrorHandler const& OnError);

	/**
	 * @brief Get a specific game profile.
	 *
	 * @param ProfileId Specified game profile ID.
	 * @param OnSuccess This will be called when the operation succeeded. The result is a FAccelByteModelsGameProfile that requested.
	 * @param OnError This will be called when the operation failed.
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr GetGameProfile(FString const& ProfileId
		, THandler<FAccelByteModelsGameProfile> const& OnSuccess
		, FErrorHandler const& OnError);

	/**
	 * @brief Update a specific game profile.
	 *
	 * @param ProfileId Specified game profile ID.
	 * @param GameProfileRequest Game profile that will be updated.
	 * @param OnSuccess This will be called when the operation succeeded. The result is a FAccelByteModelsGameProfile that updated.
	 * @param OnError This will be called when the operation failed.
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr UpdateGameProfile(FString const& ProfileId
		, FAccelByteModelsGameProfileRequest const& GameProfileRequest
		, THandler<FAccelByteModelsGameProfile> const& OnSuccess
		, FErrorHandler const& OnError);

	/**
	 * @brief Delete a specific game profile.
	 *
	 * @param ProfileId Specified game profile ID.
	 * @param OnSuccess This will be called when the operation succeeded.
	 * @param OnError This will be called when the operation failed.
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr DeleteGameProfile(FString const& ProfileId
		, FVoidHandler const& OnSuccess
		, FErrorHandler const& OnError);

	/**
	 * @brief Get an attribute value from a game profile.
	 *
	 * @param ProfileId Specified game profile ID.
	 * @param AttributeName Specified attribute name.
	 * @param OnSuccess This will be called when the operation succeeded. The result is an attribute name and value as string.
	 * @param OnError This will be called when the operation failed.
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr GetGameProfileAttribute(FString const& ProfileId
		, FString const& AttributeName
		, THandler<FAccelByteModelsGameProfileAttribute> const& OnSuccess
		, FErrorHandler const& OnError);

	/**
	 * @brief Update an attribute from a game profile.
	 *
	 * @param ProfileId Specified game profile ID.
	 * @param Attribute Consist of attribute name and value that will be updated.
	 * @param OnSuccess This will be called when the operation succeeded. The result is a FAccelByteModelsGameProfile with an updated attribute.
	 * @param OnError This will be called when the operation failed.
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr UpdateGameProfileAttribute(FString const& ProfileId
		, FAccelByteModelsGameProfileAttribute const& Attribute
		, THandler<FAccelByteModelsGameProfile> const& OnSuccess
		, FErrorHandler const& OnError);
	
private:
	GameProfile() = delete;
	GameProfile(GameProfile const&) = delete;
	GameProfile(GameProfile&&) = delete;
};

typedef TSharedRef<GameProfile, ESPMode::ThreadSafe> GameProfileRef;
typedef TSharedPtr<GameProfile, ESPMode::ThreadSafe> GameProfilePtr;
typedef TWeakPtr<GameProfile, ESPMode::ThreadSafe> GameProfileWPtr;

} // Namespace Api
} // Namespace AccelByte
