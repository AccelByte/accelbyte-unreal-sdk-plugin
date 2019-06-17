# Quickstart Guide
Here's how to get it up and running quickly.

## Setup

`1.` The first step is to download and install [Visual Studio 2017](https://visualstudio.microsoft.com/downloads/).

`2.` Download and install [Unreal Engine 4.18](https://www.unrealengine.com).

`3.` If you want to run the examples, skip to step 7.

`4.` Create or open existing Unreal Engine 4 project.

`5.` Copy the plugin folder `Plugins/AccelByteUe4Sdk` to your game's plugins folder. 

`6.` Add the plugin to your `/MyAwesomeGame.uproject` file,
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
also in `/Source/MyAwesomeGame/MyAwesomeGame.Build.cs`.
```cs
 PublicDependencyModuleNames.AddRange(
			new string[] {
				...
                "AccelByteUe4Sdk",
                ...
            }
```
`7.` Open the editor, click File -> Project Settings.

![](images/setup_001.png)

`8.` Fill out the settings then click on set as default. The settings will be saved to `DefaultEngine.ini`.

![](images/setup_002.png)


## How to run the examples

Open the `/AccelByteUe4SdkDemo.uproject` in Unreal Engine 4 Editor. 

![](images/qsg_001.png)

Play in standalone game mode.

![](images/qsg_002.png)

The examples include how to implement the following functionalities in blueprint:

**General**
- Error screen based on numeric error code
- Loading screen

**User authentication**
- Login with client credentials **We are planning to remove this. This is for game server/confidential client only!**
- Login with device ID (anonymous login)
- Login with username and password
- Login with Steam session ticket
- Refresh token
- Logout

![](images/qsg_003.png)

**User management**
- Create user account
- Verify user account
- Request password reset
- Verify password reset

![](images/qsg_004.png)

**User profile**
- Get user profile
- Update user profile
- Create user profile

**Ecommerce**
- List all categories in an online store
- List all items in a category
- View item information
- Create order

![](images/qsg_005.png)

**Lobby**
- Connect to and disconnect from lobby server
- Send ping regularly
- Chat
    - Party chat
    - Private message (PM)
- Party
    - Get information about current party
    - Create a party
    - Leave current party
    - Invite someone to party
    - Accept a party invitation
    - Kick a party member
- Notification
    - Party invitation notice (Someone has invited you to party)
    - Party invitation acceptance notice (someone has accepted your invitation to the party)
Presence
    - Get all online users

**Coming soon(tm)™® by the Social server team(tm)™®. "Coming soon" is a registered trademark of AccelByte, Inc.**

![](images/qsg_006.png)
