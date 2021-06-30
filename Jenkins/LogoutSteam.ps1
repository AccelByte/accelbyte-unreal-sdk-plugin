Push-Location "$Env:WORKSPACE\SteamHelper"
& ".\SteamTicketHelper.exe" logout
Pop-Location

if (Test-Path -Path "$Env:WORKSPACE\SteamHelper\steamticket.txt" -PathType Leaf) {
    Remove-Item "$Env:WORKSPACE\SteamHelper\steamticket.txt"
}

Stop-Process -Name Steam
