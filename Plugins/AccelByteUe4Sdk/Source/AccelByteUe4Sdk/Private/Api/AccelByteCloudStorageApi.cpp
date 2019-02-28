// Copyright (c) 2018 - 2019 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "AccelByteCloudStorageApi.h"
#include "AccelByteError.h"
#include "JsonUtilities.h"
#include "AccelByteRegistry.h"

namespace AccelByte
{
namespace Api
{
	void CloudStorage::GetAllSlots(const THandler<TArray<FAccelByteModelsSlot>>& OnSuccess, const FErrorHandler& OnError)
	{
		FString Authorization = FString::Printf(TEXT("Bearer %s"), *FRegistry::Credentials.GetUserAccessToken());
		FString Url				= FString::Printf(TEXT("%s/public/namespaces/%s/users/%s/slots"), *FRegistry::Settings.CloudStorageServerUrl, *FRegistry::Credentials.GetUserNamespace(), *FRegistry::Credentials.GetUserId());
		FString Verb			= TEXT("GET");
		FString ContentType		= TEXT("application/json");
		FString Accept			= TEXT("application/json");
		FString Content			= TEXT("");

		FHttpRequestPtr Request = FHttpModule::Get().CreateRequest();
		Request->SetURL(Url);
		Request->SetHeader(TEXT("Authorization"), Authorization);
		Request->SetVerb(Verb);
		Request->SetHeader(TEXT("Content-Type"), ContentType);
		Request->SetHeader(TEXT("Accept"), Accept);
		Request->SetContentAsString(Content);
		Request->OnProcessRequestComplete() = CreateHttpResultHandler(OnSuccess, OnError);
		Request->ProcessRequest();
	}

	void CloudStorage::CreateSlot(TArray<uint8> BinaryData, const FString& FileName, const TArray<FString>& Tags, const FString& Label, const FString& CustomAttribute, const THandler<FAccelByteModelsSlot>& OnSuccess, FHttpRequestProgressDelegate OnProgress, const FErrorHandler& OnError)
	{
		FString Authorization = FString::Printf(TEXT("Bearer %s"), *FRegistry::Credentials.GetUserAccessToken());
		FString Url = FString::Printf(TEXT("%s/public/namespaces/%s/users/%s/slots"), *FRegistry::Settings.CloudStorageServerUrl, *FRegistry::Credentials.GetUserNamespace(), *FRegistry::Credentials.GetUserId());

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

		FString Verb = TEXT("POST");
		FString Accept = TEXT("*/*");
		FString BoundaryGuid = FGuid::NewGuid().ToString();
		TArray<uint8> Content = CustomAttributeFormDataBuilder(CustomAttribute, BoundaryGuid, false);
		Content.Append(FormDataBuilder(BinaryData, BoundaryGuid, FileName));

		FHttpRequestPtr Request = FHttpModule::Get().CreateRequest();
		Request->SetURL(Url);
		Request->SetHeader(TEXT("Authorization"), Authorization);
		Request->SetVerb(Verb);
		Request->SetHeader(TEXT("Accept"), Accept);
		Request->SetHeader(TEXT("Content-Type"), FString::Printf(TEXT("multipart/form-data; boundary=%s"), *BoundaryGuid));

		Request->SetContent(Content);
		Request->OnRequestProgress() = OnProgress;
		Request->OnProcessRequestComplete() = CreateHttpResultHandler(OnSuccess, OnError);
		Request->ProcessRequest();
		UE_LOG(LogTemp, Log, TEXT("[AccelByte] Cloud Storage Start uploading..."));
	}

	void CloudStorage::GetSlot(FString SlotID, const THandler<TArray<uint8>> & OnSuccess, const FErrorHandler & OnError)
	{
		FString Authorization = FString::Printf(TEXT("Bearer %s"), *FRegistry::Credentials.GetUserAccessToken());
		FString Url = FString::Printf(TEXT("%s/public/namespaces/%s/users/%s/slots/%s"), *FRegistry::Settings.CloudStorageServerUrl, *FRegistry::Credentials.GetUserNamespace(), *FRegistry::Credentials.GetUserId(), *SlotID);
		FString Verb = TEXT("GET");
		FString ContentType = TEXT("application/json");
		FString Accept = TEXT("*/*");
		FString Content = TEXT("");

		FHttpRequestPtr Request = FHttpModule::Get().CreateRequest();
		Request->SetURL(Url);
		Request->SetHeader(TEXT("Authorization"), Authorization);
		Request->SetVerb(Verb);
		Request->SetHeader(TEXT("Content-Type"), ContentType);
		Request->SetHeader(TEXT("Accept"), Accept);
		Request->SetContentAsString(Content);
		Request->OnProcessRequestComplete() = CreateHttpResultHandler(OnSuccess, OnError);
		Request->ProcessRequest();
	}

	void CloudStorage::UpdateSlot(FString SlotID, const TArray<uint8> BinaryData, const FString& FileName, const TArray<FString> & Tags, const FString& Label, const FString& CustomAttribute, const THandler<FAccelByteModelsSlot> & OnSuccess, FHttpRequestProgressDelegate OnProgress, const FErrorHandler & OnError)
	{
		FString Authorization = FString::Printf(TEXT("Bearer %s"), *FRegistry::Credentials.GetUserAccessToken());
		
		FString Url = FString::Printf(TEXT("%s/public/namespaces/%s/users/%s/slots/%s"), *FRegistry::Settings.CloudStorageServerUrl, *FRegistry::Credentials.GetUserNamespace(), *FRegistry::Credentials.GetUserId(), *SlotID);

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

		FString Verb = TEXT("PUT");
		FString Accept = TEXT("*/*");
		FString BoundaryGuid = FGuid::NewGuid().ToString();
		TArray<uint8> Content = CustomAttributeFormDataBuilder(CustomAttribute, BoundaryGuid, false);
		Content.Append(FormDataBuilder(BinaryData, BoundaryGuid, FileName));

		FHttpRequestPtr Request = FHttpModule::Get().CreateRequest();
		Request->SetURL(Url);
		Request->SetHeader(TEXT("Authorization"), Authorization);
		Request->SetVerb(Verb);
		Request->SetHeader(TEXT("Accept"), Accept);
		Request->SetHeader(TEXT("Content-Type"), FString::Printf(TEXT("multipart/form-data; boundary=%s"), *BoundaryGuid));

		Request->SetContent(Content);
		Request->OnRequestProgress() = OnProgress;
		Request->OnProcessRequestComplete() = CreateHttpResultHandler(OnSuccess, OnError);
		Request->ProcessRequest();
		UE_LOG(LogTemp, Log, TEXT("[AccelByte] Cloud Storage Start uploading..."));
	}

	void CloudStorage::UpdateSlotMetadata(FString SlotID, const FString& FileName, const TArray<FString> & Tags, const FString& Label, const FString& CustomAttribute, const THandler<FAccelByteModelsSlot> & OnSuccess, FHttpRequestProgressDelegate OnProgress, const FErrorHandler & OnError)
	{
		FString Authorization = FString::Printf(TEXT("Bearer %s"), *FRegistry::Credentials.GetUserAccessToken());
		
		FString Url = FString::Printf(TEXT("%s/public/namespaces/%s/users/%s/slots/%s/metadata"), *FRegistry::Settings.CloudStorageServerUrl, *FRegistry::Credentials.GetUserNamespace(), *FRegistry::Credentials.GetUserId(), *SlotID);

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

		FString Verb = TEXT("PUT");
		FString Accept = TEXT("*/*");
		FString BoundaryGuid = FGuid::NewGuid().ToString();
		TArray<uint8> Content = CustomAttributeFormDataBuilder(CustomAttribute, BoundaryGuid, true);

		FHttpRequestPtr Request = FHttpModule::Get().CreateRequest();
		Request->SetURL(Url);
		Request->SetHeader(TEXT("Authorization"), Authorization);
		Request->SetVerb(Verb);
		Request->SetHeader(TEXT("Accept"), Accept);
		Request->SetHeader(TEXT("Content-Type"), FString::Printf(TEXT("multipart/form-data; boundary=%s"), *BoundaryGuid));
		
		Request->SetContent(Content);
		Request->OnRequestProgress() = OnProgress;
		Request->OnProcessRequestComplete() = CreateHttpResultHandler(OnSuccess, OnError);
		Request->ProcessRequest();
	}

	void CloudStorage::DeleteSlot(FString SlotID, const FVoidHandler & OnSuccess, const FErrorHandler & OnError)
	{
		FString Authorization = FString::Printf(TEXT("Bearer %s"), *FRegistry::Credentials.GetUserAccessToken());
		FString Url = FString::Printf(TEXT("%s/public/namespaces/%s/users/%s/slots/%s"), *FRegistry::Settings.CloudStorageServerUrl, *FRegistry::Credentials.GetUserNamespace(), *FRegistry::Credentials.GetUserId(), *SlotID);
		FString Verb = TEXT("DELETE");
		FString ContentType = TEXT("application/json");
		FString Accept = TEXT("*/*");
		FString Content = TEXT("");

		FHttpRequestPtr Request = FHttpModule::Get().CreateRequest();
		Request->SetURL(Url);
		Request->SetHeader(TEXT("Authorization"), Authorization);
		Request->SetVerb(Verb);
		Request->SetHeader(TEXT("Content-Type"), ContentType);
		Request->SetHeader(TEXT("Accept"), Accept);
		Request->SetContentAsString(Content);
		Request->OnProcessRequestComplete() = CreateHttpResultHandler(OnSuccess, OnError);
		Request->ProcessRequest();
	}

	TArray<uint8> CloudStorage::FormDataBuilder(TArray<uint8> BinaryData, FString BoundaryGuid, FString FileName)
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

	TArray<uint8> CloudStorage::CustomAttributeFormDataBuilder(const FString& CustomAttribute, FString BoundaryGuid, bool CloseFooter)
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