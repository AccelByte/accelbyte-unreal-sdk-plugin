// Copyright (c) 2024 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "Core/AccelByteWebSocketReconnection.h"

AccelByte::FReconnectionStrategy::FReconnectionStrategy(
	EReconnectionStrategyType InStrategyType,
	FTotalTimeoutDuration InTotalTimeoutToGiveUp,
	FInitialBackoffDelay InInitialBackoffDelay,
	FAggressiveRetryInterval InAggressiveRetryInterval,
	FBalancedMaxRetryInterval InBalancedMaxRetryInterval
) :
	_StrategyType(InStrategyType),
	_TotalTimeout(InTotalTimeoutToGiveUp),
	_InitialBackoffDelay(InInitialBackoffDelay),
	_AggressiveRetryInterval(InAggressiveRetryInterval),
	_BalancedMaxRetryInterval(InBalancedMaxRetryInterval),
	StrategyType(_StrategyType),
	TotalTimeout(_TotalTimeout),
	InitialBackoffDelay(_InitialBackoffDelay),
	AggressiveRetryInterval(_AggressiveRetryInterval),
	BalancedMaxRetryInterval(_BalancedMaxRetryInterval)
{}

AccelByte::FReconnectionStrategy& AccelByte::FReconnectionStrategy::operator=(const FReconnectionStrategy& Copy){
	this->_StrategyType = Copy._StrategyType;
	this->_TotalTimeout = Copy._TotalTimeout;
	this->_InitialBackoffDelay = Copy._InitialBackoffDelay;
	this->_AggressiveRetryInterval = Copy._AggressiveRetryInterval;
	this->_BalancedMaxRetryInterval = Copy._BalancedMaxRetryInterval;
	this->Advanced_MassiveOutageDuration = Copy.Advanced_MassiveOutageDuration;
	this->Advanced_RetryBackoffBaseFactor = Copy.Advanced_RetryBackoffBaseFactor;
	return *this;
}

AccelByte::FReconnectionStrategy& AccelByte::IWebsocketConfigurableReconnectStrategy::GetDefaultReconnectionStrategy()
{
	return DefaultReconnectionStrategy;
}

TMap<int32, AccelByte::FReconnectionStrategy>& AccelByte::IWebsocketConfigurableReconnectStrategy::GetReconnectionStrategyDictionary()
{
	return ReconnectionStrategyDictionary;
}

void AccelByte::IWebsocketConfigurableReconnectStrategy::SetDefaultReconnectionStrategy(const FReconnectionStrategy& Strategy)
{
	DefaultReconnectionStrategy = Strategy;
}

void AccelByte::IWebsocketConfigurableReconnectStrategy::SetSpecificReconnectionStrategy(EWebsocketClosureCodeForSpecificRetry WebsocketDisconnectionCode, const FReconnectionStrategy& Strategy)
{
	ReconnectionStrategyDictionary.Add((int32)WebsocketDisconnectionCode, Strategy);
}

void AccelByte::IWebsocketConfigurableReconnectStrategy::SetSpecificReconnectionStrategy(int32 RawWebsocketConnectionCode, const FReconnectionStrategy& Strategy)
{
	ReconnectionStrategyDictionary.Add(RawWebsocketConnectionCode, Strategy);
}