// Copyright (c) 2018 - 2020 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "Api/AccelByteCloudStorageApi.h"
#include "Core/AccelByteError.h"
#include "JsonUtilities.h"
#include "Core/AccelByteRegistry.h"
#include "Core/AccelByteReport.h"
#include "Core/AccelByteSettings.h"

namespace AccelByte
{
namespace Api
{
CloudStorage::CloudStorage(Credentials const& InCredentialsRef
	, Settings const& InSettingsRef
	, FHttpRetryScheduler& InHttpRef)
	: FApiBase(InCredentialsRef, InSettingsRef, InHttpRef)
{}

CloudStorage::~CloudStorage()
{}

void CloudStorage::GetAllSlots(const THandler<TArray<FAccelByteModelsSlot>>& OnSuccess
	, const FErrorHandler& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	const FString Url = FString::Printf(TEXT("%s/public/namespaces/%s/users/%s/slots")
		, *SettingsRef.CloudStorageServerUrl
		, *CredentialsRef.GetNamespace()
		, *CredentialsRef.GetUserId());

	HttpClient.ApiRequest(TEXT("GET"), Url, {}, FString(), OnSuccess, OnError);
}

void CloudStorage::CreateSlot(TArray<uint8> BinaryData
	, const FString& FileName
	, const TArray<FString>& Tags
	, const FString& Label
	, const FString& CustomAttribute
	, const THandler<FAccelByteModelsSlot>& OnSuccess
	, FHttpRequestProgressDelegate OnProgress
	, const FErrorHandler& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	FString Authorization = FString::Printf(TEXT("Bearer %s"), *CredentialsRef.GetAccessToken());
	FString Url = FString::Printf(TEXT("%s/public/namespaces/%s/users/%s/slots")
		, *SettingsRef.CloudStorageServerUrl
		, *CredentialsRef.GetNamespace()
		, *CredentialsRef.GetUserId());

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

	HttpClient.Request(TEXT("POST"), Url, {}, Content, Headers, OnSuccess, OnProgress, OnError);
	FReport::Log(TEXT("[AccelByte] Cloud Storage Start uploading..."));
}

void CloudStorage::GetSlot(FString SlotID
	, const THandler<TArray<uint8>> & OnSuccess
	, const FErrorHandler & OnError)
{
	FReport::Log(FString(__FUNCTION__));

	FString Authorization = FString::Printf(TEXT("Bearer %s"), *CredentialsRef.GetAccessToken());
	FString Url = FString::Printf(TEXT("%s/public/namespaces/%s/users/%s/slots/%s")
		, *SettingsRef.CloudStorageServerUrl
		, *CredentialsRef.GetNamespace()
		, *CredentialsRef.GetUserId()
		, *SlotID);

	const TMap<FString, FString> Headers = {
		{TEXT("Authorization"), Authorization},
		{TEXT("Content-Type"), TEXT("application/json")},
		{TEXT("Accept"), TEXT("*/*")}
	};

	HttpClient.Request(TEXT("GET"), Url, {}, FString(), Headers, OnSuccess, OnError);
}

void CloudStorage::UpdateSlot(FString SlotID
	, const TArray<uint8> BinaryData
	, const FString& FileName
	, const TArray<FString> & Tags
	, const FString& Label
	, const FString& CustomAttribute
	, const THandler<FAccelByteModelsSlot> & OnSuccess
	, FHttpRequestProgressDelegate OnProgress
	, const FErrorHandler & OnError)
{
	FReport::Log(FString(__FUNCTION__));

	FString Authorization = FString::Printf(TEXT("Bearer %s"), *CredentialsRef.GetAccessToken());	
	FString Url = FString::Printf(TEXT("%s/public/namespaces/%s/users/%s/slots/%s")
		, *SettingsRef.CloudStorageServerUrl
		, *CredentialsRef.GetNamespace()
		, *CredentialsRef.GetUserId()
		, *SlotID);

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

	HttpClient.Request(TEXT("PUT"), Url, {}, Content, Headers, OnSuccess, OnProgress, OnError);

	FReport::Log(TEXT("[AccelByte] Cloud Storage Start uploading..."));
}

void CloudStorage::UpdateSlotMetadata(const FString& SlotId
	, const FString& FileName
	, const TArray<FString>& Tags
	, const FString& Label
	, const FString& CustomAttribute
	, const THandler<FAccelByteModelsSlot>& OnSuccess
	, FHttpRequestProgressDelegate OnProgress
	, const FErrorHandler& OnError)
{
	FReport::LogDeprecated(
		FString(__FUNCTION__),
		TEXT("Use UpdateSlotMetadata(const FString&, const TArray<FString>& Tags, const FString& Label, const FString& CustomAttribute, const THandler<FAccelByteModelsSlot>& OnSuccess, FHttpRequestProgressDelegate OnProgress, const FErrorHandler& OnError) instead."));

	UpdateSlotMetadata(SlotId, Tags, Label, CustomAttribute, OnSuccess, OnProgress, OnError);
}

void CloudStorage::UpdateSlotMetadata(const FString& SlotId
	, const TArray<FString>& Tags
	, const FString& Label
	, const FString& CustomAttribute
	, const THandler<FAccelByteModelsSlot>& OnSuccess
	, FHttpRequestProgressDelegate OnProgress
	, const FErrorHandler& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	FString Authorization = FString::Printf(TEXT("Bearer %s"), *CredentialsRef.GetAccessToken());
	FString Url = FString::Printf(TEXT("%s/public/namespaces/%s/users/%s/slots/%s/metadata")
		, *SettingsRef.CloudStorageServerUrl
		, *CredentialsRef.GetNamespace()
		, *CredentialsRef.GetUserId()
		, *SlotId);

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
	
	HttpClient.Request(TEXT("PUT"), Url, {}, Content, Headers, OnSuccess, OnProgress, OnError);
}

void CloudStorage::DeleteSlot(FString SlotID
	, const FVoidHandler & OnSuccess
	, const FErrorHandler & OnError)
{
	FReport::Log(FString(__FUNCTION__));

	FString Url = FString::Printf(TEXT("%s/public/namespaces/%s/users/%s/slots/%s")
		, *SettingsRef.CloudStorageServerUrl
		, *CredentialsRef.GetNamespace()
		, *CredentialsRef.GetUserId()
		, *SlotID);

	TMap<FString, FString> Headers = {
		{TEXT("Content-Type"), TEXT("application/json")},
		{TEXT("Accept"), TEXT("*/*")}
	};

	HttpClient.ApiRequest(TEXT("DELETE"), Url, {}, FString(), Headers, OnSuccess, OnError);
}

TArray<uint8> CloudStorage::FormDataBuilder(TArray<uint8> BinaryData
	, FString BoundaryGuid
	, FString FileName)
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

TArray<uint8> CloudStorage::CustomAttributeFormDataBuilder(const FString& CustomAttribute
	, FString BoundaryGuid
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

}
}
