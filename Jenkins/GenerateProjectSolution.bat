::Re-generate project solution

"%ENGINE_PATH%\Engine\Binaries\DotNET\UnrealBuildTool.exe" AccelByteUe4SdkDemo Development Win64 -project="%WORKSPACE%\AccelByteUe4SdkDemo.uproject" -editorrecompile -progress -NoHotReloadFromIDE
"%ENGINE_PATH%\Engine\Binaries\DotNet\UnrealBuildTool.exe" -projectfiles  -project="%WORKSPACE%\AccelByteUe4SdkDemo.uproject" -game -rocket -progress