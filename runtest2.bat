SET IAM_DATABASE_URL=
SET NAMESPACE=
SET TABLE_PREFIX=
SET AWS_ACCESS_KEY_ID=
SET AWS_SECRET_ACCESS_KEY=
SET AWS_REGION=
:start
pushd SteamHelper
SteamTicketHelper.exe login
popd
"C:\Program Files\Epic Games\UE_4.18\Engine\Binaries\Win64\UE4Editor-Cmd.exe" "C:\Users\User\source\repos\accelbyte\justice-ue4-sdk\AccelByteUe4SdkDemo.uproject" -Server -unattended -nopause -ExecCmds="Automation RunTests LogAccelByteEcommerceTest;Quit" -log
"C:\Program Files\Epic Games\UE_4.18\Engine\Binaries\Win64\UE4Editor-Cmd.exe" "C:\Users\User\source\repos\accelbyte\justice-ue4-sdk\AccelByteUe4SdkDemo.uproject" -Server -unattended -nopause -ExecCmds="Automation RunTests LogAccelByteTest;Quit" -log
pushd SteamHelper
SteamTicketHelper.exe logout
popd

pause
goto start
