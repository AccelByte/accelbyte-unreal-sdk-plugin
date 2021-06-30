param([string]$tests, [string]$log, [string]$report)

& "$Env:ENGINE_PATH\Engine\Binaries\Win64\UE4Editor-Cmd.exe" "$Env:WORKSPACE\AccelByteUe4SdkDemo.uproject" `
-Server -stdout -unattended -nopause `
-ExecCmds="Automation RunTests $tests; Quit" `
-log -log="$log" -provider=baremetal -LogCmds="log LogAccelByte verbose" `
-ReportOutputPath="$report"