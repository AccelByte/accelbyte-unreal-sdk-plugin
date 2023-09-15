@ECHO OFF
SETLOCAL ENABLEDELAYEDEXPANSION

IF [%1]==[] GOTO usage

SET ProjectDir=%1

SET ConfigFile="%ProjectDir:"=%\Config\DefaultGame.ini"
SET OriginalConfigFile="%ConfigFile:"=%.orig"

:revert_file
    IF EXIST %OriginalConfigFile% (
        COPY /y %OriginalConfigFile% %ConfigFile% >NUL
        DEL %OriginalConfigFile%
    )
EXIT /b 0

:usage
    ECHO Usage: PostBuildProcessor.bat {project_dir}
EXIT /b 1

:end
ENDLOCAL