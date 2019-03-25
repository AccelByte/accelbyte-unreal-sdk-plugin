#Automation test preparation

##Steam account login
$STEAM_PATH = $Env:STEAM_PATH
$STEAM_ID = $Env:STEAM_ID
$STEAM_PASS = $Env:STEAM_PASS

& $STEAM_PATH -login $STEAM_ID $STEAM_PASS

##Set the content of the configuration file
$CONFIG_FILE="$Env:WORKSPACE\Config\DefaultEngine.ini"
$CONFIG_FIELD =
"[/Script/AccelByteUe4Sdk.AccelByteSettings]`r`n" + 
"ClientId=$Env:UE4_CLIENT_ID`r`n" +
"ClientSecret=$Env:UE4_CLIENT_SECRET`r`n" +
"Namespace=$Env:ADMIN_NAMESPACE`r`n" +
"PublisherNamespace=$Env:UE4_PublisherNamespace`r`n" +
"IamServerUrl=`"$Env:UE4_BASE_URL`/iam`"`r`n" +
"PlatformServerUrl=`"$Env:UE4_BASE_URL`/platform`"`r`n" +
"LobbyServerUrl=`"$Env:LOBBY_SERVER_URL`/lobby`/`"`r`n" +
"BasicServerUrl=`"$Env:UE4_BASE_URL`/basic`"`r`n" +
"CloudStorageServerUrl=`"$Env:UE4_BASE_URL`/binary-store`"`r`n" +
"GameProfileServerUrl=`"$Env:UE4_BASE_URL`/soc-profile`"`r`n"

##Write the configuration to the targeted config file
(get-content -raw $CONFIG_FILE) -replace '(?ms)^(\[\/Script\/AccelByteUe4Sdk\.AccelByteSettings\]).*' , $CONFIG_FIELD | out-file $CONFIG_FILE -Encoding UTF8

##Print the content of the configuration file, except the client ID & secret
foreach ($LINE in get-content $CONFIG_FILE)
{
    if (($LINE -match "ClientId" -or $LINE -match "ClientSecret" ) -eq $false)
    {
        echo $LINE
    }
}