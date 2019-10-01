:::::Logout the logged in Steam account
pushd SteamHelper
SteamTicketHelper.exe logout
popd

::::Cannot exit from test stage if "Steam" is active.
taskkill /F /IM Steam.exe