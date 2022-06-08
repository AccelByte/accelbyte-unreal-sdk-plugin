// Copyright (c) 2022 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "JsonObjectWrapper.h"
#include "TestUtilities.h"
#include "Core/AccelByteJwtWrapper.h"
#include "GenericPlatform/GenericPlatformCrashContext.h"

DECLARE_LOG_CATEGORY_EXTERN(LogAccelByteJsonWebTokenTest, Log, All);
DEFINE_LOG_CATEGORY(LogAccelByteJsonWebTokenTest);

const FString ValidPublicKey(TEXT(
	R"(-----BEGIN PUBLIC KEY-----
MIIBIjANBgkqhkiG9w0BAQEFAAOCAQ8AMIIBCgKCAQEAnzyis1ZjfNB0bBgKFMSv
vkTtwlvBsaJq7S5wA+kzeVOVpVWwkWdVha4s38XM/pa/yr47av7+z3VTmvDRyAHc
aT92whREFpLv9cj5lTeJSibyr/Mrm/YtjCZVWgaOYIhwrXwKLqPr/11inWsAkfIy
tvHWTxZYEcXLgAXFuUuaS3uF9gEiNQwzGTU1v0FqkqTBr4B8nW3HCN47XUu0t8Y0
e+lf4s4OxQawWD79J9/5d3Ry0vbV3Am1FtGJiJvOwRsIfVChDpYStTcHTCMqtvWb
V6L11BWkpzGXSW4Hv43qa+GSYOD2QU68Mb59oSk2OB+BtOLpJofmbGEGgvmwyCI9
MwIDAQAB
-----END PUBLIC KEY-----)"));

const FString InvalidPublicKey(TEXT(R"(-----BEGIN PUBLIC KEY-----
								AIIBIjANBgkqhkiG9w0BAQEFAAOCAQ8AMIIBCgKCAQEAnzyis1ZjfNB0bBgKFMSv
								vkTtwlvBsaJq7S5wA+kzeVOVpVWwkWdVha4s38XM/pa/yr47av7+z3VTmvDRyAHc
								aT92whREFpLv9cj5lTeJSibyr/Mrm/YtjCZVWgaOYIhwrXwKLqPr/11inWsAkfIy
								tvHWTxZYEcXLgAXFuUuaS3uF9gEiNQwzGTU1v0FqkqTBr4B8nW3HCN47XUu0t8Y0
								e+lf4s4OxQawWD79J9/5d3Ry0vbV3Am1FtGJiJvOwRsIfVChDpYStTcHTCMqtvWb
								V6L11BWkpzGXSW4Hv43qa+GSYOD2QU68Mb59aSk2OB+BtOLpJofmbGEGgvmwyCI9
								MwIDAQAB
								-----END PUBLIC KEY-----)"));

static const int32 AutomationFlagMaskJWTTest = (EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::CommandletContext | EAutomationTestFlags::ClientContext);

IMPLEMENT_SIMPLE_AUTOMATION_TEST(JWTDecode_ValidFields_Sucess, "AccelByte.Tests.Core.JsonWebToken.A.JWTDecode_ValidFields_Sucess", AutomationFlagMaskJWTTest);
bool JWTDecode_ValidFields_Sucess::RunTest(const FString& Parameter)
{
	// Arrange
	const FString Token("eyJhbGciOiJSUzI1NiIsInR5cCI6IkpXVCJ9.eyJzdWIiOiIxMjM0NTY3ODkwIiwib2JqIjp7InN0cmluZ1ZhbHVlIjoiYWxwaGEiLCJhbm90aGVyU3RyaW5nVmFsdWUiOiJiZXRhIiwiaW50VmFsdWUiOjF9LCJpYXQiOjE1MTYyMzkwMjIsImV4cCI6MjAxNjIzOTAyMn0.PrqM8pQivNB8EJTBWmsGd4Gjo5-nYcThsyIR-iicNQFzwJRujUjF3Yaf0GCEp8g5fIs3wdspKMTTA_P650g0ojg5jUAumGaSdlYtFHvzr3MnCKwC4pjSU9uNQS-tpi5M41z8-vaQirEGjGKCApjhv_kJBktw8ZdpVaRcfzxhs06IJkR9hduoeL6BcVs5Ha3zhUwPs29rNF3x-9ZDzyKvmBh-ETI5dtLj8LqUN4DWVv9Kw3eSStUg5LrFhQXST1WNc5LcdSh5wIH1hQtPt7rD5Qxgz-aluBhYYgkYbpn5IiFey5SxstYhLMa9l0LGzf9BZrVV1wzdxKI74TseJoTq3w");
	const FDateTime ExpectedIat(FDateTime::FromUnixTimestamp(1516239022));
	const FDateTime ExpectedExp(FDateTime::FromUnixTimestamp(2016239022));
	const FString ExpectedStringValue("alpha");
	const FString ExpectedAnotherStringValue("beta");
	constexpr int32 ExpectedIntValue(1);
	const FString ExpectedSubValue("1234567890");
	
	/// Act
	TSharedPtr<FJsonObject> Result;
	FAccelByteJwtError Error;
	const bool bSuccess = AccelByteJwtWrapper::TryDecode(Token, ValidPublicKey, Result, Error);

	if(!bSuccess)
	{
		UE_LOG(LogAccelByteJsonWebTokenTest, Display, TEXT("%s"), *Error.Message);
	}

	const FString SubValue = Result->GetStringField("sub");
	const FDateTime Iat = FDateTime::FromUnixTimestamp(Result->GetIntegerField("iat"));
	const FDateTime Exp = FDateTime::FromUnixTimestamp(Result->GetIntegerField("exp"));
	const TSharedPtr<FJsonObject> ObjContent =  Result->GetObjectField("obj");
	const FString ObjStringValue = ObjContent->GetStringField("stringValue");
	const FString ObjAnotherStringValue = ObjContent->GetStringField("anotherStringValue");
	const int32 IntValue = ObjContent->GetIntegerField("intValue");
	
	// Asserts
	AB_TEST_TRUE(bSuccess);
	AB_TEST_EQUAL(Iat, ExpectedIat);
	AB_TEST_EQUAL(Exp, ExpectedExp);
	AB_TEST_EQUAL(ObjStringValue, ExpectedStringValue);
	AB_TEST_EQUAL(ObjAnotherStringValue, ExpectedAnotherStringValue);
	AB_TEST_EQUAL(IntValue, ExpectedIntValue);
	AB_TEST_EQUAL(SubValue, ExpectedSubValue);
	AB_TEST_EQUAL(Error.Code, 0);
	AB_TEST_TRUE(Error.Message.IsEmpty());
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(JWTDecode_InvalidPublicKey_NotSuccess, "AccelByte.Tests.Core.JsonWebToken.A.JWTDecode_InvalidPublicKey_NotSuccess", AutomationFlagMaskJWTTest);
bool JWTDecode_InvalidPublicKey_NotSuccess::RunTest(const FString& Parameter)
{
	// Arrange
	const FString token("eyJhbGciOiJSUzI1NiIsInR5cCI6IkpXVCJ9.eyJzdWIiOiIxMjM0NTY3ODkwIiwib2JqIjp7InN0cmluZ1ZhbHVlIjoiYWxwaGEiLCJhbm90aGVyU3RyaW5nVmFsdWUiOiJiZXRhIiwiaW50VmFsdWUiOjF9LCJpYXQiOjE1MTYyMzkwMjIsImV4cCI6MjAxNjIzOTAyMn0.PrqM8pQivNB8EJTBWmsGd4Gjo5-nYcThsyIR-iicNQFzwJRujUjF3Yaf0GCEp8g5fIs3wdspKMTTA_P650g0ojg5jUAumGaSdlYtFHvzr3MnCKwC4pjSU9uNQS-tpi5M41z8-vaQirEGjGKCApjhv_kJBktw8ZdpVaRcfzxhs06IJkR9hduoeL6BcVs5Ha3zhUwPs29rNF3x-9ZDzyKvmBh-ETI5dtLj8LqUN4DWVv9Kw3eSStUg5LrFhQXST1WNc5LcdSh5wIH1hQtPt7rD5Qxgz-aluBhYYgkYbpn5IiFey5SxstYhLMa9l0LGzf9BZrVV1wzdxKI74TseJoTq3w");

	/// Act
	TSharedPtr<FJsonObject> result;
	FAccelByteJwtError Error;
	bool isSuccess = AccelByteJwtWrapper::TryDecode(token, InvalidPublicKey, result, Error);

	// Asserts
	AB_TEST_FALSE(isSuccess);
	AB_TEST_FALSE(result.IsValid());
	AB_TEST_NOT_EQUAL(Error.Code, 0);
	AB_TEST_FALSE(Error.Message.IsEmpty());
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(JWTDecode_PastExpiration_NotSuccessWithNotValidResult, "AccelByte.Tests.Core.JsonWebToken.A.JWTDocode_PastExpiration_NotSuccessWithNotValidResult", AutomationFlagMaskJWTTest);
bool JWTDecode_PastExpiration_NotSuccessWithNotValidResult::RunTest(const FString& Parameter)
{
	// Arrange
	const FString token("eyJhbGciOiJSUzI1NiIsInR5cCI6IkpXVCJ9.eyJzdWIiOiIxMjM0NTY3ODkwIiwib2JqIjp7InN0cmluZ1ZhbHVlIjoiYWxwaGEiLCJhbm90aGVyU3RyaW5nVmFsdWUiOiJiZXRhIiwiaW50VmFsdWUiOjF9LCJpYXQiOjE1MTYyMzkwMjIsImV4cCI6MTUxNjIzOTAyM30.Avwb5oe8KoERr39Uv29ySsaz4cTAl0qjqhEDwM9UwcV3xEjutTtBQhV8egAnG63WT5FJw3eHKchzoI7fe_yoc7KxjRlqOCffvvbDR7bo_0wB62ySN1OaCbYl3ZTf_fkkoJbJWHMZdX5e1pU7a18fAhBXsyaf2AeorT51PYbXwxcXC4gBsL_tdMk-b8weblV1oiTf8SOLnbVr45nJurqOoSgwnIQyP6ZtWKm3tlk7RZECINL0SA3XilEor5_f7-9sUjm4WOWpDbhnVYZXTG6HMrsg4LkWHsekp2G0ieX2NpJ5onLky6gUIk-WeJK8W37Vbv4NSMCVloLN4YYj6UP1kA");

	/// Act
	TSharedPtr<FJsonObject> result;
	FAccelByteJwtError Error;
	bool isSuccess = AccelByteJwtWrapper::TryDecode(token, ValidPublicKey, result, Error);

	// Asserts
	AB_TEST_FALSE(isSuccess);
	AB_TEST_FALSE(result.IsValid());
	AB_TEST_NOT_EQUAL(Error.Code, 0);
	AB_TEST_FALSE(Error.Message.IsEmpty());
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(JWTDecode_InvalidPublicKeyWithNoVerifySignature_Success, "AccelByte.Tests.Core.JsonWebToken.A.JWTDocode_InvalidPublicKeyWithNoVerifySignature_Success", AutomationFlagMaskJWTTest);
bool JWTDecode_InvalidPublicKeyWithNoVerifySignature_Success::RunTest(const FString& Parameter)
{
	// Arrange
	const FString token("eyJhbGciOiJSUzI1NiIsInR5cCI6IkpXVCJ9.eyJzdWIiOiIxMjM0NTY3ODkwIiwib2JqIjp7InN0cmluZ1ZhbHVlIjoiYWxwaGEiLCJhbm90aGVyU3RyaW5nVmFsdWUiOiJiZXRhIiwiaW50VmFsdWUiOjF9LCJpYXQiOjE1MTYyMzkwMjIsImV4cCI6MjAxNjIzOTAyMn0.PrqM8pQivNB8EJTBWmsGd4Gjo5-nYcThsyIR-iicNQFzwJRujUjF3Yaf0GCEp8g5fIs3wdspKMTTA_P650g0ojg5jUAumGaSdlYtFHvzr3MnCKwC4pjSU9uNQS-tpi5M41z8-vaQirEGjGKCApjhv_kJBktw8ZdpVaRcfzxhs06IJkR9hduoeL6BcVs5Ha3zhUwPs29rNF3x-9ZDzyKvmBh-ETI5dtLj8LqUN4DWVv9Kw3eSStUg5LrFhQXST1WNc5LcdSh5wIH1hQtPt7rD5Qxgz-aluBhYYgkYbpn5IiFey5SxstYhLMa9l0LGzf9BZrVV1wzdxKI74TseJoTq3w");

	/// Act
	TSharedPtr<FJsonObject> result;
	FAccelByteJwtError Error;
	bool isSuccess = AccelByteJwtWrapper::TryDecode(token, InvalidPublicKey, result, Error, false);

	// Asserts
	AB_TEST_TRUE(isSuccess);
	AB_TEST_TRUE(result.IsValid());
	AB_TEST_EQUAL(Error.Code, 0);
	AB_TEST_TRUE(Error.Message.IsEmpty());
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(JWTDecode_ExpiredTokenWithNotVerifyExpiration_Success, "AccelByte.Tests.Core.JsonWebToken.A.JWTDocode_ExpiredTokenWithNotVerifyExpiration_Success", AutomationFlagMaskJWTTest);
bool JWTDecode_ExpiredTokenWithNotVerifyExpiration_Success::RunTest(const FString& Parameter)
{
	// Arrange
	const FString token("eyJhbGciOiJSUzI1NiIsInR5cCI6IkpXVCJ9.eyJzdWIiOiIxMjM0NTY3ODkwIiwib2JqIjp7InN0cmluZ1ZhbHVlIjoiYWxwaGEiLCJhbm90aGVyU3RyaW5nVmFsdWUiOiJiZXRhIiwiaW50VmFsdWUiOjF9LCJpYXQiOjE1MTYyMzkwMjIsImV4cCI6MTUxNjIzOTAyM30.Avwb5oe8KoERr39Uv29ySsaz4cTAl0qjqhEDwM9UwcV3xEjutTtBQhV8egAnG63WT5FJw3eHKchzoI7fe_yoc7KxjRlqOCffvvbDR7bo_0wB62ySN1OaCbYl3ZTf_fkkoJbJWHMZdX5e1pU7a18fAhBXsyaf2AeorT51PYbXwxcXC4gBsL_tdMk-b8weblV1oiTf8SOLnbVr45nJurqOoSgwnIQyP6ZtWKm3tlk7RZECINL0SA3XilEor5_f7-9sUjm4WOWpDbhnVYZXTG6HMrsg4LkWHsekp2G0ieX2NpJ5onLky6gUIk-WeJK8W37Vbv4NSMCVloLN4YYj6UP1kA");

	/// Act
	TSharedPtr<FJsonObject> result;
	FAccelByteJwtError Error;
	bool isSuccess = AccelByteJwtWrapper::TryDecode(token, ValidPublicKey, result, Error, true, false);

	// Asserts
	AB_TEST_TRUE(isSuccess);
	AB_TEST_TRUE(result.IsValid());
	AB_TEST_EQUAL(Error.Code, 0);
	AB_TEST_TRUE(Error.Message.IsEmpty());
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(JWTDecode_WithValidSub_Success, "AccelByte.Tests.Core.JsonWebToken.A.JWTDocode_WithValidSub_Success", AutomationFlagMaskJWTTest);
bool JWTDecode_WithValidSub_Success::RunTest(const FString& Parameter)
{
	// Arrange
	const FString token("eyJhbGciOiJSUzI1NiIsInR5cCI6IkpXVCJ9.eyJzdWIiOiIxMjM0NTY3ODkwIiwib2JqIjp7InN0cmluZ1ZhbHVlIjoiYWxwaGEiLCJhbm90aGVyU3RyaW5nVmFsdWUiOiJiZXRhIiwiaW50VmFsdWUiOjF9LCJpYXQiOjE1MTYyMzkwMjIsImV4cCI6MjAxNjIzOTAyMn0.PrqM8pQivNB8EJTBWmsGd4Gjo5-nYcThsyIR-iicNQFzwJRujUjF3Yaf0GCEp8g5fIs3wdspKMTTA_P650g0ojg5jUAumGaSdlYtFHvzr3MnCKwC4pjSU9uNQS-tpi5M41z8-vaQirEGjGKCApjhv_kJBktw8ZdpVaRcfzxhs06IJkR9hduoeL6BcVs5Ha3zhUwPs29rNF3x-9ZDzyKvmBh-ETI5dtLj8LqUN4DWVv9Kw3eSStUg5LrFhQXST1WNc5LcdSh5wIH1hQtPt7rD5Qxgz-aluBhYYgkYbpn5IiFey5SxstYhLMa9l0LGzf9BZrVV1wzdxKI74TseJoTq3w");

	/// Act
	TSharedPtr<FJsonObject> result;
	FAccelByteJwtError Error;
	bool isSuccess = AccelByteJwtWrapper::TryDecode(token, ValidPublicKey, result, Error, true, true, TEXT("1234567890"));

	// Asserts
	AB_TEST_TRUE(isSuccess);
	AB_TEST_TRUE(result.IsValid());
	AB_TEST_EQUAL(Error.Code, 0);
	AB_TEST_TRUE(Error.Message.IsEmpty());

	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(JWTDecode_WithInvalidSub_FailWithNotValidResult, "AccelByte.Tests.Core.JsonWebToken.A.JWTDocode_WithInvalidSub_FailWithNotValidResult", AutomationFlagMaskJWTTest);
bool JWTDecode_WithInvalidSub_FailWithNotValidResult::RunTest(const FString& Parameter)
{
	// Arrange
	const FString token("eyJhbGciOiJSUzI1NiIsInR5cCI6IkpXVCJ9.eyJzdWIiOiIxMjM0NTY3ODkwIiwib2JqIjp7InN0cmluZ1ZhbHVlIjoiYWxwaGEiLCJhbm90aGVyU3RyaW5nVmFsdWUiOiJiZXRhIiwiaW50VmFsdWUiOjF9LCJpYXQiOjE1MTYyMzkwMjIsImV4cCI6MjAxNjIzOTAyMn0.PrqM8pQivNB8EJTBWmsGd4Gjo5-nYcThsyIR-iicNQFzwJRujUjF3Yaf0GCEp8g5fIs3wdspKMTTA_P650g0ojg5jUAumGaSdlYtFHvzr3MnCKwC4pjSU9uNQS-tpi5M41z8-vaQirEGjGKCApjhv_kJBktw8ZdpVaRcfzxhs06IJkR9hduoeL6BcVs5Ha3zhUwPs29rNF3x-9ZDzyKvmBh-ETI5dtLj8LqUN4DWVv9Kw3eSStUg5LrFhQXST1WNc5LcdSh5wIH1hQtPt7rD5Qxgz-aluBhYYgkYbpn5IiFey5SxstYhLMa9l0LGzf9BZrVV1wzdxKI74TseJoTq3w");

	/// Act
	TSharedPtr<FJsonObject> result;
	FAccelByteJwtError Error;
	bool isSuccess = AccelByteJwtWrapper::TryDecode(token, ValidPublicKey, result, Error, true, true, TEXT("0000"));

	// Asserts
	AB_TEST_FALSE(isSuccess);
	AB_TEST_FALSE(result.IsValid());
	AB_TEST_NOT_EQUAL(Error.Code, 0);
	AB_TEST_FALSE(Error.Message.IsEmpty());

	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(JWTGetExpiration_Valid_Ok, "AccelByte.Tests.Core.JsonWebToken.A.JWTGetExpiration_Valid_Ok", AutomationFlagMaskJWTTest);
bool JWTGetExpiration_Valid_Ok::RunTest(const FString& Parameter)
{
	// Arrange
	const FString token("eyJhbGciOiJSUzI1NiIsInR5cCI6IkpXVCJ9.eyJzdWIiOiIxMjM0NTY3ODkwIiwib2JqIjp7InN0cmluZ1ZhbHVlIjoiYWxwaGEiLCJhbm90aGVyU3RyaW5nVmFsdWUiOiJiZXRhIiwiaW50VmFsdWUiOjF9LCJpYXQiOjE1MTYyMzkwMjIsImV4cCI6MjAxNjIzOTAyMn0.PrqM8pQivNB8EJTBWmsGd4Gjo5-nYcThsyIR-iicNQFzwJRujUjF3Yaf0GCEp8g5fIs3wdspKMTTA_P650g0ojg5jUAumGaSdlYtFHvzr3MnCKwC4pjSU9uNQS-tpi5M41z8-vaQirEGjGKCApjhv_kJBktw8ZdpVaRcfzxhs06IJkR9hduoeL6BcVs5Ha3zhUwPs29rNF3x-9ZDzyKvmBh-ETI5dtLj8LqUN4DWVv9Kw3eSStUg5LrFhQXST1WNc5LcdSh5wIH1hQtPt7rD5Qxgz-aluBhYYgkYbpn5IiFey5SxstYhLMa9l0LGzf9BZrVV1wzdxKI74TseJoTq3w");
	const FDateTime ExpectedDate {FDateTime::FromUnixTimestamp(2016239022)};
	
	/// Act
	FDateTime Result;
	AccelByteJwtWrapper::GetExpirationDateTime(token, Result);

	// Asserts
	AB_TEST_EQUAL(Result, ExpectedDate);

	return true;
}