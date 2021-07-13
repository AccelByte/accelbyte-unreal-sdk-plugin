param([string]$platform = "Win64")

# Build editor for build host WITHOUT clean to reduce compile time (Win64 only)

& "$Env:ENGINE_PATH\Engine\Build\BatchFiles\RunUAT.bat" BuildCookRun `
-project="$Env:WORKSPACE\AccelByteUe4SdkDemo.uproject" `
-noP4 -nocompile -editortest -installed `
-utf8output `
-config=Development -platform="Win64" -build

# Build game WITH clean to improve consistency (Win64 or Linux)

& "$Env:ENGINE_PATH\Engine\Build\BatchFiles\RunUAT.bat" BuildCookRun `
-project="$Env:WORKSPACE\AccelByteUe4SdkDemo.uproject" `
-noP4 -nocompile -nocompileeditor -installed -clean `
-utf8output `
-config=Development -platform="$platform" -build -skipcook
