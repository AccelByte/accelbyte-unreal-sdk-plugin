// Copyright (c) 2017-2018 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "JusticeSDKDemoGameModeBase.h"
#include "Misc/ConfigCacheIni.h"
THIRD_PARTY_INCLUDES_START
#include "steam/steam_api.h"
THIRD_PARTY_INCLUDES_END

#include "JusticeUser.h"


AJusticeSDKDemoGameModeBase::AJusticeSDKDemoGameModeBase()
{
}

void AJusticeSDKDemoGameModeBase::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);

	OnlineSub = IOnlineSubsystem::Get();
	check(OnlineSub);
	Identity = OnlineSub->GetIdentityInterface();

	// Setup Login delegates
	LoginCompleteHandle = Identity->AddOnLoginCompleteDelegate_Handle(
		0,
		FOnLoginCompleteDelegate::CreateUObject(this, &AJusticeSDKDemoGameModeBase::OnLoginCompleteDelegate));
}

void AJusticeSDKDemoGameModeBase::BeginPlay()
{
	Super::BeginPlay();
	JusticeIdentity::ClientLogin(); // this function is locking






	FOnlineAccountCredentials* NewAccount = new FOnlineAccountCredentials;
	NewAccount->Id = "480";
	Identity->Login(0, *NewAccount);

	FTelemetryEventDataExample TelemetryData;
	TelemetryData.ExampleField1 = 256;
	TelemetryData.ExampleField2 = "512";
	TelemetryData.ExampleField3 = 1024;
	FString Data = TelemetryData.ToJson();

	FGuid Guid = FGuid::NewGuid();
	FString UUID = FString::Printf(TEXT("%x%x%x%x"), Guid.A, Guid.B, Guid.C, Guid.D);

	FTelemetryEvent Telemetry;
	Telemetry.AppID = 1555;
	Telemetry.ClientID = FJusticeSDKModule::Get().ClientID;
	Telemetry.Data = Data;
	Telemetry.DeviceID = FPlatformMisc::GetDeviceId();
	Telemetry.EventID = 1555;
	Telemetry.EventLevel = 1555;
	Telemetry.EventTime = FDateTime::Now().ToIso8601();
	Telemetry.EventType = 1555;
	Telemetry.UserID = "exampleUserID";
	Telemetry.UUID = UUID;
	Telemetry.UX = 1555;
	
	JusticeTelemetry::SendTelemetryEvent(Telemetry);
}


void AJusticeSDKDemoGameModeBase::OnClientCredentialReady_Implementation()
{

}



void AJusticeSDKDemoGameModeBase::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	JusticeIdentity::ClientLogout();
}



void AJusticeSDKDemoGameModeBase::Tick(float DeltaSeconds)
{
}

void AJusticeSDKDemoGameModeBase::OnLoginCompleteDelegate(int32 LocalUserNum, bool bSuccessful, const FUniqueNetId& UserId, const FString& ErrorStr)
{
	FString SteamTicket = Identity->GetAuthToken(0);
	FString SteamDislayName = Identity->GetPlayerNickname(0);
	JusticeIdentity::SetSteamAuthTicket(SteamTicket);
	JusticeIdentity::SetSteamNickName(SteamDislayName);
	
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, SteamTicket);
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, UserId.ToString());
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, SteamDislayName);
	}

	uint32 Width;
	uint32 Height;

	if (SteamAPI_Init())
	{
		//Getting the PictureID from the SteamAPI and getting the Size with the ID
		int Picture = SteamFriends()->GetMediumFriendAvatar(SteamUser()->GetSteamID());
		SteamUtils()->GetImageSize(Picture, &Width, &Height);
		if (Width > 0 && Height > 0)
		{
			BYTE *oAvatarRGBA = new BYTE[Width * Height * 4];
			SteamUtils()->GetImageRGBA(Picture, (uint8*)oAvatarRGBA, 4 * Height * Width * sizeof(char));
			//Swap R and B channels 
			for (uint32 i = 0; i < (Width * Height * 4); i += 4)
			{
				uint8 Temp = oAvatarRGBA[i + 0];
				oAvatarRGBA[i + 0] = oAvatarRGBA[i + 2];
				oAvatarRGBA[i + 2] = Temp;
			}
			UTexture2D* Avatar = UTexture2D::CreateTransient(Width, Height, PF_B8G8R8A8);
			uint8* MipData = (uint8*)Avatar->PlatformData->Mips[0].BulkData.Lock(LOCK_READ_WRITE);
			FMemory::Memcpy(MipData, (void*)oAvatarRGBA, Height * Width * 4);
			Avatar->PlatformData->Mips[0].BulkData.Unlock();
			Avatar->PlatformData->NumSlices = 1;
			Avatar->NeverStream = true;
			Avatar->UpdateResource();
			JusticeIdentity::SetAvatar(Avatar);
		}

        //test
   //     JusticeUser::LoginByPublisher(Steam, SteamTicket, FUserLoginCompleteDelegate::CreateLambda([&](bool bSuccessful, FString ErrorStr, FOAuthTokenJustice* OAuthToken) {
			//if (bSuccessful)
			//{
			//	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("FINAL LoginByPublisher With Steam Ticket Success...."));
			//}
			//else
			//{
			//	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("FINAL LoginByPublisher With Steam Ticket FAILED...."));
			//}
			//
   //     }));



		

	}
}
