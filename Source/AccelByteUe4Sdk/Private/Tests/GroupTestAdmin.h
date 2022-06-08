// Copyright (c) 2022 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "TestUtilities.h"
#include "GroupTestAdmin.generated.h"

#pragma region Models

/** @brief 1~15. TODO: Move this to IAm or common area */
UENUM(BlueprintType)
enum class EAccelByteRoleAction : uint8
{
	/** @brief API will not accept 0 */
	None = 0,
	
	Create = 1,
	Read = 2,
	CreateRead = 3,
	Update = 4,
	UpdateCreate = 5,
	UpdateRead = 6,
	UpdateReadCreate = 7,
	Delete = 8,
	DeleteCreate = 9,
	DeleteRead = 10,
	DeleteReadCreate = 11,
	DeleteUpdate = 12,
	DeleteUpdateCreate = 13,
	DeleteUpdateRead = 14,

	/** @brief All Permissions */
	DeleteUpdateReadCreate = 15,
};

/**
* @brief This endpoint is used to initiate configuration.
* This endpoint will automatically create default configuration and member roles with default permission
* Default Permission for admin role will cover these permission:
*	Permission to invite user to group
*	Permission to accept or reject join request
*	Permission to kick group member
*	Default max member value will be 50 and global rules will be empty
 */
USTRUCT(BlueprintType)
struct FInitGroupConfigRequest
{
	GENERATED_BODY()
	
	/** @brief Set internally: No need to set this while testing. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Group | InitGroupConfigRequest")
	FString Namespace{};
};

/**
 * @brief Contains `configurationCode` for CreateGroup.
 * Response for: FInitGroupConfigRequest, or as 'data' (TArray<this>) from other related.
 */
USTRUCT(BlueprintType)
struct FGroupConfigResponse
{
	GENERATED_BODY()
	/** @brief Used as a param for CreateGroup */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Group | GroupConfigResponse")
	FString ConfigurationCode{};
};

/** @brief Contains `configurationCode` for CreateGroup. */
USTRUCT(BlueprintType)
struct FGetGroupConfigListResponse
{
	GENERATED_BODY()
	/** @brief Used as a param for CreateGroup */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Group | GetGroupConfigListResponse")
	TArray<FGroupConfigResponse> Data{};
	FAccelByteModelsPaging Paging{};
};

/**
 * @brief GET: List of existing configuration, used as the main rule of the service.
 * Each namespace will have its own configuration.
 * 
 */
USTRUCT(BlueprintType)
struct FGetGroupConfigListRequest
{
	GENERATED_BODY()
	
	/** @brief Set internally: No need to set this while testing. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Group | GetGroupConfigListRequest")
	FString Namespace{};
	
	/** @brief Optional: Size of displayed data */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Group | GetGroupConfigListRequest")
	int32 Limit{1};
	
	/** @brief Optional: Start position that points to query data */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Group | GetGroupConfigListRequest")
	int32 Offset{0};
};

/**
 * @brief This endpoint is used to delete group configuration.
 * This Configuration is used as the main rule of the service.
 * Each namespace will have its own configuration.
 *
 * => Success response is void
 */
USTRUCT(BlueprintType)
struct FDeleteGroupConfigRequest
{
	GENERATED_BODY()
	
	/** @brief Set internally: No need to set this while testing. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Group | DeleteGroupConfigRequest")
	FString Namespace{};
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Group | DeleteGroupConfigRequest")
	FString ConfigurationCode{};
};

/** @brief for FCreateMemberRoleRequest */
USTRUCT(BlueprintType)
struct FRolePermission
{
	GENERATED_BODY()

	/**
	 * @brief 1~15 from EAccelByteRoleAction: Combinations for Delete, Update, Read, Create
	 * - 15 == All Permissions
	 * - Not to be confused with an API doc ~5-digit "Action Code"
	 * - Use SetAction() to set via the EAccelByteRoleAction enum
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Group | RolePermission")
	uint8 Action{};
	
	/**
	 * @brief "Required Member Role Permission" mentions from api docs.
	 *
	 * Example:
	 	 * - Requires ResourceName "GROUP:INVITE" (where CREATE is action 
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Group | RolePermission")
	FString ResourceName{};

	/**
	 * @brief The same as setting Action manually, but with more context (via enum).
	 * @param NewAction 
	 */
	void SetAction(EAccelByteRoleAction NewAction)
	{
		Action = static_cast<uint8>(NewAction);
	}
};

/** @brief for FCreateMemberRoleRequest */
USTRUCT(BlueprintType)
struct FCreateMemberRoleRequest
{
	GENERATED_BODY()
	
	/** @brief Arbitrary name */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Group | CreateMemberRoleRequest")
	FString MemberRoleName{};
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Group | CreateMemberRoleRequest")
	TArray<FRolePermission> MemberRolePermissions{};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FCreateMemberRoleResponse
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Group | CreateMemberRoleResponse")
	FString MemberRoleId{};
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Group | CreateMemberRoleResponse")
	FString MemberRoleName{};
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Group | CreateMemberRoleResponse")
	TArray<FRolePermission> MemberRolePermissions{};
};
#pragma endregion /Models


/**
 * @brief Initiate configuration.
 * - This endpoint will automatically create default configuration and member roles with default permission
 * - Default Permission for admin role will cover these permission:
 *   - Permission to invite user to group
 *	 - Permission to accept or reject join request
 *	 - Permission to kick group member
 * - Default max member value will be 50 and global rules will be empty
 *
 * Action Code: 73104
 *	
 * @param OnSuccess 
 * @param OnError 
 */
void AdminInitGroupConfig(const THandler<FGroupConfigResponse>& OnSuccess, const FErrorHandler& OnError);

/**
 * @brief Get list of existing configurations
 * - Used as the main rule of the service.
 * - Each namespace will have its own configuration.
 *
 * Action Code: 73101
 * 
 * @param GetGroupConfigListRequest 
 * @param OnSuccess 
 * @param OnError 
 */
void AdminGetGroupConfigList(
	FGetGroupConfigListRequest& GetGroupConfigListRequest,
	const THandler<FGetGroupConfigListResponse>& OnSuccess,
	const FErrorHandler& OnError);

/**
 * @brief Delete a group configuration.
 * - Required permission 'ADMIN:NAMESPACE:{namespace}:GROUP:CONFIGURATION [DELETE]'
 * - This Configuration is used as the main rule of the service.
 * - Each namespace will have its own configuration
 * 
 * @param GroupConfigurationCode
 * @param OnSuccess 
 * @param OnError 
 */
void AdminDeleteGroupConfig(
	const FString& GroupConfigurationCode,
	const FVoidHandler& OnSuccess,
	const FErrorHandler& OnError);

 /**
 * @brief Creates a new member role 
 * - Required permission ADMIN:NAMESPACE:{namespace}:GROUP:ROLE [CREATE]
 * 
 * Action Code: 73202
 * 
 * @param CreateMemberRoleRequest { MemberRoleName:str, MemberRolePermissions:[] }
 * @param OnSuccess 
 * @param OnError 
 */
void AdminCreateMemberRole(
	const FCreateMemberRoleRequest& CreateMemberRoleRequest,
	const THandler<FCreateMemberRoleResponse>& OnSuccess,
	const FErrorHandler& OnError);
