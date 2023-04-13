// Copyright (c) 2020-2022 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "Core/AccelByteOpenSSL.h"

#include "Misc/Base64.h"
#include "Modules/ModuleManager.h"

#if PLATFORM_WINDOWS
#include "Windows/AllowWindowsPlatformTypes.h"
#endif

DEFINE_LOG_CATEGORY_STATIC(LogAccelByteOpenSSL, Log, All);

#if !defined(OPENSSL_VERSION_NUMBER) || OPENSSL_VERSION_NUMBER < 0x10100000L
#define USE_LEGACY_OPENSSL 1
#else
#define USE_LEGACY_OPENSSL 0
#endif

namespace AccelByte
{

void FRSAEncryptionOpenSSL::Initialize()
{
	if (State != EState::Uninitialized)
	{
		return;
	}

	State = EState::Initialized;
}

void FRSAEncryptionOpenSSL::Empty()
{
	if (nullptr != Key)
	{
		DestroyKey();
		Key = nullptr;
	}

	Modulus.Empty();
	PublicExponent.Empty();
	PrivateExponent.Empty();

	State = EState::Uninitialized;
}

#if !PLATFORM_SWITCH
void FRSAEncryptionOpenSSL::LoadBinaryIntoBigNum(const TArray<uint8>& InData, openssl::BIGNUM* InBigNum)
{
#if USE_LEGACY_OPENSSL
	TArray<uint8> Bytes(InData.GetData(), InData.Num());
	Algo::Reverse(Bytes);
	BN_bin2bn(Bytes.GetData(), Bytes.Num(), InBigNum);
#else
	BN_lebin2bn(InData.GetData(), InData.Num(), InBigNum);
#endif
}

void FRSAEncryptionOpenSSL::BigNumToArray(const openssl::BIGNUM* InNum, TArray<uint8>& OutBytes, int32 InKeySize)
{
	int32 NumBytes = BN_num_bytes(InNum);
	check(NumBytes <= InKeySize);
	OutBytes.SetNumZeroed(NumBytes);

#if USE_LEGACY_OPENSSL
	openssl::BN_bn2bin(InNum, OutBytes.GetData());
	Algo::Reverse(OutBytes);
#else
	openssl::BN_bn2lebinpad(InNum, static_cast<unsigned char*>(OutBytes.GetData()), NumBytes);
#endif
}
#endif // !PLATFORM_SWITCH

FRSAKeyHandle FRSAEncryptionOpenSSL::GenerateNewKey(int32 InKeySizeInBits)
{
	if (State != EState::Uninitialized)
	{
		Empty();
	}

	if (InKeySizeInBits != MAX_EXPONENT_BITS)
	{
		KeySizeInBits = InKeySizeInBits;
		KeySizeInBytes = InKeySizeInBits / 8;
	}

#if !PLATFORM_SWITCH
	Key = openssl::RSA_new();
	openssl::BIGNUM* E = openssl::BN_new();
	openssl::BN_set_word(E, RSA_F4);
	openssl::RSA_generate_key_ex((openssl::RSA*)Key, KeySizeInBits, E, nullptr);

#if USE_LEGACY_OPENSSL
	const openssl::BIGNUM* localPublicModulus = Key->n;
	const openssl::BIGNUM* localPublicExponent = Key->e;
	const openssl::BIGNUM* localPrivateExponent = Key->d;
#else
	const openssl::BIGNUM* localPublicModulus = openssl::RSA_get0_n((openssl::RSA*)Key);
	const openssl::BIGNUM* localPublicExponent = openssl::RSA_get0_e((openssl::RSA*)Key);
	const openssl::BIGNUM* localPrivateExponent = openssl::RSA_get0_d((openssl::RSA*)Key);
#endif

	BigNumToArray(localPublicModulus, Modulus, KeySizeInBytes);
	BigNumToArray(localPublicExponent, PublicExponent, KeySizeInBytes);
	BigNumToArray(localPrivateExponent, PrivateExponent, KeySizeInBytes);
#endif // !PLATFORM_SWITCH

	Initialize();

	return Key;
}

FRSAKeyHandle FRSAEncryptionOpenSSL::CreateKey(const TArray<uint8>& InPublicExponent, const TArray<uint8>& InPrivateExponent, const TArray<uint8>& InModulus)
{
	if (State != EState::Uninitialized)
	{
		Empty();
	}

	PublicExponent = InPublicExponent;
	PrivateExponent = InPrivateExponent;
	Modulus = InModulus;

#if !PLATFORM_SWITCH

	Key = openssl::RSA_new();

	openssl::BIGNUM* BN_PublicExponent = PublicExponent.Num() > 0 ? openssl::BN_new() : nullptr;
	openssl::BIGNUM* BN_PrivateExponent = PrivateExponent.Num() > 0 ? openssl::BN_new() : nullptr;
	openssl::BIGNUM* BN_Modulus = openssl::BN_new();

	if (PublicExponent.Num())
	{
		LoadBinaryIntoBigNum(PublicExponent, BN_PublicExponent);
	}

	if (PrivateExponent.Num())
	{
		LoadBinaryIntoBigNum(PrivateExponent, BN_PrivateExponent);
	}

	LoadBinaryIntoBigNum(Modulus, BN_Modulus);

#if USE_LEGACY_OPENSSL
	Key->n = BN_Modulus;
	Key->e = BN_PublicExponent;
	Key->d = BN_PrivateExponent;
#else
	openssl::RSA_set0_key((openssl::RSA*)Key, BN_Modulus, BN_PublicExponent, BN_PrivateExponent);
#endif

#endif // !PLATFORM_SWITCH

	Initialize();

	return Key;
}

#if !PLATFORM_SWITCH
bool FRSAEncryptionOpenSSL::OnSign(const FRSAKeyHandle InKey, const TArray<uint8>& InMsg, TArray<uint8>& Out)
{
	bool Result = false;

	bool NeedSigning = true;
	size_t OutLength = 0;

	openssl::EVP_MD_CTX* RSASignCtx = openssl::EVP_MD_CTX_create();
	if (RSASignCtx == nullptr)
	{
		return Result;
	}

	openssl::EVP_PKEY* RSAKeyPairSpec = openssl::EVP_PKEY_new();

	openssl::EVP_PKEY_assign_RSA(RSAKeyPairSpec, InKey);

	NeedSigning &= openssl::EVP_DigestSignInit(RSASignCtx, nullptr, openssl::EVP_sha256(), nullptr, RSAKeyPairSpec) > 0;
	NeedSigning &= openssl::EVP_DigestSignUpdate(RSASignCtx, InMsg.GetData(), InMsg.Num()) > 0;
	NeedSigning &= openssl::EVP_DigestSignFinal(RSASignCtx, nullptr, &OutLength) > 0;

	if (NeedSigning)
	{
		Out.AddUninitialized(OutLength);
		Out[Out.Num() - 1] = 0;
		if (EVP_DigestSignFinal(RSASignCtx, Out.GetData(), &OutLength) > 0)
		{
			Result = true;
		}
	}

	openssl::EVP_MD_CTX_destroy(RSASignCtx);
	RSASignCtx = nullptr;



	return Result;
}

bool FRSAEncryptionOpenSSL::OnVerifySignature(FRSAKeyHandle InKey, const TArray<uint8>& Signature, const TArray<uint8>& InMsg)
{
	bool Result = false;

	bool NeedVerify = true;

	openssl::EVP_MD_CTX* m_RSAVerifyCtx = openssl::EVP_MD_CTX_create();
	if (m_RSAVerifyCtx == nullptr)
	{
		return Result;
	}

	openssl::EVP_PKEY* RSAKeyPairSpec = openssl::EVP_PKEY_new();
	openssl::EVP_PKEY_assign_RSA(RSAKeyPairSpec, InKey);

	NeedVerify &= openssl::EVP_DigestVerifyInit(m_RSAVerifyCtx, NULL, openssl::EVP_sha256(), NULL, RSAKeyPairSpec) > 0;
	NeedVerify &= openssl::EVP_DigestVerifyUpdate(m_RSAVerifyCtx, InMsg.GetData(), InMsg.Num()) > 0;

	if (NeedVerify)
	{
		int AuthStatus = openssl::EVP_DigestVerifyFinal(m_RSAVerifyCtx, Signature.GetData(), Signature.Num());
		if (AuthStatus == 1)
		{
			Result = true;
		}
	}

	openssl::EVP_MD_CTX_destroy(m_RSAVerifyCtx);
	m_RSAVerifyCtx = nullptr;

	return Result;
}
#endif // !PLATFORM_SWITCH

bool FRSAEncryptionOpenSSL::SignMessage(const TArray<uint8>& InMsg, TArray<uint8>& Out)
{
	if (State != EState::Initialized)
	{
		UE_LOG(LogAccelByteOpenSSL, Warning, TEXT("FRSAEncryptionOpenSSL::SignMessage: Invalid state. Was %i, but should be Initialized"), State);
		return false;
	}
#if PLATFORM_SWITCH
	return false;
#else
	TArray<uint8> EncMsg;
	if (OnSign(Key, InMsg, EncMsg))
	{
		FString OutStr = FBase64::Encode(EncMsg);
		Out.AddUninitialized(OutStr.Len());
		if (0 < StringToBytes(OutStr, Out.GetData(), OutStr.Len()))
		{
			return true;
		}
	}
	return false;
#endif // PLATFORM_SWITCH
}

bool FRSAEncryptionOpenSSL::VerifySignature(const TArray<uint8>& InMsg, const TArray<uint8>& InSignature)
{
	if (State != EState::Initialized)
	{
		UE_LOG(LogAccelByteOpenSSL, Warning, TEXT("FRSAEncryptionOpenSSL::VerifySignature: Invalid state. Was %i, but should be Initialized"), State);
		return false;
	}

#if PLATFORM_SWITCH
	return false;
#else
	FString InStr = BytesToString(InSignature.GetData(), InSignature.Num());

	TArray<uint8> EncMsg;
	EncMsg.AddUninitialized(InStr.Len());
	EncMsg[EncMsg.Num() - 1] = 0;

	FBase64::Decode(InStr, EncMsg);

	bool Result = OnVerifySignature(Key, EncMsg, InMsg);
	return Result;
#endif // PLATFORM_SWITCH
}

int32 FRSAEncryptionOpenSSL::EncryptPublic(const TArrayView<const uint8> InSource, TArray<uint8>& OutDestination)
{
	if (State != EState::Initialized)
	{
		UE_LOG(LogAccelByteOpenSSL, Warning, TEXT("FRSAEncryptionOpenSSL::EncryptPublic: Invalid state. Was %i, but should be Initialized"), State);
		return -1;
	}

#if PLATFORM_SWITCH
	OutDestination.AddZeroed(InSource.Num());
	OutDestination.SetNum(InSource.Num());
	UE_LOG(LogAccelByteOpenSSL, Warning, TEXT("RSA's EncryptPublic() is disabled on nintendo switch!"));
	FMemory::Memcpy(OutDestination.GetData(), InSource.GetData(), InSource.Num());
	return OutDestination.Num();
#else
	if (InSource.Num() > GetMaxDataSize())
	{
		UE_LOG(LogAccelByteOpenSSL, Warning, TEXT("FRSAEncryptionOpenSSL::EncryptPublic: The data to be encrypted exceeds the maximum for this modulus of (%d) bytes."), InSource.Num(), GetMaxDataSize());
		return -1;
	}

	OutDestination.SetNum(GetKeySizeInBytes());
	int NumEncryptedBytes = -1;
	NumEncryptedBytes = openssl::RSA_public_encrypt(InSource.Num(), InSource.GetData(), OutDestination.GetData(), (openssl::RSA*)Key, PaddingMode);
	if (NumEncryptedBytes == -1)
	{
		UE_LOG(LogAccelByteOpenSSL, Warning, TEXT("FRSAEncryptionOpenSSL::EncryptPublic: not supported PaddingMode."));
		OutDestination.Empty(0);
	}
	return NumEncryptedBytes;
#endif // PLATFORM_SWITCH
}

int32 FRSAEncryptionOpenSSL::DecryptPublic(const TArrayView<const uint8> InSource, TArray<uint8>& OutDestination)
{
	if (State != EState::Initialized)
	{
		UE_LOG(LogAccelByteOpenSSL, Warning, TEXT("FRSAEncryptionOpenSSL::DecryptPublic: Invalid state. Was %i, but should be Initialized"), State);
		return -1;
	}

#if PLATFORM_SWITCH
	OutDestination.AddZeroed(InSource.Num());
	OutDestination.SetNum(InSource.Num());
	UE_LOG(LogAccelByteOpenSSL, Warning, TEXT("RSA's DecryptPublic() is disabled on nintendo switch!"));
	FMemory::Memcpy(OutDestination.GetData(), InSource.GetData(), InSource.Num());
	return OutDestination.Num();
#else
	OutDestination.SetNum(GetMaxDataSize());

	int NumDecryptedBytes = -1;
	NumDecryptedBytes = openssl::RSA_public_decrypt(InSource.Num(), InSource.GetData(), OutDestination.GetData(), (openssl::RSA*)Key, PaddingMode);

	if (NumDecryptedBytes == -1)
	{
		OutDestination.Empty(0);
	}
	else
	{
		OutDestination.SetNum(NumDecryptedBytes);
	}
	return NumDecryptedBytes;
#endif // PLATFORM_SWITCH
}

int32 FRSAEncryptionOpenSSL::EncryptPrivate(const TArrayView<const uint8> InSource, TArray<uint8>& OutDestination)
{
	if (State != EState::Initialized)
	{
		UE_LOG(LogAccelByteOpenSSL, Warning, TEXT("FRSAEncryptionOpenSSL::EncryptPrivate: Invalid state. Was %i, but should be Initialized"), State);
		return -1;
	}

#if PLATFORM_SWITCH
	OutDestination.AddZeroed(InSource.Num());
	OutDestination.SetNum(InSource.Num());
	UE_LOG(LogAccelByteOpenSSL, Warning, TEXT("RSA's EncryptPrivate() is disabled on nintendo switch!"));
	FMemory::Memcpy(OutDestination.GetData(), InSource.GetData(), InSource.Num());
	return OutDestination.Num();
#else
	if (InSource.Num() > GetMaxDataSize())
	{
		UE_LOG(LogAccelByteOpenSSL, Warning, TEXT("FRSAEncryptionOpenSSL::EncryptPublic: The data to be encrypted exceeds the maximum for this modulus of (%d) bytes."), InSource.Num(), GetMaxDataSize());
		return -1;
	}

	OutDestination.SetNum(GetKeySizeInBytes());

	int NumEncryptedBytes = -1;
	NumEncryptedBytes = RSA_private_encrypt(InSource.Num(), InSource.GetData(), OutDestination.GetData(), (openssl::RSA*)Key, PaddingMode);

	if (NumEncryptedBytes == -1)
	{
		UE_LOG(LogAccelByteOpenSSL, Warning, TEXT("FRSAEncryptionOpenSSL::EncryptPublic: not supported PaddingMode."));
		OutDestination.Empty(0);
	}
	return NumEncryptedBytes;
#endif // PLATFORM_SWITCH
}

int32 FRSAEncryptionOpenSSL::DecryptPrivate(const TArrayView<const uint8> InSource, TArray<uint8>& OutDestination)
{
	if (State != EState::Initialized)
	{
		UE_LOG(LogAccelByteOpenSSL, Warning, TEXT("FRSAEncryptionOpenSSL::DecryptPrivate: Invalid state. Was %i, but should be Initialized"), State);
		return -1;
	}

#if PLATFORM_SWITCH
	OutDestination.AddZeroed(InSource.Num());
	OutDestination.SetNum(InSource.Num());
	UE_LOG(LogAccelByteOpenSSL, Warning, TEXT("RSA's DecryptPrivate() is disabled on nintendo switch!"));
	FMemory::Memcpy(OutDestination.GetData(), InSource.GetData(), InSource.Num());
	return OutDestination.Num();
#else
	OutDestination.SetNum(GetMaxDataSize());

	int NumDecryptedBytes = -1;
	NumDecryptedBytes = RSA_private_decrypt(InSource.Num(), InSource.GetData(), OutDestination.GetData(), (openssl::RSA*)Key, PaddingMode);

	if (NumDecryptedBytes == -1)
	{
		OutDestination.Empty(0);
	}
	else
	{
		OutDestination.SetNum(NumDecryptedBytes);
	}
	return NumDecryptedBytes;
#endif // PLATFORM_SWITCH
}

void FRSAEncryptionOpenSSL::DestroyKey(FRSAKeyHandle InKey)
{
#if !PLATFORM_SWITCH
	if (nullptr == InKey)
	{
		return openssl::RSA_free((openssl::RSA*)Key);
	}
	openssl::RSA_free((openssl::RSA*)InKey);
#endif // !PLATFORM_SWITCH
}

int32 FRSAEncryptionOpenSSL::GetKeySizeInBytes(FRSAKeyHandle InKey)
{
	if (nullptr == InKey)
	{
		return KeySizeInBytes;
	}

#if PLATFORM_SWITCH
	return KeySizeInBytes;
#else
	return openssl::RSA_size((openssl::RSA*)InKey);
#endif // PLATFORM_SWITCH
}

void FAESEncryptionOpenSSL::Initialize()
{
	if (State != EState::Uninitialized)
	{
		UE_LOG(LogAccelByteOpenSSL, Warning, TEXT("FAESEncryptionOpenSSL::Initialize: Invalid state. Was %i, but should be Uninitialized"), State);
		return;
	}

#if !PLATFORM_SWITCH
	/** Select encryption module based on cipher size (Bytes) */
	switch (Key.Num())
	{
		case 16: Cipher = openssl::EVP_aes_128_cbc(); break;
		case 24: Cipher = openssl::EVP_aes_192_cbc(); break;
		//case 32: Cipher = openssl::EVP_aes_256_cbc(); break;
		//case 32: Cipher = openssl::EVP_aes_256_cbc_hmac_sha1(); break;
		case 32: Cipher = openssl::EVP_aes_256_cbc_hmac_sha256(); break;
		default: return;
	}
#endif // !PLATFORM_SWITCH

	State = EState::Initialized;
}

void FAESEncryptionOpenSSL::Empty()
{
#if !PLATFORM_SWITCH
	Cipher = nullptr;
#endif // !PLATFORM_SWITCH

	Key.Empty();
	IV.Empty();

	State = EState::Uninitialized;
}

#if !PLATFORM_SWITCH
bool FAESEncryptionOpenSSL::GenerateRandomBytes(TArray<uint8>& OutKey, uint32& InSizeBytes)
{
	bool bResult = false;
	if (0 == InSizeBytes)
	{
		return bResult;
	}

	OutKey.Empty(InSizeBytes);
	OutKey.AddUninitialized(InSizeBytes);

	bResult = !!openssl::RAND_bytes(&OutKey[0], InSizeBytes);
	if (!bResult)
	{
		OutKey.Empty();
	}

	return bResult;
}
#endif // !PLATFORM_SWITCH

void FAESEncryptionOpenSSL::GenerateKey(uint32 InKeySizeInBytes, uint32 InBlockSize)
{
	if (State != EState::Uninitialized)
	{
		UE_LOG(LogAccelByteOpenSSL, Warning, TEXT("FAESEncryptionOpenSSL::GenerateKey: Invalid state. Was %i, but should be Uninitialized"), State);
		return;
	}

	if ((InKeySizeInBytes != 16) && (InKeySizeInBytes != 24) && (InKeySizeInBytes != 32))
	{
		return;
	}

	KeySizeInBytes = InKeySizeInBytes;
	KeySizeInBits = KeySizeInBytes * 8;
	BlockSize = InBlockSize;

#if !PLATFORM_SWITCH
	if (!GenerateRandomBytes(Key, InKeySizeInBytes))
	{
		return;
	}

	if (!GenerateRandomBytes(IV, InBlockSize))
	{
		return;
	}
#endif // !PLATFORM_SWITCH

	Initialize();
}

void FAESEncryptionOpenSSL::Decrypt(const TArrayView<const uint8>& InCiphertext, TArray<uint8>& OutPlaintext, int32& Length)
{
	if (State != EState::Initialized)
	{
		UE_LOG(LogAccelByteOpenSSL, Warning, TEXT("FAESEncryptionOpenSSL::Decrypt: Invalid state. Was %i, but should be Initialized"), State);
		return;
	}

	if (OutPlaintext.Num() < Length)
	{
		OutPlaintext.AddZeroed(Length - OutPlaintext.Num());
	}

#if PLATFORM_SWITCH
	OutPlaintext.SetNum(InCiphertext.Num());
	UE_LOG(LogAccelByteOpenSSL, Warning, TEXT("AES's Decrypt() is disabled on nintendo switch!"));
	FMemory::Memcpy(OutPlaintext.GetData(), InCiphertext.GetData(), InCiphertext.Num());
#else
	const uint8* const KeyPtr = Key.GetData();
	const uint8* const InitializationVectorPtr = IV.GetData();

	int32 InitResult = openssl::EVP_DecryptInit_ex(CipherCtx.Get(), Cipher, nullptr, KeyPtr, InitializationVectorPtr);

	if (InitResult != 1)
	{
		UE_LOG(LogAccelByteOpenSSL, Warning, TEXT("FAESEncryptionOpenSSL::Decrypt: EVP_DecryptInit_ex failed. Result=[%d]"), InitResult);
		return;
	}

	int32 UpdateBytesWritten = 0;
	const int UpdateResult = openssl::EVP_DecryptUpdate(CipherCtx.Get(), OutPlaintext.GetData(), &UpdateBytesWritten, InCiphertext.GetData(), InCiphertext.Num());
	if (UpdateResult != 1)
	{
		UE_LOG(LogAccelByteOpenSSL, Warning, TEXT("FAESEncryptionOpenSSL::Decrypt: EVP_DecryptUpdate failed. Result=[%d]"), UpdateResult);
		return;
	}

	if ((OutPlaintext.Num() - UpdateBytesWritten) < GetIVSizeBytes())
	{
		return;
	}

	int32 FinalizeBytesWritten = 0;
	const int FinalizeResult = openssl::EVP_DecryptFinal_ex(CipherCtx.Get(), OutPlaintext.GetData() + UpdateBytesWritten, &FinalizeBytesWritten);
	if (FinalizeResult != 1)
	{
		UE_LOG(LogAccelByteOpenSSL, Warning, TEXT("FAESEncryptionOpenSSL::Decrypt: EVP_DecryptFinal_ex failed. Result=[%d]"), FinalizeResult);
		return;
	}

	// Truncate message to final length
	OutPlaintext.SetNum(UpdateBytesWritten + FinalizeBytesWritten);
#endif // PLATFORM_SWITCH
}

void FAESEncryptionOpenSSL::Encrypt(const TArrayView<const uint8>& InPlaintext, TArray<uint8>& OutCiphertext, int32& Length)
{
	if (State != EState::Initialized)
	{
		UE_LOG(LogAccelByteOpenSSL, Warning, TEXT("FAESEncryptionOpenSSL::Encrypt: Invalid state. Was %i, but should be Initialized"), State);
		return;
	}

	if (OutCiphertext.Num() < Length)
	{
		OutCiphertext.AddZeroed(Length - OutCiphertext.Num());
	}

#if PLATFORM_SWITCH
	OutCiphertext.SetNum(Length);
	UE_LOG(LogAccelByteOpenSSL, Warning, TEXT("AES's Encrypt() is disabled on nintendo switch!"));
	FMemory::Memcpy(OutCiphertext.GetData(), InPlaintext.GetData(), InPlaintext.Num());
#else
	const uint8* const KeyPtr = Key.GetData();
	const uint8* const InitializationVectorPtr = IV.GetData();

	int32 InitResult = openssl::EVP_EncryptInit_ex(CipherCtx.Get(), Cipher, nullptr, KeyPtr, InitializationVectorPtr);

	if (InitResult != 1)
	{
		UE_LOG(LogAccelByteOpenSSL, Warning, TEXT("FAESEncryptionOpenSSL::Encrypt: EVP_EncryptInit_ex failed. Result=[%d]"), InitResult);
		return;
	}

	int32 UpdateBytesWritten = 0;
	const int UpdateResult = openssl::EVP_EncryptUpdate(CipherCtx.Get(), OutCiphertext.GetData(), &UpdateBytesWritten, InPlaintext.GetData(), InPlaintext.Num());
	if (UpdateResult != 1)
	{
		UE_LOG(LogAccelByteOpenSSL, Warning, TEXT("FAESEncryptionOpenSSL::Encrypt: EVP_EncryptUpdate failed. Result=[%d]"), UpdateResult);
		return;
	}

	if ((OutCiphertext.Num() - UpdateBytesWritten) < GetIVSizeBytes())
	{
		return;
	}

	int32 FinalizeBytesWritten = 0;
	const int FinalizeResult = openssl::EVP_EncryptFinal_ex(CipherCtx.Get(), OutCiphertext.GetData() + UpdateBytesWritten, &FinalizeBytesWritten);
	if (FinalizeResult != 1)
	{
		UE_LOG(LogAccelByteOpenSSL, Warning, TEXT("FAESEncryptionOpenSSL::Encrypt: EVP_EncryptFinal_ex failed. Result=[%d]"), FinalizeResult);
		return;
	}

	// Truncate message to final length
	OutCiphertext.SetNum(UpdateBytesWritten + FinalizeBytesWritten);
#endif // PLATFORM_SWITCH
}

} // Namespace AccelByte
