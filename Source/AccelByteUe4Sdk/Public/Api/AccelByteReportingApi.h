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
class ACCELBYTEUE4SDK_API Reporting : public FApiBase, public TSharedFromThis<Reporting, ESPMode::ThreadSafe>
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
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr SubmitReport(FAccelByteModelsReportingSubmitData const& ReportData
		, THandler<FAccelByteModelsReportingSubmitResponse> const& OnSuccess
		, FErrorHandler const& OnError);
	
	/**
	 * @brief Submit a chat report.
	 *
	 * @param ReportData contain target report and evidence
	 * @param OnSuccess This will be called when the operation succeeded. The response is a FAccelByteModelsReportingSubmitResponse.
	 * @param OnError This will be called when the operation failed.
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr SubmitChatReport(FAccelByteModelsReportingSubmitDataChat const& ReportData
		, THandler<FAccelByteModelsReportingSubmitResponse> const& OnSuccess
		, FErrorHandler const& OnError);
		
	/**
	 * @brief Get Reason under specified reason group.
	 *
	 * @param ReasonGroup Specified reason group.
	 * @param Offset Pagination offset.
	 * @param Limit Pagination limit.
	 * @param OnSuccess This will be called when the operation succeeded. The response is a FAccelByteModelsReasonsResponse.
	 * @param OnError This will be called when the operation failed.
	 * @param Title Query reason(s) by title.
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr GetReasons(FString const& ReasonGroup
		, int32 Offset
		, int32 Limit
		, THandler<FAccelByteModelsReasonsResponse> const& OnSuccess
		, FErrorHandler const& OnError
		, FString const& Title = TEXT(""));
		
	/**
	 * @brief Get Reason Groups.
	 *
	 * @param Offset Pagination offset.
	 * @param Limit Pagination limit.
	 * @param OnSuccess This will be called when the operation succeeded. The response is a FAccelByteModelsReasonGroupsResponse.
	 * @param OnError This will be called when the operation failed.
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr GetReasonGroups(int32 Offset
		, int32 Limit
		, THandler<FAccelByteModelsReasonGroupsResponse> const& OnSuccess
		, FErrorHandler const& OnError);
	
private:

	static FString AdditionalValidationByCategory(FAccelByteModelsReportingSubmitData const& InReportData);
};

typedef TSharedRef<Reporting, ESPMode::ThreadSafe> ReportingRef;
typedef TSharedPtr<Reporting, ESPMode::ThreadSafe> ReportingPtr;
typedef TWeakPtr<Reporting, ESPMode::ThreadSafe> ReportingWPtr;

} // Namespace Api
} // Namespace AccelByte
