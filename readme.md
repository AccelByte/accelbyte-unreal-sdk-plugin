# AccelByte UE4 SDK

AccelByte UE4 SDK is a plugin for Unreal Engine 4. 

## Assumptions

This documentation assumes that you are already familiar with modern C++, Unreal Engine 4 (including Blueprint), HTTP, REST API, microservice architecture, OAuth2, OpenID Connect, JWT.

## Features

AccelByte UE4 SDK features:

- Access the API from C++ and blueprint. C++ 
- Provides easy to use API for:
    - User authentication (access tokens, stored in memory)
    - User management (create user, reset password, etc.)
    - Identity provider (user profile)
    - Ecommerce (online store, view item catalog, create order, etc.)

## Architecture

The classes are categorized as follow:

- The `Model` classes are used for JSON deserialization and serialization using Unreal Engine 4 [JsonUtilities.h](https://api.unrealengine.com/INT/API/Runtime/JsonUtilities/).
- The `Api` classes provides interface to C++ functions.
- The `Blueprint` classes are simple _glue code_ to provide interface to Unreal Engine 4 Blueprint.
- The `Core` classes are for core functionalities (error handler, settings, credential store, and HTTP retry system). 

## Packaging

- `/Plugins` The actual plugin is located in `/Plugins/AccelByteUe4Sdk`. The custom web browser widget plugin `/Plugins/AccelByteCustomWebBrowserWidget` is experimental and untested. Please don't use.
- `/Content` This contains example widget blueprints and other asset.
- `/Source` This contains C++ source code for the examples.
- `/Documentation` This contains Doxyfile and this documentation.

# Quickstart Guide
Here's how to get it up and running quickly.

## Setup

1. The first step is to download and install [Visual Studio 2017](https://visualstudio.microsoft.com/downloads/).

2. Download and install [Unreal Engine 4.18](https://www.unrealengine.com).

3. If you want to run the examples, skip to step 7.

4. Create or open existing Unreal Engine 4 project.

5. Copy the plugin folder `Plugins/AccelByteUe4Sdk` to your game's plugins folder. 

6. Add the plugin to your `/MyAwesomeGame.uproject` file,
```java
"Plugins": [
...
{
    "Name": "AccelByteUe4Sdk",
    "Enabled": true
},
...
```
in `/Source/MyAwesomeGame.Target.cs`, and in `/Source/MyAwesomeGameEditor.Target.cs`.
```cs
ExtraModuleNames.AddRange(new string[]
{
    ...
    "AccelByteUe4Sdk",
    ...
}
```
7. And finally, fill out the settings in `/Plugins/AccelByteUe4Sdk/Source/AccelByteUe4Sdk/Private/Core/AccelByteSettings`.cpp if you want to use the _easy_ API. AccelByteSettings.h and AccelByteCredentials.h. The settings are `Namespace`, `ClientId`, `ClientSecret`, `Namespace`. There are 2 types of API, the _normal_ one and the _easy_ one. The _easy_ one fill out the parameters with the one in `AccelByteSettings.h` and `AccelByteCredentials.h`. The reason for this is because the backend services are not yet stable and that client secret shouldn't be in the game client ([RFC 6819 Section 5.2.3.1](https://tools.ietf.org/html/rfc6819#section-5.2.3.1)). In the future, this issue might be resolved with replacing OAuth2 with JWT or `authorization_code` `grant_type` with public client.

## How to run the examples

Open the `/AccelByteUe4SdkDemo.uproject` in Unreal Engine 4 Editor. 

![](./images/qsg_001.png)

Play in standalone game mode.

![](./images/qsg_002.png)

The examples include how to implement the following functionalities in blueprint:

**General**
- Error screen based on error code
- Loading screen

**User authentication**
- Client login
- Login with device ID
- Login with email and password
- Login with Steam session ticket

![](./images/qsg_003.png)

**User management**
- Create email account
- Verify email account
- Request password reset
- Verify password reset

![](./images/qsg_004.png)

**Identity provider**
- Get identity information (get profile)
- Update identity information (set profile)

**Ecommerce**
- List all categories in an online store
- List all items in a category
- View item information
- Create order

![](./images/qsg_005.png)

## Build Documentation

Install [Doxygen](http://www.doxygen.nl/), go to `/Documentation` folder, and run Doxygen.

```sh
cd Documentation
doxygen
```

The documentation will be in `/Doxygen` folder.
Open `/Doxygen/html/index.html` on your browser.
