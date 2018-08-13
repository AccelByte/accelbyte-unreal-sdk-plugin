## Steam Integration

To integrate your game with Steam, you need to add Steamworks SDK to your project or solution. 

Here's the [link](https://partner.steamgames.com/downloads/steamworks_sdk.zip) to download the SDK.

For a further usage in the project, please refer to this [documentation](https://partner.steamgames.com/doc/home).

Justice SDK need Steamworks SDK to login the user's if the launched from the Steam Launcher. The user will be logged in with their Steam account. **SteamManager** should be initialized to obtain the authentication ticket. The ticket is required as platform token. Here's the implementation of [login](../functions/#loginwithplatform) to game with Steam account.