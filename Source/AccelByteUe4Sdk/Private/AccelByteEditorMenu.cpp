// Copyright (c) 2026 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "AccelByteEditorMenu.h"

#if WITH_EDITOR

#include "ToolMenus.h"
#include "Interfaces/IPluginManager.h"
#include "HttpModule.h"
#include "Interfaces/IHttpRequest.h"
#include "Interfaces/IHttpResponse.h"
#include "Dom/JsonObject.h"
#include "Serialization/JsonReader.h"
#include "Serialization/JsonWriter.h"
#include "Serialization/JsonSerializer.h"
#include "Misc/FileHelper.h"
#include "Misc/Paths.h"
#include "Core/AccelByteUtilities.h"
#include "Framework/Notifications/NotificationManager.h"
#include "Widgets/Notifications/SNotificationList.h"

// STL
#include <atomic>

namespace AccelByte
{
namespace Editor
{

namespace
{
constexpr const TCHAR* GitHubReleaseBaseUrl = TEXT("https://github.com/AccelByte/accelbyte-unreal-sdk-plugin/releases/download");
constexpr const TCHAR* PreBuildProcessorFilename  = TEXT("PreBuildProcessor.bat");
constexpr const TCHAR* PostBuildProcessorFilename = TEXT("PostBuildProcessor.bat");

const FString PreBuildCommand  = TEXT("call \"$(PluginDir)\\PreBuildProcessor.bat\" \"$(EngineDir)\" \"$(ProjectDir)\" $(TargetConfiguration) $(TargetType) True");
const FString PostBuildCommand = TEXT("call \"$(PluginDir)\\PostBuildProcessor.bat\" \"$(ProjectDir)\" ");

bool ArrayContainsEntry(TSharedPtr<FJsonObject> const& StepsObject, FString const& PlatformKey, FString const& Command)
{
	if (!StepsObject.IsValid() || !StepsObject->HasTypedField<EJson::Array>(PlatformKey))
	{
		return false;
	}

	TArray<TSharedPtr<FJsonValue>> const& Arr = StepsObject->GetArrayField(PlatformKey);
	for (auto const& Entry : Arr)
	{
		if (Entry.IsValid() && Entry->AsString() == Command)
		{
			return true;
		}
	}
	return false;
}

FString GetDownloadUrl(const TCHAR* Filename)
{
	FString const Version = FAccelByteUtilities::GetPluginVersionAccelByteSDK();
	return FString::Printf(TEXT("%s/%s/%s"), GitHubReleaseBaseUrl, *Version, Filename);
}

FString GetPluginBaseDir()
{
	TSharedPtr<IPlugin> Plugin = IPluginManager::Get().FindPlugin(TEXT("AccelByteUe4Sdk"));
	if (Plugin.IsValid())
	{
		return Plugin->GetBaseDir();
	}
	return FString{};
}

void ShowNotification(FString const& Message, SNotificationItem::ECompletionState State)
{
	FNotificationInfo Info(FText::FromString(Message));
	Info.ExpireDuration = 5.0f;
	Info.bUseSuccessFailIcons = true;

	TSharedPtr<SNotificationItem> NotificationItem = FSlateNotificationManager::Get().AddNotification(Info);
	if (NotificationItem.IsValid())
	{
		NotificationItem->SetCompletionState(State);
	}
}

void DownloadFile(FString const& Url, FString const& DestPath, TFunction<void(bool bSuccess)> OnComplete)
{
	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = FHttpModule::Get().CreateRequest();
	Request->SetVerb(TEXT("GET"));
	Request->SetURL(Url);
	Request->OnProcessRequestComplete().BindLambda(
		[DestPath, OnComplete](FHttpRequestPtr const RequestPtr, FHttpResponsePtr const ResponsePtr, bool const bConnectedSuccessfully)
		{
			if (!bConnectedSuccessfully || !ResponsePtr.IsValid() || ResponsePtr->GetResponseCode() != 200)
			{
				UE_LOG(LogTemp, Error, TEXT("[AccelByte] Failed to download file from %s (HTTP %d)"),
					*RequestPtr->GetURL(),
					ResponsePtr.IsValid() ? ResponsePtr->GetResponseCode() : 0);

				if (OnComplete)
				{
					OnComplete(false);
				}
				return;
			}

			bool const bWritten = FFileHelper::SaveArrayToFile(ResponsePtr->GetContent(), *DestPath);
			if (!bWritten)
			{
				UE_LOG(LogTemp, Error, TEXT("[AccelByte] Failed to write downloaded file to %s"), *DestPath);
			}

			if (OnComplete)
			{
				OnComplete(bWritten);
			}
		});
	Request->ProcessRequest();
}

bool HasBuildStepsConfigured(FString const& PluginBaseDir)
{
	FString const UpluginPath = PluginBaseDir / TEXT("AccelByteUe4Sdk.uplugin");

	FString FileContents;
	if (!FFileHelper::LoadFileToString(FileContents, *UpluginPath))
	{
		return false;
	}

	TSharedPtr<FJsonObject> RootObject;
	TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(FileContents);
	if (!FJsonSerializer::Deserialize(Reader, RootObject) || !RootObject.IsValid())
	{
		return false;
	}

	TSharedPtr<FJsonObject> const PreBuildSteps = RootObject->GetObjectField(TEXT("PreBuildSteps"));
	TSharedPtr<FJsonObject> const PostBuildSteps = RootObject->GetObjectField(TEXT("PostBuildSteps"));

	return ArrayContainsEntry(PreBuildSteps, TEXT("Win64"), PreBuildCommand)
		&& ArrayContainsEntry(PostBuildSteps, TEXT("Win64"), PostBuildCommand);
}

bool PatchUpluginFile(FString const& PluginBaseDir)
{
	FString const UpluginPath = PluginBaseDir / TEXT("AccelByteUe4Sdk.uplugin");

	FString FileContents;
	if (!FFileHelper::LoadFileToString(FileContents, *UpluginPath))
	{
		UE_LOG(LogTemp, Error, TEXT("[AccelByte] Failed to read %s"), *UpluginPath);
		return false;
	}

	TSharedPtr<FJsonObject> RootObject;
	TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(FileContents);
	if (!FJsonSerializer::Deserialize(Reader, RootObject) || !RootObject.IsValid())
	{
		UE_LOG(LogTemp, Error, TEXT("[AccelByte] Failed to parse %s as JSON"), *UpluginPath);
		return false;
	}

	// Append our commands to existing arrays, preserving any user entries
	auto AppendBuildStep = [&RootObject](FString const& StepsFieldName, FString const& Command)
	{
		TSharedPtr<FJsonObject> StepsObject;
		if (RootObject->HasTypedField<EJson::Object>(StepsFieldName))
		{
			StepsObject = RootObject->GetObjectField(StepsFieldName);
		}
		else
		{
			StepsObject = MakeShareable(new FJsonObject());
			RootObject->SetObjectField(StepsFieldName, StepsObject);
		}

		// Skip if our command is already present
		if (ArrayContainsEntry(StepsObject, TEXT("Win64"), Command))
		{
			return;
		}

		TArray<TSharedPtr<FJsonValue>> Win64Array;
		if (StepsObject->HasTypedField<EJson::Array>(TEXT("Win64")))
		{
			Win64Array = StepsObject->GetArrayField(TEXT("Win64"));
		}
		Win64Array.Add(MakeShareable(new FJsonValueString(Command)));
		StepsObject->SetArrayField(TEXT("Win64"), Win64Array);
	};

	AppendBuildStep(TEXT("PreBuildSteps"), PreBuildCommand);
	AppendBuildStep(TEXT("PostBuildSteps"), PostBuildCommand);

	FString OutputString;
	TSharedRef<TJsonWriter<TCHAR, TPrettyJsonPrintPolicy<TCHAR>>> Writer =
		TJsonWriterFactory<TCHAR, TPrettyJsonPrintPolicy<TCHAR>>::Create(&OutputString);
	if (!FJsonSerializer::Serialize(RootObject.ToSharedRef(), Writer))
	{
		UE_LOG(LogTemp, Error, TEXT("[AccelByte] Failed to serialize modified .uplugin JSON"));
		return false;
	}

	if (!FFileHelper::SaveStringToFile(OutputString, *UpluginPath))
	{
		UE_LOG(LogTemp, Error, TEXT("[AccelByte] Failed to write modified .uplugin to %s"), *UpluginPath);
		return false;
	}

	UE_LOG(LogTemp, Log, TEXT("[AccelByte] Successfully patched %s with build step entries"), *UpluginPath);
	return true;
}

} // anonymous namespace

void RegisterMenu(void* Owner)
{
	UToolMenus::RegisterStartupCallback(
		FSimpleMulticastDelegate::FDelegate::CreateLambda([Owner]()
		{
			UToolMenu* MainMenu = UToolMenus::Get()->ExtendMenu(TEXT("LevelEditor.MainMenu"));
			if (MainMenu == nullptr)
			{
				return;
			}

			// Add "AccelByte" as a top-level menu entry
			UToolMenu* SubMenu = MainMenu->AddSubMenu(
				FToolMenuOwner{},
				NAME_None,
				TEXT("AccelByteMenu"),
				FText::FromString(TEXT("AccelByte")),
				FText::FromString(TEXT("AccelByte SDK tools and utilities"))
			);
			if (SubMenu == nullptr)
			{
				return;
			}

			FToolMenuSection& Section = SubMenu->AddSection(TEXT("BuildProcessors"), FText::FromString(TEXT("Build")));

			Section.AddMenuEntry(
				TEXT("SetupBuildProcessors"),
				FText::FromString(TEXT("Setup Build Processors")),
				FText::FromString(TEXT("Download build processor batch files and configure .uplugin build steps")),
				FSlateIcon(),
				FUIAction(FExecuteAction::CreateStatic(&ExecuteSetupBuildProcessors))
			);
		})
	);
}

void ExecuteSetupBuildProcessors()
{
	FString const PluginBaseDir = GetPluginBaseDir();
	if (PluginBaseDir.IsEmpty())
	{
		ShowNotification(TEXT("AccelByte: Could not locate AccelByteUe4Sdk plugin directory"), SNotificationItem::CS_Fail);
		return;
	}

	FString const PreBuildDest  = PluginBaseDir / PreBuildProcessorFilename;
	FString const PostBuildDest = PluginBaseDir / PostBuildProcessorFilename;

	bool const bPreBuildExists  = FPaths::FileExists(PreBuildDest);
	bool const bPostBuildExists = FPaths::FileExists(PostBuildDest);
	bool const bUpluginPatched  = HasBuildStepsConfigured(PluginBaseDir);

	// Everything already in place — nothing to do
	if (bPreBuildExists && bPostBuildExists && bUpluginPatched)
	{
		ShowNotification(TEXT("AccelByte: Build processors are already set up"), SNotificationItem::CS_Success);
		return;
	}

	// Determine how many files need downloading
	int32 const DownloadsNeeded = (bPreBuildExists ? 0 : 1) + (bPostBuildExists ? 0 : 1);

	// If both batch files exist but .uplugin needs patching, patch directly
	if (DownloadsNeeded == 0)
	{
		bool const bPatched = PatchUpluginFile(PluginBaseDir);
		if (bPatched)
		{
			ShowNotification(TEXT("AccelByte: Build processors set up successfully (.uplugin patched)"), SNotificationItem::CS_Success);
		}
		else
		{
			ShowNotification(TEXT("AccelByte: Failed to update .uplugin"), SNotificationItem::CS_Fail);
		}
		return;
	}

	// Download missing batch files, then patch .uplugin when all succeed.
	// Only SuccessCount is needed: increment on success, proceed when it reaches ExpectedCount.
	// On failure, notify immediately.
	TSharedRef<std::atomic<int32>> SuccessCount = MakeShared<std::atomic<int32>>(0);
	TSharedRef<FString> PluginDirCopy = MakeShared<FString>(PluginBaseDir);
	int32 const ExpectedCount = DownloadsNeeded;

	auto OnDownloadComplete = [SuccessCount, PluginDirCopy, ExpectedCount, bUpluginPatched](bool bSuccess)
	{
		if (!bSuccess)
		{
			ShowNotification(TEXT("AccelByte: Failed to download a build processor file"), SNotificationItem::CS_Fail);
			return;
		}

		int32 const Count = SuccessCount->fetch_add(1, std::memory_order_acq_rel) + 1;
		if (Count < ExpectedCount)
		{
			return;
		}

		if (bUpluginPatched)
		{
			ShowNotification(TEXT("AccelByte: Build processors set up successfully (files downloaded)"), SNotificationItem::CS_Success);
			return;
		}

		bool const bPatched = PatchUpluginFile(*PluginDirCopy);
		if (bPatched)
		{
			ShowNotification(TEXT("AccelByte: Build processors set up successfully"), SNotificationItem::CS_Success);
		}
		else
		{
			ShowNotification(TEXT("AccelByte: Downloaded files but failed to update .uplugin"), SNotificationItem::CS_Fail);
		}
	};

	if (!bPreBuildExists)
	{
		DownloadFile(GetDownloadUrl(PreBuildProcessorFilename), PreBuildDest, OnDownloadComplete);
	}
	if (!bPostBuildExists)
	{
		DownloadFile(GetDownloadUrl(PostBuildProcessorFilename), PostBuildDest, OnDownloadComplete);
	}
}

} // namespace Editor
} // namespace AccelByte

#endif // WITH_EDITOR
