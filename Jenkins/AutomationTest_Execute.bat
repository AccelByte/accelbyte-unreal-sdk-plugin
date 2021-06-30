::Automation test execution

::::Set an environment variable that will be read by libcurl and libwebsocket 
if "%USE_PROXY%" == "true" (
    set http_proxy=%PROXY_SERVER%
    set https_proxy=http://%PROXY_SERVER%
)

set STEAM_PATH = %STEAM_PATH%
set STEAM_ID = %STEAM_ID%
set STEAM_PASS = %STEAM_PASS%

echo "Logging in Steam"
powershell -command "& $Env:STEAM_PATH -login $Env:STEAM_ID $Env:STEAM_PASS"
powershell -command "Start-Sleep -m 10000"
echo "Logged in to Steam"

rm %WORKSPACE%\\SteamHelper\\steamticket.txt

echo "Login with Steam Account"
:::::Log in Steam account
:here
If exist %WORKSPACE%\SteamHelper\steamticket.txt (
    echo "steamticket found"
    type %WORKSPACE%\SteamHelper\steamticket.txt
) else (
    pushd SteamHelper
    powershell -command "./SteamTicketHelper.exe login"
    popd
    powershell -command "Start-Sleep -s 20"
    goto :here
)

::::Run the automation test
"%ENGINE_PATH%\Engine\Binaries\Win64\UE4Editor-Cmd.exe" "%WORKSPACE%\AccelByteUe4SdkDemo.uproject" -Server -stdout -unattended -nopause -ExecCmds="Automation RunTests %UE4_TESTS%; Quit" -log -log=%TEST_LOG% -provider=baremetal

::::Obtain the result of the test a.k.a. last exit code
set EXIT_CODE=%errorlevel%

:::::Logout the logged in Steam account
pushd SteamHelper
SteamTicketHelper.exe logout
popd

:::::Deleting SteamTicket
rm %WORKSPACE%\\SteamHelper\\steamticket.txt

::::Cannot exit from test stage if "Steam" is active.
taskkill /F /IM Steam.exe

:::::If the result of the test is not 0, then something wrong happen with the test
if NOT %EXIT_CODE% == 0 (exit 1)
