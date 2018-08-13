// Copyright (c) 2017-2018 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "Blueprints/IdentityBlueprint/GetLinkedPlatform.h"
#include "Services/JusticeIdentity.h"

UAsyncGetLinkedPlatform * UAsyncGetLinkedPlatform::GetLinkedPlatform()
{
	UAsyncGetLinkedPlatform* Node = NewObject<UAsyncGetLinkedPlatform>();
	return Node;
}

void UAsyncGetLinkedPlatform::Activate()
{
	JusticeIdentity::GetLinkedPlatform(FGetLinkedPlatformCompleteDelegate::CreateLambda([&](bool bSuccessful, FString ErrorStr, TArray<FLinkedPlatform> result) {
		TArray<ULinkedPlatform*> Result;
		if (bSuccessful)
		{
			if (OnSuccess.IsBound())
			{
				for (int32 i = 0; i < result.Num(); i++)
				{
					ULinkedPlatform* platform = ULinkedPlatform::Deserialize(result[i]);
					check(platform);
					Result.Add(platform);
				}
				OnSuccess.Broadcast(Result);
			}
		}
		else
		{
			if (OnFailed.IsBound())
			{
				OnFailed.Broadcast(Result);
			}
		}
	}));
}