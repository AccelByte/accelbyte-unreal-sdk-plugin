param([string]$target)

& "$Env:ENGINE_PATH\Engine\Build\BatchFiles\RunUAT.bat" BuildCookRun `
-project="C:\work\justice-ue4-sdk\AccelByteUe4SdkDemo.uproject" `
-noP4 -buildmachine -unattended -nullrhi `
-targetplatform="$target" -clientconfig=Development `
-clean -build