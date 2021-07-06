param([string]$platform = "Win64")

& "$Env:ENGINE_PATH\Engine\Build\BatchFiles\RunUAT.bat" BuildCookRun `
-project="$Env:WORKSPACE\AccelByteUe4SdkDemo.uproject" `
-noP4 -nocompile -installed `
-ue4exe="$Env:ENGINE_PATH\Engine\Binaries\Win64\UE4Editor-Cmd.exe" -utf8output `
-config=Development -platform="$platform" -build -skipcook