// Copyright (c) 2024 AccelByte, Inc. All rights reserved.
// This is licensed software from Accelbyte Inc, for limitations
// and restrictions contact your company contract manager

#include "Blueprints/AccelByteInstanceBlueprints.h"

#include "AccelByteUe4SdkModule.h"
#include "Blueprints/AccelByteBPServerApiClient.h"

UAccelByteInstance::UAccelByteInstance()
{}

UAccelByteInstance::UAccelByteInstance(FAccelByteInstancePtr InAccelByteInstance)
	: AccelByteInstance(InAccelByteInstance)
{
}

void UAccelByteInstance::SetAccelByteInstance(FAccelByteInstancePtr InAccelByteInstance)
{
	AccelByteInstance = InAccelByteInstance;
}

UAccelByteInstance* UAccelByteInstance::CreateAccelByteInstance()
{
	UAccelByteInstance* WrapperPtr = NewObject<UAccelByteInstance>();
	WrapperPtr->SetAccelByteInstance(IAccelByteUe4SdkModuleInterface::Get().CreateAccelByteInstance());

	return WrapperPtr;
}

UABApiClient* UAccelByteInstance::GetApiClient(FString const Key)
{
	if(!AccelByteInstance.IsValid())
	{
		return nullptr;
	}
	
	UABApiClient* WrapperPtr = NewObject<UABApiClient>();
	WrapperPtr->SetApiClient(AccelByteInstance->GetApiClient(Key));

	return WrapperPtr;
}

UServerApiClient* UAccelByteInstance::GetServerApiClient(FString const Key)
{
	if(!AccelByteInstance.IsValid())
	{
		return nullptr;
	}
	
	UServerApiClient* WrapperPtr = NewObject<UServerApiClient>();
	WrapperPtr->SetServerApiClient(AccelByteInstance->GetServerApiClient(Key));

	return WrapperPtr;
}

FAccelByteInstancePtr UAccelByteInstance::GetAccelByteInstance()
{
	return AccelByteInstance;
}
