#include "Blueprints/ABGroup.h"

void UABGroup::SetApiClient(const FApiClientPtr NewApiClientPtr)
{
	ApiClientPtr = NewApiClientPtr;
}

void UABGroup::CreateGroup(
	const FAccelByteModelsCreateGroupRequest& RequestContent,
	const FCreateGroupSuccess& OnSuccess,
	const FDErrorHandler& OnError)
{
	ApiClientPtr->Group.CreateGroup(
		RequestContent,
		THandler<FAccelByteModelsGroupInformation>::CreateLambda(
			[OnSuccess](const FAccelByteModelsGroupInformation Response)
		{
			OnSuccess.ExecuteIfBound(Response);
		}),
		FErrorHandler::CreateLambda([OnError](const int32 ErrorCode, const FString& ErrorMessage)
		{
			OnError.ExecuteIfBound(ErrorCode, ErrorMessage);
		}));
}
