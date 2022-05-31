// Copyright (c) 2022 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "AccelByteJwtWrapper.h"
#include "JsonObjectConverter.h"
#include "Core/AccelByteUtilities.h"

DECLARE_LOG_CATEGORY_EXTERN(FLogCategoryLogAccelByteJwtWrapper, Log, All);
DEFINE_LOG_CATEGORY(FLogCategoryLogAccelByteJwtWrapper);

bool AccelByteJwtWrapper::TryDecode(const FString& Token, const FString& PublicKey, TSharedPtr<FJsonObject>& Output, FAccelByteJwtError& ErrorOut, const bool VerifySignature, const bool VerifyExpiration, const FString& VerifySubject)
{
	const FJwt Jwt {Token};
	if(!Jwt.IsValid())
	{
		ErrorOut.Code = 1;
		ErrorOut.Message = TEXT("not a valid jwt token");
		return false;
	}

	if(VerifySignature)
	{
		FString pub {PublicKey};
		pub.RemoveFromStart("-----BEGIN PUBLIC KEY-----");
		pub.RemoveFromEnd("-----END PUBLIC KEY-----");
		pub = pub.TrimStartAndEnd();

		constexpr int RSA_MODULUS_FIXED_HEADER_LENGTH {44};
		constexpr int RSA_EXPONENT_HEADER_LENGTH {2};
		constexpr int RSA_EXPONENT_LENGTH {4};
		const FString Mod {pub.RightChop(RSA_MODULUS_FIXED_HEADER_LENGTH).LeftChop(RSA_EXPONENT_LENGTH + RSA_EXPONENT_HEADER_LENGTH).Replace(TEXT("\n"), TEXT(""))};
		const FString Exp {pub.Right(RSA_EXPONENT_LENGTH)};
		const FRsaPublicKey RsaPubkey {Mod, Exp};
		const EJwtResult VerifyResult = Jwt.VerifyWith(RsaPubkey);
		
		if( VerifyResult != EJwtResult::Ok)
		{
			ErrorOut.Code = 4001;
			ErrorOut.Message = "Verify signature failed";
			UE_LOG(FLogCategoryLogAccelByteJwtWrapper, Display, TEXT("JWT signature verification failed with jwt result index %d"), VerifyResult);
			return false;
		}
	}

	if(VerifyExpiration)
	{
		if(!Jwt.Payload()->HasField("exp"))
		{
			ErrorOut.Code = 4101;
			ErrorOut.Message = "Verify expiration failed, payload has no exp field";
			return false;
		}

		const FDateTime ExpDate {FDateTime::FromUnixTimestamp(Jwt.Payload()->GetIntegerField("exp"))};

		if(ExpDate < FDateTime::UtcNow())
		{
			ErrorOut.Code = 4102;
			ErrorOut.Message = "Verify expiration failed, token expired";
			return false;
		}
	}

	if(!VerifySubject.IsEmpty())
	{
		if(!Jwt.Payload()->HasField("sub"))
		{
			ErrorOut.Code = 4201;
			ErrorOut.Message = "Verify subject failed, payload has no sub field";
			return false;
		}

		if(Jwt.Payload()->GetStringField("sub") != VerifySubject)
		{
			ErrorOut.Code = 4202;
			ErrorOut.Message = "Verify subject failed, subject content not equal";
			return false;
		}
	}

	Output = Jwt.Payload();
	ErrorOut.Code = 0;
	return true;
}


void AccelByteJwtWrapper::GetExpirationDateTime(const FString& InToken, FDateTime& OutExpiration)
{
	const FJwt Jwt {InToken};
	if(!Jwt.IsValid())
	{
		UE_LOG(FLogCategoryLogAccelByteJwtWrapper, Warning, TEXT("Jwt not valid"));
		return;
	}

	if(!Jwt.Payload()->HasField("exp"))
	{
		UE_LOG(FLogCategoryLogAccelByteJwtWrapper, Warning, TEXT("Jwt exp field not found"));
		return;
	}
	
	OutExpiration = FDateTime::FromUnixTimestamp(Jwt.Payload()->GetIntegerField("exp"));
}