::Compress packaged SDK & the source code

::::Create a directory that will hold the compressed files
mkdir "%WORKSPACE%\\artifacts"

::::Compress the package
"%ZIP_PATH%\\7z.exe" a -t7z "%WORKSPACE%\\artifacts\\AccelByteUe4SdkPackage-%GIT_COMMIT%-BUILD-%BUILD_NUMBER%.7z" "%WORKSPACE%\\temp"

::::Compress the source code
"%ZIP_PATH%\\7z.exe" a -t7z "%WORKSPACE%\\artifacts\\AccelByteUe4SdkSource-%GIT_COMMIT%-BUILD-%BUILD_NUMBER%.7z" "%WORKSPACE%\\Plugins\\AccelByteUe4Sdk"