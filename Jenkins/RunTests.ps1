param([string]$tests, [string]$logfile)

& "$Env:ENGINE_PATH\Engine\Binaries\Win64\UE4Editor-Cmd.exe" "$Env:WORKSPACE\AccelByteUe4SdkDemo.uproject" `
-Server -stdout -unattended -nopause `
-ExecCmds="Automation RunTests $tests; Quit" `
-log -log="$logfile" -provider=baremetal -LogCmds="log LogAccelByte verbose"