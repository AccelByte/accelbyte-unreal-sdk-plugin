echo "Logging in Steam"
& $Env:STEAM_PATH -login $Env:STEAM_ID $Env:STEAM_PASS
Start-Sleep -s 10
Write-Host "Logged in to Steam"

if (Test-Path -Path "$Env:WORKSPACE\SteamHelper\steamticket.txt" -PathType Leaf)
{
    Remove-Item "$Env:WORKSPACE\SteamHelper\steamticket.txt"
}

while (-not(Test-Path -Path "$Env:WORKSPACE\SteamHelper\steamticket.txt" -PathType Leaf))
{
    Push-Location "$Env:WORKSPACE\SteamHelper"
    & ".\SteamTicketHelper.exe" login
    Pop-Location
    Start-Sleep -s 10
}

Write-Host "steamticket found"
Get-Content "$Env:WORKSPACE\SteamHelper\steamticket.txt"