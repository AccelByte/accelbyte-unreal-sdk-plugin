// Copyright (c) 2019 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Core/AccelByteError.h"
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
class ACCELBYTEUE4SDK_API GameProfile
{
public:
	GameProfile(const Credentials& Credentials, const Settings& Settings);
	~GameProfile();
private:
	const Credentials& Credentials;
	const Settings& Settings;

public:

	/**
	 * @brief Get public game profiles from the specified user id(s).
	 *
	 * @param UserIds Specify user id(s) those will be requested. One UserId is required.
	 * @param OnSuccess This will be called when the operation succeeded. The result is an array of FAccelByteModelsPublicGameProfile.
	 * @param OnError This will be called when the operation failed.
	*/
	void BatchGetPublicGameProfiles(TArray<FString> UserIds, const THandler<TArray<FAccelByteModelsPublicGameProfile>>& OnSuccess, const FErrorHandler& OnError);

	/**
	 * @brief Get all user's game profiles.
	 *
	 * @param OnSuccess This will be called when the operation succeeded. The result is an array FAccelByteModelsGameProfile.
	 * @param OnError This will be called when the operation failed.
	 */
	void GetAllGameProfiles(const THandler<TArray<FAccelByteModelsGameProfile>>& OnSuccess, const FErrorHandler& OnError);

	/**
	 * @brief Create a game profile.
	 *
	 * @param GameProfileRequest Game profile that will be created.
	 * @param OnSuccess This will be called when the operation succeeded. The result is a FAccelByteModelsGameProfile that created.
	 * @param OnError This will be called when the operation failed.
	*/
	void CreateGameProfile(const FAccelByteModelsGameProfileRequest& GameProfileRequest, const THandler<FAccelByteModelsGameProfile>& OnSuccess, const FErrorHandler& OnError);

	/**
	 * @brief Get a specific game profile.
	 *
	 * @param ProfileId Specified game profile ID.
	 * @param OnSuccess This will be called when the operation succeeded. The result is a FAccelByteModelsGameProfile that requested.
	 * @param OnError This will be called when the operation failed.
	*/
	void GetGameProfile(const FString& ProfileId, const THandler<FAccelByteModelsGameProfile>& OnSuccess, const FErrorHandler& OnError);

	/**
	 * @brief Update a specific game profile.
	 *
	 * @param ProfileId Specified game profile ID.
	 * @param GameProfileRequest Game profile that will be updated.
	 * @param OnSuccess This will be called when the operation succeeded. The result is a FAccelByteModelsGameProfile that updated.
	 * @param OnError This will be called when the operation failed.
	*/
	void UpdateGameProfile(const FString& ProfileId, const FAccelByteModelsGameProfileRequest& GameProfileRequest, const THandler<FAccelByteModelsGameProfile>& OnSuccess, const FErrorHandler& OnError);

	/**
	 * @brief Delete a specific game profile.
	 *
	 * @param ProfileId Specified game profile ID.
	 * @param OnSuccess This will be called when the operation succeeded.
	 * @param OnError This will be called when the operation failed.
	*/
	void DeleteGameProfile(const FString& ProfileId, const FVoidHandler& OnSuccess, const FErrorHandler& OnError);

	/**
	 * @brief Get an attribute value from a game profile.
	 *
	 * @param ProfileId Specified game profile ID.
	 * @param AttributeName Specified attribute name.
	 * @param OnSuccess This will be called when the operation succeeded. The result is an attribute name and value as string.
	 * @param OnError This will be called when the operation failed.
	*/
	void GetGameProfileAttribute(const FString& ProfileId, const FString& AttributeName, const THandler<FAccelByteModelsGameProfileAttribute>& OnSuccess, const FErrorHandler& OnError);

	/**
	 * @brief Update an attribute from a game profile.
	 *
	 * @param ProfileId Specified game profile ID.
	 * @param Attribute Consist of attribute name and value that will be updated.
	 * @param OnSuccess This will be called when the operation succeeded. The result is a FAccelByteModelsGameProfile with an updated attribute.
	 * @param OnError This will be called when the operation failed.
	*/
	void UpdateGameProfileAttribute(const FString& ProfileId, const FAccelByteModelsGameProfileAttribute& Attribute, const THandler<FAccelByteModelsGameProfile>& OnSuccess, const FErrorHandler& OnError);
	
private:
	GameProfile() = delete;
	GameProfile(GameProfile const&) = delete;
	GameProfile(GameProfile&&) = delete;
};

} // Namespace Api
} // Namespace AccelByte
