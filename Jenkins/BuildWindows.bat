::Build Windows

"%ENGINE_PATH%\Engine\Build\BatchFiles\MSBuild.bat" "%WORKSPACE%\AccelByteUe4SdkDemo.sln" /t:build /p:Configuration="Development Editor";Platform=Win64;verbosity=diagnostic /m