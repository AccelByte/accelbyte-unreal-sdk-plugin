// Copyright (c) 2021 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "TestUtilities.h"
#include "Core/AccelByteUtilities.h"
#include "Misc/AutomationTest.h"


DEFINE_SPEC(
	FCompressionSpec,
	"AccelByte.Tests.Core.JwtSpec",
	EAutomationTestFlags::ProductFilter | EAutomationTestFlags::ApplicationContextMask)



void FCompressionSpec::Define()
{
	It("Given a JWT string, When create FJwt from it, Then IsValid() only if the string is in correct format", [this]()
	{
		AB_TEST_FALSE(FJwt{TEXT("")}.IsValid());
		AB_TEST_FALSE(FJwt{TEXT(".")}.IsValid());
		AB_TEST_FALSE(FJwt{TEXT("..")}.IsValid());
		AB_TEST_FALSE(FJwt{TEXT("...")}.IsValid());
		AB_TEST_FALSE(FJwt{TEXT("InvalidHeader")}.IsValid());
		AB_TEST_FALSE(FJwt{TEXT("InvalidHeader.")}.IsValid());
		AB_TEST_FALSE(FJwt{TEXT("InvalidHeader...")}.IsValid());
		AB_TEST_FALSE(FJwt{TEXT(".InvalidPayload")}.IsValid());
		AB_TEST_FALSE(FJwt{TEXT(".InvalidPayload.")}.IsValid());
		AB_TEST_FALSE(FJwt{TEXT(".InvalidPayload..")}.IsValid());
		AB_TEST_FALSE(FJwt{TEXT("InvalidHeader.InvalidPayload")}.IsValid());
		AB_TEST_FALSE(FJwt{TEXT("InvalidHeader.InvalidPayload.")}.IsValid());
		AB_TEST_FALSE(FJwt{TEXT("InvalidHeader.InvalidPayload..")}.IsValid());
		AB_TEST_FALSE(FJwt{TEXT("..InvalidSignature")}.IsValid());
		AB_TEST_FALSE(FJwt{TEXT("..InvalidSignature.")}.IsValid());
		AB_TEST_FALSE(FJwt{TEXT("InvalidHeader.InvalidPayload.InvalidSignature")}.IsValid());
		AB_TEST_FALSE(FJwt{TEXT("InvalidHeader.InvalidPayload.InvalidSignature.InvalidAnything")}.IsValid());
		AB_TEST_FALSE(FJwt{TEXT("eyJhbGciOiJSUzI1NiIsInR5cCI6IkpXUyJ9.InvalidPayload.InvalidSignature")}.IsValid());
		AB_TEST_FALSE(FJwt{TEXT("InvalidHeader.eyJzdWIiOiJJIn0.InvalidSignature")}.IsValid());
		AB_TEST_FALSE(FJwt{TEXT("eyJhbGciOiJSUzI1NiIsInR5cCI6IkpXUyJ9.eyJzdWIiOiJJIn0.InvalidSignature")}.IsValid());

		AB_TEST_TRUE(
			FJwt{
				TEXT(
					"eyJhbGciOiJSUzI1NiIsInR5cCI6IkpXUyJ9"
					"."
					"eyJpc3MiOiJhdXRoMCJ9"
					"."
					"__________________________________________________________________________________________________"
					"__________________________________________________________________________________________________"
					"__________________________________________________________________________________________________"
					"_______________________________________________w")
			}.IsValid());

		return true;
	});

	It("Given B64Url RSA n and e, When construct FRsaPublicKey, valid only if n is 2048 bits and e is 24 bits", [this]()
	{
		AB_TEST_FALSE((FRsaPublicKey{TEXT(""), TEXT("")}).IsValid());
		AB_TEST_FALSE((FRsaPublicKey{TEXT(""), TEXT("AQAB")}).IsValid());
		AB_TEST_FALSE((FRsaPublicKey{TEXT("XXXXXXXXXXXX"), TEXT("")}).IsValid());
		AB_TEST_FALSE((FRsaPublicKey{TEXT("XXXXXXXXXXXX"), TEXT("AQAB")}).IsValid());

		AB_TEST_FALSE(
			(FRsaPublicKey{
				TEXT(
					"________________________________________________________________"
					"________________________________________________________________"
					"________________________________________________________________"
					"________________________________________________________________"
					"________________________________________________________________"
					"_____________________w"),
				TEXT("AAA")
			}).IsValid());
		
		AB_TEST_TRUE(
			(FRsaPublicKey{
				TEXT(
					"________________________________________________________________"
					"________________________________________________________________"
					"________________________________________________________________"
					"________________________________________________________________"
					"________________________________________________________________"
					"_____________________w"),
				TEXT("AQAB")
			}).IsValid());
	
		return true;
	});
	
	It("Given a valid FRsaPublicKey with B64Url parameters, When convert to PEM, Then results in armored PEM", [this]()
	{
		FRsaPublicKey const Key{
			TEXT(
				"________________________________________________________________"
				"________________________________________________________________"
				"________________________________________________________________"
				"________________________________________________________________"
				"________________________________________________________________"
				"_____________________w"),
			TEXT("AQAB")
		};

		FString const ExpectedPem =
			TEXT(
				"-----BEGIN PUBLIC KEY-----\n"
				"MIIBIjANBgkqhkiG9w0BAQEFAAOCAQ8AMIIBCgKCAQEA////////////////////"
				"////////////////////////////////////////////////////////////////"
				"////////////////////////////////////////////////////////////////"
				"////////////////////////////////////////////////////////////////"
				"////////////////////////////////////////////////////////////////"
				"////////////////////////////////////////////////////////////////"
				"/wIDAQAB\n"
				"-----END PUBLIC KEY-----");
	
		AB_TEST_EQUAL(Key.ToPem(), ExpectedPem);
	
		return true;
	});

	It("Given an invalid FJwt, When verify, Then results in MalformedJwt", [this]()
	{
		FJwt const Jwt{TEXT("")};

		AB_TEST_EQUAL(Jwt.VerifyWith(FRsaPublicKey{TEXT(""), TEXT("")}), EJwtResult::MalformedJwt);

		return true;
	});

	It("Given an invalid FRsaPublicKey, When verify, Then results in MalformedPublicKey", [this]()
	{
		FJwt const Jwt{
			TEXT(
				"eyJhbGciOiJSUzI1NiIsInR5cCI6IkpXUyJ9"
				"."
				"eyJpc3MiOiJhdXRoMCJ9"
				"."
				"________________________________________________________________"
				"________________________________________________________________"
				"________________________________________________________________"
				"________________________________________________________________"
				"________________________________________________________________"
				"_____________________w")
		};

		AB_TEST_EQUAL(Jwt.VerifyWith(FRsaPublicKey{TEXT(""), TEXT("")}), EJwtResult::MalformedPublicKey);

		return true;
	});

	It("Given valid FJwt and FRsaPublicKey, When verify, Then doesn't result in MalformedJwt or MalformedPublicKey", [this]()
	{
		FJwt const Jwt{
			TEXT(
				"eyJhbGciOiJSUzI1NiIsInR5cCI6IkpXUyJ9"
				"."
				"eyJpc3MiOiJhdXRoMCJ9"
				"."
				"________________________________________________________________"
				"________________________________________________________________"
				"________________________________________________________________"
				"________________________________________________________________"
				"________________________________________________________________"
				"_____________________w")
		};

		FRsaPublicKey const Key{
			TEXT(
				"________________________________________________________________"
				"________________________________________________________________"
				"________________________________________________________________"
				"________________________________________________________________"
				"________________________________________________________________"
				"_____________________w"),
			TEXT("AQAB")
		};
		
		AB_TEST_NOT_EQUAL(Jwt.VerifyWith(Key), EJwtResult::MalformedJwt);
		AB_TEST_NOT_EQUAL(Jwt.VerifyWith(Key), EJwtResult::MalformedPublicKey);

		return true;
	});

	It("Given 'alg' field in Jwt header, When call VerifyWith(FRsaPublicKey), Then only 'RS256' is supported", [this]()
	{
		auto VerifyWithHeader = [this](FString const& JwtHeader)
		{
			FString JwtString = JwtHeader;
			JwtString = FBase64::Encode(JwtString);
			JwtString.Append(
				TEXT(
					"."
					"eyJpc3MiOiJhdXRoMCJ9"
					"."
					"________________________________________________________________"
					"________________________________________________________________"
					"________________________________________________________________"
					"________________________________________________________________"
					"________________________________________________________________"
					"_____________________w"));

			FRsaPublicKey const Key{
				TEXT(
					"________________________________________________________________"
					"________________________________________________________________"
					"________________________________________________________________"
					"________________________________________________________________"
					"________________________________________________________________"
					"_____________________w"),
				TEXT("AQAB")
			};
			
			return FJwt{JwtString}.VerifyWith(Key);
		};
	
		AB_TEST_EQUAL(VerifyWithHeader(TEXT(R"({})")), EJwtResult::AlgorithmMismatch);
		AB_TEST_EQUAL(VerifyWithHeader(TEXT(R"({"alg":"none"})")), EJwtResult::AlgorithmMismatch);
		AB_TEST_EQUAL(VerifyWithHeader(TEXT(R"({"alg":"HS256"})")), EJwtResult::AlgorithmMismatch);
		AB_TEST_EQUAL(VerifyWithHeader(TEXT(R"({"alg":"ES256"})")), EJwtResult::AlgorithmMismatch);
		AB_TEST_NOT_EQUAL(VerifyWithHeader(TEXT(R"({"alg":"RS256"})")), EJwtResult::AlgorithmMismatch);
	
		return true;
	});

	It("Given valid Jwt access token, When extract its content, Then content is valid JSON", [this]()
	{
		FJwt const Jwt{
			TEXT(
				"eyJhbGciOiJSUzI1NiIsImtpZCI6IjdiMmE2NzEyYTZmNjQwMzEyYTlkODAzNjk1"
				"ZDdlNmNlNGQyN2I4ZGMiLCJ0eXAiOiJKV1QifQ"
				"."
				"eyJiYW5zIjpbXSwiY2xpZW50X2lkIjoiZTE4MjY5ZWRjOTY3NDlhOGExNzVmYjNi"
				"OTk4NTlhNDQiLCJjb3VudHJ5IjoiU0ciLCJkaXNwbGF5X25hbWUiOiIiLCJleHAi"
				"OjE2MzY4ODMyMTUsImlhdCI6MTYzNjg3OTYxNSwiaXNfY29tcGx5Ijp0cnVlLCJq"
				"ZmxncyI6NCwibmFtZXNwYWNlIjoic2RrdGVzdCIsIm5hbWVzcGFjZV9yb2xlcyI6"
				"W3sibmFtZXNwYWNlIjoiKiIsInJvbGVJZCI6IjIyNTE0Mzg4MzllOTQ4ZDc4M2Vj"
				"MGU1MjgxZGFmMDViIn1dLCJwZXJtaXNzaW9ucyI6W10sInJvbGVzIjpbIjIyNTE0"
				"Mzg4MzllOTQ4ZDc4M2VjMGU1MjgxZGFmMDViIl0sInNjb3BlIjoiYWNjb3VudCBj"
				"b21tZXJjZSBzb2NpYWwgcHVibGlzaGluZyBhbmFseXRpY3MiLCJzdWIiOiIyMjVi"
				"ODZkODNiNTQ0ODVkYTBiYzM2Y2Y2ZDY1NTVkNyJ9"
				"."
				"8_K7yiw9mNCdqHLbAe8ju9JzKC4psY15FmpDpvRkqo3WWncqth5gwdekblZFKfA8"
				"HpXwihXsoECjLTDLqTBy5t4Chkdn5aTcZh03lGda2gQ2spL5TKufutaNqYaQBoEQ"
				"OplNTU3y2ScXcYZ_cHXixVsgiPFJes3_IPBQYCWwOlHGB6BjyCcNpGRl3gAT_tnJ"
				"vjb2rSTVq1xzZNqQCBOMWAhbnFoJC6xNoLCNPYXwBWtvdd7zB70ZQbEhkz2HolXf"
				"CSNZhfWRiNxONQ9BIFjEOWC2Vep9kftNs7lwlfSbkMdo94tMdk4LLPDXuhjQ8bef"
				"j5J9ma6kwIiEWdLfitJM5Q")
			};

		AB_TEST_TRUE(Jwt.Header()->HasField("alg"));
		AB_TEST_TRUE(Jwt.Payload()->HasField("sub"));
		AB_TEST_TRUE(Jwt.Payload()->HasField("iat"));

		return true;
	});

	It("Given valid FJwt with correct signature and FRsaPublicKey, When verify, Then results in Ok", [this]()
	{
		FJwt const Jwt{
			TEXT(
				"eyJhbGciOiJSUzI1NiIsImtpZCI6IjdiMmE2NzEyYTZmNjQwMzEyYTlkODAzNjk1"
				"ZDdlNmNlNGQyN2I4ZGMiLCJ0eXAiOiJKV1QifQ"
				"."
				"eyJiYW5zIjpbXSwiY2xpZW50X2lkIjoiZTE4MjY5ZWRjOTY3NDlhOGExNzVmYjNi"
				"OTk4NTlhNDQiLCJjb3VudHJ5IjoiU0ciLCJkaXNwbGF5X25hbWUiOiIiLCJleHAi"
				"OjE2MzY4ODMyMTUsImlhdCI6MTYzNjg3OTYxNSwiaXNfY29tcGx5Ijp0cnVlLCJq"
				"ZmxncyI6NCwibmFtZXNwYWNlIjoic2RrdGVzdCIsIm5hbWVzcGFjZV9yb2xlcyI6"
				"W3sibmFtZXNwYWNlIjoiKiIsInJvbGVJZCI6IjIyNTE0Mzg4MzllOTQ4ZDc4M2Vj"
				"MGU1MjgxZGFmMDViIn1dLCJwZXJtaXNzaW9ucyI6W10sInJvbGVzIjpbIjIyNTE0"
				"Mzg4MzllOTQ4ZDc4M2VjMGU1MjgxZGFmMDViIl0sInNjb3BlIjoiYWNjb3VudCBj"
				"b21tZXJjZSBzb2NpYWwgcHVibGlzaGluZyBhbmFseXRpY3MiLCJzdWIiOiIyMjVi"
				"ODZkODNiNTQ0ODVkYTBiYzM2Y2Y2ZDY1NTVkNyJ9"
				"."
				"8_K7yiw9mNCdqHLbAe8ju9JzKC4psY15FmpDpvRkqo3WWncqth5gwdekblZFKfA8"
				"HpXwihXsoECjLTDLqTBy5t4Chkdn5aTcZh03lGda2gQ2spL5TKufutaNqYaQBoEQ"
				"OplNTU3y2ScXcYZ_cHXixVsgiPFJes3_IPBQYCWwOlHGB6BjyCcNpGRl3gAT_tnJ"
				"vjb2rSTVq1xzZNqQCBOMWAhbnFoJC6xNoLCNPYXwBWtvdd7zB70ZQbEhkz2HolXf"
				"CSNZhfWRiNxONQ9BIFjEOWC2Vep9kftNs7lwlfSbkMdo94tMdk4LLPDXuhjQ8bef"
				"j5J9ma6kwIiEWdLfitJM5Q")
		};

		FRsaPublicKey const Key{
			TEXT(
				"-u8A8klvZ6V4ba_JTEjiIvhPy8uimJGAMfpWFrOntX3YUgoTM_RdPyRDQ8FdNEKN"
				"yyvQESObVoqx6ElcdFWvL3PN4huVDo5fUe8EK8jRhUe1tsjyswT14zV_a5LSgIi4"
				"0VNFqGcgvwHI_Z2KOk8683LEMIBARq3pBDBPVoqW7EFknNsWLMpIR-d15qkCViht"
				"20bBCbP7hSZ7qPKMZHplPC39zAYO8GjuOyWQfu5VrL5jvTdj0j2jK6d4tOvvXPLI"
				"AqfwvjFYgQFbDUVUDJb0LXs-aQ7Jos6EQImKqlk9bnB1neK3ZjpPuzsEkrQrnqz9"
				"-zfInD8aC_efFpkgWMOG2Q"),
			TEXT("AQAB")
		};
		
		AB_TEST_EQUAL(Jwt.VerifyWith(Key), EJwtResult::Ok);

		return true;
	});
}
