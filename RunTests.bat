clear
SET IAM_DATABASE_URL=https://dynamodb.us-west-2.amazonaws.com
SET NAMESPACE=accelbyte
SET TABLE_PREFIX=alpha
SET AWS_ACCESS_KEY_ID=____LASTPASS____
SET AWS_SECRET_ACCESS_KEY=____LASTPASS____
SET AWS_REGION=us-west-2
"C:\Program Files\Epic Games\UE_4.18\Engine\Binaries\Win64\UE4Editor-Cmd.exe" D:\justice-ue4-sdk\AccelByteUe4SdkDemo.uproject -Server -unattended -nopause -ExecCmds="Automation RunTests AccelByte.Tests;Quit" -log -log=D:\justice-ue4-sdk\Log.txt 
echo %errorlevel%
pause
