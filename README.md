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

Edit your DefaultEngine.inifile, make sure you add this configuration

```ini
[JusticeSDK]
bEnabled=true
AppId=myapp01
BaseURL="https://api.justice.accelbyte.net"
ClientID=<your client id>
ClientSecret=<your client secret>
Namespace=<your game namespace>

```

Open your game build.cs file `YourAwesomeGame.Build.cs` add `JusticeSDK` to PublicDependencyModuleNames

```csharp
public class JusticeSDKDemo : ModuleRules
{
    public JusticeSDKDemo(ReadOnlyTargetRules Target) : base(Target)
    {
    	PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
    	PublicDependencyModuleNames.AddRange(new string[] 
        { 
          ...
          "JusticeSDK",          
        });
      PrivateDependencyModuleNames.AddRange(new string[] {...});
      DynamicallyLoadedModuleNames.AddRange(new string[] {...});

    }
}
```

## 4. Using The SDK:

#### C++

##### a. OAuth2 Login
Before you want to use other service, you must call `ClientLogin()` function first.
`ClientLogin()` function is actually OAuth2 Login to our service. It will login against **ClientID** and **ClientSecret** from `DefaultEngine.ini` file under `JusticeSDK` section.
If you build Game Client and Dedicated Server, you should call this function. There is **no** `ServerLogin` Function.

For more information about OAuth2 you can follow [this link](https://oauth.net/2/) and [this link](https://docs.microsoft.com/en-us/azure/active-directory/develop/active-directory-protocols-oauth-code).

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


##### b. User Login
To call login with email and password you do like this

```c++
void AJusticeSDKDemoGameModeBase::BeginPlay()
{
	...
    JusticeIdentity::UserLogin(TEXT("user@example.com"), 
      TEXT("userpassword"),       
      FUserLoginCompleteDelegate::CreateLambda([](bool IsSuccess, FString ErrorString, UOAuthTokenJustice* token) {
  		// this will be called when login request receive a response 
    }));
    ...
}
```

##### c. Anonymous Login
If you want to login without using user and password, you can use Anonymous Login. The example case would be player want to try our game without doing any registration. 

```c++
void AJusticeSDKDemoGameModeBase::BeginPlay()
{
	...
    JusticeIdentity::AnonymousLogin(
      FUserLoginCompleteDelegate::CreateLambda([](bool IsSuccess, FString ErrorString, UOAuthTokenJustice* token) {
  		// this will be called when login request receive a response 
    }));
    ...
}
```

#### Blueprint
We can call all of The Justice SDK function from blueprint. We have 2 different kind of blueprint function:

1.  Blueprint function with delegates parameter 
1.  Blueprint async function with multiple output pins


## 5. Changes:

### Stop Using OSS
Starting from March 13, 2018 at commit `48b28dd`, we stop using OnlineSubsystem (OSS). Now we use singleton class based on FModuleManager.
If you still have `OnlineSubsystemJustice` folder on Plugins folder, please remove it, since it will break your game.
 
