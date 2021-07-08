param([string]$tests, [string]$log, [string]$report)

$UEC_EXE = "$env:ENGINE_PATH\Engine\Binaries\Win64\UnrealEditor-Cmd.exe" # Default for UE5+
		
if (!(Test-Path "$UEC_EXE"))
{
	$UEC_EXE = "$env:ENGINE_PATH\Engine\Binaries\Win64\UE4Editor-Cmd.exe" # Fallback for UE4
}

& "$UEC_EXE" "$Env:WORKSPACE\AccelByteUe4SdkDemo.uproject" `
-Server -stdout -unattended -nopause `
-ExecCmds="Automation RunTests $tests; Quit" `
-log -log="$log" -provider=baremetal -LogCmds="log LogAccelByte verbose" `
-ReportOutputPath="$report"