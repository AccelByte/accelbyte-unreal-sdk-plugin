#include "Blueprints/AccelByteBPUser.h"

void UUser::SetApiClient(FApiClientPtr NewApiClientPtr)
{
	ApiClientPtr = NewApiClientPtr;
}

void UUser::Registerv3(
	FRegisterRequestv3 const& RegisterRequest,
	FDRegisterResponseHandler const& OnSuccess,
	FDErrorHandler const& OnError)
{
	ApiClientPtr->User.Registerv3(
		RegisterRequest,
		THandler<FRegisterResponse>::CreateLambda(
			[OnSuccess](FRegisterResponse const& Response)
			{
				auto _ = OnSuccess.ExecuteIfBound(Response);
			}),
		FErrorHandler::CreateLambda(
			[OnError](int Code, FString const& Message)
			{
				auto _ = OnError.ExecuteIfBound(Code, Message);
			}));
}

void UUser::LoginWithUsername(
	FString const& Username,
	FString const& Password,
	FDHandler const& OnSuccess,
	FDErrorHandler const& OnError)
{
	ApiClientPtr->User.LoginWithUsername(
		Username,
		Password,
		FVoidHandler::CreateLambda(
			[OnSuccess]()
			{
				auto _ = OnSuccess.ExecuteIfBound();
			}),
		FErrorHandler::CreateLambda(
			[OnError](int Code, FString const& Message)
			{
				auto _ = OnError.ExecuteIfBound(Code, Message);
			}));
}

void UUser::LoginWithOtherPlatform(
	EAccelBytePlatformType const PlatformType,
	FString const& PlatformToken,
	FDHandler const& OnSuccess,
	FDErrorHandler const& OnError)
{
	ApiClientPtr->User.LoginWithOtherPlatform(
		PlatformType,
		PlatformToken,
		FVoidHandler::CreateLambda(
			[OnSuccess]()
			{
				auto _ = OnSuccess.ExecuteIfBound();
			}),
		FErrorHandler::CreateLambda(
			[OnError](int Code, FString const& Message)
			{
				auto _ = OnError.ExecuteIfBound(Code, Message);
			}));
}