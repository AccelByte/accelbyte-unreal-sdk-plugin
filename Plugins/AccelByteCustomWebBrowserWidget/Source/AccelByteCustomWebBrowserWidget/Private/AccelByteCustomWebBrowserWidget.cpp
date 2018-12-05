// Copyright (c) 2018 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "AccelByteCustomWebBrowserWidget.h"
#include "Engine.h"
#include "SWebBrowser.h"
#include "Serialization/JsonSerializerMacros.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/Text/STextBlock.h"
#include "Base64.h"

#define LOCTEXT_NAMESPACE "AccelByteCustomWebBrowserWidget"

/////////////////////////////////////////////////////
// UAccelByteCustomWebBrowserWidget

class CefData : public FJsonSerializable
{
public:
	FString UserRefreshToken;

	BEGIN_JSON_SERIALIZER
		JSON_SERIALIZE("refresh_token", UserRefreshToken);
	END_JSON_SERIALIZER
};

class CefRequest : public FJsonSerializable
{
public:
	FString Action;
	CefData Data;

	BEGIN_JSON_SERIALIZER
		JSON_SERIALIZE("action", Action);
		JSON_SERIALIZE_OBJECT_SERIALIZABLE("data", Data);
	END_JSON_SERIALIZER
};


class CefResponse : public FJsonSerializable
{
public:
	FString ClientId;
	FString ClientAuthorizationHeader;
	FString GlobalNamespace;
	FString ServerUrl;
	FString ReturnTo;

	BEGIN_JSON_SERIALIZER
		JSON_SERIALIZE("clientId", ClientId);
		JSON_SERIALIZE("clientAuthorizationHeader", ClientAuthorizationHeader);
		JSON_SERIALIZE("globalNamespace", GlobalNamespace);
		JSON_SERIALIZE("serverUrl", ServerUrl);
		JSON_SERIALIZE("returnTo", ReturnTo);
	END_JSON_SERIALIZER
};





UAccelByteCustomWebBrowserWidget::UAccelByteCustomWebBrowserWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	bIsVariable = true;
}

void UAccelByteCustomWebBrowserWidget::LoadUrl(FString NewUrl)
{
	if ( WebBrowserWidget.IsValid() )
	{
		return WebBrowserWidget->LoadURL(NewUrl);
	}
}


void UAccelByteCustomWebBrowserWidget::SetRedirectSuccessUrl(FString NewUrl)
{
	RedirectSuccessUrl = NewUrl;
}

void UAccelByteCustomWebBrowserWidget::LoadString(FString Contents, FString DummyUrl)
{
	if ( WebBrowserWidget.IsValid() )
	{
		return WebBrowserWidget->LoadString(Contents, DummyUrl);
	}
}

void UAccelByteCustomWebBrowserWidget::ExecuteJavascript(const FString& ScriptText)
{
	if (WebBrowserWidget.IsValid())
	{
		return WebBrowserWidget->ExecuteJavascript(ScriptText);
	}
}

FText UAccelByteCustomWebBrowserWidget::GetTitleText() const
{
	if ( WebBrowserWidget.IsValid() )
	{
		return WebBrowserWidget->GetTitleText();
	}

	return FText::GetEmpty();
}

FString UAccelByteCustomWebBrowserWidget::GetUrl() const
{
	if (WebBrowserWidget.IsValid())
	{
		return WebBrowserWidget->GetUrl();
	}

	return FString();
}

void UAccelByteCustomWebBrowserWidget::ReleaseSlateResources(bool bReleaseChildren)
{
	Super::ReleaseSlateResources(bReleaseChildren);

	WebBrowserWidget.Reset();
}

TSharedRef<SWidget> UAccelByteCustomWebBrowserWidget::RebuildWidget()
{
	if ( IsDesignTime() )
	{
		return SNew(SBox)
			.HAlign(HAlign_Center)
			.VAlign(VAlign_Center)
			[
				SNew(STextBlock)
				.Text(LOCTEXT("Web Browser", "Web Browser"))
			];
	}
	else
	{
		WebBrowserWidget = SNew(SWebBrowser)
			.InitialURL(InitialUrl)
			.ShowControls(false)
			.SupportsTransparency(bSupportsTransparency)
			.OnUrlChanged(BIND_UOBJECT_DELEGATE(FOnTextChanged, HandleOnUrlChanged));

		WebBrowserWidget->BindUObject("jsbridge", this);

		return WebBrowserWidget.ToSharedRef();
	}
}

void UAccelByteCustomWebBrowserWidget::SynchronizeProperties()
{
	Super::SynchronizeProperties();

	if ( WebBrowserWidget.IsValid() )
	{

	}
}

void UAccelByteCustomWebBrowserWidget::HandleOnUrlChanged(const FText& InText)
{
	OnUrlChanged.Broadcast(InText);
	if (InText.ToString().Contains(RedirectSuccessUrl))
	{
		FString Url = InText.ToString();
		FString Token = Url.Replace(*RedirectSuccessUrl, TEXT(""));
		OnRedirectSuccess.Broadcast(InText, FText::FromString(Token) );
	}
}

FString UAccelByteCustomWebBrowserWidget::CefQuery(FString value)
{
	UE_LOG(LogTemp, Log, TEXT("CefQuery Response: %s"), *value);
	TSharedPtr<FJsonObject> JsonObject;
	CefRequest request;
	TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(value);
	if (FJsonSerializer::Deserialize(Reader, JsonObject) && JsonObject.IsValid())
	{
		if (request.FromJson(JsonObject))
		{
			if (request.Action.Contains(TEXT("request-initial-data-injections")))
			{
				CefResponse response;
				FString Namespace;
				FString ClientId;
				FString ClientSecret;

				if (!GConfig->GetString(TEXT("AccelByteUe4Sdk"), TEXT("Namespace"), Namespace, GEngineIni))
				{
					UE_LOG(LogTemp, Error, TEXT("Missing Namespace= in [AccelByteUe4Sdk] of DefaultEngine.ini"));
				}
				if (!GConfig->GetString(TEXT("AccelByteUe4Sdk"), TEXT("ClientId"), ClientId, GEngineIni))
				{
					UE_LOG(LogTemp, Error, TEXT("Missing ClientId= in [AccelByteUe4Sdk] of DefaultEngine.ini"));
				}
				if (!GConfig->GetString(TEXT("AccelByteUe4Sdk"), TEXT("ClientSecret"), ClientSecret, GEngineIni))
				{
					UE_LOG(LogTemp, Error, TEXT("Missing ClientSecret= in [AccelByteUe4Sdk] of DefaultEngine.ini"));
				}

				response.ClientId = ClientId;
				response.GlobalNamespace = Namespace;
				response.ClientAuthorizationHeader = TEXT("Basic " + FBase64::Encode(ClientId + ":" + ClientSecret));


				return response.ToJson();
			}
			else if (request.Action.Contains(TEXT("success")))
			{
				UE_LOG(LogTemp, Log, TEXT("CefQuery WE GOT REFRESH TOKEN: %s"), *request.Data.UserRefreshToken);
				WebLoggedIn.Broadcast(FText::FromString(request.Data.UserRefreshToken));
				return TEXT("");
			}
		}
	}

	return TEXT("Empty");
}


#if WITH_EDITOR

const FText UAccelByteCustomWebBrowserWidget::GetPaletteCategory()
{
	return LOCTEXT("Experimental", "Experimental");
}

#endif

/////////////////////////////////////////////////////

#undef LOCTEXT_NAMESPACE
