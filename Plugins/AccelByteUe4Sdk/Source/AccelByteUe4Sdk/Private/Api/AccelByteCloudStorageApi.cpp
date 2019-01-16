// Copyright (c) 2018 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "AccelByteCloudStorageApi.h"
#include "AccelByteError.h"
#include "JsonUtilities.h"
#include "AccelByteSettings.h"
#include "AccelByteCredentials.h"

namespace AccelByte
{
namespace Api
{
	void CloudStorage::GetAllSlots(const FGetAllSlotsSuccess& OnSuccess, const FErrorHandler& OnError)
	{
		FString Authorization = FString::Printf(TEXT("Bearer %s"), *Credentials::Get().GetUserAccessToken());
		FString Url				= FString::Printf(TEXT("%s/namespaces/%s/users/%s/slots"), *Settings::CloudStorageServerUrl, *Credentials::Get().GetUserNamespace(), *Credentials::Get().GetUserId());
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
		Request->OnProcessRequestComplete().BindStatic(OnGetAllSlotsResponse, OnSuccess, OnError);
		Request->ProcessRequest();
	}

	void CloudStorage::CreateSlot(TArray<uint8> BinaryData, const FString& FileName, const FString& Tags, const FString& Label, const FCreateSlotSuccess& OnSuccess, FHttpRequestProgressDelegate OnProgress, const FErrorHandler& OnError)
	{
		FString Authorization = FString::Printf(TEXT("Bearer %s"), *Credentials::Get().GetUserAccessToken());
		FString Url = FString::Printf(TEXT("%s/namespaces/%s/users/%s/slots"), *Settings::CloudStorageServerUrl, *Credentials::Get().GetUserNamespace(), *Credentials::Get().GetUserId());

		if (!Tags.IsEmpty() || !Label.IsEmpty())
		{
			Url.Append(TEXT("?"));
		}

		if (!Tags.IsEmpty())
		{
			Url.Append(FString::Printf(TEXT("tags=%s&"), *FGenericPlatformHttp::UrlEncode(Tags)));
		}
		if (!Label.IsEmpty())
		{
			Url.Append(FString::Printf(TEXT("label=%s"), *FGenericPlatformHttp::UrlEncode(Label)));
		}

		FString Verb = TEXT("POST");
		FString Accept = TEXT("*/*");
		FString BoundaryGuid = FGuid::NewGuid().ToString();
		TArray<uint8> Content = FormDataBuilder(BinaryData, BoundaryGuid, FileName);

		FHttpRequestPtr Request = FHttpModule::Get().CreateRequest();
		Request->SetURL(Url);
		Request->SetHeader(TEXT("Authorization"), Authorization);
		Request->SetVerb(Verb);
		Request->SetHeader(TEXT("Accept"), Accept);
		Request->SetHeader(TEXT("Content-Type"), FString::Printf(TEXT("multipart/form-data; boundary=%s"), *BoundaryGuid));

		Request->SetContent(Content);
		Request->OnRequestProgress() = OnProgress;
		Request->OnProcessRequestComplete().BindStatic(OnCreateSlotResponse, OnSuccess, OnError);
		Request->ProcessRequest();
		UE_LOG(LogTemp, Log, TEXT("[AccelByte] Cloud Storage Start uploading..."));
	}

	void CloudStorage::GetSlot(FString SlotID, const FGetSlotSuccess & OnSuccess, const FErrorHandler & OnError)
	{
		FString Authorization = FString::Printf(TEXT("Bearer %s"), *Credentials::Get().GetUserAccessToken());
		FString Url = FString::Printf(TEXT("%s/namespaces/%s/users/%s/slots/%s"), *Settings::CloudStorageServerUrl, *Credentials::Get().GetUserNamespace(), *Credentials::Get().GetUserId(), *SlotID);
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
		Request->OnProcessRequestComplete().BindStatic(OnGetSlotResponse, OnSuccess, OnError);
		Request->ProcessRequest();
	}

	void CloudStorage::UpdateSlot(FString SlotID, const TArray<uint8> BinaryData, const FString& FileName, const FString & Tags, const FString& Label, const FUpdateSlotSuccess & OnSuccess, FHttpRequestProgressDelegate OnProgress, const FErrorHandler & OnError)
	{
		FString Authorization = FString::Printf(TEXT("Bearer %s"), *Credentials::Get().GetUserAccessToken());
		
		FString Url = FString::Printf(TEXT("%s/namespaces/%s/users/%s/slots/%s"), *Settings::CloudStorageServerUrl, *Credentials::Get().GetUserNamespace(), *Credentials::Get().GetUserId(), *SlotID);

		if (!Tags.IsEmpty() || !Label.IsEmpty())
		{
			Url.Append(TEXT("?"));
		}

		if (!Tags.IsEmpty())
		{
			Url.Append(FString::Printf(TEXT("tags=%s&"), *FGenericPlatformHttp::UrlEncode(Tags)));
		}
		if (!Label.IsEmpty())
		{
			Url.Append(FString::Printf(TEXT("label=%s"), *FGenericPlatformHttp::UrlEncode(Label)));
		}

		FString Verb = TEXT("PUT");
		FString Accept = TEXT("*/*");
		FString BoundaryGuid = FGuid::NewGuid().ToString();
		TArray<uint8> Content = FormDataBuilder(BinaryData, BoundaryGuid, FileName);

		FHttpRequestPtr Request = FHttpModule::Get().CreateRequest();
		Request->SetURL(Url);
		Request->SetHeader(TEXT("Authorization"), Authorization);
		Request->SetVerb(Verb);
		Request->SetHeader(TEXT("Accept"), Accept);
		Request->SetHeader(TEXT("Content-Type"), FString::Printf(TEXT("multipart/form-data; boundary=%s"), *BoundaryGuid));

		Request->SetContent(Content);
		Request->OnRequestProgress() = OnProgress;
		Request->OnProcessRequestComplete().BindStatic(OnUpdateSlotResponse, OnSuccess, OnError);
		Request->ProcessRequest();
		UE_LOG(LogTemp, Log, TEXT("[AccelByte] Cloud Storage Start uploading..."));
	}

	void CloudStorage::DeleteSlot(FString SlotID, const FDeleteSlotSuccess & OnSuccess, const FErrorHandler & OnError)
	{
		FString Authorization = FString::Printf(TEXT("Bearer %s"), *Credentials::Get().GetUserAccessToken());
		FString Url = FString::Printf(TEXT("%s/namespaces/%s/users/%s/slots/%s"), *Settings::CloudStorageServerUrl, *Credentials::Get().GetUserNamespace(), *Credentials::Get().GetUserId(), *SlotID);
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
		Request->OnProcessRequestComplete().BindStatic(OnDeleteSlotResponse, OnSuccess, OnError);
		Request->ProcessRequest();
	}

	TArray<uint8> CloudStorage::FormDataBuilder(TArray<uint8> BinaryData, FString BoundaryGuid, FString FileName = "untitled.txt")
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

	void CloudStorage::OnGetAllSlotsResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, bool Successful, FGetAllSlotsSuccess OnSuccess, FErrorHandler OnError)
	{
		int32 Code;
		FString Message;
		TArray<FAccelByteModelsSlot> Result;
		if (EHttpResponseCodes::IsOk(Response->GetResponseCode()))
		{
			if (FJsonObjectConverter::JsonArrayStringToUStruct(Response->GetContentAsString(), &Result, 0, 0))
			{
				OnSuccess.ExecuteIfBound(Result);
				return;
			}
			Code = static_cast<int32>(ErrorCodes::JsonDeserializationFailed);
		}
		HandleHttpError(Request, Response, Code, Message);
		OnError.ExecuteIfBound(Code, Message);
	}

	void CloudStorage::OnCreateSlotResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, bool Successful, FCreateSlotSuccess OnSuccess, FErrorHandler OnError)
	{
		int32 Code;
		FString Message;
		FAccelByteModelsSlot Result;
		if (EHttpResponseCodes::IsOk(Response->GetResponseCode()))
		{
			if (FJsonObjectConverter::JsonObjectStringToUStruct(Response->GetContentAsString(), &Result, 0, 0))
			{
				OnSuccess.ExecuteIfBound(Result);
				return;
			}
			Code = static_cast<int32>(ErrorCodes::JsonDeserializationFailed);
		}
		HandleHttpError(Request, Response, Code, Message);
		OnError.ExecuteIfBound(Code, Message);
	}

	void CloudStorage::OnUpdateSlotResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, bool Successful, FCreateSlotSuccess OnSuccess, FErrorHandler OnError)
	{
		int32 Code;
		FString Message;
		FAccelByteModelsSlot Result;
		if (EHttpResponseCodes::IsOk(Response->GetResponseCode()))
		{
			if (FJsonObjectConverter::JsonObjectStringToUStruct(Response->GetContentAsString(), &Result, 0, 0))
			{
				OnSuccess.ExecuteIfBound(Result);
				return;
			}
			Code = static_cast<int32>(ErrorCodes::JsonDeserializationFailed);
		}
		HandleHttpError(Request, Response, Code, Message);
		OnError.ExecuteIfBound(Code, Message);
	}

	void CloudStorage::OnGetSlotResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, bool Successful, FGetSlotSuccess OnSuccess, FErrorHandler OnError)
	{
		if (Successful)
		{
			OnSuccess.ExecuteIfBound(Response->GetContent());
			return;
		}
		int32 Code;
		FString Message;
		HandleHttpError(Request, Response, Code, Message);
		OnError.ExecuteIfBound(Code, Message);
	}

	void CloudStorage::OnDeleteSlotResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, bool Successful, FDeleteSlotSuccess OnSuccess, FErrorHandler OnError)
	{
		if (Successful)
		{
			OnSuccess.ExecuteIfBound();
			return;
		}
		int32 Code;
		FString Message;
		HandleHttpError(Request, Response, Code, Message);
		OnError.ExecuteIfBound(Code, Message);
	}

}
}