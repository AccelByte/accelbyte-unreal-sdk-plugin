#Automation test preparation

##Set the content of the configuration file
$CONFIG_FILE="$Env:WORKSPACE\Config\DefaultEngine.ini"
$CONFIG_FIELD =
"[/Script/AccelByteUe4Sdk.AccelByteSettings]`r`n" + 
"ClientId=$Env:CLIENT_ID`r`n" +
"ClientSecret=$Env:CLIENT_SECRET`r`n" +
"Namespace=$Env:CLIENT_NAMESPACE`r`n" +
"PublisherNamespace=$Env:PUBLISHER_NAMESPACE`r`n" +
"BaseUrl=`"$Env:BASE_URL`"`r`n" +
"NonApiBaseUrl=`"$Env:ADMIN_BASE_URL`"`r`n" +
"IamServerUrl=`"$Env:BASE_URL$Env:IAM_PREFIX`"`r`n" +
"PlatformServerUrl=`"$Env:BASE_URL$Env:PLATFORM_PREFIX`"`r`n" +
"LobbyServerUrl=`"$Env:LOBBY_SERVER_URL`"`r`n" +
"BasicServerUrl=`"$Env:BASE_URL$Env:BASIC_PREFIX`"`r`n" +
"CloudStorageServerUrl=`"$Env:BASE_URL$Env:CLOUD_STORAGE_PREFIX`"`r`n" +
"GameProfileServerUrl=`"$Env:BASE_URL$Env:GAME_PROFILE_PREFIX`"`r`n" +
"StatisticServerUrl=`"$Env:BASE_URL$Env:STATISTIC_PREFIX`"`r`n" + 
"QosManagerServerUrl=`"$Env:BASE_URL$Env:QOS_PREFIX`"`r`n" + 
"LeaderboardServerUrl=`"$Env:BASE_URL$Env:LEADERBOARD_PREFIX`"`r`n" +
"GameTelemetryServerUrl=`"$Env:BASE_URL$Env:GAMETELEMETRY_PREFIX`"`r`n" +
"CloudSaveServerUrl=`"$Env:BASE_URL$Env:CLOUD_SAVE_PREFIX`"`r`n"

$SERVER_CONFIG_FIELD =
"[/Script/AccelByteUe4Sdk.AccelByteServerSettings]`r`n" +
"ClientId=$Env:SERVER_CLIENT_ID`r`n" +
"ClientSecret=$Env:SERVER_CLIENT_SECRET`r`n" +
"Namespace=$Env:CLIENT_NAMESPACE`r`n" +
"PublisherNamespace=$Env:PUBLISHER_NAMESPACE`r`n" +
"RedirectURI=`"http://127.0.0.1`"`r`n" +
"IamServerUrl=`"$Env:ADMIN_BASE_URL$Env:IAM_PREFIX`"`r`n" +
"DSMControllerServerUrl=`"$Env:ADMIN_BASE_URL$Env:DSM_PREFIX`"`r`n" +
"StatisticServerUrl=`"$Env:ADMIN_BASE_URL$Env:STATISTIC_PREFIX`"`r`n" +
"PlatformServerUrl=`"$Env:ADMIN_BASE_URL$Env:PLATFORM_PREFIX`"`r`n" +
"QosManagerServerUrl=`"$Env:ADMIN_BASE_URL$Env:QOS_PREFIX`"`r`n" +
"GameTelemetryServerUrl=`"$Env:ADMIN_BASE_URL$Env:GAMETELEMETRY_PREFIX`"`r`n" 

##Write the configuration to the targeted config file
(get-content -raw $CONFIG_FILE) -replace '(?ms)^(\[\/Script\/AccelByteUe4Sdk\.AccelByteSettings\])\r*\n(?:(?!^\[).)*(?=\r*\n)' , $CONFIG_FIELD | out-file $CONFIG_FILE -Encoding UTF8
(get-content -raw $CONFIG_FILE) -replace '(?ms)^(\[\/Script\/AccelByteUe4Sdk\.AccelByteServerSettings\])\r*\n(?:(?!^\[).)*(?=\r*\n)' , $SERVER_CONFIG_FIELD | out-file $CONFIG_FILE -Encoding UTF8

##Print the content of the configuration file, except the client ID & secret
foreach ($LINE in get-content $CONFIG_FILE)
{
    if (($LINE -match "ClientId" -or $LINE -match "ClientSecret" ) -eq $false)
    {
        echo $LINE
    }
}