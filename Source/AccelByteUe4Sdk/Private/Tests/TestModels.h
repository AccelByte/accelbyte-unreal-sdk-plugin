#pragma once
#include "CoreMinimal.h"
#include "TestModels.generated.h"

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FStringFields
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString FieldA;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString FieldB;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString FieldC;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString FieldD;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString FieldE;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString FieldF;
};
        
USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FParent
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FStringFields ChildA;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FStringFields ChildB;
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FGrandParent
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FParent ParentA;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FParent ParentB;
};

#pragma region AWS

USTRUCT(BlueprintType)
struct FAwsAuthenticationResult
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | AWS")
	FString AccessToken;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | AWS")
	int ExpiresIn;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | AWS")
	FString IdToken;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | AWS")
	FString RefreshToken;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | AWS")
	FString TokenType;
};

USTRUCT(BlueprintType)
struct FAwsCognitoAuthResult
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | AWS")
	FAwsAuthenticationResult AuthenticationResult;
};

USTRUCT(BlueprintType)
struct FAwsErrorResult
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | AWS")
	FString __type;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | AWS")
	FString Message;
};

#pragma endregion AWS