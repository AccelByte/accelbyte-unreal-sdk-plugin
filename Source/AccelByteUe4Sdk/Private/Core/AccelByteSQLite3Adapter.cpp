// Copyright (c) 2022 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "Core/AccelByteSQLite3Adapter.h"

#ifdef SQLITE3_ENABLED
namespace AccelByte
{
	bool AccelByteSQLite3Adapter::OpenConnection(const FString& DBFileName)
	{
		if (USQLiteDatabase::IsValidDatabase(DBFileName, true))
		{
			if (USQLiteDatabase::IsDatabaseRegistered(DatabaseName))
			{
				return true;
			}
			else
			{
				return USQLiteDatabase::RegisterDatabase(DatabaseName, DBFileName, true);
			}
		}
		else
		{
			USQLiteDatabase::CreateDatabase(DBFileName, true);
			return USQLiteDatabase::RegisterDatabase(DatabaseName, DBFileName, true);
		}
	}

	AccelByteSQLite3Adapter::AccelByteSQLite3Adapter(const FString& DatabaseName)
		:IAccelByteDBAdapter(DatabaseName)
		, DatabaseName{ DatabaseName }
	{
		OpenConnection(FString::Printf(TEXT("%s.db"), *DatabaseName));
	}

	void AccelByteSQLite3Adapter::CreateTable(const FString& TableName, UScriptStruct* ScriptStruct, const THandler<bool>& Result)
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
			// Get Struct's first member
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

	void AccelByteSQLite3Adapter::Reset(const THandler<bool>& Result, const FString& TableName)
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

	void AccelByteSQLite3Adapter::DeleteItem(const FString& Key, const FVoidHandler OnDone, const FString& TableName)
	{
		const FString Condition = FString::Printf(TEXT("Key = '\"%s\"'"), *Key);
		USQLiteDatabase::DeleteRowsInTable(DatabaseName, TableName, FSQLiteQueryFinalizedQuery{ Condition });
		OnDone.ExecuteIfBound();
	}

	void AccelByteSQLite3Adapter::CreateKeyValuePairTable(const THandler<bool>& Result, const FString& TableName)
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

	void AccelByteSQLite3Adapter::SaveItem(const FString& Key, const TArray<uint8>& DataToInsert, const THandler<bool>& OnDone, const FString& TableName)
	{
		CreateKeyValuePairTable(THandler<bool>::CreateLambda([this, OnDone, Key, DataToInsert, TableName](bool Result)
			{
				if (Result)
				{
					FSQLiteTableRowSimulator Row;
					FSQLiteTableField KeyField;
					FSQLiteTableField ValueField;

					KeyField.FieldName = TEXT("'Key'");
					KeyField.FieldValue = FString::Printf(TEXT("\"%s\""), *Key);

					ValueField.FieldName = TEXT("'Value'");
					ValueField.FieldType = TEXT("BLOB");
					ValueField.FieldBlobValue = DataToInsert;

					Row.rowsOfFields.Add(KeyField);
					Row.rowsOfFields.Add(ValueField);

					USQLiteDatabase::InsertRowsIntoTableUpsert(DatabaseName, TableName, TArray<FSQLiteTableRowSimulator>{ { Row }}, TEXT("Key"));
					OnDone.ExecuteIfBound(true);
				}
				else
				{
					OnDone.ExecuteIfBound(Result);
				}
			}), TableName);
	}

	void AccelByteSQLite3Adapter::SaveItem(const FString& Key, const FString& DataToInsert, const THandler<bool>& OnDone, const FString& TableName)
	{
		CreateKeyValuePairTable(THandler<bool>::CreateLambda([this, OnDone, Key, DataToInsert, TableName](bool Result)
			{
				if (Result)
				{
					FSQLiteTableRowSimulator Row;
					FSQLiteTableField KeyField;
					FSQLiteTableField ValueField;

					KeyField.FieldName = TEXT("'Key'");
					KeyField.FieldType = TEXT("TEXT");
					KeyField.FieldValue = FString::Printf(TEXT("%s"), *Key);

					ValueField.FieldName = TEXT("'Value'");
					ValueField.FieldType = TEXT("TEXT");
					ValueField.FieldValue = DataToInsert;

					Row.rowsOfFields.Add(KeyField);
					Row.rowsOfFields.Add(ValueField);

					USQLiteDatabase::InsertRowsIntoTableUpsert(DatabaseName, TableName, TArray<FSQLiteTableRowSimulator>{ { Row }}, TEXT("Key"));
					OnDone.ExecuteIfBound(true);
				}
				else
				{
					OnDone.ExecuteIfBound(Result);
				}
			}), TableName);
	}

	void AccelByteSQLite3Adapter::SaveItem(const FString& Key, const FJsonObjectWrapper& DataToInsert, const THandler<bool>& OnDone, const FString& TableName)
	{
		CreateKeyValuePairTable(THandler<bool>::CreateLambda([this, OnDone, Key, DataToInsert, TableName](bool Result)
			{
				if (Result)
				{
					FSQLiteTableRowSimulator Row;
					FSQLiteTableField KeyField;
					FSQLiteTableField ValueField;

					KeyField.FieldName = TEXT("'Key'");
					KeyField.FieldType = TEXT("TEXT");
					KeyField.FieldValue = FString::Printf(TEXT("%s"), *Key);

					ValueField.FieldName = TEXT("'Value'");
					ValueField.FieldType = TEXT("TEXT");
					DataToInsert.JsonObjectToString(ValueField.FieldValue);

					Row.rowsOfFields.Add(KeyField);
					Row.rowsOfFields.Add(ValueField);

					USQLiteDatabase::InsertRowsIntoTableUpsert(DatabaseName, TableName, TArray<FSQLiteTableRowSimulator>{ { Row }}, TEXT("Key"));
					OnDone.ExecuteIfBound(true);
				}
				else
				{
					OnDone.ExecuteIfBound(Result);
				}
			}), TableName);
	}

	void AccelByteSQLite3Adapter::GetItem(const FString& Key, const THandler<TPair<FString, TArray<uint8>>>& OnDone, const FString& TableName)
	{
		FSQLiteDatabaseReference DBRef{ DatabaseName, {TableName} };
		const FString Condition = FString::Printf(TEXT("Key = '%s'"), *Key);

		FAutoDeleteAsyncTask<DBQueryTask<FSQLiteQueryResult>>* MyTask = new FAutoDeleteAsyncTask<DBQueryTask<FSQLiteQueryResult>>(MakeShared<FSQLiteQueryResult>(USQLiteDatabase::GetDataBP(
			DBRef,
			{ "*" },
			FSQLiteQueryFinalizedQuery{ Condition })),
			THandler<FSQLiteQueryResult>::CreateLambda([this, Key, OnDone](FSQLiteQueryResult Result)
				{
					if (Result.Success)
					{
						TPair<FString, TArray<uint8>> Data{};
						for (int i = 0; i < Result.ResultRows.Num(); i++)
						{
							if (Result.ResultRows[i].Fields[0].Key == TEXT("Key"))
							{
								if (Result.ResultRows[i].Fields[0].Value.TrimQuotes() == Key)
								{
									Data = TPair<FString, TArray<uint8>>{ Key, Result.ResultRows[i].Fields[1].ByteValue };
									break;
								}
								else
								{
									continue;
								}
							}
						}
						OnDone.ExecuteIfBound(Data);
					}
					else
					{
						OnDone.ExecuteIfBound(TPair<FString, TArray<uint8>>{});
					}
				}));
		MyTask->StartBackgroundTask();
	}

	void AccelByteSQLite3Adapter::GetItem(const FString& Key, const THandler<TPair<FString, FString>>& OnDone, const FString& TableName)
	{
		FSQLiteDatabaseReference DBRef{ DatabaseName, {TableName} };
		const FString Condition = FString::Printf(TEXT("Key = '%s'"), *Key);

		FAutoDeleteAsyncTask<DBQueryTask<FSQLiteQueryResult>>* MyTask = new FAutoDeleteAsyncTask<DBQueryTask<FSQLiteQueryResult>>(MakeShared<FSQLiteQueryResult>(USQLiteDatabase::GetDataBP(
			DBRef,
			{ "*" },
			FSQLiteQueryFinalizedQuery{ Condition })),
			THandler<FSQLiteQueryResult>::CreateLambda([this, Key, OnDone](FSQLiteQueryResult Result)
				{
					if (Result.Success)
					{
						TPair<FString, FString> Data{};
						for (int i = 0; i < Result.ResultRows.Num(); i++)
						{
							if (Result.ResultRows[i].Fields[0].Key == TEXT("Key"))
							{
								if (Result.ResultRows[i].Fields[0].Value.TrimQuotes() == Key)
								{
									Data = TPair<FString, FString>{ Key, Result.ResultRows[i].Fields[1].Value };
									break;
								}
								else
								{
									continue;
								}
							}
						}
						OnDone.ExecuteIfBound(Data);
					}
					else
					{
						OnDone.ExecuteIfBound(TPair<FString, FString>{});
					}
				}));
		MyTask->StartBackgroundTask();
	}

	void AccelByteSQLite3Adapter::GetItem(const FString& Key, const THandler<TPair<FString, FJsonObjectWrapper>>& OnDone, const FString& TableName)
	{
		FSQLiteDatabaseReference DBRef{ DatabaseName, {TableName} };
		const FString Condition = FString::Printf(TEXT("Key = '%s'"), *Key);

		FAutoDeleteAsyncTask<DBQueryTask<FSQLiteQueryResult>>* MyTask = new FAutoDeleteAsyncTask<DBQueryTask<FSQLiteQueryResult>>(MakeShared<FSQLiteQueryResult>(USQLiteDatabase::GetDataBP(
			DBRef,
			{ "*" },
			FSQLiteQueryFinalizedQuery{ Condition })),
			THandler<FSQLiteQueryResult>::CreateLambda([this, Key, OnDone](FSQLiteQueryResult Result)
				{
					if (Result.Success)
					{
						TPair<FString, FJsonObjectWrapper> Data{};
						for (int i = 0; i < Result.ResultRows.Num(); i++)
						{
							if (Result.ResultRows[i].Fields[0].Key == TEXT("Key"))
							{
								if (Result.ResultRows[i].Fields[0].Value.TrimQuotes() == Key)
								{
									const FString JsonString = Result.ResultRows[i].Fields[1].Value; // An FString containing a serialized JSON 
									FJsonObjectWrapper JsonObjectWrapper;

									if (JsonObjectWrapper.JsonObjectFromString(JsonString))
									{
										Data = TPair<FString, FJsonObjectWrapper>{ Key, JsonObjectWrapper };
									}
									break;
								}
								else
								{
									continue;
								}
							}
						}
						OnDone.ExecuteIfBound(Data);
					}
					else
					{
						OnDone.ExecuteIfBound(TPair<FString, FJsonObjectWrapper>{});
					}
				}));
		MyTask->StartBackgroundTask();
	}

	const FString AccelByteSQLite3Adapter::ClassFieldToSQLiteDataStruct(const FName FieldName)
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

	const FString AccelByteSQLite3Adapter::JsonObjectToSQLiteValue(const TSharedPtr<FJsonObject>& JsonObject, const FString& FieldName, const FString& FieldType)
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
			else
			{
				return TEXT("0");
			}
		}
		if (FieldType == TEXT("BOOLEAN"))
		{
			bool bValue = false;
			if (JsonObject->TryGetBoolField(FieldName, bValue))
			{
				return FString::Printf(TEXT("%d"), bValue);
			}
			else
			{
				return TEXT("0");
			}
		}
		if (FieldType == TEXT("FLOAT")
			|| FieldType == TEXT("DOUBLE"))
		{
			double fValue = 0;
			if (JsonObject->TryGetNumberField(FieldName, fValue))
			{
				return FString::Printf(TEXT("%lf"), fValue);
			}
			else
			{
				return TEXT("0");
			}
		}
		return TEXT("0");
	}

	const FSQLiteTableField AccelByteSQLite3Adapter::JsonObjectToTableField(const TSharedPtr<FJsonObject>& JsonObject, const FString& FieldName, const FString& FieldType)
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
		else if (JsonObject->HasField(FieldName))
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