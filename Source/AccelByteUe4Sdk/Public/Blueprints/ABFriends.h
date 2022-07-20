// Copyright (c) 2021 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "CoreMinimal.h"
#include "Core/AccelByteError.h"
#include "Models/AccelByteLobbyModels.h"
#include "Core/AccelByteMultiRegistry.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "ABFriends.generated.h"

using namespace AccelByte;

#pragma region MODEL_AND_DELEGATE_FOR_REQUEST_RESPONSE

DECLARE_DYNAMIC_DELEGATE_OneParam(FDRequestFriendsResponse, FAccelByteModelsRequestFriendsResponse, Response);

DECLARE_DYNAMIC_DELEGATE_OneParam(FDUnfriendResponse, FAccelByteModelsUnfriendResponse, Response);

DECLARE_DYNAMIC_DELEGATE_OneParam(FDListOutgoingFriendsResponse, FAccelByteModelsListOutgoingFriendsResponse, Response);

DECLARE_DYNAMIC_DELEGATE_OneParam(FDCancelFriendsResponse, FAccelByteModelsCancelFriendsResponse, Response);

DECLARE_DYNAMIC_DELEGATE_OneParam(FDListIncomingFriendsResponse, FAccelByteModelsListIncomingFriendsResponse, Response);

DECLARE_DYNAMIC_DELEGATE_OneParam(FDAcceptFriendsResponse, FAccelByteModelsAcceptFriendsResponse, Response);

DECLARE_DYNAMIC_DELEGATE_OneParam(FDRejectFriendsResponse, FAccelByteModelsRejectFriendsResponse, Response);

DECLARE_DYNAMIC_DELEGATE_OneParam(FDLoadFriendListResponse, FAccelByteModelsLoadFriendListResponse, Response);

DECLARE_DYNAMIC_DELEGATE_OneParam(FDGetFriendshipStatusResponse, FAccelByteModelsGetFriendshipStatusResponse, Response);

#pragma endregion 

#pragma region MODEL_AND_DELEGATE_FOR_NOTIFICATION

DECLARE_DYNAMIC_DELEGATE_OneParam(FDAcceptFriendsNotif, FAccelByteModelsAcceptFriendsNotif, Notif);

DECLARE_DYNAMIC_DELEGATE_OneParam(FDRequestFriendsNotif, FAccelByteModelsRequestFriendsNotif, Notif);

DECLARE_DYNAMIC_DELEGATE_OneParam(FDUnfriendNotif, FAccelByteModelsUnfriendNotif, Notif);

DECLARE_DYNAMIC_DELEGATE_OneParam(FDCancelFriendsNotif, FAccelByteModelsCancelFriendsNotif, Notif);

DECLARE_DYNAMIC_DELEGATE_OneParam(FDRejectFriendsNotif, FAccelByteModelsRejectFriendsNotif, Notif);

#pragma endregion

//Sub-API from AccelByte Lobby
UCLASS(Blueprintable, BlueprintType)
class UABFriends : public UObject
{
	GENERATED_BODY()
public:
	void SetApiClient(FApiClientPtr const& NewApiClientPtr);

//Request-Response
public: 
	UFUNCTION(BlueprintCallable, Category = "AccelByte | Friends | Api")
	void RequestFriend(FString UserId, FDRequestFriendsResponse OnResponse, FDErrorHandler OnError);

	UFUNCTION(BlueprintCallable, Category = "AccelByte | Friends | Api")
	void Unfriend(FString UserId, FDUnfriendResponse OnResponse, FDErrorHandler OnError);

	UFUNCTION(BlueprintCallable, Category = "AccelByte | Friends | Api")
	void ListOutgoingFriends(FDListOutgoingFriendsResponse OnResponse, FDErrorHandler OnError);

	UFUNCTION(BlueprintCallable, Category = "AccelByte | Friends | Api")
	void CancelFriendRequest(FString UserId, FDCancelFriendsResponse OnResponse, FDErrorHandler OnError);

	UFUNCTION(BlueprintCallable, Category = "AccelByte | Friends | Api")
	void ListIncomingFriends(FDListIncomingFriendsResponse OnResponse, FDErrorHandler OnError);

	UFUNCTION(BlueprintCallable, Category = "AccelByte | Friends | Api")
	void AcceptFriend(FString UserId, FDAcceptFriendsResponse OnResponse, FDErrorHandler OnError);

	UFUNCTION(BlueprintCallable, Category = "AccelByte | Friends | Api")
	void RejectFriend(FString UserId, FDRejectFriendsResponse OnResponse, FDErrorHandler OnError);

	UFUNCTION(BlueprintCallable, Category = "AccelByte | Friends | Api")
	void LoadFriendsList(FDLoadFriendListResponse OnResponse, FDErrorHandler OnError);

	UFUNCTION(BlueprintCallable, Category = "AccelByte | Friends | Api")
	void GetFriendshipStatus(FString UserId, FDGetFriendshipStatusResponse OnResponse, FDErrorHandler OnError);
	
	UFUNCTION(BlueprintCallable, Category = "AccelByte | Friends | Api")
	void BulkFriendRequest(FAccelByteModelsBulkFriendsRequest UserIds, FDHandler OnSuccess, FDErrorHandler OnError);

//Notification
public:
	UFUNCTION(BlueprintCallable, Category = "AccelByte | Friends | Api | Delegate")
	void SetOnFriendRequestAcceptedNotifDelegate(FDAcceptFriendsNotif OnNotif);

	UFUNCTION(BlueprintCallable, Category = "AccelByte | Friends | Api | Delegate")
	void SetOnIncomingRequestFriendsNotifDelegate(FDRequestFriendsNotif OnNotif);

	UFUNCTION(BlueprintCallable, Category = "AccelByte | Friends | Api | Delegate")
	void SetOnUnfriendNotifDelegate(FDUnfriendNotif OnNotif);

	UFUNCTION(BlueprintCallable, Category = "AccelByte | Friends | Api | Delegate")
	void SetOnCancelFriendsNotifDelegate(FDCancelFriendsNotif OnNotif);
	
	UFUNCTION(BlueprintCallable, Category = "AccelByte | Friends | Api | Delegate")
	void SetOnRejectFriendsNotifDelegate(FDRejectFriendsNotif OnNotif);

private:
	FApiClientPtr ApiClientPtr;
};
