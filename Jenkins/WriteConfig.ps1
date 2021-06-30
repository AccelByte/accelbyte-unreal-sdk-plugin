#Automation test preparation

##Set the content of the configuration file
$CONFIG_FILE="$Env:WORKSPACE\Config\DefaultEngine.ini"
$CONFIG_FIELD =
"[/Script/AccelByteUe4Sdk.AccelByteSettings]`r`n" + 
"ClientId=$Env:CLIENT_ID`r`n" +
"ClientSecret=$Env:CLIENT_SECRET`r`n" +
"Namespace=$Env:CLIENT_NAMESPACE`r`n" +
"PublisherNamespace=$Env:PUBLISHER_NAMESPACE`r`n" +
"BaseUrl=`"$Env:BASE_URL`"`r`n"

$SERVER_CONFIG_FIELD =
"[/Script/AccelByteUe4Sdk.AccelByteServerSettings]`r`n" +
"ClientId=$Env:SERVER_CLIENT_ID`r`n" +
"ClientSecret=$Env:SERVER_CLIENT_SECRET`r`n" +
"Namespace=$Env:CLIENT_NAMESPACE`r`n" +
"PublisherNamespace=$Env:PUBLISHER_NAMESPACE`r`n" +
"RedirectURI=`"http://127.0.0.1`"`r`n" +
"BaseUrl=`"$Env:ADMIN_BASE_URL`"`r`n"

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
