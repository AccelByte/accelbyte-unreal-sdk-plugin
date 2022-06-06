// Copyright (c) 2021 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "CoreMinimal.h"
#include "Interfaces/IHttpResponse.h"

/*
* Class for make mock response to handled by HandleHttpError
*/
class FMockResponse : public IHttpResponse
{

private:

	int32 ResponseCode{};

	FString Content{};

	TArray<uint8> rawContent{};

public:

	void SetMockResponseCode(int32 InResponseCode)
	{
		ResponseCode = InResponseCode;
	}

	void SetMockContent(FString InContent)
	{
		Content = InContent;
	}

	virtual int32 GetResponseCode() const override
	{
		return ResponseCode;
	}

	virtual FString GetContentAsString() const override
	{
		return Content;
	}

	virtual FString GetURL() const override
	{
		return "";
	}

	virtual FString GetURLParameter(const FString& ParameterName) const override
	{
		return "";
	}

	virtual FString GetHeader(const FString& HeaderName) const override
	{
		return "";
	}

	virtual TArray<FString> GetAllHeaders() const override
	{
		return { "" };
	}

	virtual FString GetContentType() const override
	{
		return "";
	}

	virtual int32 GetContentLength() const override
	{
		return 0;
	}

	virtual const TArray<uint8>& GetContent() const override
	{
		return rawContent;
	}
};