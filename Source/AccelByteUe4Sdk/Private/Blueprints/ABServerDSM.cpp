// Copyright (c) 2021 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "Blueprints/ABServerDSM.h"

void UABServerDSM::SetServerApiClient(FServerApiClientPtr NewServerApiClientPtr)
{
	ApiClientPtr = NewServerApiClientPtr;
}

void UABServerDSM::RegisterServerToDSM(
	int32 const& Port,
	FDHandler OnSuccess,
	FDErrorHandler OnError)
{
	ApiClientPtr->ServerDSM.RegisterServerToDSM(
		Port,
		FVoidHandler::CreateLambda(
			[OnSuccess]()
			{
				OnSuccess.ExecuteIfBound();
			}),
		FErrorHandler::CreateLambda(
			[OnError](int Code, FString const& Message)
			{
				OnError.ExecuteIfBound(Code, Message);
			}));
}

void UABServerDSM::RegisterLocalServerToDSM(
	FString const& IPAddress,
	int32 const& Port,
	FString const& ServerName,
	FDHandler OnSuccess,
	FDErrorHandler OnError)
{
	ApiClientPtr->ServerDSM.RegisterLocalServerToDSM(
		IPAddress,
		Port,
		ServerName,
		FVoidHandler::CreateLambda(
			[OnSuccess]()
			{
				OnSuccess.ExecuteIfBound();
			}),
		FErrorHandler::CreateLambda(
			[OnError](int Code, FString const& Message)
			{
				OnError.ExecuteIfBound(Code, Message);
			}));
}

void UABServerDSM::SendShutdownToDSM(
	bool const& KillMe,
	FString const& MatchId,
	FDHandler OnSuccess,
	FDErrorHandler OnError)
{
	ApiClientPtr->ServerDSM.SendShutdownToDSM(
		KillMe,
		MatchId,
		FVoidHandler::CreateLambda(
			[OnSuccess]()
			{
				OnSuccess.ExecuteIfBound();
			}),
		FErrorHandler::CreateLambda(
			[OnError](int Code, FString const& Message)
			{
				OnError.ExecuteIfBound(Code, Message);
			}));
}

void UABServerDSM::DeregisterLocalServerFromDSM(
	FString const& ServerName,
	FDHandler OnSuccess,
	FDErrorHandler OnError)
{
	ApiClientPtr->ServerDSM.DeregisterLocalServerFromDSM(
		ServerName,
		FVoidHandler::CreateLambda(
			[OnSuccess]()
			{
				OnSuccess.ExecuteIfBound();
			}),
		FErrorHandler::CreateLambda(
			[OnError](int Code, FString const& Message)
			{
				OnError.ExecuteIfBound(Code, Message);
			}));
}

void UABServerDSM::GetSessionId(
	FDServerSessionResponseDelegate OnSuccess,
	FDErrorHandler OnError)
{
	ApiClientPtr->ServerDSM.GetSessionId(
		THandler<FAccelByteModelsServerSessionResponse>::CreateLambda(
			[OnSuccess](FAccelByteModelsServerSessionResponse const& Response)
			{
				OnSuccess.ExecuteIfBound(Response);
			}),
		FErrorHandler::CreateLambda(
			[OnError](int Code, FString const& Message)
			{
				OnError.ExecuteIfBound(Code, Message);
			}));
}

void UABServerDSM::GetServerInfo(
	FDServerInfoDelegate OnSuccess,
	FDErrorHandler OnError)
{
	ApiClientPtr->ServerDSM.GetServerInfo(
		THandler<FAccelByteModelsServerInfo>::CreateLambda(
			[OnSuccess](FAccelByteModelsServerInfo const& Response)
			{
				OnSuccess.ExecuteIfBound(Response);
			}),
		FErrorHandler::CreateLambda(
			[OnError](int Code, FString const& Message)
			{
				OnError.ExecuteIfBound(Code, Message);
			}));
}

void UABServerDSM::ConfigureAutoShutdown(
	int32 TickSeconds,
	int CountdownStart)
{
	ApiClientPtr->ServerDSM.ConfigureAutoShutdown(
		TickSeconds,
		CountdownStart
	);
}

void UABServerDSM::SetOnAutoShutdownResponse(FDHandler OnAutoShutdown)
{
	ApiClientPtr->ServerDSM.SetOnAutoShutdownResponse(
		FVoidHandler::CreateLambda(
			[OnAutoShutdown]()
			{
				OnAutoShutdown.ExecuteIfBound();
			}));
}

void UABServerDSM::SetOnAutoShutdownErrorDelegate(FDErrorHandler OnShutdownError)
{
	ApiClientPtr->ServerDSM.SetOnAutoShutdownErrorDelegate(
		FErrorHandler::CreateLambda(
			[OnShutdownError](int Code, FString const& Message)
			{
				OnShutdownError.ExecuteIfBound(Code, Message);
			}));
}

void UABServerDSM::SetServerName(const FString Name)
{
	ApiClientPtr->ServerDSM.SetServerName(Name);
}

void UABServerDSM::SetServerType(UEnumServerType Type)
{
	ApiClientPtr->ServerDSM.SetServerType((EServerType)(int)Type);
}

int32 UABServerDSM::GetPlayerNum()
{
	return ApiClientPtr->ServerDSM.GetPlayerNum();
}
