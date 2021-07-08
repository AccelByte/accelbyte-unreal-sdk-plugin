param([string]$platform = "Win64")

& "$Env:ENGINE_PATH\Engine\Build\BatchFiles\RunUAT.bat" BuildCookRun `
-project="$Env:WORKSPACE\AccelByteUe4SdkDemo.uproject" `
-noP4 -nocompile -installed `
-utf8output `
-config=Development -platform="$platform" -build -skipcook
