// Copyright (c) 2021 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "CoreMinimal.h"

#include "Core/AccelByteError.h"
#include "Models/AccelByteUGCModels.h"

namespace AccelByte
{
class Credentials;
class Settings;

namespace Api
{
/**
* Provide APIs to access UGC service.
*/
class ACCELBYTEUE4SDK_API UGC
{

public:

	UGC(const Credentials& Credentials, const Settings& Settings);
	~UGC();

private:

	const Credentials& Credentials;
	const Settings& Settings;

public:

	/** @brief Create a content with FString preview and get the payload url to upload the content.
	*
	* @param ChannelId The id of the content's channel.
	* @param UGCRequest Detail information for the content request.
	* @param OnSuccess This will be called when the operation succeeded. The result is FAccelByteModelsUGCResponse.
	* @param OnError This will be called when the operation failed.
	*/
	void CreateContent(const FString& ChannelId, const FAccelByteModelsUGCRequest& UGCRequest, const THandler<FAccelByteModelsUGCResponse>& OnSuccess, const FErrorHandler& OnError);
	
	/** @brief Create a content with TArray<uint8> Preview paramater and get the payload url to upload the content.
	*
	* @param ChannelId The id of the content's channel.
	* @param Name Name of the content.
	* @param Type Type of the content.
	* @param SubType SubType of the content.
	* @param Tags Tags of the content.
	* @param Preview The TArray<uint8> of the content's Preview
	* @param FileExtension FileExtension of the content.
	* @param OnSuccess This will be called when the operation succeeded. The result is FAccelByteModelsUGCResponse.
	* @param OnError This will be called when the operation failed.
	*/
	void CreateContent(const FString& ChannelId, const FString& Name, const FString& Type, const FString& SubType, const TArray<FString>& Tags, const TArray<uint8>& Preview,
						const FString& FileExtension, const THandler<FAccelByteModelsUGCResponse>& OnSuccess, const FErrorHandler& OnError);

	/** @brief Modify existing content to update some information with FString preview.
	*
	* @param ChannelId The id of the content's channel.
	* @param ContentId The id of the content that will be modified.
	* @param UGCRequest Detail information for the content request that will be modified.
	* @param OnSuccess This will be called when the operation succeeded. The result is FAccelByteModelsUGCResponse.
	* @param OnError This will be called when the operation failed.
	*/
	void ModifyContent(const FString& ChannelId, const FString& ContentId, const FAccelByteModelsUGCRequest& UGCRequest, const THandler<FAccelByteModelsUGCResponse>& OnSuccess, const FErrorHandler& OnError);
	
	/** @brief Modify existing content to update some information with TArray<uint8> Preview paramater.
	*
	* @param ChannelId The id of the content's channel.
	* @param ContentId The id of the content that will be modified.
	* @param Name Name of the content.
	* @param Type Type of the content.
	* @param SubType SubType of the content.
	* @param Tags Tags of the content.
	* @param Preview The TArray<uint8> of the content's Preview
	* @param FileExtension FileExtension of the content.
	* @param OnSuccess This will be called when the operation succeeded. The result is FAccelByteModelsUGCResponse.
	* @param OnError This will be called when the operation failed.
	*/
	void ModifyContent(const FString& ChannelId, const FString& ContentId, const FString& Name, const FString& Type, const FString& SubType, const TArray<FString>& Tags, const TArray<uint8>& Preview,
						const FString& FileExtension, const THandler<FAccelByteModelsUGCResponse>& OnSuccess, const FErrorHandler& OnError);
	
	/** @brief Delete a content based on the its channel id and content id.
	*
	* @param ChannelId The id of the content's channel.
	* @param ContentId The id of the content that will be deleted.
	* @param OnSuccess This will be called when the operation succeeded.
	* @param OnError This will be called when the operation failed.
	*/
	void DeleteContent(const FString& ChannelId, const FString& ContentId, const FVoidHandler& OnSuccess, const FErrorHandler& OnError);
	
	/** @brief Get a content information by its content id
	*
	* @param ContentId The id of the content that will be fetched.
	* @param OnSuccess This will be called when the operation succeeded. The result is FAccelByteModelsUGCContentResponse.
	* @param OnError This will be called when the operation failed.
	*/
	void GetContentByContentId(const FString& ContentId, const THandler<FAccelByteModelsUGCContentResponse>& OnSuccess, const FErrorHandler& OnError);
	
	/** @brief Get a content information by its share code.
	*
	* @param ShareCode The share code of the content that will be fetched.
	* @param OnSuccess This will be called when the operation succeeded. The result is FAccelByteModelsUGCContentResponse.
	* @param OnError This will be called when the operation failed.
	*/
	void GetContentByShareCode(const FString& ShareCode, const THandler<FAccelByteModelsUGCContentResponse>& OnSuccess, const FErrorHandler& OnError);

	/** @brief Get content Preview as FAccelByteModelsUGCPreview.
	*
	* @param ContentId The id of the Preview's content that will be fetched.
	* @param OnSuccess This will be called when the operation succeeded. The result is FAccelByteModelsUGCPreview.
	* @param OnError This will be called when the operation failed.
	*/
	void GetContentPreview(const FString& ContentId, const THandler<FAccelByteModelsUGCPreview>& OnSuccess, const FErrorHandler& OnError);
	
	/** @brief Get content preview as TArray<uint8>.
	*
	* @param ContentId The id of the Preview's content that will be fetched.
	* @param OnSuccess This will be called when the operation succeeded. The result is TArray<uint8>.
	* @param OnError This will be called when the operation failed.
	*/
	void GetContentPreview(const FString& ContentId, const THandler<TArray<uint8>>& OnSuccess, const FErrorHandler& OnError);
	
	/** @brief Get all tags.
	*
	* @param OnSuccess This will be called when the operation succeeded. The result is FAccelByteModelsUGCTagsPagingResponse.
	* @param OnError This will be called when the operation failed.
	* @param Limit The limit of the tags results. Default value is 1000.
	* @param Offset The offset of the tags results. Default value is 0.
	*/
	void GetTags(const THandler<FAccelByteModelsUGCTagsPagingResponse>& OnSuccess, const FErrorHandler& OnError, int32 Limit = 1000, int32 Offset = 0);
	
	/** @brief Get all types and subtypes.
	*
	* @param OnSuccess This will be called when the operation succeeded. The result is FAccelByteModelsUGCTypesPagingResponse.
	* @param OnError This will be called when the operation failed.
	* @param Limit The limit of the types and subtypes results. Default value is 1000.
	* @param Offset The offset of the types and subtypes results. Default value is 0.
	*/
	void GetTypes(const THandler<FAccelByteModelsUGCTypesPagingResponse>& OnSuccess, const FErrorHandler& OnError, int32 Limit = 1000, int32 Offset = 0);

	/** @brief Create a player's channel with specific channel name.
	*
	* @param ChannelName The name of the channel.
	* @param OnSuccess This will be called when the operation succeeded. The result is FAccelByteModelsUGCChannelResponse.
	* @param OnError This will be called when the operation failed.
	*/
	void CreateChannel(const FString& ChannelName, const THandler<FAccelByteModelsUGCChannelResponse>& OnSuccess, const FErrorHandler& OnError);
	
	/** @brief Get all of the player's channels.
	*
	* @param OnSuccess This will be called when the operation succeeded. The result is FAccelByteModelsUGCChannelsPagingResponse.
	* @param OnError This will be called when the operation failed.
	* @param Limit The limit of the channel results. Default value is 1000.
	* @param Offset The offset of the channel results. Default value is 0.
	*/
	void GetChannels(const THandler<FAccelByteModelsUGCChannelsPagingResponse>& OnSuccess, const FErrorHandler& OnError, int32 Limit = 1000, int32 Offset = 0);

	/** @brief Delete a player's channel based on the its channel id.
	*
	* @param ChannelId The id of the channel that will be deleted.
	* @param OnSuccess This will be called when the operation succeeded.
	* @param OnError This will be called when the operation failed.
	*/
	void DeleteChannel(const FString& ChannelId, const FVoidHandler& OnSuccess, const FErrorHandler& OnError);
	
	private:
	UGC() = delete;
	UGC(UGC const&) = delete;
	UGC(UGC&&) = delete;
};
}
}
