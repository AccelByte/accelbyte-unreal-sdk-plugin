@ECHO OFF
SETLOCAL ENABLEDELAYEDEXPANSION 

IF [%1]==[] GOTO usage
IF [%2]==[] GOTO usage
IF [%3]==[] GOTO usage
IF [%4]==[] GOTO usage

SET EngineDir=%1
SET ProjectDir=%2
SET TargetConfiguration=%3
SET TargetType=%4

SET ConfigFile="%ProjectDir:"=%\Config\DefaultGame.ini"
SET BuildVersionFile="%EngineDir:"=%\Build\Build.version"
SET IniSection=+IniSectionBlacklist

SET ServerSettingsSectionList=/Script/AccelByteUe4Sdk.AccelByteServerSettings ^
 /Script/AccelByteUe4Sdk.AccelByteServerSettingsDev ^
 /Script/AccelByteUe4Sdk.AccelByteServerSettingsCert ^
 /Script/AccelByteUe4Sdk.AccelByteServerSettingsProd

SET PackagingSettingSection=[/Script/UnrealEd.ProjectPackagingSettings]

CALL :get_engine_version Version
IF %Version%==5 SET IniSection=+IniSectionDenylist

IF NOT %TargetConfiguration%==Shipping GOTO remove_section_blacklist
IF %TargetType%==Editor GOTO remove_section_blacklist
IF %TargetType%==Server GOTO remove_section_blacklist

CALL :append_section_blacklist
EXIT /b 0

:usage
    ECHO Usage: PreBuildProcessor.bat {engine_dir} {project_dir} {target_configuration} {target_type}
EXIT /b 1

:append_section_blacklist
    SET ConfigFileTmp="%ConfigFile:"=%.tmp"
    SET CurrentSection=
    SET FirstLine=True
    SET IsPackagingSettingSection=False
    
    (FOR /f "usebackq eol=` delims=" %%i IN (%ConfigFile%) DO (
        SET Line=%%i
        IF "x!Line:~0,1!"=="x[" (
            IF !FirstLine!==True (
                SET FirstLine=False
            ) ELSE (
                ECHO:
            )
            SET CurrentSection=!Line!
            IF !CurrentSection!==!PackagingSettingSection! (
                SET IsPackagingSettingSection=True
            )
        ) ELSE (
            IF !CurrentSection!==!PackagingSettingSection! (
                FOR %%a IN (!ServerSettingsSectionList!) DO (
                    IF !Line!==!IniSection!=%%a (
                        SET Line=
                    )
                )
            ) ELSE (
                IF !IsPackagingSettingSection!==True (
                    FOR %%a IN (!ServerSettingsSectionList!) DO (
                        ECHO !IniSection!=%%a
                    )
                    SET IsPackagingSettingSection=False
                )
            )
        )
        IF NOT [!Line!] == [] ECHO !Line!
    ))>%ConfigFileTmp%
    
    IF !IsPackagingSettingSection!==True (
        (FOR %%a IN (!ServerSettingsSectionList!) DO (
            ECHO !IniSection!=%%a
        ))>>"%ConfigFileTmp%"
    )

    COPY /y %ConfigFileTmp% %ConfigFile% >NUL
    DEL %ConfigFileTmp%
    EXIT /B 0

:remove_section_blacklist
    SET ConfigFileTmp="%ConfigFile:"=%.tmp"
    SET CurrentSection=
    
    SET FirstLine=True
    (FOR /f "usebackq eol=` delims=" %%i IN (%ConfigFile%) DO (
        SET Line=%%i
        IF "x!Line:~0,1!"=="x[" (
            IF !FirstLine!==True (
                SET FirstLine=False
            ) ELSE (
                ECHO:
            )
            SET CurrentSection=!Line!
        ) ELSE (
            IF !CurrentSection!==!PackagingSettingSection! (
                FOR %%a IN (!ServerSettingsSectionList!) DO (
                    IF !Line!==!IniSection!=%%a (
                        SET Line=
                    )
                )
            )
        )
        IF NOT [!Line!] == [] ECHO !Line!
    ))>%ConfigFileTmp%

    COPY /y %ConfigFileTmp% %ConfigFile% >NUL
    DEL %ConfigFileTmp%
    EXIT /B 0

:get_engine_version
    SET BuildVersion=
    FOR /f "usebackq tokens=1,2" %%a IN (%BuildVersionFile%) DO (
        SET VersionInfoKey=%%a
        SET VersionInfoValue=%%b
        SET MajorVersionKey=!VersionInfoKey:MajorVersion=!
        IF NOT !VersionInfoKey!==!MajorVersionKey! (
            SET BuildVersion=!VersionInfoValue:,=!
        )
    )
    SET %~1=%BuildVersion%
    EXIT /B 0
    
:end
ENDLOCAL