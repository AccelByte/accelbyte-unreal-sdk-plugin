# Justice SDK for Unreal Engine 4 (UE4 SDK)

## 1. Overview:
The Justice SDK is a plugin for Unreal Engine 4 (4.18.).
This plugin use to interact with Justice Online Service

The Justice SDK includes:
* Justice Plugin Source Code (located at `/Plugin/JusticeSDK` folder)
* Justice Custom Web Browser (located at `/Plugin/JusticeWebBrowserWidget` folder)
* Justice Sample Game (Generate VS .sln from  `/JusticeSDKDemo.uproject` file)


## 2. High Level Features:
* C++ Interface to access SDK from C++ code
* Blueprint Interface to access SDK UMG or Blueprint Script



## 3. Prerequisites:
* [Unreal Engine Prerequisites](https://docs.unrealengine.com/en-US/GettingStarted/RecommendedSpecifications)
* Unreal Engine 4.18 (you can download prebuilt binary from epic launcer, or integrate with UE4 from source code)
* Visual Studio 2015 or later (Community, Professional or any version)


## 4. Integrate with Your Game:

Copy `JusticeSDK` and `JusticeWebBrowserWidget` folders from  Plugin folder to your Game Plugins' folder.

Make sure you add dependency to your .uproject file
```javascript
{
    "FileVersion": 3,
    "EngineAssociation": "4.18",
    "Category": "",
    "Description": "",
    "Modules": [{
        "Name": "YourAwesomeGame",
        "Type": "Runtime",
        "LoadingPhase": "Default"
    }],
    "Plugins": [
        .
        .
        .

        {
            "Name": "JusticeSDK",
            "Enabled": true
        }, 
        {
            "Name": "JusticeWebBrowserWidget",
            "Enabled": true
        }
    ]
}
```

Edit your `DefaultEngine.ini` file, make sure you add this configuration

```ini
[JusticeSDK]
bEnabled=true
AppId=myapp01
BaseURL="https://api.justice.accelbyte.net"
ClientID=<your client id>
ClientSecret=<your client secret>
Namespace=<your game namespace>

```

## 4. Using The SDK:

#### C++
On your start up game class, usually named `YourAwesomeGameModeBase.cpp` with class named `AYourAwesomeGameModeBase`. Add this code on `BeginPlay` Function or whatever function that will call on startup


```c++
void AYourAwesomeGameModeBase::BeginPlay()
{
	Super::BeginPlay();
	JusticeIdentity::ClientLogin();
}
void AYourAwesomeGameModeBase::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	JusticeIdentity::ClientLogout();
}

```

To call login with email and password you do like this

```c++
void AJusticeSDKDemoGameModeBase::BeginPlay()
{
	...
    JusticeIdentity::Login(TEXT("user@example.com"), 
      TEXT("password"), 
      FGrantTypeJustice::PasswordGrant, 
      FUserLoginCompleteDelegate::CreateLambda([](bool IsSuccess, FString ErrorString, UOAuthTokenJustice* token) {
  		// this will be called when login request receive a response 
    }));
    ...
}
```


#### Blueprint
We can call all of Justice SDK function from blueprint. We have 2 different kind of blueprint function:
1.  Blueprint function with delegates parameter 
2.  Blurprint async function with multiple output pins



## 5. Changes:

### Stop Suporting OSS
Starting from March 13, 2018 at commit `48b28dd`, we stop using OnlineSubsystem (OSS). Now we use singleton class based on FModuleManager.
If you still have `OnlineSubsystemJustice` folder on Plugins folder, please remove it, since it will break your game.
 




