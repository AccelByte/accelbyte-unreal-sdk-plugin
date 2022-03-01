// Copyright (c) 2022 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

namespace AccelByte
{
class ACCELBYTEUE4SDK_API IAccelByteTokenGenerator : public TSharedFromThis<IAccelByteTokenGenerator>
{
	public:
	IAccelByteTokenGenerator() = default;
	virtual ~IAccelByteTokenGenerator() = default;
	DECLARE_EVENT_OneParam(IAccelByteTokenGenerator, FTokenReceivedEvent, const FString&);

	virtual void RequestToken() = 0;
	virtual bool IsTokenValid() const = 0;
	virtual  FTokenReceivedEvent& OnTokenReceived() = 0;

	protected:
	FString Token;
	FTokenReceivedEvent TokenReceivedEvent;
};
}
