::Package the SDK project into "/temp" directory

::::Remove the content of this directory first
IF EXIST "%WORKSPACE%\Saved" (
    rd /s /q "%WORKSPACE%\Saved"
)

::::Remove the content of this directory first
IF EXIST "%WORKSPACE%\Intermediate" (
    rd /s /q "%WORKSPACE%\Intermediate"
)

::::Packaging
"%ENGINE_PATH%\Engine\Build\BatchFiles\RunUAT.bat" -ScriptsForProject="%WORKSPACE%\AccelByteUe4SdkDemo.uproject" BuildCookRun -nocompileeditor -nop4 -project="%WORKSPACE%\AccelByteUe4SdkDemo.uproject" -cook -stage -archive -archivedirectory="%WORKSPACE%\temp" -package -clientconfig=Shipping -ue4exe=UE4Editor-Cmd.exe  -pak -prereqs -targetplatform=Win64 -build -AccelByteUe4SdkDemo -utf8output  