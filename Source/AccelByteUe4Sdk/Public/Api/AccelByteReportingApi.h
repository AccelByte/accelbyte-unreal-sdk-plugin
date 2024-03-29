// Copyright (c) 2022 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "Core/AccelByteApiBase.h"
#include "Core/AccelByteError.h"
#include "Core/AccelByteHttpRetryScheduler.h"
#include "Models/AccelByteReportingModels.h"

namespace AccelByte
{
class Credentials;
class Settings;
namespace Api
{

/**
 * @brief Reporting API for Reporting content.
 */
class ACCELBYTEUE4SDK_API Reporting : public FApiBase
{
public:
	Reporting(Credentials const& InCredentialsRef, Settings const& InSettingsRef, FHttpRetryScheduler& InHttpRef);
	~Reporting();

	/**
	 * @brief Submit Report.
	 *
	 * @param ReportData contain target report and evidence
	 * @param OnSuccess This will be called when the operation succeeded. The response is a FAccelByteModelsReportingSubmitResponse.
	 * @param OnError This will be called when the operation failed.
	 */
	void SubmitReport(const FAccelByteModelsReportingSubmitData ReportData
		, const THandler<FAccelByteModelsReportingSubmitResponse>& OnSuccess
		, const FErrorHandler& OnError);
	
	/**
	 * @brief Submit a chat report.
	 *
	 * @param ReportData contain target report and evidence
	 * @param OnSuccess This will be called when the operation succeeded. The response is a FAccelByteModelsReportingSubmitResponse.
	 * @param OnError This will be called when the operation failed.
	 */
	void SubmitChatReport(const FAccelByteModelsReportingSubmitDataChat& ReportData
		, const THandler<FAccelByteModelsReportingSubmitResponse>& OnSuccess
		, const FErrorHandler& OnError);
		
	/**
	 * @brief Get Reason under specified reason group.
	 *
	 * @param ReasonGroup Specified reason group.
	 * @param Offset Pagination offset.
	 * @param Limit Pagination limit.
	 * @param OnSuccess This will be called when the operation succeeded. The response is a FAccelByteModelsReasonsResponse.
	 * @param OnError This will be called when the operation failed.
	 * @param Title Query reason(s) by title.
	 */
	void GetReasons(const FString& ReasonGroup
		, int32 const& Offset
		, int32 const& Limit
		, const THandler<FAccelByteModelsReasonsResponse>& OnSuccess
		, const FErrorHandler& OnError
		, FString const& Title = TEXT(""));
		
	/**
	 * @brief Get Reason Groups.
	 *
	 * @param Offset Pagination offset.
	 * @param Limit Pagination limit.
	 * @param OnSuccess This will be called when the operation succeeded. The response is a FAccelByteModelsReasonGroupsResponse.
	 * @param OnError This will be called when the operation failed.
	 */
	void GetReasonGroups(int32 const& Offset
		, int32 const& Limit
		, const THandler<FAccelByteModelsReasonGroupsResponse>& OnSuccess
		, const FErrorHandler& OnError);
	
};

} // Namespace Api
} // Namespace AccelByte
