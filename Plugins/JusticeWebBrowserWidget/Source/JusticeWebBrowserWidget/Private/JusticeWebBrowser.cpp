// Copyright (c) 2017-2018 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "JusticeWebBrowser.h"
#include "SWebBrowser.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/Text/STextBlock.h"
#include "Serialization/JsonSerializerMacros.h"
#include "JusticeSDK/Private/HTTPJustice.h"
#include "Engine.h"

#define LOCTEXT_NAMESPACE "JusticeWebBrowser"

/////////////////////////////////////////////////////
// UJusticeWebBrowser

class CefData : public FJsonSerializable
{
public:
	FString RefreshToken;

	BEGIN_JSON_SERIALIZER
		JSON_SERIALIZE("refresh_token", RefreshToken);
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
	FString JusticeApiUrl;
	FString ReturnTo;

	BEGIN_JSON_SERIALIZER
		JSON_SERIALIZE("clientId", ClientId);
		JSON_SERIALIZE("clientAuthorizationHeader", ClientAuthorizationHeader);
		JSON_SERIALIZE("globalNamespace", GlobalNamespace);
		JSON_SERIALIZE("justiceApiUrl", JusticeApiUrl);
		JSON_SERIALIZE("returnTo", ReturnTo);
	END_JSON_SERIALIZER
};





UJusticeWebBrowser::UJusticeWebBrowser(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	bIsVariable = true;
}

void UJusticeWebBrowser::LoadURL(FString NewURL)
{
	if ( WebBrowserWidget.IsValid() )
	{
		return WebBrowserWidget->LoadURL(NewURL);
	}
}

void UJusticeWebBrowser::LoadString(FString Contents, FString DummyURL)
{
	if ( WebBrowserWidget.IsValid() )
	{
		return WebBrowserWidget->LoadString(Contents, DummyURL);
	}
}

void UJusticeWebBrowser::ExecuteJavascript(const FString& ScriptText)
{
	if (WebBrowserWidget.IsValid())
	{
		return WebBrowserWidget->ExecuteJavascript(ScriptText);
	}
}

FText UJusticeWebBrowser::GetTitleText() const
{
	if ( WebBrowserWidget.IsValid() )
	{
		return WebBrowserWidget->GetTitleText();
	}

	return FText::GetEmpty();
}

FString UJusticeWebBrowser::GetUrl() const
{
	if (WebBrowserWidget.IsValid())
	{
		return WebBrowserWidget->GetUrl();
	}

	return FString();
}

void UJusticeWebBrowser::ReleaseSlateResources(bool bReleaseChildren)
{
	Super::ReleaseSlateResources(bReleaseChildren);

	WebBrowserWidget.Reset();
}

TSharedRef<SWidget> UJusticeWebBrowser::RebuildWidget()
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
			.InitialURL(InitialURL)
			.ShowControls(false)
			.SupportsTransparency(bSupportsTransparency)
			.OnUrlChanged(BIND_UOBJECT_DELEGATE(FOnTextChanged, HandleOnUrlChanged));

		WebBrowserWidget->BindUObject("jsbridge", this);

		return WebBrowserWidget.ToSharedRef();
	}
}

void UJusticeWebBrowser::SynchronizeProperties()
{
	Super::SynchronizeProperties();

	if ( WebBrowserWidget.IsValid() )
	{

	}
}

void UJusticeWebBrowser::HandleOnUrlChanged(const FText& InText)
{
	OnUrlChanged.Broadcast(InText);
	if (InText.ToString().Contains(RedirectSuccessUrl))
	{
		OnRedirectSuccess.Broadcast(InText);
	}
}

FString UJusticeWebBrowser::CefQuery(FString value)
{
	UE_LOG(LogTemp, Log, TEXT("JusticeWebBrowser::CefQuery Response: %s"), *value);
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
				FString ClientID;
				FString ClientSecret;

				if (!GConfig->GetString(TEXT("JusticeSDK"), TEXT("Namespace"), Namespace, GEngineIni))
				{
					UE_LOG(LogTemp, Error, TEXT("Missing Namespace= in [JusticeSDK] of DefaultEngine.ini"));
				}
				if (!GConfig->GetString(TEXT("JusticeSDK"), TEXT("ClientId"), ClientID, GEngineIni))
				{
					UE_LOG(LogTemp, Error, TEXT("Missing ClientId= in [JusticeSDK] of DefaultEngine.ini"));
				}
				if (!GConfig->GetString(TEXT("JusticeSDK"), TEXT("ClientSecret"), ClientSecret, GEngineIni))
				{
					UE_LOG(LogTemp, Error, TEXT("Missing ClientSecret= in [JusticeSDK] of DefaultEngine.ini"));
				}

				response.ClientId = ClientID;
				response.GlobalNamespace = Namespace;
				response.ClientAuthorizationHeader = FHTTPJustice::BasicAuth(ClientID, ClientSecret);


				return response.ToJson();
			}
			else if (request.Action.Contains(TEXT("success")))
			{
				UE_LOG(LogTemp, Log, TEXT("JusticeWebBrowser::CefQuery WE GOT REFRESH TOKEN: %s"), *request.Data.RefreshToken);
				OnJusticeWebLoggedIn.Broadcast(FText::FromString(request.Data.RefreshToken));
				return TEXT("");
			}
		}
	}

	return TEXT("Empty");
}


#if WITH_EDITOR

const FText UJusticeWebBrowser::GetPaletteCategory()
{
	return LOCTEXT("Experimental", "Experimental");
}

#endif

/////////////////////////////////////////////////////

#undef LOCTEXT_NAMESPACE
