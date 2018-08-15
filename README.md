# Justice SDK for Unreal Engine 4 (UE4 SDK)

- [Justice SDK for Unreal Engine 4 (UE4 SDK)](#justice-sdk-for-unreal-engine-4-ue4-sdk)
    - [1. Overview](#1-overview)
    - [2. High Level Features](#2-high-level-features)
    - [3. Prerequisites](#3-prerequisites)
    - [4. Integrate with Your Game](#4-integrate-with-your-game)
    - [5. Using The SDK](#5-using-the-sdk)
        - [C++](#c)
            - [a. OAuth2 Login](#a-oauth2-login)
            - [b. User Login](#b-user-login)
        - [Blueprint](#blueprint)
    - [6. Changes](#6-changes)
        - [Stop Using OSS](#stop-using-oss)
    - [7. Complete Documentation](#7-complete-documentation)


## 1. Overview

Justice SDK is a plugin for Unreal Engine 4 (4.18.).
This plugin is used to interact with Justice Online Service.

Justice SDK includes:

* Justice Plugin Source Code (located at `/Plugin/JusticeSDK` folder)
* Justice Custom Web Browser (located at `/Plugin/JusticeWebBrowserWidget` folder)
* Justice Sample Game (Generate VS .sln from  `/JusticeSDKDemo.uproject` file)

Folder Structure:

```text
 .
 +-- Config                            // Configuration files go here
 +-- Content
 +-- Plugins
 |   +-- JusticeSDK                    // Justice SDK Plugin
 |       +-- Source
 |            +-- JusticeSDK
 |                +-- Private
 |                    +-- Blueprints   // contain source(.cpp) of blueprint version of API
 |                    +-- Models       // contain source(.cpp) of Structure used in API
 |                    +-- Services     // contain service class (purchase, identity, etc..)
 |                    +-- Tests        // contain unity test file
 |                    +-- Utilities    // contain source(.cpp) helper function class  
 |                    +-- JusticeSDK.cpp
 |                +-- Public
 |                    +-- Blueprints   // contain header(.h) of blueprint version of API
 |                    +-- Models       // contain header(.h) of Structure used in API
 |                    +-- Services     // contain header(.h) service class
 |                    +-- Utilities    // contain header(.h) helper function class  
 |                    +-- JusticeSDK.h
 |   +-- JusticeWebBrowserWidget       // Custom In-Game-Browser Plugin
 +-- Source
 |   +-- JusticeSDKDemo                // Sample Demo Game
 +-- JusticeSDKDemo.uproject           // Unreal Engine Project
 +-- README.md                         // This file
```

## 2. High Level Features

* C++ Interface to access SDK from C++ code
* Blueprint Interface to access SDK UMG or Blueprint Script

## 3. Prerequisites

* [Unreal Engine Prerequisites](https://docs.unrealengine.com/en-US/GettingStarted/RecommendedSpecifications)
* Unreal Engine 4.18 (you can download prebuilt binary from epic launcer, or integrate with UE4 from source code)
* Visual Studio 2015 or later (Community, Professional or any version)

## 4. Integrate with Your Game

Copy `JusticeSDK` folder and `JusticeWebBrowserWidget` folder from Plugin folder to your Game Plugins folder.

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

## 5. Using The SDK

### C++

#### a. OAuth2 Login

Before you use other services, you need to call `ClientLogin()` function first.
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

#### b. User Login

Here is what you likely do to call login with email and password

```c++
void AJusticeSDKDemoGameModeBase::BeginPlay()
{
    ...
    JusticeIdentity::UserLogin(TEXT("user@example.com"), 
      TEXT("userpassword"),
      FUserLoginCompleteDelegate::CreateLambda([](bool IsSuccess, FString ErrorString, UOAuthTokenJustice* token) {
        // this will be called when login request receives a response
    }));
    ...
}
```

### Blueprint

We are able to call all Justice SDK function from the blueprint. We have 2 different kinds of blueprint function:

1. Blueprint function with delegates parameter 
2. Blueprint async function with multiple output pins

## 6. Changes

### Stop Using OSS

Starting from March 13, 2018 at commit `48b28dd`, we stop using OnlineSubsystem (OSS). Now we use singleton class based on FModuleManager.
If you still have `OnlineSubsystemJustice` folder in Plugins folder, please remove it, for it breaks your game.
 
## 7. Complete Documentation

For more detailed documentation, please take a look on docs folder. There is a more specific documentation on each file. You can also use mkdocs to view this docs in HTML format.

Go to root folder of this repository, and run this command

```bash
mkdocs serve
```

Open `http://127.0.0.1:8000` on your browser
