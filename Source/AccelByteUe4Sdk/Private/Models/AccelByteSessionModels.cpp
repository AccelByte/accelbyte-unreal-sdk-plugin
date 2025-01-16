// Copyright (c) 2022 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "Models/AccelByteSessionModels.h"
#include "Json.h"
#include "Core/AccelByteReport.h"

FAccelByteModelsV2GameSessionQuery::FAccelByteModelsV2GameSessionQuery()
{
	JsonWrapper.JsonObject = MakeShared<FJsonObject>();
};

// Macro to reduce code duplication for setting query fields in the JSON wrapper
#define SET_SESSION_QUERY_FIELD(JsonType, Value)                                                                       \
	FString ComparisonFieldName;                                                                                       \
	const TSharedPtr<FJsonObject> ComparisonField = InitializeComparisonField(Comparison, ComparisonFieldName);        \
	ComparisonField->Set##JsonType##Field(AttributeName, Value);                                                       \
	AddToQuery(ComparisonFieldName, ComparisonField);                                                                  \

void FAccelByteModelsV2GameSessionQuery::AddParam(const FString& AttributeName
	, const EAccelByteV2SessionQueryComparisonOp Comparison
	, const FString& AttributeValue) const
{
	if(Comparison != EAccelByteV2SessionQueryComparisonOp::EQUAL &&
		Comparison != EAccelByteV2SessionQueryComparisonOp::NOT_EQUAL)
	{
		UE_LOG(LogAccelByte, Warning, TEXT("Invalid session query operator/type combination!"));
		return;
	}

	SET_SESSION_QUERY_FIELD(String, AttributeValue);
}

void FAccelByteModelsV2GameSessionQuery::AddParam(const FString& AttributeName
	, const EAccelByteV2SessionQueryComparisonOp Comparison
	, const double AttributeValue) const
{
	if(Comparison != EAccelByteV2SessionQueryComparisonOp::EQUAL &&
		Comparison != EAccelByteV2SessionQueryComparisonOp::NOT_EQUAL &&
		Comparison != EAccelByteV2SessionQueryComparisonOp::GREATER_THAN &&
		Comparison != EAccelByteV2SessionQueryComparisonOp::GREATER_THAN_EQUAL &&
		Comparison != EAccelByteV2SessionQueryComparisonOp::LESS_THAN &&
		Comparison != EAccelByteV2SessionQueryComparisonOp::LESS_THAN_EQUAL)
	{
		UE_LOG(LogAccelByte, Warning, TEXT("Invalid session query operator/type combination!"));
		return;
	}

	SET_SESSION_QUERY_FIELD(Number, AttributeValue);
}

void FAccelByteModelsV2GameSessionQuery::AddParam(const FString& AttributeName
	, const EAccelByteV2SessionQueryComparisonOp Comparison
	, const TArray<FString>& AttributeValue) const
{
	if(Comparison != EAccelByteV2SessionQueryComparisonOp::CONTAINS &&
		Comparison != EAccelByteV2SessionQueryComparisonOp::NOT_CONTAINS)
	{
		UE_LOG(LogAccelByte, Warning, TEXT("Invalid session query operator/type combination!"));
		return;
	}

	TArray<TSharedPtr<FJsonValue>> JsonArray;
	for(const FString& Item : AttributeValue)
	{
		JsonArray.Add(MakeShared<FJsonValueString>(Item));
	}

	SET_SESSION_QUERY_FIELD(Array, JsonArray);
}

void FAccelByteModelsV2GameSessionQuery::AddParam(const FString& AttributeName
	, const EAccelByteV2SessionQueryComparisonOp Comparison
	, const TArray<double>& AttributeValue) const
{
	if (Comparison != EAccelByteV2SessionQueryComparisonOp::CONTAINS &&
		Comparison != EAccelByteV2SessionQueryComparisonOp::NOT_CONTAINS)
	{
		UE_LOG(LogAccelByte, Warning, TEXT("Invalid session query operator/type combination!"));
		return;
	}

	TArray<TSharedPtr<FJsonValue>> JsonArray;
	for(const double Item : AttributeValue)
	{
		JsonArray.Add(MakeShared<FJsonValueNumber>(Item));
	}

	SET_SESSION_QUERY_FIELD(Array, JsonArray);
}

#undef SET_SESSION_QUERY_FIELD

TSharedPtr<FJsonObject> FAccelByteModelsV2GameSessionQuery::InitializeComparisonField(const EAccelByteV2SessionQueryComparisonOp Op
	, FString& OutFieldName) const
{
	if(!JsonWrapper.JsonObject->HasTypedField<EJson::Object>(TEXT("query")))
	{
		JsonWrapper.JsonObject->SetObjectField(TEXT("query"), MakeShared<FJsonObject>());
	}

	const TSharedPtr<FJsonObject> QueryObject = JsonWrapper.JsonObject->GetObjectField(TEXT("query"));

	switch(Op)
	{
	case EAccelByteV2SessionQueryComparisonOp::EQUAL:
		OutFieldName = TEXT("equals");
		break;
	case EAccelByteV2SessionQueryComparisonOp::NOT_EQUAL:
		OutFieldName = TEXT("notEquals");
		break;
	case EAccelByteV2SessionQueryComparisonOp::GREATER_THAN:
		OutFieldName = TEXT("greaterThan");
		break;
	case EAccelByteV2SessionQueryComparisonOp::GREATER_THAN_EQUAL:
		OutFieldName = TEXT("greaterThanEquals");
		break;
	case EAccelByteV2SessionQueryComparisonOp::LESS_THAN:
		OutFieldName = TEXT("lessThan");
		break;
	case EAccelByteV2SessionQueryComparisonOp::LESS_THAN_EQUAL:
		OutFieldName = TEXT("lessThanEquals");
		break;
	case EAccelByteV2SessionQueryComparisonOp::CONTAINS:
		OutFieldName = TEXT("in");
		break;
	case EAccelByteV2SessionQueryComparisonOp::NOT_CONTAINS:
		OutFieldName = TEXT("notIn");
		break;
	default: break;
	}

	if(!QueryObject->HasTypedField<EJson::Object>(OutFieldName))
	{
		QueryObject->SetObjectField(OutFieldName, MakeShared<FJsonObject>());
	}

	return QueryObject->GetObjectField(OutFieldName);
}

void FAccelByteModelsV2GameSessionQuery::AddToQuery(const FString& ComparisonFieldName
	, const TSharedPtr<FJsonObject>& ComparisonField) const
{
	// #NOTE: The AddToQuery method should only ever be called after InitializeComparisonField has been called, but
	// putting this here just in case something changes in the future
	if(!JsonWrapper.JsonObject->HasTypedField<EJson::Object>(TEXT("query")))
	{
		JsonWrapper.JsonObject->SetObjectField(TEXT("query"), MakeShared<FJsonObject>());
	}

	const TSharedPtr<FJsonObject> QueryObject =	JsonWrapper.JsonObject->GetObjectField(TEXT("query"));
	QueryObject->SetObjectField(ComparisonFieldName, ComparisonField);

	JsonWrapper.JsonObject->SetObjectField(TEXT("query"), QueryObject);
}

FAccelByteModelsGameSessionExcludedSession::FAccelByteModelsGameSessionExcludedSession(ExclusionType Type)
	: CurrentType(this->_CurrentType)
	, ExcludedPastSessionCount(this->_ExcludedPastSessionCount)
{
	_CurrentType = Type;
}

FAccelByteModelsGameSessionExcludedSession FAccelByteModelsGameSessionExcludedSession::CreateNoExclusion()
{
	return FAccelByteModelsGameSessionExcludedSession(ExclusionType::NONE);
}

FAccelByteModelsGameSessionExcludedSession FAccelByteModelsGameSessionExcludedSession::CreateExclusionCount(uint32 ExcludedPastSessionCount) 
{
	auto Output = FAccelByteModelsGameSessionExcludedSession(ExclusionType::N_PAST_SESSION);
	Output._ExcludedPastSessionCount = ExcludedPastSessionCount;
	return Output;
}

FAccelByteModelsGameSessionExcludedSession FAccelByteModelsGameSessionExcludedSession::CreateExclusionList(const TSet<FString>& ExcludedGameSessionIDs) 
{
	auto Output = FAccelByteModelsGameSessionExcludedSession(ExclusionType::EXPLICIT_LIST);
	Output._ExcludedGameSessionIDs = ExcludedGameSessionIDs;
	return Output;
}

FAccelByteModelsGameSessionExcludedSession FAccelByteModelsGameSessionExcludedSession::CreateExclusionEntireSessionMemberPastSession()
{
	return FAccelByteModelsGameSessionExcludedSession(ExclusionType::ALL_MEMBER_CACHED_SESSION);
}

FAccelByteModelsGameSessionExcludedSession& FAccelByteModelsGameSessionExcludedSession::operator=(const FAccelByteModelsGameSessionExcludedSession& Copy)
{
	this->_CurrentType = Copy._CurrentType;
	this->_ExcludedPastSessionCount = Copy._ExcludedPastSessionCount;
	this->_ExcludedGameSessionIDs = Copy._ExcludedGameSessionIDs;
	return *this;
}
