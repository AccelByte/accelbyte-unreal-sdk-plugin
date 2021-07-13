#Setup proxy to registy

$internetSettings = "HKCU:\Software\Microsoft\Windows\CurrentVersion\Internet Settings"
Set-ItemProperty -Path $internetSettings -Name ProxyServer -Value "$Env:PROXY_SERVER"
Set-ItemProperty -Path $internetSettings -Name ProxyEnable -Value 1
