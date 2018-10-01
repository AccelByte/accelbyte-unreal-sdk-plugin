\page getting_started Getting Started

\section getting_started_overview Overview

Justice SDK is a plugin for Unreal Engine 4 (4.18).
This plugin is used to interact with Justice Online Service.

Justice SDK includes:

* Justice Plugin Source Code (located at `/Plugin/JusticeSDK` folder)
* Justice Custom Web Browser (located at `/Plugin/JusticeWebBrowserWidget` folder)
* Justice Sample Game (Generate VS .sln from  `/JusticeSDKDemo.uproject` file)

\subsection getting_started_directory_structure Directory structure

```
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
\subsection getting_started_high_level_features High level features
* C++ Interface to access SDK from C++ code
* Blueprint Interface to access SDK UMG or Blueprint Script

\subsection getting_started_prerequisites Prerequisites
* [Unreal Engine Prerequisites](https://docs.unrealengine.com/en-US/GettingStarted/RecommendedSpecifications)
* Unreal Engine 4.18 (you can download prebuilt binary from epic launcer, or integrate with UE4 from source code)
* Visual Studio 2015 or later (Community, Professional or any version)

\subsection onlinesubsystem_notice OnlineSubsystem Notice
Starting from March 13, 2018 at commit `48b28dd`, we stop using OnlineSubsystem (OSS). Now we use singleton class based on FModuleManager.
If you still have `OnlineSubsystemJustice` folder on Plugins folder, please remove it, since it breaks your game.

\section getting_started_configuration Configuration

Copy `JusticeSDK` folder and `JusticeWebBrowserWidget` folder from Plugin folder to your Game Plugins folder.

Make sure you add dependency to your .uproject file
```java
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
```
[JusticeSDK]
bEnabled=true
AppId=myapp01
BaseURL="https://api.justice.accelbyte.net"
ClientId=<your client id>
ClientSecret=<your client secret>
Namespace=<your game namespace>
```

Open your game build.cs file `YourAwesomeGame.Build.cs` add `JusticeSDK` to `PublicDependencyModuleNames`

```cs
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
