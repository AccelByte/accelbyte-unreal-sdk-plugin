// Copyright (c) 2018 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "Widget.h"
#include "AccelByteCustomWebBrowserWidget.generated.h"

/**
 * 
 */
UCLASS()
class ACCELBYTECUSTOMWEBBROWSERWIDGET_API UAccelByteCustomWebBrowserWidget : public UWidget
{
	GENERATED_UCLASS_BODY()

public:
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnUrlChanged, const FText&, Url);
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnRedirectSuccess, const FText&, NewUrl, const FText&, Token);

	/**
	 * Load the specified Url
	 *
	 * @param NewUrl New Url to load
	 */
	UFUNCTION(BlueprintCallable, Category="AccelByte Custom Web Browser")
	void LoadUrl(FString NewUrl);

	UFUNCTION(BlueprintCallable, Category = "AccelByte Custom Web Browser")
	void SetRedirectSuccessUrl(FString NewUrl);

	/**
	 * Load a string as data to create a web page
	 *
	 * @param Contents String to load
	 * @param DummyUrl Dummy Url for the page
	 */
	UFUNCTION(BlueprintCallable, Category="AccelByte Custom Web Browser")
	void LoadString(FString Contents, FString DummyUrl);

	/**
	* Executes a JavaScript string in the context of the web page
	*
	* @param ScriptText JavaScript string to execute
	*/
	UFUNCTION(BlueprintCallable, Category = "AccelByte Custom Web Browser")
	void ExecuteJavascript(const FString& ScriptText);

	/**
	 * Get the current title of the web page
	 */
	UFUNCTION(BlueprintCallable, Category="AccelByte Custom Web Browser")
	FText GetTitleText() const;

	UFUNCTION(BlueprintCallable, Category = "AccelByte Custom Web Browser")
	FString CefQuery(FString value);


	/**
	* Gets the currently loaded Url.
	*
	* @return The Url, or empty string if no document is loaded.
	*/
	UFUNCTION(BlueprintCallable, Category = "AccelByte Custom Web Browser")
	FString GetUrl() const;

	/** Called when the Url changes. */
	UPROPERTY(BlueprintAssignable, Category = "AccelByte Custom Web Browser|Event")
	FOnUrlChanged OnUrlChanged;

	UPROPERTY(BlueprintAssignable, Category = "AccelByte Custom Web Browser|Event")
	FOnRedirectSuccess OnRedirectSuccess;

	UPROPERTY(BlueprintAssignable, Category = "AccelByte Custom Web Browser|Event")
	FOnUrlChanged WebLoggedIn;




public:

	//~ Begin UWidget interface
	virtual void SynchronizeProperties() override;
	// End UWidget interface

	virtual void ReleaseSlateResources(bool bReleaseChildren) override;

#if WITH_EDITOR
	virtual const FText GetPaletteCategory() override;
#endif

protected:
	/** Url that the browser will initially navigate to. The Url should include the protocol, eg http:// */
	UPROPERTY(EditAnywhere, Category=Appearance)
	FString InitialUrl;


	UPROPERTY(EditAnywhere, Category = Appearance)
	FString RedirectSuccessUrl;


	/** Should the browser window support transparency. */
	UPROPERTY(EditAnywhere, Category=Appearance)
	bool bSupportsTransparency;

protected:
	TSharedPtr<class SWebBrowser> WebBrowserWidget;

protected:
	// UWidget interface
	virtual TSharedRef<SWidget> RebuildWidget() override;
	// End of UWidget interface

	void HandleOnUrlChanged(const FText& Text);

};
