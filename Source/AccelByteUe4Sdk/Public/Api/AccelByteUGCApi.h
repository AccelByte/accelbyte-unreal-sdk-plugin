// Copyright (c) 2021 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "CoreMinimal.h"
#include "Core/AccelByteApiBase.h"

#include "Core/AccelByteError.h"
#include "Core/AccelByteHttpRetryScheduler.h"
#include "Models/AccelByteUGCModels.h"

namespace AccelByte
{
class Credentials;
class Settings;

namespace Api
{
/**
 * @brief Provide APIs to access UGC service.
 */
class ACCELBYTEUE4SDK_API UGC : public FApiBase
{
public:
	UGC(Credentials const& InCredentialsRef, Settings const& InSettingsRef, FHttpRetryScheduler& InHttpRef);
	~UGC();

	/**
	 * @brief Create a content with FString preview and get the payload url to upload the content.
	 *
	 * @param ChannelId The id of the content's channel.
	 * @param UGCRequest Detail information for the content request.
	 * @param OnSuccess This will be called when the operation succeeded. The result is FAccelByteModelsUGCResponse.
	 * @param OnError This will be called when the operation failed.
	 */
	void CreateContent(FString const& ChannelId
		, FAccelByteModelsUGCRequest const& UGCRequest
		, THandler<FAccelByteModelsUGCResponse> const& OnSuccess
		, FErrorHandler const& OnError);
	
	/**
	 * @brief Create a content with TArray<uint8> Preview parameter and get the payload url to upload the content.
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
	 * @param ContentType The specific type of the content's created, default value is "application/octet-stream".
	 */
	void CreateContent(FString const& ChannelId
		, FString const& Name
		, FString const& Type
		, FString const& SubType
		, TArray<FString> const& Tags
		, TArray<uint8> const& Preview
		, FString const& FileExtension
		, THandler<FAccelByteModelsUGCResponse> const& OnSuccess
		, FErrorHandler const& OnError
		, FString ContentType = TEXT("application/octet-stream"));

	/**
	 * @brief Modify existing content to update some information with FString preview.
	 *
	 * @param ChannelId The id of the content's channel.
	 * @param ContentId The id of the content that will be modified.
	 * @param ModifyRequest Detail information for the content request that will be modified.
	 * @param OnSuccess This will be called when the operation succeeded. The result is FAccelByteModelsUGCResponse.
	 * @param OnError This will be called when the operation failed.
	 */
	void ModifyContent(FString const& ChannelId
		, FString const& ContentId
		, FAccelByteModelsUGCUpdateRequest const& ModifyRequest
		, THandler<FAccelByteModelsUGCResponse> const& OnSuccess
		, FErrorHandler const& OnError);

	/**
	 * @brief Modify existing content to update some information with FString preview.
	 *
	 * @param ChannelId The id of the content's channel.
	 * @param ContentId The id of the content that will be modified.
	 * @param UGCRequest Detail information for the content request that will be modified.
	 * @param OnSuccess This will be called when the operation succeeded. The result is FAccelByteModelsUGCResponse.
	 * @param OnError This will be called when the operation failed.
	 * @param bUpdateContent This will be used to update the content too or only content information . Default value is false.
	 *
	 * @deprecated This method will be removed in the future, so please use
	 * 	void ModifyContent(FString const& ChannelId
		, FString const& ContentId
		, FAccelByteModelsUGCUpdateRequest const& ModifyRequest
		, THandler<FAccelByteModelsUGCResponse> const& OnSuccess
		, FErrorHandler const& OnError);
	 */
	void ModifyContent(FString const& ChannelId
		, FString const& ContentId
		, FAccelByteModelsUGCRequest const& UGCRequest
		, THandler<FAccelByteModelsUGCResponse> const& OnSuccess
		, FErrorHandler const& OnError
		, bool bUpdateContent = false);

	/**
	 * @brief Modify existing content to update some information with TArray<uint8> Preview parameter.
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
	 * @param ContentType The specific type of the content's modified. Default value is "application/octet-stream".
	 * @param bUpdateContent This will be used to update the content too or only content information . Default value is false.
	 *
	 * @deprecated This method will be removed in the future, so please use
	 * 	void ModifyContent(FString const& ChannelId
		, FString const& ContentId
		, FAccelByteModelsUGCUpdateRequest const& ModifyRequest
		, THandler<FAccelByteModelsUGCResponse> const& OnSuccess
		, FErrorHandler const& OnError);
	 */
	void ModifyContent(FString const& ChannelId
		, FString const& ContentId
		, FString const& Name
		, FString const& Type
		, FString const& SubType
		, TArray<FString> const& Tags
		, TArray<uint8> const& Preview
		, FString const& FileExtension
		, THandler<FAccelByteModelsUGCResponse> const& OnSuccess
		, FErrorHandler const& OnError
		, FString ContentType = TEXT("application/octet-stream")
		, bool bUpdateContent = false);
	
	/**
	 * @brief Delete a content based on the its channel id and content id.
	 *
	 * @param ChannelId The id of the content's channel.
	 * @param ContentId The id of the content that will be deleted.
	 * @param OnSuccess This will be called when the operation succeeded.
	 * @param OnError This will be called when the operation failed.
	 */
	void DeleteContent(FString const& ChannelId
		, FString const& ContentId
		, FVoidHandler const& OnSuccess
		, FErrorHandler const& OnError);
	
	/**
	 * @brief Get a content information by its content id
	 *
	 * @param ContentId The id of the content that will be fetched.
	 * @param OnSuccess This will be called when the operation succeeded. The result is FAccelByteModelsUGCContentResponse.
	 * @param OnError This will be called when the operation failed.
	 */
	void GetContentByContentId(FString const& ContentId
		, THandler<FAccelByteModelsUGCContentResponse> const& OnSuccess
		, FErrorHandler const& OnError);
	
	/**
	 * @brief Get a content information by its share code.
	 *
	 * @param ShareCode The share code of the content that will be fetched.
	 * @param OnSuccess This will be called when the operation succeeded. The result is FAccelByteModelsUGCContentResponse.
	 * @param OnError This will be called when the operation failed.
	 */
	void GetContentByShareCode(FString const& ShareCode
		, THandler<FAccelByteModelsUGCContentResponse> const& OnSuccess
		, FErrorHandler const& OnError);

	/**
	 * @brief Get content Preview as FAccelByteModelsUGCPreview.
	 *
	 * @param ContentId The id of the Preview's content that will be fetched.
	 * @param OnSuccess This will be called when the operation succeeded. The result is FAccelByteModelsUGCPreview.
	 * @param OnError This will be called when the operation failed.
	 */
	void GetContentPreview(FString const& ContentId
		, THandler<FAccelByteModelsUGCPreview> const& OnSuccess
		, FErrorHandler const& OnError);
	
	/**
	 * @brief Get content preview as TArray<uint8>.
	 *
	 * @param ContentId The id of the Preview's content that will be fetched.
	 * @param OnSuccess This will be called when the operation succeeded. The result is TArray<uint8>.
	 * @param OnError This will be called when the operation failed.
	 */
	void GetContentPreview(FString const& ContentId
		, THandler<TArray<uint8>> const& OnSuccess
		, FErrorHandler const& OnError);
	
	/**
	 * @brief Get all tags.
	 *
	 * @param OnSuccess This will be called when the operation succeeded. The result is FAccelByteModelsUGCTagsPagingResponse.
	 * @param OnError This will be called when the operation failed.
	 * @param Limit The limit of the tags results. Default value is 1000.
	 * @param Offset The offset of the tags results. Default value is 0.
	 */
	void GetTags(THandler<FAccelByteModelsUGCTagsPagingResponse> const& OnSuccess
		, FErrorHandler const& OnError
		, int32 Limit = 1000
		, int32 Offset = 0);
	
	/**
	 * @brief Get all types and subtypes.
	 *
	 * @param OnSuccess This will be called when the operation succeeded. The result is FAccelByteModelsUGCTypesPagingResponse.
	 * @param OnError This will be called when the operation failed.
	 * @param Limit The limit of the types and subtypes results. Default value is 1000.
	 * @param Offset The offset of the types and subtypes results. Default value is 0.
	 */
	void GetTypes(THandler<FAccelByteModelsUGCTypesPagingResponse> const& OnSuccess
		, FErrorHandler const& OnError
		, int32 Limit = 1000
		, int32 Offset = 0);

	/**
	 * @brief Create a player's channel with specific channel name.
	 *
	 * @param ChannelName The name of the channel.
	 * @param OnSuccess This will be called when the operation succeeded. The result is FAccelByteModelsUGCChannelResponse.
	 * @param OnError This will be called when the operation failed.
	 */
	void CreateChannel(FString const& ChannelName
		, THandler<FAccelByteModelsUGCChannelResponse> const& OnSuccess
		, FErrorHandler const& OnError);

	/**
	 * @brief Update a player's channel with specific channel name.
	 *
	 * @param ChannelId The id of the channel that will be updated.
	 * @param ChannelName The name of the channel.
	 * @param OnSuccess This will be called when the operation succeeded. The result is FAccelByteModelsUGCChannelResponse.
	 * @param OnError This will be called when the operation failed.
	 */
	void UpdateChannel(FString const& ChannelId
		, FString const& ChannelName
		, THandler<FAccelByteModelsUGCChannelResponse> const& OnSuccess
		, FErrorHandler const& OnError);

	/**
	 * @brief Get all of the player's channels.
	 *
	 * @param OnSuccess This will be called when the operation succeeded. The result is FAccelByteModelsUGCChannelsPagingResponse.
	 * @param OnError This will be called when the operation failed.
	 * @param Limit The limit of the channel results. Default value is 1000.
	 * @param Offset The offset of the channel results. Default value is 0.
	 * @param ChannelName The name of the channel you want to query.
	 */
	void GetChannels(THandler<FAccelByteModelsUGCChannelsPagingResponse> const& OnSuccess
		, FErrorHandler const& OnError
		, int32 Limit = 1000
		, int32 Offset = 0
		, FString const& ChannelName = "");

	/**
	 * @brief Get all of the player's channels.
	 *
	 * @param UserId User Id
	 * @param OnSuccess This will be called when the operation succeeded. The result is FAccelByteModelsUGCChannelsPagingResponse.
	 * @param OnError This will be called when the operation failed.
	 * @param Limit The limit of the channel results. Default value is 1000.
	 * @param Offset The offset of the channel results. Default value is 0.
	 * @param ChannelName The name of the channel you want to query.
	 */
	void GetChannels(FString const& UserId
		, THandler<FAccelByteModelsUGCChannelsPagingResponse> const& OnSuccess
		, FErrorHandler const& OnError
		, int32 Limit = 1000
		, int32 Offset = 0
		, FString const& ChannelName = "");

	/**
	 * @brief Delete a player's channel based on the its channel id.
	 *
	 * @param ChannelId The id of the channel that will be deleted.
	 * @param OnSuccess This will be called when the operation succeeded.
	 * @param OnError This will be called when the operation failed.
	 */
	void DeleteChannel(FString const& ChannelId
		, FVoidHandler const& OnSuccess
		, FErrorHandler const& OnError);

	/**
	 * @brief Public user can access without token or if token specified, requires valid user token.   
	 *
	 * @param Name Content Name.
	 * @param Creator Creator Name.
	 * @param Type Content Type.
	 * @param Subtype Content Subtype.
	 * @param Tags Content Tags.
	 * @param IsOfficial Filter only official contents
	 * @param UserId User Id 
	 * @param OnSuccess This will be called when the operation succeeded.
	 * @param OnError This will be called when the operation failed.
	 * @param SortBy Sorting criteria, name,download,like,date. default=date.
	 * @param OrderBy Sorting order: asc, desc. default=desc
	 * @param Limit Number of content per page. Default value : 1000
	 * @param Offset The offset number to retrieve. Default value : 0
	 */
	void SearchContents(const FString& Name
		, const FString& Creator
		, const FString& Type
		, const FString& Subtype
		, const TArray<FString>& Tags
		, bool IsOfficial
		, const FString& UserId
		, THandler<FAccelByteModelsUGCSearchContentsPagingResponse> const& OnSuccess
		, FErrorHandler const& OnError
		, EAccelByteUgcSortBy SortBy = EAccelByteUgcSortBy::DATE
		, EAccelByteUgcOrderBy OrderBy = EAccelByteUgcOrderBy::DESC
		, int32 Limit = 1000
		, int32 Offset = 0);

	/**
	 * @brief Update like/unlike status to a content
	 *
	 * @param ContentId The content id that will be updated.
	 * @param bLikeStatus New like Status value.
	 * @param OnSuccess This will be called when the operation succeeded.
	 * @param OnError This will be called when the operation failed.
	 */
	void UpdateLikeStatusToContent(const FString& ContentId
		, bool bLikeStatus
		, THandler<FAccelByteModelsUGCUpdateLikeStatusToContentResponse> const& OnSuccess
		, FErrorHandler const& OnError);

	/**
	 * @brief Get list of followers.
	 *
	 * @param UserId The id of the user that will be given it's followers.
	 * @param OnSuccess This will be called when the operation succeeded.
	 * @param OnError This will be called when the operation failed.
	 * @param Limit Number of user per page, Default value : 1000.
	 * @param Offset The offset number to retrieve, Default value : 0.
	 */
	void GetListFollowers(const FString& UserId
		, THandler<FAccelByteModelsUGCGetListFollowersPagingResponse> const& OnSuccess
		, FErrorHandler const& OnError
		, int32 Limit = 1000
		, int32 Offset = 0);

	/**
	 * @brief Update follow/unfollow status to a user.
	 *
	 * @param UserId The id of the user that will be updated.
	 * @param bFollowStatus The new follow status value.
	 * @param OnSuccess This will be called when the operation succeeded.
	 * @param OnError This will be called when the operation failed.
	 */
	void UpdateFollowStatusToUser(const FString& UserId
		, bool bFollowStatus
		, THandler<FAccelByteModelsUGCUpdateFollowStatusToUserResponse> const& OnSuccess
		, FErrorHandler const& OnError);

	/**
	 * @brief Search contents specific to a channel.   
	 *
	 * @param ChannelId Channel Id.
	 * @param Name Content Name.
	 * @param Creator Creator Name.
	 * @param Type Content Type.
	 * @param Subtype Content Subtype.
	 * @param Tags Content Tags.
	 * @param IsOfficial Filter only official contents
	 * @param UserId User Id 
	 * @param OnSuccess This will be called when the operation succeeded.
	 * @param OnError This will be called when the operation failed.
	 * @param SortBy Sorting criteria, name,download,like,date. default=date.
	 * @param OrderBy Sorting order: asc, desc. default=desc
	 * @param Limit Number of content per page. Default value : 1000
	 * @param Offset The offset number to retrieve. Default value : 0
	 */
	void SearchContentsSpecificToChannel(const FString& ChannelId
		, const FString& Name
		, const FString& Creator
		, const FString& Type
		, const FString& Subtype
		, const TArray<FString>& Tags
		, bool IsOfficial
		, const FString& UserId
		, THandler<FAccelByteModelsUGCSearchContentsPagingResponse> const& OnSuccess
		, FErrorHandler const& OnError
		, EAccelByteUgcSortBy SortBy = EAccelByteUgcSortBy::DATE
		, EAccelByteUgcOrderBy OrderBy = EAccelByteUgcOrderBy::DESC
		, int32 Limit = 1000
		, int32 Offset = 0);

	/**
	 * @brief Get contents by content Ids
	 *
	 * @param ContentIds Content Ids Array  
	 * @param OnSuccess This will be called when the operation succeeded.
	 * @param OnError This will be called when the operation failed.
	 */	
	void GetContentBulk(const TArray<FString>& ContentIds
		, THandler<TArray<FAccelByteModelsUGCContentResponse>> const& OnSuccess
		, FErrorHandler const& OnError);

	/**
	 * @brief Get user's generated contents
	 *
	 * @param UserId User Id 
	 * @param OnSuccess This will be called when the operation succeeded.
	 * @param OnError This will be called when the operation failed.
	 * @param Limit Number of content per page. Default value : 1000
	 * @param Offset The offset number to retrieve. Default value : 0
	 */	
	void GetUserContent(const FString& UserId
		, THandler<FAccelByteModelsUGCContentPageResponse> const& OnSuccess
		, FErrorHandler const& OnError
		, int32 Limit = 1000
		, int32 Offset = 0);

	/**
	 * @brief Upload screenshots for content.   
	 *
	 * @param ContentId Content Id.
	 * @param UserId User Id
	 * @param ScreenshotsRequest Screenshots Request  
	 *	Supported file extensions: pjp, jpg, jpeg, jfif, bmp, png.
	 *	Maximum description length: 1024.
	 * @param OnSuccess This will be called when the operation succeeded.
	 * @param OnError This will be called when the operation failed.
	 */	
	void UploadContentScreenshot(const FString& ContentId
		, const FString& UserId
		, FAccelByteModelsUGCScreenshotsRequest ScreenshotsRequest
		, THandler<FAccelByteModelsUGCUpdateContentScreenshotResponse> const& OnSuccess
		, FErrorHandler const& OnError);

	/**
	 * @brief Get contents from followed creators.
	 * 
	 * @param OnSuccess This will be called when the operation succeeded.
	 * @param OnError This will be called when the operation failed. 
	 * @param Limit Number of content per page. Default value : 1000
	 * @param Offset The offset number to retrieve. Default value : 0
	 */	
	void GetFollowedContent(THandler<FAccelByteModelsUGCContentPageResponse> const& OnSuccess
		, FErrorHandler const& OnError
		, int32 Limit = 1000
		, int32 Offset = 0);

	/**
	 * @brief Get followed creators.   
	 *
	 * @param OnSuccess This will be called when the operation succeeded.
	 * @param OnError This will be called when the operation failed.
	 * @param Limit Number of content per page. Default value : 1000
	 * @param Offset The offset number to retrieve. Default value : 0
	 */	
	void GetFollowedUsers(THandler<FAccelByteModelsUGCFollowedUsersResponse> const& OnSuccess
		, FErrorHandler const& OnError
		, int32 Limit = 1000
		, int32 Offset = 0);

	/**
	 * @brief Get liked contents.   
	 *
	 * @param Tags Content Tags. 
	 * @param Name Content Name. 
	 * @param Type Content Type.
	 * @param Subtype Content Subtype.
	 * @param IsOfficial Filter only official contents
	 * @param OnSuccess This will be called when the operation succeeded.
	 * @param OnError This will be called when the operation failed.
	 * @param Limit Number of content per page. Default value : 1000
	 * @param Offset The offset number to retrieve. Default value : 0
	 * @param SortBy Sorting criteria, name,download,like,date. default=date.
	 * @param OrderBy Sorting order: asc, desc. default=desc	
	 * 
	 * @deprecated This method will be removed in the future, so please use
	 * 	void GetLikedContent(const TArray<FString>& Tags
		, ...
		, EAccelByteLikedContentSortBy SortBy = EAccelByteLikedContentSortBy::DATE
		, EAccelByteUgcOrderBy OrderBy = EAccelByteUgcOrderBy::DESC);
	 */
	void GetLikedContent(const TArray<FString>& Tags
		, const FString& Name
		, const FString& Type
		, const FString& Subtype
		, bool IsOfficial
		, THandler<FAccelByteModelsUGCContentPageResponse> const& OnSuccess
		, FErrorHandler const& OnError
		, int32 Limit = 1000
		, int32 Offset = 0
		, EAccelByteUgcSortBy SortBy = EAccelByteUgcSortBy::DATE
		, EAccelByteUgcOrderBy OrderBy = EAccelByteUgcOrderBy::DESC);

	/**
	 * @brief Get liked contents.   
	 *
	 * @param Tags Content Tags. 
	 * @param Name Content Name. 
	 * @param Type Content Type.
	 * @param Subtype Content Subtype.
	 * @param IsOfficial Filter only official contents
	 * @param OnSuccess This will be called when the operation succeeded.
	 * @param OnError This will be called when the operation failed.
	 * @param Limit Number of content per page. Default value : 1000
	 * @param Offset The offset number to retrieve. Default value : 0
	 * @param SortBy Sorting criteria, name,download,like,date. default=date.
	 * @param OrderBy Sorting order: asc, desc. default=desc	
	 */	
	void GetLikedContent(const TArray<FString>& Tags
		, const FString& Name
		, const FString& Type
		, const FString& Subtype
		, bool IsOfficial
		, THandler<FAccelByteModelsUGCContentPageResponse> const& OnSuccess
		, FErrorHandler const& OnError
		, int32 Limit = 1000
		, int32 Offset = 0
		, EAccelByteLikedContentSortBy SortBy = EAccelByteLikedContentSortBy::DATE
		, EAccelByteUgcOrderBy OrderBy = EAccelByteUgcOrderBy::DESC);

	/**
	 * @brief Get creator stats: number of total like by other user, number of total following and follower user.   
	 *
	 * @param UserId User Id 
	 * @param OnSuccess This will be called when the operation succeeded.
	 * @param OnError This will be called when the operation failed.
	 */	
	void GetCreator(const FString& UserId
		, THandler<FAccelByteModelsUGCGetListFollowersResponse> const& OnSuccess
		, FErrorHandler const& OnError);

	/**
	 * @brief Get all user groups 
	 *
	 * @param UserId User Id 
	 * @param OnSuccess This will be called when the operation succeeded.
	 * @param OnError This will be called when the operation failed.
	 * @param Limit Number of content per page. Default value : 1000
	 * @param Offset The offset number to retrieve. Default value : 0
	 */	
	void GetGroups(const FString& UserId
		, THandler<FAccelByteModelsUGCGetGroupsResponse> const& OnSuccess
		, FErrorHandler const& OnError
		, int32 Limit = 1000
		, int32 Offset = 0);
	
private:
	UGC() = delete;
	UGC(UGC const&) = delete;
	UGC(UGC&&) = delete;

	static FString ConvertLikedContentSortByToString(const EAccelByteLikedContentSortBy& SortBy);
	static FString ConvertUGCSortByToString(const EAccelByteUgcSortBy& SortBy);
	static FString ConvertUGCOrderByToString(const EAccelByteUgcOrderBy& OrderBy);
};

}
}
