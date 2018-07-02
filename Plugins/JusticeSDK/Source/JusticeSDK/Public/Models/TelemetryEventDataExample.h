// Copyright (c) 2017-2018 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Serialization/JsonSerializerMacros.h"
#include "JusticeBaseModel.h"
#include "TelemetryEventDataExample.generated.h"

class TelemetryEventDataExample : public FJsonSerializable
{
public:
	int32 ExampleField1;
	FString ExampleField2;
	int32 ExampleField3;

	BEGIN_JSON_SERIALIZER
		JSON_SERIALIZE("Field1", ExampleField1);
		JSON_SERIALIZE("Field2", ExampleField2);
		JSON_SERIALIZE("Field3", ExampleField3);
	END_JSON_SERIALIZER
};

UCLASS()
class UTelemetryEventDataExample : public UObject, public TelemetryEventDataExample, public JusticeBaseModel<UTelemetryEventDataExample, TelemetryEventDataExample>
{
	GENERATED_BODY()

		UFUNCTION(BlueprintPure, Category = "EventDataExample")
		int32 GetExampleField1() { return ExampleField1; };

		UFUNCTION(BlueprintPure, Category = "EventDataExample")
		FString GetExampleField2() { return ExampleField2; };

		UFUNCTION(BlueprintPure, Category = "EventDataExample")
		int32 GetExampleField3() { return ExampleField3; };
};