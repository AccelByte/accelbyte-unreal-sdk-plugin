::Upload archived SDK package & source code

::::SDK package
aws s3 cp "%WORKSPACE%\\artifacts\\AccelByteUe4SdkPackage-%VERSION%.7z" s3://justice-sdk/UE4/AccelByteUe4SdkPackage-%VERSION%.7z --storage-class STANDARD_IA --metadata GitSha=%GIT_COMMIT%,BuildNo=%BUILD_NUMBER%

::::SDK source code
aws s3 cp "%WORKSPACE%\\artifacts\\AccelByteUe4SdkSource-%VERSION%.7z" s3://justice-sdk/UE4/AccelByteUe4SdkSource-%VERSION%.7z --storage-class STANDARD_IA --metadata GitSha=%GIT_COMMIT%,BuildNo=%BUILD_NUMBER%