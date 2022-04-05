// Copyright (c) 2022 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "SQLiteAdapterTest.h"
#include "Misc/AutomationTest.h"
#include "Http.h"
#include "HttpModule.h"
#include "HttpManager.h"
#include "Core/AccelByteSQLite3.h"
#include "TestUtilities.h"

DECLARE_LOG_CATEGORY_EXTERN(LogAccelByteDBAdapterTest, Log, All);
DEFINE_LOG_CATEGORY(LogAccelByteDBAdapterTest);

const int32 AutomationFlagMaskDBAdapter = (EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::CommandletContext | EAutomationTestFlags::ClientContext);

BEGIN_DEFINE_SPEC(AccelByteDBAdapterTestSpec, "AccelByte.Tests.SQLiteAdapter", EAutomationTestFlags::ProductFilter | EAutomationTestFlags::ApplicationContextMask)
const FString DBName = TEXT("TestDatabase");
const FString DBFileName = TEXT("TestDatabase.db");
const FString TableName = TEXT("Test");
const FString TableKey = TEXT("TestKey");
const FString WrongTableKey = TEXT("WrongKey");
const FString IntKey = TEXT("IntKey");
const FString FloatKey = TEXT("FloatKey");
const FString StringKey = TEXT("StringKey");
const FString JsonKey = TEXT("JsonKey");
const FString BlobKey = TEXT("BlobKey");
const FString InvalidKey = TEXT("Invalid");
const int32 InitialIntData = 10;
const float InitialFloatData = 0.5;
const FString InitialStringData = TEXT("Init");
FJsonObjectWrapper InitialJsonObjectData;
const FString JsonObjectField = TEXT("Value");
//Placeholder.com is a free image placeholder service for web designers, serving billions and billions of images each year.
//It might be taken down at some point, we probably need to search for another source or find the other ways to provide the sample image for these tests. 
const FString InitialBlobUrl = TEXT("https://via.placeholder.com/150");
TArray<uint8> InitialBlobData;
const int32 UpdatedIntData = 100;
const float UpdatedFloatData = 0.25;
const FString UpdatedStringData = TEXT("Updated");
FJsonObjectWrapper UpdatedJsonObjectData;
const FString UpdatedBlobUrl = TEXT("https://via.placeholder.com/200");
TArray<uint8> UpdatedBlobData;
#ifdef SQLITE3_ENABLED
FAccelByteSQLite3 Adapter{DBName};
#endif
END_DEFINE_SPEC(AccelByteDBAdapterTestSpec)

void AccelByteDBAdapterTestSpec::Define()
{
#ifdef SQLITE3_ENABLED
	Describe("A.DBTables", [this]()
		{
			It("A.Should Successfully Insert Item to a Table", [this]()
				{
					bool bIsDone = false;
					bool bIsSuccess = false;
					Adapter.SaveItemToTable(TableName, TableKey, FTestModel{}, THandler<bool>::CreateLambda([&bIsDone, &bIsSuccess](bool Result)
						{
							bIsDone = true;
							bIsSuccess = Result;
						}));
					WaitUntil(bIsDone, "Wait for Result...");
					TestTrue(TEXT("Successfully Insert"), bIsSuccess);

					return bIsDone;
				});

			It("B.Should Successfully Get the Item from the Table", [this]() 
				{
					bool bIsDone = false;
					bool bIsSuccess = false;
					TPair<FString, FTestModel> GetResult;
					Adapter.GetItemFromTable(TableName, TableKey, THandler<TPair<FString, FTestModel>>::CreateLambda([&bIsDone, &GetResult](const TPair<FString, FTestModel>& Result)
						{
							bIsDone = true;
							GetResult = Result;
						}));
					WaitUntil(bIsDone, "Wait for Result...");
					if (!GetResult.Key.IsEmpty())
					{
						bIsSuccess = true;
					}
					TestTrue(TEXT("Successfully Get"), bIsSuccess);

					return bIsDone;
				});

			It("C.Should Successfully Update an Item on the Table", [this]()
				{
					bool bIsDone = false;
					bool bIsSuccess = false;
					FTestModel TestModel{};
					TestModel.I32 = 10000;
					Adapter.SaveItemToTable(TableName, TableKey, TestModel, THandler<bool>::CreateLambda([&bIsDone, &bIsSuccess](bool Result)
						{
							bIsDone = true;
							bIsSuccess = Result;
						}));
					WaitUntil(bIsDone, "Wait for Result...");
					TestTrue(TEXT("Successfully Update"), bIsSuccess);

					return bIsDone;
				});

			It("D.Should Successfully Get the Updated Item from the Table", [this]()
				{
					bool bIsDone = false;
					bool bIsSuccess = false;
					TPair<FString, FTestModel> GetResult;
					Adapter.GetItemFromTable(TableName, TableKey, THandler<TPair<FString, FTestModel>>::CreateLambda([&bIsDone, &GetResult](const TPair<FString, FTestModel>& Result)
						{
							bIsDone = true;
							GetResult = Result;
						}));
					WaitUntil(bIsDone, "Wait for Result...");
					if (GetResult.Value.I32 == 10000)
					{
						bIsSuccess = true;
					}
					TestTrue(TEXT("Successfully Get"), bIsSuccess);

					return bIsDone;
				});

			It("B.Should Successfully Get all Items from the Table", [this]()
				{
					bool bIsDone = false;
					bool bIsSuccess = false;
					TMap<FString, FTestModel> GetResult;
					Adapter.GetItemsFromTable(TableName, THandler<TMap<FString, FTestModel>>::CreateLambda([&bIsDone, &GetResult](const TMap<FString, FTestModel>& Result)
						{
							bIsDone = true;
							GetResult = Result;
						}));
					WaitUntil(bIsDone, "Wait for Result...");
					if (GetResult.Num() >= 1)
					{
						bIsSuccess = true;
					}
					TestTrue(TEXT("Successfully Get"), bIsSuccess);

					return bIsDone;
				});

			It("Y.Should Successfully Delete an Item from the Table", [this]()
				{
					bool bIsDone = false;
					Adapter.DeleteItem(TableKey, FVoidHandler::CreateLambda([&bIsDone]()
						{
							bIsDone = true;
						}), TableName);
					WaitUntil(bIsDone, "Wait for Result...");
					TestTrue(TEXT("Successfully Delete"), bIsDone);

					return bIsDone;
				});

			It("Y.Should Failed Get a non existed Item from the Table", [this]()
				{
					bool bIsDone = false;
					bool bIsSuccess = false;
					TPair<FString, FTestModel> GetResult;
					Adapter.GetItemFromTable(TableName, WrongTableKey, THandler<TPair<FString, FTestModel>>::CreateLambda([&bIsDone, &GetResult](const TPair<FString, FTestModel>& Result)
						{
							bIsDone = true;
							GetResult = Result;
						}));
					WaitUntil(bIsDone, "Wait for Result...");
					if (!GetResult.Key.IsEmpty())
					{
						bIsSuccess = true;
					}
					TestFalse(TEXT("Successfully Get"), bIsSuccess);

					return bIsDone;
				});

			It("Y.Should Failed Insert an Item to a Wrong Table Model", [this]()
				{
					bool bIsDone = false;
					bool bIsSuccess = false;
					FWrongTestModel WrongModel{};
					WrongModel.WrongField = TEXT("Wrong");
					Adapter.SaveItemToTable(TableName, TableKey, WrongModel, THandler<bool>::CreateLambda([&bIsDone, &bIsSuccess](bool Result)
						{
							bIsDone = true;
							bIsSuccess = Result;
						}));
					WaitUntil(bIsDone, "Wait for Result...");
					TestFalse("Successfully Insert", bIsSuccess);

					return bIsDone;
				});

			It("Z.Should Successfully Drop the Table", [this]()
				{
					bool bIsDone = false;
					bool bTableDropped = false;
					Adapter.Reset(THandler<bool>::CreateLambda([&bIsDone, &bTableDropped](bool QueryResult)
						{
							bIsDone = true;
							bTableDropped = QueryResult;
						}), TableName);
					WaitUntil(bIsDone, "Wait for Result...");
					TestTrue("Successfully Dropped", bTableDropped);

					return bIsDone;
				});
		});

	Describe("B.DBTablesKeyValue", [this]()
		{
			It("A.Should Prepare Setup", [this]()
				{
					FJsonObject InitialJson{};
					InitialJson.SetStringField(JsonObjectField, InitialStringData);
					InitialJsonObjectData.JsonObject = MakeShared<FJsonObject>(InitialJson);

					FJsonObject UpdatedJson{};
					UpdatedJson.SetStringField(JsonObjectField, UpdatedStringData);
					UpdatedJsonObjectData.JsonObject = MakeShared<FJsonObject>(UpdatedJson);
					
					FHttpRequestPtr InitialBlobRequest = FHttpModule::Get().CreateRequest();
					InitialBlobRequest->SetVerb("GET");
					InitialBlobRequest->SetURL(InitialBlobUrl);
					bool bIsDone = false;
					TArray<uint8> BlobData{};
					InitialBlobRequest->OnProcessRequestComplete().BindLambda([&bIsDone, &BlobData](FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
						{
							if (bWasSuccessful && Response.IsValid())
							{
								BlobData = Response->GetContent();
								bIsDone = true;
							}
						});
					InitialBlobRequest->ProcessRequest();
					WaitUntil(bIsDone, "Waiting for get BlobData");
					InitialBlobData = BlobData;

					FHttpRequestPtr UpdatedBlobRequest = FHttpModule::Get().CreateRequest();
					UpdatedBlobRequest->SetVerb("GET");
					UpdatedBlobRequest->SetURL(UpdatedBlobUrl);
					bIsDone = false;
					BlobData = TArray<uint8>{};
					UpdatedBlobRequest->OnProcessRequestComplete().BindLambda([&bIsDone, &BlobData](FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
						{
							if (bWasSuccessful && Response.IsValid())
							{
								BlobData = Response->GetContent();
								bIsDone = true;
							}
						});
					UpdatedBlobRequest->ProcessRequest();
					WaitUntil(bIsDone, "Waiting for get BlobData");
					UpdatedBlobData = BlobData;

					return bIsDone;
				});

			It("B.Should Successfully Insert an integer", [this]()
				{
					bool bIsDone = false;
					bool bIsSuccess = false;
					
					Adapter.SaveItem(IntKey, FString::Printf(TEXT("%d"), InitialIntData), THandler<bool>::CreateLambda([&bIsDone, &bIsSuccess](bool Result)
						{
							bIsDone = true;
							bIsSuccess = Result;
						}));
					WaitUntil(bIsDone, "Wait for Result...");
					TestTrue(TEXT("Successfully Insert"), bIsSuccess);

					return bIsDone;
				});

			It("B.Should Successfully Insert a float", [this]()
				{
					bool bIsDone = false;
					bool bIsSuccess = false;

					Adapter.SaveItem(FloatKey, FString::Printf(TEXT("%f"), InitialFloatData), THandler<bool>::CreateLambda([&bIsDone, &bIsSuccess](bool Result)
						{
							bIsDone = true;
							bIsSuccess = Result;
						}));
					WaitUntil(bIsDone, "Wait for Result...");
					TestTrue(TEXT("Successfully Insert"), bIsSuccess);

					return bIsDone;
				});

			It("B.Should Successfully Insert a String", [this]()
				{
					bool bIsDone = false;
					bool bIsSuccess = false;

					Adapter.SaveItem(StringKey, InitialStringData, THandler<bool>::CreateLambda([&bIsDone, &bIsSuccess](bool Result)
						{
							bIsDone = true;
							bIsSuccess = Result;
						}));
					WaitUntil(bIsDone, "Wait for Result...");
					TestTrue(TEXT("Successfully Insert"), bIsSuccess);

					return bIsDone;
				});

			It("B.Should Successfully Insert a blob", [this]()
				{
					bool bIsDone = false;
					bool bIsSuccess = false;

					Adapter.SaveItem(BlobKey, InitialBlobData, THandler<bool>::CreateLambda([&bIsDone, &bIsSuccess](bool Result)
						{
							bIsDone = true;
							bIsSuccess = Result;
						}));
					WaitUntil(bIsDone, "Wait for Result...");
					TestTrue(TEXT("Successfully Insert"), bIsSuccess);

					return bIsDone;
				});

			It("B.Should Successfully Insert a Json Object", [this]()
				{
					bool bIsDone = false;
					bool bIsSuccess = false;

					Adapter.SaveItem(JsonKey, InitialJsonObjectData, THandler<bool>::CreateLambda([&bIsDone, &bIsSuccess](bool Result)
						{
							bIsDone = true;
							bIsSuccess = Result;
						}));
					WaitUntil(bIsDone, "Wait for Result...");
					TestTrue(TEXT("Successfully Insert"), bIsSuccess);

					return bIsDone;
				});

			It("C.Should Successfully Update an integer", [this]()
				{
					bool bIsDone = false;
					bool bIsSuccess = false;

					Adapter.SaveItem(IntKey, FString::Printf(TEXT("%d"), UpdatedIntData), THandler<bool>::CreateLambda([&bIsDone, &bIsSuccess](bool Result)
						{
							bIsDone = true;
							bIsSuccess = Result;
						}));
					WaitUntil(bIsDone, "Wait for Result...");
					TestTrue(TEXT("Successfully Insert"), bIsSuccess);

					return bIsDone;
				});

			It("C.Should Successfully Update a float", [this]()
				{
					bool bIsDone = false;
					bool bIsSuccess = false;

					Adapter.SaveItem(FloatKey, FString::Printf(TEXT("%f"), UpdatedFloatData), THandler<bool>::CreateLambda([&bIsDone, &bIsSuccess](bool Result)
						{
							bIsDone = true;
							bIsSuccess = Result;
						}));
					WaitUntil(bIsDone, "Wait for Result...");
					TestTrue(TEXT("Successfully Insert"), bIsSuccess);

					return bIsDone;
				});

			It("C.Should Successfully Update a String", [this]()
				{
					bool bIsDone = false;
					bool bIsSuccess = false;

					Adapter.SaveItem(StringKey, UpdatedStringData, THandler<bool>::CreateLambda([&bIsDone, &bIsSuccess](bool Result)
						{
							bIsDone = true;
							bIsSuccess = Result;
						}));
					WaitUntil(bIsDone, "Wait for Result...");
					TestTrue(TEXT("Successfully Insert"), bIsSuccess);

					return bIsDone;
				});

			It("C.Should Successfully Update a blob", [this]()
				{
					bool bIsDone = false;
					bool bIsSuccess = false;

					Adapter.SaveItem(BlobKey, UpdatedBlobData, THandler<bool>::CreateLambda([&bIsDone, &bIsSuccess](bool Result)
						{
							bIsDone = true;
							bIsSuccess = Result;
						}));
					WaitUntil(bIsDone, "Wait for Result...");
					TestTrue(TEXT("Successfully Insert"), bIsSuccess);

					return bIsDone;
				});

			It("C.Should Successfully Update a Json Object", [this]()
				{
					bool bIsDone = false;
					bool bIsSuccess = false;

					Adapter.SaveItem(JsonKey, UpdatedJsonObjectData, THandler<bool>::CreateLambda([&bIsDone, &bIsSuccess](bool Result)
						{
							bIsDone = true;
							bIsSuccess = Result;
						}));
					WaitUntil(bIsDone, "Wait for Result...");
					TestTrue(TEXT("Successfully Insert"), bIsSuccess);

					return bIsDone;
				});

			It("Y.Should Successfully Get an integer", [this]()
				{
					bool bIsDone = false;
					bool bIsSuccess = false;
					TPair<FString, FString> GetResult{};

					Adapter.GetItem(IntKey, THandler<TPair<FString, FString>>::CreateLambda([&bIsDone, &bIsSuccess, &GetResult](const TPair<FString, FString>& Result)
						{
							bIsDone = true;
							bIsSuccess = !Result.Key.IsEmpty();
							GetResult = Result;
						}));
					WaitUntil(bIsDone, "Wait for Result...");
					TestTrue(TEXT("Successfully Get"), bIsSuccess);
					TestTrue(TEXT("Expected Value"), GetResult.Value.IsNumeric() ? (FCString::Atoi(*GetResult.Value) == UpdatedIntData) : false);

					return bIsDone;
				});

			It("Y.Should Successfully Get a float", [this]()
				{
					bool bIsDone = false;
					bool bIsSuccess = false;
					TPair<FString, FString> GetResult{};

					Adapter.GetItem(FloatKey, THandler<TPair<FString, FString>>::CreateLambda([&bIsDone, &bIsSuccess, &GetResult](const TPair<FString, FString>& Result)
						{
							bIsDone = true;
							bIsSuccess = !Result.Key.IsEmpty();
							GetResult = Result;
						}));
					WaitUntil(bIsDone, "Wait for Result...");
					TestTrue(TEXT("Successfully Get"), bIsSuccess);
					TestTrue(TEXT("Expected Value"), GetResult.Value.IsNumeric() ? (FCString::Atof(*GetResult.Value) == UpdatedFloatData) : false);

					return bIsDone;
				});

			It("Y.Should Successfully Get a String", [this]()
				{
					bool bIsDone = false;
					bool bIsSuccess = false;
					TPair<FString, FString> GetResult{};

					Adapter.GetItem(StringKey, THandler<TPair<FString, FString>>::CreateLambda([&bIsDone, &bIsSuccess, &GetResult](const TPair<FString, FString>& Result)
						{
							bIsDone = true;
							bIsSuccess = !Result.Key.IsEmpty();
							GetResult = Result;
						}));
					WaitUntil(bIsDone, "Wait for Result...");
					TestTrue(TEXT("Successfully Get"), bIsSuccess);
					TestTrue(TEXT("Expected Value"), GetResult.Value == UpdatedStringData);

					return bIsDone;
				});

			It("Y.Should Successfully Get a blob", [this]()
				{
					bool bIsDone = false;
					bool bIsSuccess = false;
					TPair<FString, TArray<uint8>> GetResult{};

					Adapter.GetItem(BlobKey, THandler<TPair<FString, TArray<uint8>>>::CreateLambda([&bIsDone, &bIsSuccess, &GetResult](const TPair<FString, TArray<uint8>>& Result)
						{
							bIsDone = true;
							bIsSuccess = !Result.Key.IsEmpty();
							GetResult = Result;
						}));
					WaitUntil(bIsDone, "Wait for Result...");
					TestTrue(TEXT("Successfully Get"), bIsSuccess);
					TestTrue(TEXT("Expected Value"), GetResult.Value == UpdatedBlobData);

					return bIsDone;
				});

			It("Y.Should Successfully Get a Json Object", [this]()
				{
					bool bIsDone = false;
					bool bIsSuccess = false;
					TPair<FString, FJsonObjectWrapper> GetResult{};

					Adapter.GetItem(JsonKey, THandler<TPair<FString, FJsonObjectWrapper>>::CreateLambda([&bIsDone, &bIsSuccess, &GetResult](const TPair<FString, FJsonObjectWrapper>& Result)
						{
							bIsDone = true;
							bIsSuccess = !Result.Key.IsEmpty();
							GetResult = Result;
						}));
					WaitUntil(bIsDone, "Wait for Result...");
					TestTrue(TEXT("Successfully Get"), bIsSuccess);
					FString JsonFieldValue;
					if (TestTrue(TEXT("Expected Json Field"), GetResult.Value.JsonObject->TryGetStringField(JsonObjectField, JsonFieldValue)))
					{
						TestTrue(TEXT("Expected Value"), JsonFieldValue == UpdatedStringData);
					}

					return bIsDone;
				});

			It("Y.Should Failed Get a non exist key", [this]()
				{
					bool bIsDone = false;
					bool bIsSuccess = false;
					TPair<FString, FString> GetResult{};

					Adapter.GetItem(InvalidKey, THandler<TPair<FString, FString>>::CreateLambda([&bIsDone, &bIsSuccess, &GetResult](const TPair<FString, FString>& Result)
						{
							bIsDone = true;
							bIsSuccess = !Result.Key.IsEmpty();
							GetResult = Result;
						}));
					WaitUntil(bIsDone, "Wait for Result...");
					TestFalse(TEXT("Successfully Get"), bIsSuccess);

					return bIsDone;
				});

			It("Y.Should Failed Get a blob as Json Object", [this]()
				{
					bool bIsDone = false;
					bool bIsSuccess = false;
					TPair<FString, FJsonObjectWrapper> GetResult{};

					Adapter.GetItem(BlobKey, THandler<TPair<FString, FJsonObjectWrapper>>::CreateLambda([&bIsDone, &bIsSuccess, &GetResult](const TPair<FString, FJsonObjectWrapper>& Result)
						{
							bIsDone = true;
							bIsSuccess = !Result.Key.IsEmpty();
							GetResult = Result;
						}));
					WaitUntil(bIsDone, "Wait for Result...");
					TestFalse(TEXT("Successfully Get"), bIsSuccess);
					TestFalse(TEXT("Expected Empty Value"), GetResult.Value.JsonObject.IsValid());

					return bIsDone;
				});

			It("Y.Should Failed Get a String as blob", [this]()
				{
					bool bIsDone = false;
					bool bIsSuccess = false;
					TPair<FString, TArray<uint8>> GetResult{};

					Adapter.GetItem(StringKey, THandler<TPair<FString, TArray<uint8>>>::CreateLambda([&bIsDone, &bIsSuccess, &GetResult](const TPair<FString, TArray<uint8>>& Result)
						{
							bIsDone = true;
							bIsSuccess = !Result.Key.IsEmpty();
							GetResult = Result;
						}));
					WaitUntil(bIsDone, "Wait for Result...");
					TestTrue(TEXT("Successfully Get"), bIsSuccess);
					TestFalse(TEXT("Expected Empty Value"), GetResult.Value.Num() > 0);

					return bIsDone;
				});

			It("Y.Should Failed Get a String as Json Object", [this]()
				{
					bool bIsDone = false;
					bool bIsSuccess = false;
					TPair<FString, FJsonObjectWrapper> GetResult{};

					Adapter.GetItem(StringKey, THandler<TPair<FString, FJsonObjectWrapper>>::CreateLambda([&bIsDone, &bIsSuccess, &GetResult](const TPair<FString, FJsonObjectWrapper>& Result)
						{
							bIsDone = true;
							bIsSuccess = !Result.Key.IsEmpty();
							GetResult = Result;
						}));
					WaitUntil(bIsDone, "Wait for Result...");
					TestFalse(TEXT("Successfully Get"), bIsSuccess);
					TestFalse(TEXT("Expected Empty Value"), GetResult.Value.JsonObject.IsValid());

					return bIsDone;
				});

			It("Y.Should Failed Get a Json Object as blob", [this]()
				{
					bool bIsDone = false;
					bool bIsSuccess = false;
					TPair<FString, TArray<uint8>> GetResult{};

					Adapter.GetItem(JsonKey, THandler<TPair<FString, TArray<uint8>>>::CreateLambda([&bIsDone, &bIsSuccess, &GetResult](const TPair<FString, TArray<uint8>>& Result)
						{
							bIsDone = true;
							bIsSuccess = !Result.Key.IsEmpty();
							GetResult = Result;
						}));
					WaitUntil(bIsDone, "Wait for Result...");
					TestTrue(TEXT("Successfully Get"), bIsSuccess);
					TestFalse(TEXT("Expected Empty Value"), GetResult.Value.Num() > 0);

					return bIsDone;
				});

			It("Z.Should Successfully Teardown the table", [this]()
				{
					bool bIsDone = false;
					bool bTableDropped = false;
					Adapter.Reset(THandler<bool>::CreateLambda([&bIsDone, &bTableDropped](bool QueryResult)
						{
							bIsDone = true;
							bTableDropped = QueryResult;
						}));
					WaitUntil(bIsDone, "Wait for Result...");
					TestTrue("Sucessfully Dropped", bTableDropped);

					return bIsDone;
				});
		});
#endif
}