// Copyright (c) 2021 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "ParseErrorTest.h"
#include "Core/AccelByteError.h"
#include "TestUtilities.h"


DECLARE_LOG_CATEGORY_EXTERN(LogAccelByteParseErrorTest, Log, All);
DEFINE_LOG_CATEGORY(LogAccelByteParseErrorTest);

const int32 AutomationFlagMaskParseError = (EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::CommandletContext | EAutomationTestFlags::ClientContext);

IMPLEMENT_SIMPLE_AUTOMATION_TEST(ParseErrorTestUndefinedResponseStatusIsErrorTrue, "AccelByte.Tests.Core.ParseError.UndefinedResponseStatusIsErrorTrue", AutomationFlagMaskParseError)
bool ParseErrorTestUndefinedResponseStatusIsErrorTrue::RunTest(const FString& Parameters)
{
	int32 Code = 1234;

	int32 OutCode;
	FString OutMessages;
	TSharedPtr<FMockResponse, ESPMode::ThreadSafe> TestResponse = MakeShared<FMockResponse, ESPMode::ThreadSafe>();
	TestResponse->SetMockResponseCode(Code);

	HandleHttpError(nullptr, TestResponse, OutCode, OutMessages);
	
	AB_TEST_EQUAL(OutCode, Code);

	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(ParseErrorTestNoResponse, "AccelByte.Tests.Core.ParseError.NoResponse", AutomationFlagMaskParseError)
bool ParseErrorTestNoResponse::RunTest(const FString& Parameters)
{
	int32 OutCode;
	FString OutMessages;
	HandleHttpError(nullptr, nullptr, OutCode, OutMessages);

	AB_TEST_EQUAL(OutCode, 14005);
	AB_TEST_FALSE(OutMessages.IsEmpty());

	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(ParseErrorTestErrorResponseNoBodyErrorIsHttpStatus, "AccelByte.Tests.Core.ParseError.ErrorResponseNoBodyErrorIsHttpStatus", AutomationFlagMaskParseError)
bool ParseErrorTestErrorResponseNoBodyErrorIsHttpStatus::RunTest(const FString& Parameters)
{
	int32 Code = 405;
	int32 OutCode;
	FString OutMessages;
	TSharedPtr<FMockResponse, ESPMode::ThreadSafe> TestResponse = MakeShared<FMockResponse, ESPMode::ThreadSafe>();
	TestResponse->SetMockResponseCode(Code);

	HandleHttpError(nullptr, TestResponse, OutCode, OutMessages);

	AB_TEST_EQUAL(OutCode, Code);
	AB_TEST_FALSE(OutMessages.IsEmpty());

	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(ParseErrorTestErrorResponseGenericErrorBodyErrorIsBodyErrorCode, "AccelByte.Tests.Core.ParseError.ErrorResponseGenericErrorBodyErrorIsBodyErrorCode", AutomationFlagMaskParseError)
bool ParseErrorTestErrorResponseGenericErrorBodyErrorIsBodyErrorCode::RunTest(const FString& Parameters)
{
	int32 Code = 403;

	FErrorInfo ErrorInfo;
	ErrorInfo.ErrorCode = 20013;
	ErrorInfo.ErrorMessage = "insufficient permissions";

	FString Content;
	FJsonObjectConverter::UStructToJsonObjectString(ErrorInfo, Content, 0, 0);

	int32 OutCode;
	FString OutMessages;
	TSharedPtr<FMockResponse, ESPMode::ThreadSafe> TestResponse = MakeShared<FMockResponse, ESPMode::ThreadSafe>();
	TestResponse->SetMockResponseCode(Code);
	TestResponse->SetMockContent(Content);

	HandleHttpError(nullptr, TestResponse, OutCode, OutMessages);

	AB_TEST_EQUAL(OutCode, ErrorInfo.ErrorCode);
	AB_TEST_EQUAL(OutMessages, ErrorInfo.ErrorMessage);

	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(ParseErrorTestErrorResponsePlatformErrorBodyErrorIsBodyErrorCode, "AccelByte.Tests.Core.ParseError.ErrorResponsePlatformErrorBodyErrorIsBodyErrorCode", AutomationFlagMaskParseError)
bool ParseErrorTestErrorResponsePlatformErrorBodyErrorIsBodyErrorCode::RunTest(const FString& Parameters)
{
	FErrorInfo ErrorInfo;
	ErrorInfo.NumericErrorCode = 12345;
	ErrorInfo.ErrorMessage = "msg 1";

	FString Content;
	FJsonObjectConverter::UStructToJsonObjectString(ErrorInfo, Content, 0, 0);

	int32 OutCode;
	FString OutMessages;
	TSharedPtr<FMockResponse, ESPMode::ThreadSafe> TestResponse = MakeShared<FMockResponse, ESPMode::ThreadSafe>();
	TestResponse->SetMockContent(Content);

	HandleHttpError(nullptr, TestResponse, OutCode, OutMessages);

	AB_TEST_EQUAL(OutCode, ErrorInfo.NumericErrorCode);
	AB_TEST_EQUAL(OutMessages, ErrorInfo.ErrorMessage);

	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(ParseErrorTestErrorResponseOAuthErrorBodyErrorIsHttpStatus, "AccelByte.Tests.Core.ParseError.ErrorResponseOAuthErrorBodyErrorIsHttpStatus", AutomationFlagMaskParseError)
bool ParseErrorTestErrorResponseOAuthErrorBodyErrorIsHttpStatus::RunTest(const FString& Parameters)
{
	int32 Code = 400;
	FErrorInfo ErrorInfo;
	ErrorInfo.Error = "invalid request";

	FString Content;
	FJsonObjectConverter::UStructToJsonObjectString(ErrorInfo, Content, 0, 0);

	FJsonObject OutMessageVariables;
	int32 OutCode;
	FString OutMessages;
	TSharedPtr<FMockResponse, ESPMode::ThreadSafe> TestResponse = MakeShared<FMockResponse, ESPMode::ThreadSafe>();
	TestResponse->SetMockResponseCode(Code);
	TestResponse->SetMockContent(Content);

	HandleHttpCustomError(nullptr, TestResponse, OutCode, OutMessages, OutMessageVariables);

	AB_TEST_EQUAL(OutCode, Code);
	AB_TEST_EQUAL(OutMessages, ErrorInfo.Error);
	
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(ParseErrorTestErrorResponseCloudSaveErrorBodyErrorIsBodyErrorCode, "AccelByte.Tests.Core.ParseError.ErrorResponseCloudSaveErrorBodyErrorIsBodyErrorCode", AutomationFlagMaskParseError)
bool ParseErrorTestErrorResponseCloudSaveErrorBodyErrorIsBodyErrorCode::RunTest(const FString& Parameters)
{
	FErrorInfo ErrorInfo;
	ErrorInfo.Code = 12345;
	ErrorInfo.Message = "msg 1";

	FString Content;
	FJsonObjectConverter::UStructToJsonObjectString(ErrorInfo, Content, 0, 0);

	int32 OutCode;
	FString OutMessages;
	TSharedPtr<FMockResponse, ESPMode::ThreadSafe> newMockResponse = MakeShared<FMockResponse, ESPMode::ThreadSafe>();
	newMockResponse->SetMockContent(Content);

	HandleHttpError(nullptr, newMockResponse, OutCode, OutMessages);

	AB_TEST_EQUAL(OutCode, ErrorInfo.Code);
	AB_TEST_EQUAL(OutMessages, ErrorInfo.Message);

	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(ParseErrorTestErrorResponseMixedErrorNumericErrorCodeBeforeErrorCode, "AccelByte.Tests.Core.ParseError.ErrorResponseMixedErrorNumericErrorCodeBeforeErrorCode", AutomationFlagMaskParseError)
bool ParseErrorTestErrorResponseMixedErrorNumericErrorCodeBeforeErrorCode::RunTest(const FString& Parameters)
{
	FErrorInfo ErrorInfo1;
	ErrorInfo1.ErrorCode = 54321;
	ErrorInfo1.NumericErrorCode = 12345;
	ErrorInfo1.ErrorMessage = "msg 1";
	ErrorInfo1.Message = "msg 2";

	FString Content1;
	FJsonObjectConverter::UStructToJsonObjectString(ErrorInfo1, Content1, 0, 0);

	int32 OutCode;
	FString OutMessages;
	TSharedPtr<FMockResponse, ESPMode::ThreadSafe> TestResponse1 = MakeShared<FMockResponse, ESPMode::ThreadSafe>();
	TestResponse1->SetMockContent(Content1);

	HandleHttpError(nullptr, TestResponse1, OutCode, OutMessages);

	AB_TEST_EQUAL(OutCode, ErrorInfo1.NumericErrorCode);
	AB_TEST_EQUAL(OutMessages, ErrorInfo1.ErrorMessage);

	FErrorInfo ErrorInfo2;
	ErrorInfo2.Code = 12345;
	ErrorInfo2.NumericErrorCode = 54321;
	ErrorInfo2.ErrorMessage = "msg 1";
	ErrorInfo2.Message = "msg 2";

	FString Content2 = "";
	FJsonObjectConverter::UStructToJsonObjectString(ErrorInfo2, Content2, 0, 0);

	OutCode = 0;
	OutMessages = 0;
	TSharedPtr<FMockResponse, ESPMode::ThreadSafe> TestResponse2 = MakeShared<FMockResponse, ESPMode::ThreadSafe>();
	TestResponse2->SetMockContent(Content2);

	HandleHttpError(nullptr, TestResponse2, OutCode, OutMessages);

	AB_TEST_EQUAL(OutCode, ErrorInfo2.NumericErrorCode);
	AB_TEST_EQUAL(OutMessages, ErrorInfo2.ErrorMessage);

	FErrorInfo ErrorInfo3;
	ErrorInfo3.ErrorCode = 54321;
	ErrorInfo3.Code = 12345;
	ErrorInfo3.Message = "msg 1";

	FString OutString3;
	FJsonObjectConverter::UStructToJsonObjectString(ErrorInfo3, OutString3, 0, 0);

	OutCode = 0;
	OutMessages = "";
	TSharedPtr<FMockResponse, ESPMode::ThreadSafe> TestResponse3 = MakeShared<FMockResponse, ESPMode::ThreadSafe>();
	TestResponse3->SetMockContent(OutString3);

	HandleHttpError(nullptr, TestResponse3, OutCode, OutMessages);

	AB_TEST_EQUAL(OutCode, ErrorInfo3.ErrorCode);
	AB_TEST_EQUAL(OutMessages, ErrorInfo3.Message);

	return true;
}