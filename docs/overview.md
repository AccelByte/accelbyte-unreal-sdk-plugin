The Justice SDK is a plugin for Unreal Engine 4 (4.18.).
This plugin use to interact with Justice Online Service.

The Justice SDK includes:

* Justice Plugin Source Code (located at `/Plugin/JusticeSDK` folder)
* Justice Custom Web Browser (located at `/Plugin/JusticeWebBrowserWidget` folder)
* Justice Sample Game (Generate VS .sln from  `/JusticeSDKDemo.uproject` file)

## Directory Structure

```
 .
 +-- Config                            // Configuration files goes here 
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

## High Level Features:
* C++ Interface to access SDK from C++ code
* Blueprint Interface to access SDK UMG or Blueprint Script

## Prerequisites:
* [Unreal Engine Prerequisites](https://docs.unrealengine.com/en-US/GettingStarted/RecommendedSpecifications)
* Unreal Engine 4.18 (you can download prebuilt binary from epic launcer, or integrate with UE4 from source code)
* Visual Studio 2015 or later (Community, Professional or any version)

### Stop Using OSS
Starting from March 13, 2018 at commit `48b28dd`, we stop using OnlineSubsystem (OSS). Now we use singleton class based on FModuleManager.
If you still have `OnlineSubsystemJustice` folder on Plugins folder, please remove it, since it will break your game.