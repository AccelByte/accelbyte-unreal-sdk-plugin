#Clean workspace directories those need to be cleaned

#This directory will hold a packaged SDK
if (Test-Path -Path "$Env:WORKSPACE\temp")
{
    Remove-Item -Recurse -Force -Path "$Env:WORKSPACE\temp"
}

#This directory will hold compressed SDK package & compressed SDK source code
if (Test-Path -Path "$Env:WORKSPACE\artifacts")
{
    Remove-Item -Recurse -Force -Path "$Env:WORKSPACE\artifacts"
}

#This directory will hold the documentation that generated automatically
if (Test-Path -Path "$Env:WORKSPACE\Doxygen")
{
    Remove-Item -Recurse -Force -Path "$Env:WORKSPACE\Doxygen"
}

#This directory linux binaries
if (Test-Path -Path "$Env:WORKSPACE\Plugins\AccelByteUe4Sdk\Binaries\Linux")
{
    Remove-Item -Recurse -Force -Path "$Env:WORKSPACE\Plugins\AccelByteUe4Sdk\Binaries\Linux"
}