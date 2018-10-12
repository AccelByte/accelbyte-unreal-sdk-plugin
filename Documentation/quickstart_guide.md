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
