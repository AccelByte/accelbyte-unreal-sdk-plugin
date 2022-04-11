// Copyright (c) 2022 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "Core/AccelByteSQLite3.h"

#ifdef SQLITE3_ENABLED
namespace AccelByte
{
	bool FAccelByteSQLite3::OpenConnection(const FString& DBFileName)
	{
		if (USQLiteDatabase::IsValidDatabase(DBFileName, true))
		{
			if (USQLiteDatabase::IsDatabaseRegistered(DatabaseName))
			{
				return true;
			}
		}
		else
		{
			USQLiteDatabase::CreateDatabase(DBFileName, true);
		}
		
		return USQLiteDatabase::RegisterDatabase(DatabaseName, DBFileName, true);
	}

	FAccelByteSQLite3::FAccelByteSQLite3(const FString& InDatabaseName)
		: DatabaseName{InDatabaseName}
	{
		OpenConnection(FString::Printf(TEXT("%s.db"), *DatabaseName));
	}

	void FAccelByteSQLite3::CreateTable(const FString& TableName, UScriptStruct* ScriptStruct, const THandler<bool>& Result)
	{
		if (USQLiteDatabase::IsTableExists(DatabaseName, TableName))
		{
			RegisteredTable.Add(TableName);
			Result.ExecuteIfBound(true);
		}
		else
		{
			TArray<FSQLiteTableField> TableFields;
			FSQLiteTableField KeyField;

			KeyField.ResultStr = FString::Printf(TEXT("'Key' TEXT UNIQUE NOT NULL PRIMARY KEY"));

			TableFields.Add(KeyField);
			// Get first member in the Struct
			FField* Field = ScriptStruct->ChildProperties;
			while (Field)
			{
				FSQLiteTableField TableField;
				// Get Blueprint name
				FString FieldName = Field->GetName();

				TableField.ResultStr = FString::Printf(TEXT("'%s' %s"), *FieldName, *ClassFieldToSQLiteDataStruct(Field->GetClass()->GetFName()));

				TableFields.Add(TableField);

				// Go to next member
				Field = Field->Next;
			}

			FAutoDeleteAsyncTask<DBQueryTask<FSQLiteTable>>* MyTask = new FAutoDeleteAsyncTask<DBQueryTask<FSQLiteTable>>(MakeShared<FSQLiteTable>(USQLiteDatabase::CreateTable(
				DatabaseName,
				TableName,
				TableFields,
				FSQLitePrimaryKey{})),
				THandler<FSQLiteTable>::CreateLambda([this, TableName, Result](const FSQLiteTable& QueryResult)
					{
						RegisteredTable.Add(TableName);
						Result.ExecuteIfBound(QueryResult.Created);
					}));
			MyTask->StartBackgroundTask();
		}
	}

	void FAccelByteSQLite3::Reset(const THandler<bool>& Result, const FString& TableName)
	{
		FAutoDeleteAsyncTask<DBQueryTask<bool>>* MyTask = new FAutoDeleteAsyncTask<DBQueryTask<bool>>(MakeShared<bool>(USQLiteDatabase::DropTable(
			DatabaseName,
			TableName)),
			THandler<bool>::CreateLambda([Result](bool QueryResult)
				{
					Result.ExecuteIfBound(QueryResult);
				}));
		MyTask->StartBackgroundTask();
	}

	void FAccelByteSQLite3::DeleteItem(const FString& Key, const FVoidHandler OnDone, const FString& TableName)
	{
		const FString Condition = FString::Printf(TEXT("Key = '\"%s\"'"), *Key);
		USQLiteDatabase::DeleteRowsInTable(DatabaseName, TableName, FSQLiteQueryFinalizedQuery{ Condition });
		OnDone.ExecuteIfBound();
	}

	void FAccelByteSQLite3::CreateKeyValuePairTable(const THandler<bool>& Result, const FString& TableName)
	{
		if (USQLiteDatabase::IsTableExists(DatabaseName, TableName))
		{
			RegisteredTable.Add(TableName);
			Result.ExecuteIfBound(true);
		}
		else
		{
			TArray<FSQLiteTableField> TableFields;
			FSQLiteTableField KeyField;
			FSQLiteTableField ValueFIeld;

			KeyField.ResultStr = FString::Printf(TEXT("'Key' TEXT UNIQUE NOT NULL PRIMARY KEY"));
			ValueFIeld.ResultStr = FString::Printf(TEXT("'Value' BLOB"));

			TableFields.Add(KeyField);
			TableFields.Add(ValueFIeld);

			FAutoDeleteAsyncTask<DBQueryTask<FSQLiteTable>>* MyTask = new FAutoDeleteAsyncTask<DBQueryTask<FSQLiteTable>>(MakeShared<FSQLiteTable>(USQLiteDatabase::CreateTable(
				DatabaseName,
				TableName,
				TableFields,
				FSQLitePrimaryKey{})),
				THandler<FSQLiteTable>::CreateLambda([this, Result, TableName](const FSQLiteTable& QueryResult)
					{
						RegisteredTable.Add(TableName);
						Result.ExecuteIfBound(QueryResult.Created);
					}));
			MyTask->StartBackgroundTask();
		}
	}

	void FAccelByteSQLite3::ExecuteSaveCommand(const FString& Key, const FSQLiteTableField& Value, const THandler<bool>& OnDone, const FString& TableName)
	{
		CreateKeyValuePairTable(THandler<bool>::CreateLambda([this, OnDone, Key, Value, TableName](bool Result)
			{
				if (Result)
				{
					FSQLiteTableRowSimulator Row;
					FSQLiteTableField KeyField;

					KeyField.FieldName = TEXT("'Key'");
					KeyField.FieldValue = FString::Printf(TEXT("\"%s\""), *Key);

					Row.rowsOfFields.Add(KeyField);
					Row.rowsOfFields.Add(Value);

					USQLiteDatabase::InsertRowsIntoTableUpsert(DatabaseName, TableName, TArray<FSQLiteTableRowSimulator>{ { Row }}, TEXT("Key"));
				}
				OnDone.ExecuteIfBound(Result);
			}), TableName);
	}

	void FAccelByteSQLite3::SaveItem(const FString& Key, const TArray<uint8>& DataToInsert, const THandler<bool>& OnDone, const FString& TableName)
	{
		FSQLiteTableField ValueField;
		
		ValueField.FieldName = TEXT("'Value'");
		ValueField.FieldType = TEXT("BLOB");
		ValueField.FieldBlobValue = DataToInsert;

		ExecuteSaveCommand(Key, ValueField, OnDone, TableName);
	}

	void FAccelByteSQLite3::SaveItem(const FString& Key, const FString& DataToInsert, const THandler<bool>& OnDone, const FString& TableName)
	{
		FSQLiteTableField ValueField;

		ValueField.FieldName = TEXT("'Value'");
		ValueField.FieldType = TEXT("TEXT");
		ValueField.FieldValue = DataToInsert;

		ExecuteSaveCommand(Key, ValueField, OnDone, TableName);
	}

	void FAccelByteSQLite3::SaveItem(const FString& Key, const FJsonObjectWrapper& DataToInsert, const THandler<bool>& OnDone, const FString& TableName)
	{
		FSQLiteTableField ValueField;

		ValueField.FieldName = TEXT("'Value'");
		ValueField.FieldType = TEXT("TEXT");
		DataToInsert.JsonObjectToString(ValueField.FieldValue);

		ExecuteSaveCommand(Key, ValueField, OnDone, TableName);
	}

	FSQLiteKeyValuePair FAccelByteSQLite3::FindItemWithKey(const FString& Key, const FSQLiteQueryResult& Data)
	{
		FSQLiteKeyValuePair Result;
		
		for (const auto& Row : Data.ResultRows)
		{
			if (Row.Fields[0].Key == TEXT("Key") && Row.Fields[0].Value.TrimQuotes() == Key)
			{
				Result = Row.Fields[1];
				break;
			}
		}

		return Result;
	}

	void FAccelByteSQLite3::GetItem(const FString& Key, const THandler<TPair<FString, TArray<uint8>>>& OnDone, const FString& TableName)
	{
		FSQLiteDatabaseReference DBRef{ DatabaseName, {TableName} };
		const FString Condition = FString::Printf(TEXT("Key = '%s'"), *Key);

		FAutoDeleteAsyncTask<DBQueryTask<FSQLiteQueryResult>>* MyTask = new FAutoDeleteAsyncTask<DBQueryTask<FSQLiteQueryResult>>(MakeShared<FSQLiteQueryResult>(USQLiteDatabase::GetDataBP(
			DBRef,
			{ "*" },
			FSQLiteQueryFinalizedQuery{ Condition })),
			THandler<FSQLiteQueryResult>::CreateLambda([this, Key, OnDone](FSQLiteQueryResult Result)
				{
					TPair<FString, TArray<uint8>> Data{};
					if (Result.Success && Result.ResultRows.Num() > 0)
					{
						FSQLiteKeyValuePair Pair = FindItemWithKey(Key, Result);
						Data = TPair<FString, TArray<uint8>>{ Key, Pair.ByteValue };
					}
					OnDone.ExecuteIfBound(Data);
				}));
		MyTask->StartBackgroundTask();
	}

	void FAccelByteSQLite3::GetItem(const FString& Key, const THandler<TPair<FString, FString>>& OnDone, const FString& TableName)
	{
		FSQLiteDatabaseReference DBRef{ DatabaseName, {TableName} };
		const FString Condition = FString::Printf(TEXT("Key = '%s'"), *Key);

		FAutoDeleteAsyncTask<DBQueryTask<FSQLiteQueryResult>>* MyTask = new FAutoDeleteAsyncTask<DBQueryTask<FSQLiteQueryResult>>(MakeShared<FSQLiteQueryResult>(USQLiteDatabase::GetDataBP(
			DBRef,
			{ "*" },
			FSQLiteQueryFinalizedQuery{ Condition })),
			THandler<FSQLiteQueryResult>::CreateLambda([this, Key, OnDone](FSQLiteQueryResult Result)
				{
					TPair<FString, FString> Data{};
					if (Result.Success && Result.ResultRows.Num() > 0)
					{
						FSQLiteKeyValuePair Pair = FindItemWithKey(Key, Result);
						Data = TPair<FString, FString>{ Key, Pair.Value };
					}
					OnDone.ExecuteIfBound(Data);
				}));
		MyTask->StartBackgroundTask();
	}

	void FAccelByteSQLite3::GetItem(const FString& Key, const THandler<TPair<FString, FJsonObjectWrapper>>& OnDone, const FString& TableName)
	{
		FSQLiteDatabaseReference DBRef{ DatabaseName, {TableName} };
		const FString Condition = FString::Printf(TEXT("Key = '%s'"), *Key);

		FAutoDeleteAsyncTask<DBQueryTask<FSQLiteQueryResult>>* MyTask = new FAutoDeleteAsyncTask<DBQueryTask<FSQLiteQueryResult>>(MakeShared<FSQLiteQueryResult>(USQLiteDatabase::GetDataBP(
			DBRef,
			{ "*" },
			FSQLiteQueryFinalizedQuery{ Condition })),
			THandler<FSQLiteQueryResult>::CreateLambda([this, Key, OnDone](FSQLiteQueryResult Result)
				{
					TPair<FString, FJsonObjectWrapper> Data{};
					if (Result.Success && Result.ResultRows.Num() > 0)
					{
						FSQLiteKeyValuePair Pair = FindItemWithKey(Key, Result);
						const FString JsonString = Pair.Value; // An FString containing a serialized JSON 
						FJsonObjectWrapper JsonObjectWrapper;

						if (JsonObjectWrapper.JsonObjectFromString(JsonString))
						{
							Data = TPair<FString, FJsonObjectWrapper>{ Key, JsonObjectWrapper };
						}
					}
					OnDone.ExecuteIfBound(Data);
				}));
		MyTask->StartBackgroundTask();
	}

	const FString FAccelByteSQLite3::ClassFieldToSQLiteDataStruct(const FName& FieldName)
	{
		if (FieldName.GetPlainNameString() == TEXT("NumericProperty")) return TEXT("NUMERIC");
		if (FieldName.GetPlainNameString() == TEXT("Int8Property")) return TEXT("SMALLINT");
		if (FieldName.GetPlainNameString() == TEXT("Int16Property")) return TEXT("MEDIUMINT");
		if (FieldName.GetPlainNameString() == TEXT("UInt16Property")) return TEXT("UNSIGNED MEDIUMINT");
		if (FieldName.GetPlainNameString() == TEXT("IntProperty")) return TEXT("INT");
		if (FieldName.GetPlainNameString() == TEXT("UInt32Property")) return TEXT("UNSIGNED INT");
		if (FieldName.GetPlainNameString() == TEXT("Int64Property")) return TEXT("BIGINT");
		if (FieldName.GetPlainNameString() == TEXT("UInt64Property")) return TEXT("UNSIGNED BIGINT");
		if (FieldName.GetPlainNameString() == TEXT("BoolProperty")) return TEXT("BOOLEAN");
		if (FieldName.GetPlainNameString() == TEXT("FloatProperty")) return TEXT("FLOAT");
		if (FieldName.GetPlainNameString() == TEXT("DoubleProperty")) return TEXT("DOUBLE");
		if (FieldName.GetPlainNameString() == TEXT("ByteProperty")) return TEXT("BLOB");
		return TEXT("TEXT");
	}

	const FString FAccelByteSQLite3::JsonObjectToSQLiteValue(const TSharedPtr<FJsonObject>& JsonObject, const FString& FieldName, const FString& FieldType)
	{
		if (FieldType == TEXT("NUMERIC")
			|| FieldType == TEXT("SMALLINT")
			|| FieldType == TEXT("MEDIUMINT")
			|| FieldType == TEXT("UNSIGNED MEDIUMINT")
			|| FieldType == TEXT("INT")
			|| FieldType == TEXT("UNSIGNED INT")
			|| FieldType == TEXT("BIGINT")
			|| FieldType == TEXT("UNSIGNED BIGINT"))
		{
			int64 NumValue = 0;
			if (JsonObject->TryGetNumberField(FieldName, NumValue))
			{
				return FString::Printf(TEXT("%d"), NumValue);
			}
		}
		else if (FieldType == TEXT("BOOLEAN"))
		{
			bool bValue = false;
			if (JsonObject->TryGetBoolField(FieldName, bValue))
			{
				return FString::Printf(TEXT("%d"), bValue);
			}
		}
		else if (FieldType == TEXT("FLOAT")
			|| FieldType == TEXT("DOUBLE"))
		{
			double fValue = 0;
			if (JsonObject->TryGetNumberField(FieldName, fValue))
			{
				return FString::Printf(TEXT("%lf"), fValue);
			}
		}
		return TEXT("0");
	}

	const FSQLiteTableField FAccelByteSQLite3::JsonObjectToTableField(const TSharedPtr<FJsonObject>& JsonObject, const FString& FieldName, const FString& FieldType)
	{
		FSQLiteTableField Field;
		Field.FieldName = FString::Printf(TEXT("'%s'"), *FieldName);
		Field.FieldType = FieldType;
		
		FString StringValue;
		TSharedPtr<FJsonObject> const* ObjectValue;
		TArray<FString> ArrayStringValue;
		TArray<TSharedPtr<FJsonValue>> const* ArrayValue;
		if (FieldName.Equals("Key"))
		{
			return Field;
		}
		
		if (JsonObject->HasField(FieldName))
		{
			if (JsonObject->TryGetStringField(FieldName, StringValue))
			{
				Field.FieldValue = FString::Printf(TEXT("\"%s\""), *StringValue);
			}
			else if (JsonObject->TryGetObjectField(FieldName, ObjectValue))
			{
				TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&StringValue);
				FJsonSerializer::Serialize(ObjectValue->ToSharedRef(), Writer);
				Field.FieldValue = StringValue;
			}
			else if (JsonObject->TryGetStringArrayField(FieldName, ArrayStringValue))
			{
				StringValue = "[";
				for (int i = 0; i < ArrayStringValue.Num(); i++)
				{
					if (i != 0)
					{
						StringValue += TEXT(", ");
					}
					if (ArrayStringValue[i].IsNumeric())
					{
						StringValue += FString::Printf(TEXT("%s"), *ArrayStringValue[i]);
					}
					else
					{
						StringValue += FString::Printf(TEXT("\"%s\""), *ArrayStringValue[i]);
					}
				}
				StringValue += "]";
				Field.FieldValue = StringValue;
			}
			else if (JsonObject->TryGetArrayField(FieldName, ArrayValue))
			{
				TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&StringValue);
				FJsonSerializer::Serialize(*ArrayValue, Writer);
				Field.FieldValue = StringValue;
			}
			else
			{
				Field.FieldValue = JsonObjectToSQLiteValue(JsonObject, FieldName, Field.FieldType);
			}
			return Field;
		}
		return FSQLiteTableField{};
	}
}
#endif