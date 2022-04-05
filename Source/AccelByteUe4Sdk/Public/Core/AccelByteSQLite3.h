// Copyright (c) 2022 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.
#pragma once

#include "CoreMinimal.h"
#include "JsonUtilities.h"
#include "JsonObjectConverter.h"
#include "Core/AccelByteError.h"
#include "Core/IAccelByteDataStorage.h"
#ifdef SQLITE3_ENABLED
#include "SQLiteDatabase.h"

template<typename T>
class DBQueryTask : public FNonAbandonableTask
{
	friend class FAutoDeleteAsyncTask<DBQueryTask>;
	const TSharedPtr<T> Result;
	const AccelByte::THandler<T> OnDone;
	const double DelayTime;
	const double TotalTimeOut;

	DBQueryTask(const TSharedPtr<T>& Result, const AccelByte::THandler<T>& OnDone, double Delay = 5.0, double TimeOut = 60.0)
		: Result(Result)
		, OnDone(OnDone)
		, DelayTime(Delay)
		, TotalTimeOut(TimeOut)
	{}

	void DoWork()
	{
		const double TaskInitialTime = FPlatformTime::Seconds();

		while (!Result.IsValid() && ((FPlatformTime::Seconds() - TaskInitialTime) < TotalTimeOut))
		{
			FPlatformProcess::Sleep(DelayTime);
			UE_LOG(LogTemp, Warning, TEXT("Result: %d"), Result.IsValid());
		}
			
		OnDone.ExecuteIfBound(*Result.Get());
	}
	FORCEINLINE TStatId GetStatId() const { RETURN_QUICK_DECLARE_CYCLE_STAT(DBQueryTask, STATGROUP_ThreadPoolAsyncTasks); }

};

namespace AccelByte
{
	class ACCELBYTEUE4SDK_API FAccelByteSQLite3 : public IAccelByteDataStorage
	{
	public:
		FAccelByteSQLite3(const FString& InDatabaseName);
		virtual ~FAccelByteSQLite3() {}

		/**
		 * @brief Drop an existing table.
		 *
		 * @param Result This will be called when the operation done. The result is bool.
		 * @param TableName optional. The name of the table. Default will drop the default KeyValue table.
		*/
		virtual void Reset(const THandler<bool>& Result, const FString& TableName = TEXT("DefaultKeyValueTable")) override;

		/**
		 * @brief Insert an Item to a table, Override it if already exist.
		 *
		 * @param TableName The name of the table.
		 * @param Key The Key of the Item, this will act as the primary key, also will be needed when querying the Item.
		 * @param DataToInsert The data to be inserted to the table, is a UStruct
		 * @param OnDone This will be called when the operation done. The result is bool.
		*/
		template< typename T>
		void SaveItemToTable(const FString& TableName, const FString& Key, const T& DataToInsert, const THandler<bool>& OnDone);

		/**
		 * @brief Delete an Item on the table.
		 *
		 * @param Key The Key of the Item.
		 * @param OnDone This will be called when the operation done.
		 * @param TableName optional. The name of the table. Default will drop the default KeyValue table.
		*/
		virtual void DeleteItem(const FString& Key, const FVoidHandler OnDone, const FString& TableName = TEXT("DefaultKeyValueTable")) override;

		/**
		 * @brief Get Items from the table.
		 *
		 * @param TableName The name of the table.
		 * @param OnDone This will be called when the operation done. The result is TMap of UStruct.
		 * @param Limit the Items limit.
		 * @param Offset the Items offset on the table, put 0 to query from the first item.
		*/
		template< typename T>
		void GetItemsFromTable(const FString& TableName, const THandler<TMap<FString, T>>& OnDone, int Limit = -1, int Offset = 0);

		/**
		 * @brief Get an Item from the table.
		 *
		 * @param TableName The name of the table.
		 * @param Key The Key of the Item.
		 * @param OnDone This will be called when the operation done. The result is TPair of UStruct.
		*/
		template< typename T>
		void GetItemFromTable(const FString& TableName, const FString& Key, const THandler<TPair<FString, T>>& OnDone);

		/**
		 * @brief Insert Item to the Key Value Table, override it if already exist.
		 *
		 * @param Key The Key of the Item.
		 * @param DataToInsert The Data to be inserted to the Table. The Data would be an array of uint8.
		 * @param OnDone This will be called when the operation done. The result is bool.
		 * @param TableName optional. The name of the table. Default will insert an item to the default KeyValue table.
		*/
		virtual void SaveItem(const FString& Key, const TArray<uint8>& DataToInsert, const THandler<bool>& OnDone, const FString& TableName = TEXT("DefaultKeyValueTable")) override;

		/**
		 * @brief Insert Item to the Key Value Table, override it if already exist.
		 *
		 * @param Key The Key of the Item.
		 * @param DataToInsert The Data to be inserted to the Table. The Data would be a FString.
		 * @param OnDone This will be called when the operation done. The result is bool.
		 * @param TableName optional. The name of the table. Default will insert an item to the default KeyValue table.
		*/
		virtual void SaveItem(const FString& Key, const FString& DataToInsert, const THandler<bool>& OnDone, const FString& TableName = TEXT("DefaultKeyValueTable")) override;

		/**
		 * @brief Insert Item to the Key Value Table, override it if already exist.
		 *
		 * @param Key The Key of the Item.
		 * @param DataToInsert The Data to be inserted to the Table. The Data would be a FJsonObjectWrapper.
		 * @param OnDone This will be called when the operation done. The result is bool.
		 * @param TableName optional. The name of the table. Default will insert an item to the default KeyValue table.
		*/
		virtual void SaveItem(const FString& Key, const FJsonObjectWrapper& DataToInsert, const THandler<bool>& OnDone, const FString& TableName = TEXT("DefaultKeyValueTable")) override;

		/**
		 * @brief Get an Item from the Key Value Table.
		 *
		 * @param Key The Key of the Item to Get.
		 * @param OnDone This will be called when the operation done. The result is Pair of a FString Key, and an array of uint8 Value.
		 * @param TableName optional. The name of the table. Default will get an item from the default KeyValue table.
		*/
		virtual void GetItem(const FString& Key, const THandler<TPair<FString, TArray<uint8>>>& OnDone, const FString& TableName = TEXT("DefaultKeyValueTable")) override;

		/**
		 * @brief Get an Item from the Key Value Table.
		 *
		 * @param Key The Key of the Item to Get.
		 * @param OnDone This will be called when the operation done. The result is Pair of a FString Key, and a FString Value.
		 * @param TableName optional. The name of the table. Default will get an item from the default KeyValue table.
		*/
		virtual void GetItem(const FString& Key, const THandler<TPair<FString, FString>>& OnDone, const FString& TableName = TEXT("DefaultKeyValueTable")) override;

		/**
		 * @brief Get an Item from the Key Value Table.
		 *
		 * @param Key The Key of the Item to Get.
		 * @param OnDone This will be called when the operation done. The result is Pair of a FString Key, and a FJsonObjectWrapper Value.
		 * @param TableName optional. The name of the table. Default will get an item from the default KeyValue table.
		*/
		virtual void GetItem(const FString& Key, const THandler<TPair<FString, FJsonObjectWrapper>>& OnDone, const FString& TableName = TEXT("DefaultKeyValueTable")) override;

	private:
		FString DatabaseName;
		TArray<FString> RegisteredTable;

		const FString ClassFieldToSQLiteDataStruct(const FName& FieldClass);

		const FString JsonObjectToSQLiteValue(const TSharedPtr<FJsonObject>& JsonObject, const FString& FieldName, const FString& FieldType);

		const FSQLiteTableField JsonObjectToTableField(const TSharedPtr<FJsonObject>& JsonObject, const FString& FieldName, const FString& FieldType);

		/**
		 * @brief Try to open a connection to the Database, will create the DB if not exist.
		 *
		 * @param DBFileName The Relative Path to the DB file, referring to the 'Content' folder
		*/
		bool OpenConnection(const FString& DBFileName);

		/**
		 * @brief Create a table from a UStruct, if the table already exist, will do nothing.
		 *
		 * @param TableName The name of the table, duplicated name will not be created.
		 * @param ScriptStruct The UStruct Structure
		 * @param Result This will be called when the operation done. The result is bool.
		*/
		void CreateTable(const FString& TableName, UScriptStruct* ScriptStruct, const THandler<bool>& Result);

		/**
		 * @brief Create a Key Value Table.
		 *
		 * @param Result This will be called when the operation done. The result is bool.
		 * @param TableName optional. The name of the table. Default will create the default KeyValue table.
		*/
		void CreateKeyValuePairTable(const THandler<bool>& Result, const FString& TableName);

		/**
		 * @brief Execute a save item command.
		 *
		 * @param Key The Key of the Item.
		 * @param Value The Value of the Item.
		 * @param OnDone This will be called when the operation done. The result is Pair of a FString Key, and a FString Value.
		 * @param TableName optional. The name of the table. Default will get an item from the default KeyValue table.
		 */
		void ExecuteSaveCommand(const FString& Key, const FSQLiteTableField& Value, const THandler<bool>& OnDone, const FString& TableName);

		/**
		 * @brief Find an Item with specified Key from the Query Result.
		 *
		 * @param Key The Key of the Item.
		 * @param Data The Query Result data.
		 */
		FSQLiteKeyValuePair FindItemWithKey(const FString& Key, const FSQLiteQueryResult& Data);
	};

	template<typename T>
	inline void FAccelByteSQLite3::SaveItemToTable(const FString& TableName, const FString& Key, const T& DataToInsert, const THandler<bool>& OnDone)
	{
		CreateTable(TableName, DataToInsert.StaticStruct(), THandler<bool>::CreateLambda([this, TableName, Key, DataToInsert, OnDone](bool CreateResult)
			{
				if (CreateResult)
				{
					FAutoDeleteAsyncTask<DBQueryTask<SQLiteQueryResult>>* MyTask = new FAutoDeleteAsyncTask<DBQueryTask<SQLiteQueryResult>>(MakeShareable(USQLiteDatabase::RunQueryAndGetResults(
						DatabaseName,
						FString::Printf(TEXT("Select name, type FROM PRAGMA_table_info('%s') ORDER BY cid;"), *TableName)).Release()),
						THandler<SQLiteQueryResult>::CreateLambda([this, Key, DataToInsert, TableName, OnDone](SQLiteQueryResult Result)
							{

								FSQLiteTableRowSimulator Row;
								TSharedPtr<FJsonObject> JsonObject = FJsonObjectConverter::UStructToJsonObject(DataToInsert);
								bool bIsSuccess = true;
								for (auto Column : Result.Results)
								{
									FSQLiteTableField Field = JsonObjectToTableField(JsonObject, Column.Fields[0].StringValue, Column.Fields[1].StringValue);
									if (Field.FieldName.IsEmpty())
									{
										bIsSuccess = false;
										break;
									}
									if (Field.FieldName == TEXT("'Key'"))
									{
										Field.FieldValue = FString::Printf(TEXT("\"%s\""), *Key);
									}
									Row.rowsOfFields.Add(Field);
								}
								if (bIsSuccess)
								{
									USQLiteDatabase::InsertRowsIntoTableUpsert(DatabaseName, TableName, TArray<FSQLiteTableRowSimulator>{ { Row }}, TEXT("Key"));
								}
								OnDone.ExecuteIfBound(bIsSuccess);
							}));
					MyTask->StartBackgroundTask();
				}
				else
				{
					OnDone.ExecuteIfBound(CreateResult);
				}
			}));
	}

	template<typename T>
	inline void FAccelByteSQLite3::GetItemsFromTable(const FString& TableName, const THandler<TMap<FString, T>>& OnDone, int Limit, int Offset)
	{
		FSQLiteDatabaseReference DBRef{ DatabaseName, {TableName} };

		FAutoDeleteAsyncTask<DBQueryTask<FSQLiteQueryResult>>* MyTask = new FAutoDeleteAsyncTask<DBQueryTask<FSQLiteQueryResult>>(MakeShared<FSQLiteQueryResult>(USQLiteDatabase::GetDataBP(
			DBRef,
			{ "*" },
			FSQLiteQueryFinalizedQuery{},
			Limit,
			Offset)),
			THandler<FSQLiteQueryResult>::CreateLambda([this, OnDone](FSQLiteQueryResult Result)
				{
					if (Result.Success)
					{
						TMap<FString, T> Data;
						for (int i = 0; i < Result.ResultRows.Num(); i++)
						{
							FString Key;
							FString JsonString = TEXT("{");
							bool bIsNotFirstField = false;
							for (int j = 0; j < Result.ResultRows[i].Fields.Num(); j++)
							{
								if (Result.ResultRows[i].Fields[j].Key == TEXT("Key"))
								{
									Key = Result.ResultRows[i].Fields[j].Value;
									Key.TrimQuotesInline();
									continue;
								}
								if (bIsNotFirstField)
								{
									JsonString += TEXT(", ");
								}
								else
								{
									bIsNotFirstField = true;
								}
								JsonString += FString::Printf(TEXT("\"%s\" : %s"), *Result.ResultRows[i].Fields[j].Key, *Result.ResultRows[i].Fields[j].Value);
							}
							JsonString += TEXT("}");
							typename std::remove_const<typename std::remove_reference<T>::type>::type UStructData;
							FJsonObjectConverter::JsonObjectStringToUStruct(JsonString, &UStructData, 0, 0);
							Data.Add(Key, UStructData);
						}
						OnDone.ExecuteIfBound(Data);
					}
					else
					{
						OnDone.ExecuteIfBound(TMap<FString, T>{});
					}
				}));
		MyTask->StartBackgroundTask();
	}

	template<typename T>
	inline void FAccelByteSQLite3::GetItemFromTable(const FString& TableName, const FString& Key, const THandler<TPair<FString, T>>& OnDone)
	{
		FSQLiteDatabaseReference DBRef{ DatabaseName, {TableName} };
		const FString Condition = FString::Printf(TEXT("Key = '\"%s\"'"), *Key);

		FAutoDeleteAsyncTask<DBQueryTask<FSQLiteQueryResult>>* MyTask = new FAutoDeleteAsyncTask<DBQueryTask<FSQLiteQueryResult>>(MakeShared<FSQLiteQueryResult>(USQLiteDatabase::GetDataBP(
			DBRef,
			{ "*" },
			FSQLiteQueryFinalizedQuery{ Condition })),
			THandler<FSQLiteQueryResult>::CreateLambda([this, Key, OnDone](FSQLiteQueryResult Result)
				{
					if (Result.Success)
					{
						TPair<FString, T> Data{};
						for (int i = 0; i < Result.ResultRows.Num(); i++)
						{
							FString JsonString = TEXT("{");
							bool bIsNotFirstField = false;
							bool bKeyFound = false;
							for (int j = 0; j < Result.ResultRows[i].Fields.Num(); j++)
							{
								if (Result.ResultRows[i].Fields[j].Key == TEXT("Key"))
								{
									if (Result.ResultRows[i].Fields[j].Value.TrimQuotes() == Key)
									{
										bKeyFound = true;
									}
									else
									{
										break;
									}
									continue;
								}
								if (bIsNotFirstField)
								{
									JsonString += TEXT(", ");
								}
								else
								{
									bIsNotFirstField = true;
								}
								JsonString += FString::Printf(TEXT("\"%s\" : %s"), *Result.ResultRows[i].Fields[j].Key, *Result.ResultRows[i].Fields[j].Value);
							}
							JsonString += TEXT("}");
							if (bKeyFound)
							{
								typename std::remove_const<typename std::remove_reference<T>::type>::type UStructData;
								FJsonObjectConverter::JsonObjectStringToUStruct(JsonString, &UStructData, 0, 0);
								Data = TPair<FString, T>{ Key, UStructData };
								break;
							}
						}
						OnDone.ExecuteIfBound(Data);
					}
					else
					{
						OnDone.ExecuteIfBound(TPair<FString, T>{});
					}
				}));
		MyTask->StartBackgroundTask();
	}
}
#endif