::Clean workspace directories those need to be cleaned

::This directory will hold a packaged SDK
IF EXIST "%WORKSPACE%\temp" (
    rd /s /q "%WORKSPACE%\temp"
)

::This directory will hold compressed SDK package & compressed SDK source code
IF EXIST "%WORKSPACE%\artifacts" (
    rd /s /q "%WORKSPACE%\artifacts"
)

::This directory will hold the documentation that generated automatically
IF EXIST "%WORKSPACE%\Doxygen" (
    rd /s /q "%WORKSPACE%\Doxygen"
)