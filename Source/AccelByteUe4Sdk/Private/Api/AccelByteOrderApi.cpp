// Copyright (c) 2018 - 2019 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "Api/AccelByteOrderApi.h"
#include "JsonUtilities.h"
#include "Core/AccelByteRegistry.h"
#include "Core/AccelByteReport.h"
#include "Core/AccelByteHttpRetryScheduler.h"
#include "Core/AccelByteUtilities.h"
#include "Core/AccelByteSettings.h"

namespace AccelByte
{
namespace Api
{

Order::Order(Credentials const& InCredentialsRef
	, Settings const& InSettingsRef
	, FHttpRetryScheduler& InHttpRef)
	: FApiBase(InCredentialsRef, InSettingsRef, InHttpRef)
{}

Order::~Order()
{}

FAccelByteTaskWPtr Order::CreateNewOrder(FAccelByteModelsOrderCreate const& OrderCreate
	, THandler<FAccelByteModelsOrderInfo> const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	const FString Url = FString::Printf(TEXT("%s/public/namespaces/%s/users/%s/orders")
		, *SettingsRef.PlatformServerUrl
		, *CredentialsRef->GetNamespace()
		, *CredentialsRef->GetUserId());

	FString Content;
	const TSharedPtr<FJsonObject> JsonObject = FJsonObjectConverter::UStructToJsonObject(OrderCreate);
	FAccelByteUtilities::RemoveEmptyStrings(JsonObject);
	FAccelByteUtilities::RemoveEmptyFieldsFromJson(JsonObject);
	const TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&Content);
	FJsonSerializer::Serialize(JsonObject.ToSharedRef(), Writer);

	return HttpClient.ApiRequest(TEXT("POST"), Url, {}, Content, OnSuccess, OnError);
}

FAccelByteTaskWPtr Order::CancelOrder(FString const& OrderNo
	, THandler<FAccelByteModelsOrderInfo> const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	const FString Url = FString::Printf(TEXT("%s/public/namespaces/%s/users/%s/orders/%s/cancel")
		, *SettingsRef.PlatformServerUrl
		, *CredentialsRef->GetNamespace()
		, *CredentialsRef->GetUserId()
		, *OrderNo);

	return HttpClient.ApiRequest(TEXT("PUT"), Url, {}, FString(), OnSuccess, OnError);
}

FAccelByteTaskWPtr Order::GetUserOrder(FString const& OrderNo
	, THandler<FAccelByteModelsOrderInfo> const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	const FString Url = FString::Printf(TEXT("%s/public/namespaces/%s/users/%s/orders/%s")
		, *SettingsRef.PlatformServerUrl
		, *CredentialsRef->GetNamespace()
		, *CredentialsRef->GetUserId()
		, *OrderNo);

	return HttpClient.ApiRequest(TEXT("GET"), Url, {}, FString(), OnSuccess, OnError);
}

FAccelByteTaskWPtr Order::GetUserOrders(int32 Page
	, int32 Size
	, THandler<FAccelByteModelsPagedOrderInfo> const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	const FString Url = FString::Printf(TEXT("%s/public/namespaces/%s/users/%s/orders")
		, *SettingsRef.PlatformServerUrl
		, *CredentialsRef->GetNamespace()
		, *CredentialsRef->GetUserId());

	const TMultiMap<FString, FString> QueryParams = {
		{TEXT("page"), FString::FromInt(Page)},
		{TEXT("size"), FString::FromInt(Size)}
	};

	return HttpClient.ApiRequest(TEXT("GET"), Url, QueryParams, FString(), OnSuccess, OnError);
}

FAccelByteTaskWPtr Order::QueryUserOrders(FAccelByteModelsUserOrdersRequest const& UserOrderRequest
    , THandler<FAccelByteModelsPagedOrderInfo> const& OnSuccess
	, FErrorHandler const& OnError) 
{
	FReport::Log(FString(__FUNCTION__));

	const FString Url = FString::Printf(TEXT("%s/public/namespaces/%s/users/%s/orders")
		, *SettingsRef.PlatformServerUrl
		, *CredentialsRef->GetNamespace()
		, *CredentialsRef->GetUserId());

	FString Status = UserOrderRequest.Status != EAccelByteOrderStatus::NONE ? 
		FAccelByteUtilities::GetUEnumValueAsString(UserOrderRequest.Status) : TEXT("");
	const TMultiMap<FString, FString> QueryParams = {
		{TEXT("itemId"), UserOrderRequest.ItemId},
		{TEXT("status"), Status },
		{TEXT("offset"), FString::FromInt(UserOrderRequest.Offset)},
		{TEXT("limit"), FString::FromInt(UserOrderRequest.Limit)}
	};

	return HttpClient.ApiRequest(TEXT("GET"), Url, QueryParams, FString(), OnSuccess, OnError);
}

FAccelByteTaskWPtr Order::GetUserOrderHistory(FString const& OrderNo
	, THandler<TArray<FAccelByteModelsOrderHistoryInfo>> const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	const FString Url = FString::Printf(TEXT("%s/public/namespaces/%s/users/%s/orders/%s/history")
		, *SettingsRef.PlatformServerUrl
		, *CredentialsRef->GetNamespace()
		, *CredentialsRef->GetUserId()
		, *OrderNo);

	return HttpClient.ApiRequest(TEXT("GET"), Url, {}, FString(), OnSuccess, OnError);
}

FAccelByteTaskWPtr Order::PreviewUserOrder(FAccelByteModelsUserPreviewOrderRequest const& PreviewOrderRequest,
	THandler<FAccelByteModelsUserPreviewOrderResponse> const& OnSuccess, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	const FString Url = FString::Printf(TEXT("%s/public/namespaces/%s/users/%s/orders/discount/preview")
		, *SettingsRef.PlatformServerUrl
		, *CredentialsRef->GetNamespace()
		, *CredentialsRef->GetUserId());

	return HttpClient.ApiRequest(TEXT("POST"), Url, {}, PreviewOrderRequest, OnSuccess, OnError);
}
	
} // Namespace Api
} // Namespace AccelByte
