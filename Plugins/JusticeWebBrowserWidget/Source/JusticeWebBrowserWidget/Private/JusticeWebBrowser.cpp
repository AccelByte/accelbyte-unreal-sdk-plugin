// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "JusticeWebBrowser.h"
#include "SWebBrowser.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/Text/STextBlock.h"

#define LOCTEXT_NAMESPACE "JusticeWebBrowser"

/////////////////////////////////////////////////////
// UJusticeWebBrowser

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
}

FString UJusticeWebBrowser::CefQuery(FString value)
{
	UE_LOG(LogTemp, Log, TEXT("JusticeWebBrowser::CefQuery Response: %s"), *value);
	if (value.Compare("{\"action\":\"request-initial-data-injections\"}") == 0)
	{
		return TEXT("{\"clientId\":\"string/optional\",\"clientAuthorizationHeader\" : \"string/optional\",\"globalNamespace\" : \"string/optional\",\"justiceApiUrl\" : \"string/optional\",\"returnTo\" : \"string/optional\"}");
	}
	else
	{
		return TEXT("Empty");
	}
}


#if WITH_EDITOR

const FText UJusticeWebBrowser::GetPaletteCategory()
{
	return LOCTEXT("Experimental", "Experimental");
}

#endif

/////////////////////////////////////////////////////

#undef LOCTEXT_NAMESPACE
