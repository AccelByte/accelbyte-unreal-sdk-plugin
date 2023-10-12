// Copyright (c) 2023 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "CoreMinimal.h"
#pragma once

namespace AccelByte
{
	class ACCELBYTEUE4SDK_API IAccelBytePlatformWrapper
	{
	public:
		virtual void FetchPlatformToken(TDelegate<void(FString)> Delegate) = 0;
	};

	class ACCELBYTEUE4SDK_API FAccelBytePlatformHandler
	{
	public:
		TSharedPtr<IAccelBytePlatformWrapper> GetPlatformWrapper() { return PlatformWrapper; }
		void SetPlatformWrapper(TSharedPtr<IAccelBytePlatformWrapper> Wrapper) { PlatformWrapper = Wrapper; }
	private:
		TSharedPtr<IAccelBytePlatformWrapper> PlatformWrapper = nullptr;
	};
}