::Build Linux

"%ENGINE_PATH%\Engine\Build\BatchFiles\MSBuild.bat" "%WORKSPACE%\AccelByteUe4SdkDemo.sln" /t:build /p:Configuration="DebugGame";Platform=Linux;verbosity=diagnostic /m