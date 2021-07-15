// Copyright (c) 2019 - 2021 AccelByte Inc. All Rights Reserved.
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
* 
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
	/** @brief Create a content and get the payload url to upload the content 
	 *
	 * @param ChannelId Save the content into this channel
	 * @param Request Detail info for the content
	 * @param OnSuccess Fired when create content is succeed. Returning the detailed info of the Content
	 * @param OnError Fired when fail
	 */
	void CreateContent(const FString& ChannelId, const FAccelByteModelsUGCRequest& CreateRequest, const THandler<FAccelByteModelsUGCResponse>& OnSuccess, const FErrorHandler& OnError);
	
	/** @brief Create a content and get the payload url to upload the content. This is an alternative version, that you can pass as TArray<uint8> for the Preview.
	*
	* @param ChannelId Save the content into this channel
	* @param Name name of the content
	* @param Type of the content
	* @param SubType of the content
	* @param Tags of the content
	* @param Preview of the content. It will automatically Encoded into FString
	* @param FileExtension of the content
	* @param OnSuccess Fired when create content is succeed. Returning the detailed info of the Content
	* @param OnError Fired when fail
	*/
	void CreateContent(const FString& ChannelId, const FString& Name, const FString& Type, const FString& SubType, const TArray<FString>& Tags, const TArray<uint8>& Preview,
						const FString& FileExtension, const THandler<FAccelByteModelsUGCResponse>& OnSuccess, const FErrorHandler& OnError);

	/** @brief Modify existing content to update some information
	*
	* @param ChannelId Save the content into this channel
	* @param ContentId Content you want to modified
	* @param ModifyRequest Detail info for the content
	* @param OnSuccess Fired when Modify content is succeed.
	* @param OnError Fired when fail
	*/
	void ModifyContent(const FString& ChannelId, const FString& ContentId, const FAccelByteModelsUGCRequest& ModifyRequest, const THandler<FAccelByteModelsUGCResponse>& OnSuccess, const FErrorHandler& OnError);
	
	/** @brief Modify existing content to update some information
	*
	* @param ChannelId Modify the content from this channel
	* @param ContentId Content you want to modified
	* @param Name name of the content
	* @param Type of the content
	* @param SubType of the content
	* @param Tags of the content
	* @param Preview of the content. It will automatically Encoded into FString
	* @param FileExtension of the content
	* @param OnSuccess Fired when Modify content is succeed. Returning the detailed info of the Content
	* @param OnError Fired when fail
	*/
	void ModifyContent(const FString& ChannelId, const FString& ContentId, const FString& Name, const FString& Type, const FString& SubType, const TArray<FString>& Tags, const TArray<uint8>& Preview,
						const FString& FileExtension, const THandler<FAccelByteModelsUGCResponse>& OnSuccess, const FErrorHandler& OnError);
	
	/** @brief Modify existing content to update some information
	*
	* @param ChannelId Delete the content from this channel
	* @param ContentId Content you want to deleted
	* @param OnSuccess Fired when Delete content is succeed.
	* @param OnError Fired when fail
	*/
	void DeleteContent(const FString& ChannelId, const FString& ContentId, const FVoidHandler& OnSuccess, const FErrorHandler& OnError);
	
	/** @brief Get Content by Content Id
	*
	* @param ContentId you want to get
	* @param OnSuccess Fired when create content is succeed. Returning the detailed info of the Content
	* @param OnError Fired when fail
	*/
	void GetContentByContentId(const FString& ContentId, const THandler<FAccelByteModelsUGCContentResponse>& OnSuccess, const FErrorHandler& OnError);
	
	/** @brief Get content by shared code
	*
	* @param ShareCode you want to get the content.
	* @param OnSuccess Fired when Get content is succeed. Returning the detailed info of the Content
	* @param OnError Fired when fail
	*/
	void GetContentByShareCode(const FString& ShareCode, const THandler<FAccelByteModelsUGCContentResponse>& OnSuccess, const FErrorHandler& OnError);

	/** @brief Get content Preview
	*
	* @param ContentId you want to get the content.
	* @param OnSuccess Fired when Get content is succeed. Returning the detailed info of the Content
	* @param OnError Fired when fail
	*/
	void GetContentPreview(const FString& ContentId, const THandler<FAccelByteModelsUGCPreview>& OnSuccess, const FErrorHandler& OnError);
	
	/** @brief Get content Preview
	*
	* @param ContentId you want to get the content.
	* @param OnSuccess Fired when Get content is succeed. Returning the detailed info of the Content
	* @param OnError Fired when fail
	*/
	void GetContentPreview(const FString& ContentId, const THandler<TArray<uint8>>& OnSuccess, const FErrorHandler& OnError);
	
	/** @brief Get all Tags
	*
	* @param OnSuccess Fired when Get Tags is succeed. Returning the info of the Tags
	* @param OnError Fired when fail
	* @param Limit How many tags you'll get
	* @param Offset Start from
	*/
	void GetTags(const THandler<FAccelByteModelsUGCTagsPagingResponse>& OnSuccess, const FErrorHandler& OnError, int32 Limit = 1000, int32 Offset = 0);
	
	/** @brief Get all Types, including SubTypes
	*
	* @param OnSuccess Fired when Get Types is succeed. Returning the info of the Types
	* @param OnError Fired when fail
	* @param Limit How many types you'll get
	* @param Offset Start from
	*/
	void GetTypes(const THandler<FAccelByteModelsUGCTypesPagingResponse>& OnSuccess, const FErrorHandler& OnError, int32 Limit = 1000, int32 Offset = 0);

	/** @brief Create Channel into the user
	*
	* @param ChannelName The name of the channel
	* @param OnSuccess Fired when create channel is succeed. Returning the detailed info of the channel
	* @param OnError Fired when fail
	*/
	void CreateChannel(const FString& ChannelName, const THandler<FAccelByteModelsUGCChannelResponse>& OnSuccess, const FErrorHandler& OnError);
	
	/** @brief Get All Channel related to the user
	*
	* @param OnSuccess Fired when create channel is succeed. Returning the detailed info of the channel
	* @param OnError Fired when fail
	* @param Limit How many channel you'll get
	* @param Offset Start from
	*/
	void GetChannels(const THandler<FAccelByteModelsUGCChannelsPagingResponse>& OnSuccess, const FErrorHandler& OnError, int32 Limit = 1000, int32 Offset = 0);

	/** @brief Delete Channel from the user
	*
	* @param ChannelId Channel you want to delete
	* @param OnSuccess Fired when delete channel is succeed.
	* @param OnError Fired when fail
	*/
	void DeleteChannel(const FString& ChannelId, const FVoidHandler& OnSuccess, const FErrorHandler& OnError);
	
	private:
	UGC() = delete;
	UGC(UGC const&) = delete;
	UGC(UGC&&) = delete;
};
}
}
