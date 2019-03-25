::Generate and upload inlined documentation

::::Generate doxygen documentation
pushd Documentation
doxygen Doxyfile
popd

::::Upload the generated documentation
aws s3 cp "%WORKSPACE%\Doxygen\html" s3://docs.accelbyte.net/sdk/ue4/ --recursive