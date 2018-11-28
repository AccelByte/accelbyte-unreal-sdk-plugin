pushd Documentation
doxygen Doxyfile
popd
aws s3 cp "%WORKSPACE%\\Doxygen\\html" s3://docs.accelbyte.net/sdk/ue4/ --recursive