// Copyright (c) 2020-2022 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "CoreMinimal.h"
#include "Misc/IEngineCrypto.h"

#if !PLATFORM_SWITCH
namespace openssl
{
THIRD_PARTY_INCLUDES_START
#include <openssl/evp.h>
#include <openssl/rsa.h>
#include <openssl/rand.h>
#include <openssl/aes.h>
THIRD_PARTY_INCLUDES_END
}
#endif // PLATFORM_SWITCH

namespace AccelByte
{

class ACCELBYTEUE4SDK_API FRSAEncryptionOpenSSL
{
/** Encryption size is 256 bytes (MAX_EXPONENT_BITS / 8) */
#define MAX_EXPONENT_BITS 2048

public:
	FRSAEncryptionOpenSSL() : KeySizeInBits(MAX_EXPONENT_BITS), Key(nullptr)
	{
		KeySizeInBytes = KeySizeInBits / 8;
	}
	~FRSAEncryptionOpenSSL()
	{
		Empty();
	}

	/**
	 * @brief Clear all saved data and states.
	 */
	void Empty();

	/**
	 * @brief Initialize OpenSSL module.
	 */
	void Initialize();

	/**
	 * @brief Generate random new RSA Key.
	 *
	 * @param InKeySizeInBits RSA Key size in bits value. (optional)
	 * @return RSA Key handle.
	 */
	FRSAKeyHandle GenerateNewKey(int32 InKeySizeInBits = MAX_EXPONENT_BITS);

	/**
	 * @brief Create RSA Key using 'public/private exponent' and 'modulus'.
	 *
	 * @param InPublicExponent Public Key for the RSA key.
	 * @param InPrivateExponent Private Key for the RSA key.
	 * @param InModulus Modulus data for the RSA Key.
	 * @return RSA Key handle.
	 */
	FRSAKeyHandle CreateKey(const TArray<uint8>& InPublicExponent, const TArray<uint8>& InPrivateExponent, const TArray<uint8>& InModulus);

	/**
	 * @brief Sign the message using the stored RSA Key.
	 *
	 * @param InMsg Message that will be signed.
	 * @param Out Signed/encrypted message.
	 * @return A boolean flag indicates successful/failed operation.
	 */
	bool SignMessage(const TArray<uint8>& InMsg, TArray<uint8>& Out);

	/**
	 * @brief Verify the message.
	 *
	 * @param InHashMsg Hash value of the message.
	 * @param InMsg Message that will be verified.
	 * @return A boolean flag indicates successful/failed operation.
	 */
	bool VerifySignature(const TArray<uint8>& InHashMsg, const TArray<uint8>& InMsg);

	/**
	 * @brief Encrypt with public key.
	 *
	 * @param InSource Source data.
	 * @param OutDestination Encrypted data.
	 * @return Size of the encrypted data.
	 */
	int32 EncryptPublic(const TArrayView<const uint8> InSource, TArray<uint8>& OutDestination);

	/**
	 * @brief Decrypt with public key.
	 *
	 * @param InSource Source data.
	 * @param OutDestination Decrypted data.
	 * @return Size of the decrypted data.
	 */
	int32 DecryptPublic(const TArrayView<const uint8> InSource, TArray<uint8>& OutDestination);

	/**
	 * @brief Encrypt with private key.
	 *
	 * @param InSource Source data.
	 * @param OutDestination Encrypted data.
	 * @return Size of the encrypted data.
	 */
	int32 EncryptPrivate(const TArrayView<const uint8> InSource, TArray<uint8>& OutDestination);

	/**
	 * @brief Decrypt with private key.
	 *
	 * @param InSource Source data.
	 * @param OutDestination Decrypted data.
	 * @return Size of the decrypted data.
	 */
	int32 DecryptPrivate(const TArrayView<const uint8> InSource, TArray<uint8>& OutDestination);

	/**
	 * @brief Destroy(Free) RSA Key.
	 *
	 * @param InKey Given RSA Key handle. (optional)
	 */
	void DestroyKey(FRSAKeyHandle InKey = nullptr);

	/**
	 * @brief Retrieve size of the RSA Key in bytes value.
	 *
	 * @param InKey Given RSA Key handle. (optional)
	 * @return Size of the RSA Key in bytes value.
	 */
	int32 GetKeySizeInBytes(FRSAKeyHandle InKey = nullptr);

	/**
	 * @brief Retrieve size of the stored RSA Key in bits value.
	 *
	 * @return Size of the stored RSA Key in bits value.
	 */
	int32 GetKeySizeInBits() const { return KeySizeInBits; }

	/**
	 * @brief Set key size for the stored RSA key.
	 *
	 * @param InKeySizeInBits RSA Key size in bits value.
	 */
	void SetKeySizeInBits(uint32 InKeySizeInBits) { KeySizeInBits = InKeySizeInBits; }

	/**
	 * @brief Retrieve the Modulus data of the RSA Key.
	 *
	 * @return Modulus data in bytes.
	 */
	TArray<uint8>& GetModulus() { return Modulus; }

	/**
	 * @brief Retrieve the Public Key of the RSA Key.
	 *
	 * @return Public Key in bytes.
	 */
	TArray<uint8>& GetPublicExponent() { return PublicExponent; }

	/**
	 * @brief Retrieve the Private Key of the RSA Key.
	 *
	 * @return Private Key in bytes.
	 */
	TArray<uint8>& GetPrivateExponent() { return PrivateExponent; }

	/**
	 * @brief Set Modulus data for the RSA Key.
	 *
	 * @param InModulus Modulus data for the RSA Key.
	 */
	void SetModulus(const TArray<uint8>& InModulus) { Modulus = InModulus; }

	/**
	 * @brief Set Public Key for the RSA Key.
	 *
	 * @param InPublicExponent Public Key for the RSA key.
	 */
	void SetPublicExponent(const TArray<uint8>& InPublicExponent) { PublicExponent = InPublicExponent; }

	/**
	 * @brief Set Private Key for the RSA Key.
	 *
	 * @param InPrivateExponent Private Key for the RSA key.
	 */
	void SetPrivateExponent(const TArray<uint8>& InPrivateExponent) { PrivateExponent = InPrivateExponent; }

#if !PLATFORM_SWITCH
	/**
	 * @brief Retrieve maximum data size.
	 *
	 * @return Possible maximum data size.
	 */
#if !defined(OPENSSL_VERSION_NUMBER) || OPENSSL_VERSION_NUMBER <= 0x10200000L
	int32 GetMaxDataSize() { return (GetKeySizeInBytes() - RSA_PKCS1_PADDING_SIZE); }
#else
	int32 GetMaxDataSize() { return (GetKeySizeInBytes() - 42); }
#endif
#endif

private:

#if !PLATFORM_SWITCH
	void LoadBinaryIntoBigNum(const TArray<uint8>& InData, openssl::BIGNUM* InBigNum);
	void BigNumToArray(const openssl::BIGNUM* InNum, TArray<uint8>& OutBytes, int32 InKeySize);

	bool OnSign(const FRSAKeyHandle InKey, const TArray<uint8>& InMsg, TArray<uint8>& Out);
	bool OnVerifySignature(FRSAKeyHandle InKey, const TArray<uint8>& MsgHash, const TArray<uint8>& InMsg);
#endif // !PLATFORM_SWITCH

private:
	enum class EState : uint8
	{
		/** Initialize needs to be called before data may be encrypted successfully */
		Uninitialized = 0,
		/** Initialize succeeded and is currently able to encrypt data */
		Initialized
	};

#if !PLATFORM_SWITCH
#if !defined(OPENSSL_VERSION_NUMBER) || OPENSSL_VERSION_NUMBER <= 0x10200000L
	int PaddingMode = RSA_PKCS1_PADDING;
#else
	int PaddingMode = RSA_PKCS1_OAEP_PADDING;
#endif
#endif

	/** Size of the RSA key in bits */
	int32 KeySizeInBits;

	/** Size of the RSA key in bytes
	 * Maximum plain text length that can be encrypted with public RSA key
	 */
	int32 KeySizeInBytes;

	/* RSA Key Handler. the type of 'RSA*' */
	FRSAKeyHandle Key;

	EState State = EState::Uninitialized;

	/* Modulus for RSA Key */
	TArray<uint8> Modulus;

	/* Local Public Key */
	TArray<uint8> PublicExponent;

	/* Local Private Key */
	TArray<uint8> PrivateExponent;
};

#if !PLATFORM_SWITCH
/**
 * RAII wrapper for the OpenSSL EVP Cipher context object such as EVP_EncryptInit_ex.
 */
class ACCELBYTEUE4SDK_API FCipherCtx
{
public:
	/**
	 * Creates the OpenSSL Cipher Context on construction
	 */
	FCipherCtx() : Context(openssl::EVP_CIPHER_CTX_new()) {}

	/**
	 * Free the OpenSSL Cipher Context
	 */
	~FCipherCtx()
	{
		openssl::EVP_CIPHER_CTX_free(Context);
	}

	/**
	 * Get our OpenSSL Cipher Context
	 */
	openssl::EVP_CIPHER_CTX* Get() const { return Context; }

private:
	/** Disable copying/assigning */
	FCipherCtx(const FCipherCtx& Other) = delete;
	FCipherCtx& operator=(const FCipherCtx& Other) = delete;

	/** the OpenSSL EVP Cipher context object */
	openssl::EVP_CIPHER_CTX* Context;
};

#endif // !PLATFORM_SWITCH

class ACCELBYTEUE4SDK_API FAESEncryptionOpenSSL
{
public:
	FAESEncryptionOpenSSL() : KeySizeInBits(256), BlockSize(AES_BLOCK_SIZE)
#if !PLATFORM_SWITCH
		, Cipher(nullptr)
#endif // !PLATFORM_SWITCH
	{
		KeySizeInBytes = KeySizeInBits / 8;
	}

	~FAESEncryptionOpenSSL()
	{
		Empty();
	}

	void Empty();

	void Initialize();

#if !PLATFORM_SWITCH
	bool GenerateRandomBytes(TArray<uint8>& OutKey, uint32& InSizeBytes);
#endif // !PLATFORM_SWITCH

	/* generate random AES Key. */
	void GenerateKey(uint32 InKeySizeInBytes = 32, uint32 InBlockSize = AES_BLOCK_SIZE);

	/* Encrypt with AES key */
	void Encrypt(const TArrayView<const uint8>& InPlaintext, TArray<uint8>& OutCiphertext, int32& Length);

	/* Decrypt with AES key */
	void Decrypt(const TArrayView<const uint8>& InCiphertext, TArray<uint8>& OutPlaintext, int32& Length);

	/** get the AES key */
	TArray<uint8>& GetKey() { return Key; }

	/** get the initialization vector */
	TArray<uint8>& GetIV() { return IV; }

	/** set a AES key */
	void SetKey(TArray<uint8>& In) { Key = In; }

	/** set a initialization vector */
	void SetIV(TArray<uint8>& In) { IV = In; }

	uint32 GetKeySizeInBits() const { return KeySizeInBits; }
	uint32 GetKeySizeInBytes() const { return KeySizeInBytes; }
	uint32 GetBlockSize() const { return BlockSize; }

	void SetKeySizeInBits(uint32 In) { KeySizeInBits = In; }
	void SetKeySizeInBytes(uint32 In) { KeySizeInBytes = In; }
	void SetBlockSize(uint32 In) { BlockSize = In; }

private:
	int32 GetIVSizeBytes() const { return IV.Num(); }

private:
	enum class EState : uint8
	{
		/** Initialize needs to be called before data may be encrypted successfully */
		Uninitialized = 0,
		/** Initialize succeeded and is currently able to encrypt data */
		Initialized
	};

	/** Size of the AES key in bits */
	uint32 KeySizeInBits;

	/** Size of the AES key in bytes */
	uint32 KeySizeInBytes;

	/** Size of the AES block */
	uint32 BlockSize;

#if !PLATFORM_SWITCH
	/** RAII wrapper for the OpenSSL EVP Cipher context object */
	FCipherCtx CipherCtx;

	const openssl::EVP_CIPHER* Cipher;
#endif // !PLATFORM_SWITCH

	/** The AES key */
	TArray<uint8> Key;

	/** The initialization vector */
	TArray<uint8> IV;

	EState State = EState::Uninitialized;
};

} // Namespace AccelByte
