::Automation test execution

::::Set environment variable to obtain verification code with an executable application
SET IAM_DATABASE_URL = %IAM_DATABASE_URL%
SET NAMESPACE = %NAMESPACE%
SET TABLE_PREFIX = %TABLE_PREFIX%
SET AWS_ACCESS_KEY_ID = %AWS_ACCESS_KEY_ID%
SET AWS_SECRET_ACCESS_KEY = %AWS_SECRET_ACCESS_KEY%
SET AWS_REGION = %AWS_REGION%
set TEST_LOG="AutomationTestLog.log"

::::Login Steam account to an application and generate the Steam authorization ticket
pushd SteamHelper
SteamTicketHelper.exe login
popd

::::Set an environment variable that will be read by libcurl and libwebsocket 
set http_proxy=%PROXY_SERVER%
set https_proxy=http://%PROXY_SERVER%

::::Run the automation test
"%ENGINE_PATH%\Engine\Binaries\Win64\UE4Editor-Cmd.exe" "%WORKSPACE%\AccelByteUe4SdkDemo.uproject" -Server -unattended -nopause -ExecCmds="Automation RunTests AccelByte.Tests; Quit" -log -log=%TEST_LOG%

::::Obtain the result of the test a.k.a. last exit code
set EXIT_CODE=%errorlevel%

:::::Logout the logged in Steam account
pushd SteamHelper
SteamTicketHelper.exe logout
popd

::::Print the content of the automation test log file
type "%WORKSPACE%\Saved\Logs\%TEST_LOG%"

:::::If the result of the test is not 0, then something wrong happen with the test
if NOT %EXIT_CODE% == 0 (exit 1)
