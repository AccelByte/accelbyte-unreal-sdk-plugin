// Copyright (c) 2017-2018 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "JusticeSDK.h"
#include "Engine.h"
#include "JusticeLog.h"
#include "Private/AsyncTaskManagerJustice.h"



#define LOCTEXT_NAMESPACE "FJusticeSDKModule"

void FJusticeSDKModule::StartupModule()
{
	if (!GConfig->GetString(TEXT("OnlineSubsystemJustice"), TEXT("BaseURL"), BaseURL, GEngineIni))
	{
		UE_LOG(LogJustice, Error, TEXT("Missing BaseURL= in [OnlineSubsystemJustice] of DefaultEngine.ini"));
	}
	if (!GConfig->GetString(TEXT("OnlineSubsystemJustice"), TEXT("Namespace"), Namespace, GEngineIni))
	{
		UE_LOG(LogJustice, Error, TEXT("Missing Namespace= in [OnlineSubsystemJustice] of DefaultEngine.ini"));
	}
	if (!GConfig->GetString(TEXT("OnlineSubsystemJustice"), TEXT("ClientId"), ClientID, GEngineIni))
	{
		UE_LOG(LogJustice, Error, TEXT("Missing ClientId= in [OnlineSubsystemJustice] of DefaultEngine.ini"));
	}
	if (!GConfig->GetString(TEXT("OnlineSubsystemJustice"), TEXT("ClientSecret"), ClientSecret, GEngineIni))
	{
		UE_LOG(LogJustice, Error, TEXT("Missing ClientSecret= in [OnlineSubsystemJustice] of DefaultEngine.ini"));
	}
	
	GameClientToken = new OAuthTokenJustice;
	UserToken = new OAuthTokenJustice;
	UserProfile = new UserProfileInfo;

	AsyncTaskManager = new FAsyncTaskManagerJustice();
	OnlineAsyncTaskThread = FRunnableThread::Create(AsyncTaskManager, *FString::Printf(TEXT("AsyncTaskManagerJustice")), 128 * 1024, TPri_Normal);
	UE_LOG(LogJustice, Log, TEXT("Justice AsyncTaskManagerCreated thread (ID:%d)."), OnlineAsyncTaskThread->GetThreadID())
}

void FJusticeSDKModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
}
bool FJusticeSDKModule::GameClientParseJson(TSharedPtr<FJsonObject> jsonObject)
{
	FScopeLock lock(&GameClientCritical);
	if (GameClientToken->FromJson(jsonObject))
	{
		TArray<TSharedPtr<FJsonValue>> PermissionArray = jsonObject->GetArrayField(TEXT("permissions"));
		for (TSharedPtr<FJsonValue> Permission : PermissionArray)
		{
			TSharedPtr<FJsonObject> JsonPermissionObject = Permission->AsObject();
			FString Resource = JsonPermissionObject->GetStringField(TEXT("Resource"));
			int32 Action = JsonPermissionObject->GetIntegerField(TEXT("Action"));
			FPermissionJustice PermissionObject = FPermissionJustice(Resource, Action);
			GameClientToken->Permissions.Add(PermissionObject);


		}
		return true;
	}
	return false;
}

bool FJusticeSDKModule::UserParseJson(TSharedPtr<FJsonObject> jsonObject)
{
	if (FJusticeSDKModule::Get().UserToken->FromJson(jsonObject))
	{
		TArray<TSharedPtr<FJsonValue>> PermissionArray = jsonObject->GetArrayField(TEXT("permissions"));
		for (TSharedPtr<FJsonValue> Permission : PermissionArray)
		{
			TSharedPtr<FJsonObject> JsonPermissionObject = Permission->AsObject();
			FString Resource = JsonPermissionObject->GetStringField(TEXT("Resource"));
			int32 Action = JsonPermissionObject->GetIntegerField(TEXT("Action"));
			FPermissionJustice PermissionObject = FPermissionJustice(Resource, Action);
			FJusticeSDKModule::Get().UserToken->Permissions.Add(PermissionObject);
			//FJusticeSDKModule::Get().UserAccount->SetUserAttribute(Resource, FString::FromInt(Action));
		}
		FJusticeSDKModule::Get().UserToken->SetLastRefreshTimeToNow();
		FJusticeSDKModule::Get().UserToken->ScheduleNormalRefresh();
		return true;
	}
	return false;
}



#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FJusticeSDKModule, JusticeSDK)