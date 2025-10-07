// Copyright (c) 2018 - 2020 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "Api/AccelByteCloudStorageApi.h"
#include "Core/AccelByteError.h"
#include "JsonUtilities.h"

#include "Core/AccelByteReport.h"
#include "Core/AccelByteSettings.h"

namespace AccelByte
{
namespace Api
{
CloudStorage::CloudStorage(Credentials const& InCredentialsRef
	, Settings const& InSettingsRef
	, FHttpRetrySchedulerBase& InHttpRef
	, TSharedPtr<FApiClient, ESPMode::ThreadSafe> InApiClient)
	: FApiBase(InCredentialsRef, InSettingsRef, InHttpRef, InApiClient)
{}

CloudStorage::~CloudStorage()
{}

FAccelByteTaskWPtr CloudStorage::GetAllSlots(THandler<TArray<FAccelByteModelsSlot>> const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	FReport::LogDeprecated(FString(__FUNCTION__),
		TEXT("Cloud Storage is deprecated - please use Binary Cloudsave for the replacement"));

	const FString Url = FString::Printf(TEXT("%s/public/namespaces/%s/users/%s/slots")
		, *SettingsRef.CloudStorageServerUrl
		, *FGenericPlatformHttp::UrlEncode(CredentialsRef->GetNamespace())
		, *FGenericPlatformHttp::UrlEncode(CredentialsRef->GetUserId()));

	return HttpClient.ApiRequest(TEXT("GET"), Url, {}, FString(), OnSuccess, OnError);
}

FAccelByteTaskWPtr CloudStorage::CreateSlot(TArray<uint8> const& BinaryData
	, FString const& FileName
	, TArray<FString> const& Tags
	, FString const& Label
	, FString const& CustomAttribute
	, THandler<FAccelByteModelsSlot> const& OnSuccess
	, FHttpRequestProgressDelegate const& OnProgress
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	FReport::LogDeprecated(FString(__FUNCTION__),
		TEXT("Cloud Storage is deprecated - please use Binary Cloudsave for the replacement"));

	FString Authorization = FString::Printf(TEXT("Bearer %s"), *CredentialsRef->GetAccessToken());
	FString Url = FString::Printf(TEXT("%s/public/namespaces/%s/users/%s/slots")
		, *SettingsRef.CloudStorageServerUrl
		, *FGenericPlatformHttp::UrlEncode(CredentialsRef->GetNamespace())
		, *FGenericPlatformHttp::UrlEncode(CredentialsRef->GetUserId()));

	if (Tags.Num() != 0 || !Label.IsEmpty())
	{
		Url.Append(TEXT("?"));
	}

	if (Tags.Num() != 0)
	{
		for (int i = 0; i < Tags.Num(); i++)
		{
			Url.Append(FString::Printf(TEXT("tags=%s&"), *FGenericPlatformHttp::UrlEncode(Tags[i])));
		}
	}
	if (!Label.IsEmpty())
	{
		Url.Append(FString::Printf(TEXT("label=%s"), *FGenericPlatformHttp::UrlEncode(Label)));
	}

	const FString BoundaryGuid = FGuid::NewGuid().ToString();
	TArray<uint8> Content = CustomAttributeFormDataBuilder(CustomAttribute, BoundaryGuid, false);
	Content.Append(FormDataBuilder(BinaryData, BoundaryGuid, FileName));

	const TMap<FString, FString> Headers = {
		{TEXT("Authorization"), Authorization},
		{TEXT("Content-Type"), FString::Printf(TEXT("multipart/form-data; boundary=%s"), *BoundaryGuid)},
		{TEXT("Accept"), TEXT("*/*")}
	};

	FReport::Log(TEXT("[AccelByte] Cloud Storage Start uploading..."));

	return HttpClient.Request(TEXT("POST"), Url, {}, Content, Headers, OnSuccess, OnProgress, OnError);
}

FAccelByteTaskWPtr CloudStorage::GetSlot(FString SlotID
	, const THandler<TArray<uint8>> & OnSuccess
	, const FErrorHandler & OnError)
{
	FReport::Log(FString(__FUNCTION__));

	FReport::LogDeprecated(FString(__FUNCTION__),
		TEXT("Cloud Storage is deprecated - please use Binary Cloudsave for the replacement"));

	FString Authorization = FString::Printf(TEXT("Bearer %s"), *CredentialsRef->GetAccessToken());
	FString Url = FString::Printf(TEXT("%s/public/namespaces/%s/users/%s/slots/%s")
		, *SettingsRef.CloudStorageServerUrl
		, *FGenericPlatformHttp::UrlEncode(CredentialsRef->GetNamespace())
		, *FGenericPlatformHttp::UrlEncode(CredentialsRef->GetUserId())
		, *FGenericPlatformHttp::UrlEncode(SlotID));

	const TMap<FString, FString> Headers = {
		{TEXT("Authorization"), Authorization},
		{TEXT("Content-Type"), TEXT("application/json")},
		{TEXT("Accept"), TEXT("*/*")}
	};

	return HttpClient.Request(TEXT("GET"), Url, {}, FString(), Headers, OnSuccess, OnError);
}

FAccelByteTaskWPtr CloudStorage::UpdateSlot(FString const& SlotID
	, TArray<uint8> const& BinaryData
	, FString const& FileName
	, TArray<FString> const& Tags
	, FString const& Label
	, FString const& CustomAttribute
	, THandler<FAccelByteModelsSlot> const& OnSuccess
	, FHttpRequestProgressDelegate const& OnProgress
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	FReport::LogDeprecated(FString(__FUNCTION__),
		TEXT("Cloud Storage is deprecated - please use Binary Cloudsave for the replacement"));

	FString Authorization = FString::Printf(TEXT("Bearer %s"), *CredentialsRef->GetAccessToken());	
	FString Url = FString::Printf(TEXT("%s/public/namespaces/%s/users/%s/slots/%s")
		, *SettingsRef.CloudStorageServerUrl
		, *FGenericPlatformHttp::UrlEncode(CredentialsRef->GetNamespace())
		, *FGenericPlatformHttp::UrlEncode(CredentialsRef->GetUserId())
		, *FGenericPlatformHttp::UrlEncode(SlotID));

	if (Tags.Num() != 0 || !Label.IsEmpty())
	{
		Url.Append(TEXT("?"));
	}

	if (Tags.Num() != 0)
	{
		for (int i = 0; i < Tags.Num(); i++)
		{
			Url.Append(FString::Printf(TEXT("tags=%s&"), *FGenericPlatformHttp::UrlEncode(Tags[i])));
		}
	}
	if (!Label.IsEmpty())
	{
		Url.Append(FString::Printf(TEXT("label=%s"), *FGenericPlatformHttp::UrlEncode(Label)));
	}

	FString BoundaryGuid    = FGuid::NewGuid().ToString();
	TArray<uint8> Content   = CustomAttributeFormDataBuilder(CustomAttribute, BoundaryGuid, false);
	Content.Append(FormDataBuilder(BinaryData, BoundaryGuid, FileName));

	const TMap<FString, FString> Headers = {
		{TEXT("Authorization"), Authorization},
		{TEXT("Content-Type"), FString::Printf(TEXT("multipart/form-data; boundary=%s"), *BoundaryGuid)},
		{TEXT("Accept"), TEXT("*/*")}
	};

	FReport::Log(TEXT("[AccelByte] Cloud Storage Start uploading..."));

	return HttpClient.Request(TEXT("PUT"), Url, {}, Content, Headers, OnSuccess, OnProgress, OnError);
}

FAccelByteTaskWPtr CloudStorage::UpdateSlotMetadata(FString const& SlotId
	, FString const& FileName
	, TArray<FString> const& Tags
	, FString const& Label
	, FString const& CustomAttribute
	, THandler<FAccelByteModelsSlot> const& OnSuccess
	, FHttpRequestProgressDelegate const& OnProgress
	, FErrorHandler const& OnError)
{
	FReport::LogDeprecated(
		FString(__FUNCTION__),
		TEXT("Use UpdateSlotMetadata(FString const&, const TArray<FString>& Tags, FString const& Label, FString const& CustomAttribute, const THandler<FAccelByteModelsSlot>& OnSuccess, FHttpRequestProgressDelegate OnProgress, const FErrorHandler& OnError) instead."));

	return UpdateSlotMetadata(SlotId, Tags, Label, CustomAttribute, OnSuccess, OnProgress, OnError);
}

FAccelByteTaskWPtr CloudStorage::UpdateSlotMetadata(FString const& SlotId
	, TArray<FString> const& Tags
	, FString const& Label
	, FString const& CustomAttribute
	, THandler<FAccelByteModelsSlot> const& OnSuccess
	, FHttpRequestProgressDelegate const& OnProgress
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	FReport::LogDeprecated(FString(__FUNCTION__),
		TEXT("Cloud Storage is deprecated - please use Binary Cloudsave for the replacement"));

	FString Authorization = FString::Printf(TEXT("Bearer %s"), *CredentialsRef->GetAccessToken());
	FString Url = FString::Printf(TEXT("%s/public/namespaces/%s/users/%s/slots/%s/metadata")
		, *SettingsRef.CloudStorageServerUrl
		, *FGenericPlatformHttp::UrlEncode(CredentialsRef->GetNamespace())
		, *FGenericPlatformHttp::UrlEncode(CredentialsRef->GetUserId())
		, *FGenericPlatformHttp::UrlEncode(SlotId));

	FString Content;
	FAccelByteModelsUpdateMetadataRequest UpdateMedataRequest;
	UpdateMedataRequest.Tags = Tags;
	UpdateMedataRequest.Label = Label;
	UpdateMedataRequest.CustomAttribute = CustomAttribute;
	FJsonObjectConverter::UStructToJsonObjectString(UpdateMedataRequest, Content);

	const TMap<FString, FString> Headers = {
		{TEXT("Authorization"), Authorization},
		{TEXT("Content-Type"), TEXT("application/json")},
		{TEXT("Accept"), TEXT("application/json")}
	};
	
	return HttpClient.Request(TEXT("PUT"), Url, {}, Content, Headers, OnSuccess, OnProgress, OnError);
}

FAccelByteTaskWPtr CloudStorage::DeleteSlot(FString const& SlotID
	, FVoidHandler const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	FReport::LogDeprecated(FString(__FUNCTION__),
		TEXT("Cloud Storage is deprecated - please use Binary Cloudsave for the replacement"));

	FString Url = FString::Printf(TEXT("%s/public/namespaces/%s/users/%s/slots/%s")
		, *SettingsRef.CloudStorageServerUrl
		, *FGenericPlatformHttp::UrlEncode(CredentialsRef->GetNamespace())
		, *FGenericPlatformHttp::UrlEncode(CredentialsRef->GetUserId())
		, *FGenericPlatformHttp::UrlEncode(SlotID));

	TMap<FString, FString> Headers = {
		{TEXT("Content-Type"), TEXT("application/json")},
		{TEXT("Accept"), TEXT("*/*")}
	};

	return HttpClient.ApiRequest(TEXT("DELETE"), Url, {}, FString(), Headers, OnSuccess, OnError);
}

TArray<uint8> CloudStorage::FormDataBuilder(TArray<uint8> const& BinaryData
	, FString const& BoundaryGuid
	, FString const& FileName)
{
	FString FormBoundaryHeader = "\r\n--" + BoundaryGuid + "\r\n";
	FString FormContentDisposition = "Content-Disposition: form-data; name=\"file\";  filename=\"" + FileName + "\"\r\n";
	FString FormContentType = "Content-Type: image/png\r\n\r\n";
	FString FormBoundaryFooter = "\r\n--" + BoundaryGuid + "--\r\n";

	TArray<uint8> Data;
	Data.Append((uint8*)TCHAR_TO_UTF8(*FormBoundaryHeader), FormBoundaryHeader.Len());
	Data.Append((uint8*)TCHAR_TO_UTF8(*FormContentDisposition), FormContentDisposition.Len());
	Data.Append((uint8*)TCHAR_TO_UTF8(*FormContentType), FormContentType.Len());
	Data.Append(BinaryData);
	Data.Append((uint8*)TCHAR_TO_UTF8(*FormBoundaryFooter), FormBoundaryFooter.Len());

	return Data;
}

TArray<uint8> CloudStorage::CustomAttributeFormDataBuilder(FString const& CustomAttribute
	, FString const& BoundaryGuid
	, bool CloseFooter)
{
	FString FormBoundaryHeader = "\r\n--" + BoundaryGuid + "\r\n";
	FString FormContentDisposition = "Content-Disposition: form-data; name=\"customAttribute\"\r\n\r\n";
	FString FormBoundaryFooter = CloseFooter ? ("\r\n--" + BoundaryGuid + "--\r\n") : "" ;

	TArray<uint8> Data;
	Data.Append((uint8*)TCHAR_TO_UTF8(*FormBoundaryHeader), FormBoundaryHeader.Len());
	Data.Append((uint8*)TCHAR_TO_UTF8(*FormContentDisposition), FormContentDisposition.Len());
	Data.Append((uint8*)TCHAR_TO_UTF8(*CustomAttribute), CustomAttribute.Len());
	Data.Append((uint8*)TCHAR_TO_UTF8(*FormBoundaryFooter), FormBoundaryFooter.Len());

	return Data;
}

} // Namespace Api
} // Namespace AccelByte
