### TABLE OF CONTENT ###
- [Table of Content](#table-of-content)
- [JUSTICE IDENTITY](#justice-identity)
- [PLATFORM SERVICE](#platform-service)
    - [JUSTICE CATALOG](#justice-catalog)
    - [JUSTICE PURCHASE](#justice-purchase)
    - [JUSTICE WALLET](#justice-wallet)
    - [USER PROFILE](#user-profile)

## JUSTICE IDENTITY ##
Justice Identity is the first service that required before we can proceed to platform service. 

* #### RegisterNewPlayer

    This function will register a new user with email-based account. It requires user's **email** (UserID), **password**, and **displayName** that will be registered through the parameter. It'll return a complete information from the user that has been registered, wrapped in [UserCreateResponse](../models/#usercreateresponse) object from the [FRegisterPlayerCompleteDelegate](../callback/#fregisterplayercompletedelegate) callback.
    <pre class="prettyprint lang-cpp"> 
        void JusticeIdentity::RegisterNewPlayer(
            FString UserID, 
            FString Password, 
            FString DisplayName, 
            FUserAuthTypeJustice AuthType, 
            FRegisterPlayerCompleteDelegate OnComplete){}
    </pre>

    Usage:
    <pre class="prettyprint lang-cpp"> 

        void AJusticeSDKDemoGameModeBase::BeginPlay()
        {
            FString LoginID = "johnDoe@example.com";
            FString Password = "password";
            FString DisplayName = "John_Doe";
            FUserAuthTypeJustice AuthType = Email;
            JusticeIdentity::RegisterNewPlayer(LoginID, Password, DisplayName, AuthType, 
                FRegisterPlayerCompleteDelegate::CreateLambda(
                    [](bool bSuccessful, FString ErrorStr, UserCreateResponse* Response)
                    {
                        UE_LOG(LogTmp, Log, TEXT("Register User Result: %s"), bSuccessful ? TEXT("Success") : TEXT("Failed"));
                        UE_LOG(LogTmp, Log, TEXT("User's registered display name is %s"), *Response->DisplayName)
                        UE_LOG(LogTmp, Log, TEXT("User's registered UserID is %s"), *Response->UserID)
                    }
                );
            );
        }
    </pre>

* #### ReissueVerificationCode

    This function should be called after register a new user account. This method requires user's ID (**UserID**) and user's email (**LoginId**) as parameters. User ID can be obtained from token response that obtained after user login. It will return [default](../callback/#fdefaultcompletedelegate) callback.
    <pre class="prettyprint lang-cpp"> 
    void JusticeIdentity::ReissueVerificationCode(
        FString UserID, 
        FString LoginID, 
        FDefaultCompleteDelegate OnComplete){}
    </pre>

    Usage:
    <pre class="prettyprint lang-cpp"> 

        void AJusticeSDKDemoGameModeBase::BeginPlay()
        {
            FString UserID = "1223334444";
            FString LoginID = "johnDoe@example.com";
            FUserAuthTypeJustice AuthType = Email;
            JusticeIdentity::ReissueVerificationCode(UserID, LoginID, 
                FDefaultCompleteDelegate::CreateLambda(
                    [](bool bSuccessful, FString ErrorStr)
                    {
                        UE_LOG(LogTmp, Log, TEXT("Issue Verification Code Result: %s"), bSuccessful ? TEXT("Success") : TEXT("Failed"));
                    }
                );
            );
        }
    </pre>

* #### VerifyNewPlayer

    This function will verify the email that registered, **AFTER** user received the verification code that has been send with [ReissueVerificationCode](../functions/#reissueverificationcode) to their email. This method requires **user's ID** (UserID), **VerificationCode**, **[AuthType](../models/#fuserauthtypejustice)** as parameters. It will return [default](../callback/#fdefaultcompletedelegate) callback.
    <pre class="prettyprint lang-cpp"> 
    void JusticeIdentity::VerifyNewPlayer(
        FString UserID, 
        FString VerificationCode, 
        FUserAuthTypeJustice AuthType, 
        FDefaultCompleteDelegate OnComplete){}
    </pre>

    Usage:
    <pre class="prettyprint lang-cpp"> 

        void AJusticeSDKDemoGameModeBase::BeginPlay()
        {
            FString UserID = "1223334444";
            FString VerificationCode = "13579";
            FUserAuthTypeJustice AuthType = Email;
            JusticeIdentity::VerifyNewPlayer(UserID, VerificationCode, AuthType, 
                FDefaultCompleteDelegate::CreateLambda(
                    [](bool bSuccessful, FString ErrorStr)
                    {
                        UE_LOG(LogTmp, Log, TEXT("Verify Registration Result: %s"), bSuccessful ? TEXT("Success") : TEXT("Failed"));
                    }
                );
            );
        }
    </pre>

* #### UserLogin

    Login the user with their email account. User should insert their **email** (LoginID) and **Password** as parameters. It will return [OAuthTokenJustice](../models/#oauthtokenjustice) from [FUserLoginCompleteDelegate](../callback/#fuserlogincompletedelegate) callback.
    <pre class="prettyprint lang-cpp"> 
    void JusticeIdentity::UserLogin(
        FString LoginID, 
        FString Password, 
        FUserLoginCompleteDelegate OnComplete)
    </pre>

    Usage:
    <pre class="prettyprint lang-cpp"> 

        void AJusticeSDKDemoGameModeBase::BeginPlay()
        {
            FString LoginID = "johnDoe@example.com";
            FString Password = "password";
            JusticeIdentity::UserLogin(LoginID, Password, 
                FUserLoginCompleteDelegate::CreateLambda(
                    [](bool bSuccessful, FString ErrorStr, *OAuthTokenJustice Token)
                    {
                        UE_LOG(LogTmp, Log, TEXT("User Login with Email Result: %s"), bSuccessful ? TEXT("Success") : TEXT("Failed"));
                        UE_LOG(LogTmp, Log, TEXT("User's Display Name is %s"), *Token->DisplayName);
                    }
                );
            );
        }
    </pre>

* #### DeviceLogin

    This function allow user to login without entering any information, it'll automatically obtain device's information as their account. It will return [OAuthTokenJustice](../models/#oauthtokenjustice) from [FUserLoginCompleteDelegate](../callback/#fuserlogincompletedelegate) callback.
    <!--language lang-cpp -->

        void JusticeIdentity::DeviceLogin(FUserLoginCompleteDelegate OnComplete){}

    Usage:
    <!--language lang-cpp -->

        void AJusticeSDKDemoGameModeBase::BeginPlay()
        {
            JusticeIdentity::DeviceLogin(FUserLoginCompleteDelegate::CreateLambda(
                [](bool bSuccessful, FString ErrorStr, *OAuthTokenJustice Token)
                {
                    UE_LOG(LogTmp, Log, TEXT("User Login with Device: %s"), bSuccessful ? TEXT("Success") : TEXT("Failed"));
                    UE_LOG(LogTmp, Log, TEXT("User's Display Name is %s"), *Token->DisplayName);
                }
            ););
        }

* #### AuthCodeLogin

    This function will login the user automatically from Justice Launcher with their account on Justice Launcher too. **AuthCode** parameter should be filled with "JUSTICE_AUTHORIZATION_CODE" environment variable. **RedirectURI** is optional. This function will return [OAuthTokenJustice](../models/#oauthtokenjustice) from [FUserLoginCompleteDelegate](../callback/#fuserlogincompletedelegate) callback.
    <!--language lang-cpp -->

        void JusticeIdentity::AuthCodeLogin(FString AuthCode, FString RedirectURI, FUserLoginCompleteDelegate OnComplete){}

    Usage:
    <!--language lang-cpp -->

        void AJusticeSDKDemoGameModeBase::BeginPlay()
        {
            FString AuthCode = "0123456789";
            FString RedirectURI = "https://example.com";
            JusticeIdentity::AuthCodeLogin(AuthCode, RedirectURI, FUserLoginCompleteDelegate::CreateLambda(
                [](bool bSuccessful, FString ErrorStr, *OAuthTokenJustice Token)
                {
                    UE_LOG(LogTmp, Log, TEXT("User Login with Authorization Code (Launcher) Result: %s"), bSuccessful ? TEXT("Success") : TEXT("Failed"));
                    UE_LOG(LogTmp, Log, TEXT("User's Display Name is %s"), *Token->DisplayName);
                }
            ););
        }

* #### UpgradeHeadlessAccount

    This function will upgrade user's headless account. Headless account is an account that doesn't have an email-password. If the user login with device/platform and they can't login with email-password, their account is considered as headless account. It requires **Email** and **Password** as parameters. It will return [default](../callback/#fdefaultcompletedelegate) callback.
    <!--language lang-cpp -->

        void JusticeIdentity::UpgradeHeadlessAccount(FString Email, FString Password, FDefaultCompleteDelegate OnComplete){}

    Usage:
    <!--language lang-cpp -->

        void AJusticeSDKDemoGameModeBase::BeginPlay()
        {
            // Login with device first if you want to upgrade it with email and password

            FString Email = "johnDoe@example.com";
            FString Password = "password";
            JusticeIdentity::UpgradeHeadlessAccount(Email, Password, FDefaultCompleteDelegate::CreateLambda(
                [](bool bSuccessful, FString ErrorStr)
                {
                    UE_LOG(LogTmp, Log, TEXT("User Upgrade Headless Account Result: %s"), bSuccessful ? TEXT("Success") : TEXT("Failed"));
                }
            ););
        }

* #### ForgotPassword

    Send a request to reset user's password. This function requires user's email to reset their password. It requires email as **LoginID** parameter. It will return [default](../callback/#fdefaultcompletedelegate) callback.
    <!--language lang-cpp -->

        void JusticeIdentity::ForgotPassword(FString LoginID, FDefaultCompleteDelegate OnComplete){}

    Usage:
    <!--language lang-cpp -->

        void AJusticeSDKDemoGameModeBase::BeginPlay()
        {
            FString LoginID = "johnDoe@example.com";
            JusticeIdentity::ForgotPassword(LoginID, FDefaultCompleteDelegate::CreateLambda(
                [](bool bSuccessful, FString ErrorStr)
                {
                    UE_LOG(LogTmp, Log, TEXT("User Request to Reset Password Result: %s"), bSuccessful ? TEXT("Success") : TEXT("Failed"));
                }
            ););
        }

* #### ResetPassword

    This function will verify the user's [request to reset password](#forgotpassword). User should insert their **email** (UserID), **VerificationCode** that has been sent to their email, and **NewPassword** as parameters. It will return [default](../callback/#fdefaultcompletedelegate) callback.
    <!--language lang-cpp -->

        void JusticeIdentity::ResetPassword(FString UserID, FString VerificationCode, FString NewPassword, FDefaultCompleteDelegate OnComplete){}
    
    Usage:
    <!--language lang-cpp -->

        void AJusticeSDKDemoGameModeBase::BeginPlay()
        {
            FString VerificationCode = "123456";
            FString UserID = "johnDoe@example.com";
            FString NewPassword = "freshPassword";
            JusticeIdentity::ResetPassword(UserID, VerificationCode, NewPassword, FDefaultCompleteDelegate::CreateLambda(
                [](bool bSuccessful, FString ErrorStr)
                {
                    UE_LOG(LogTmp, Log, TEXT("User Reset Password Result: %s"), bSuccessful ? TEXT("Success") : TEXT("Failed"));
                }
            ););
        }

* #### LinkPlatform

    This function will link user's current account to another account in another platform. Required parameter is **PlatformID** and **Ticket**. Ticket for each platform (PlatformToken) can be obtained from browser with platform linking URL (e.g. Facebook, Google, Twitch platform). The browser will redirect the url to a site with a code in the form parameter URL. This function will return [default](../callback/#fdefaultcompletedelegate) callback.
    <!--language lang-cpp -->

        void JusticeIdentity::LinkPlatform(FString PlatformID, FString Ticket, FDefaultCompleteDelegate OnComplete){}

    Usage:
    <!--language lang-cpp -->
    
        void AJusticeSDKDemoGameModeBase::BeginPlay()
        {
            // Login user first

            FString PlatformID = "facebook";
            FString Ticket = "0123456789";
            JusticeIdentity::LinkPlatform(PlatformID, Ticket, FDefaultCompleteDelegate::CreateLambda(
                [](bool bSuccessful, FString ErrorStr)
                {
                    UE_LOG(LogTmp, Log, TEXT("User Linking Account Result: %s"), bSuccessful ? TEXT("Success") : TEXT("Failed"));
                }
            ););
        }

* #### UnlinkPlatform

    This function unlink user's platfrom account that linked with current account. It only requires the **PlatformID** as the parameter. That platform account will be unlinked. It will return [default](../callback/#fdefaultcompletedelegate) callback.

    <!--language lang-cpp -->

        void JusticeIdentity::UnlinkPlatform(FString PlatformID, FDefaultCompleteDelegate OnComplete){}

    Usage:
    <!--language lang-cpp -->

        void AJusticeSDKDemoGameModeBase::BeginPlay()
        {
            // Login user first

            FString PlatformID = "facebook";
            JusticeIdentity::UnlinkPlatform(PlatformID, FDefaultCompleteDelegate::CreateLambda(
                [](bool bSuccessful, FString ErrorStr)
                {
                    UE_LOG(LogTmp, Log, TEXT("User Unlinking Account Result: %s"), bSuccessful ? TEXT("Success") : TEXT("Failed"));
                }
            ););
        }

* #### GetLinkedUserPlatform

    Get user's platform accounts those have been linked. It doesn't need any parameter. It will return a TArray<[LinkedPlatform](../models/#linkedplatform)> from [FGetLinkedPlatformCompleteDelegate](../callback/#fgetlinkedplatformcompletedelegate) callback.
    <!--language lang-cpp -->

        void JusticeIdentity::GetLinkedPlatform(FGetLinkedPlatformCompleteDelegate OnComplete){}

    Usage:
    <!--language lang-cpp -->

        void AJusticeSDKDemoGameModeBase::BeginPlay()
        {
            // Login user first

            JusticeIdentity::GetLinkedPlatform(FGetLinkedPlatformCompleteDelegate::CreateLambda(
                [](bool bSuccessful, FString ErrorStr, TArray<LinkedPlatform> LinkedPlatforms)
                {
                    UE_LOG(LogTmp, Log, TEXT("User Get Linked Account Result: %s"), bSuccessful ? TEXT("Success") : TEXT("Failed"));
                    UE_LOG(LogTmp, Log, TEXT("Found Linked Account: %d"), LinkedPlatforms.Num());
                }
            ););
        }

* #### Logout

    Logout the user's from game. It doesn't require any parameter. It will return [default](../callback/#fdefaultcompletedelegate) callback.
    <!--language lang-cpp -->

        void JusticeIdentity::UserLogout(FDefaultCompleteDelegate OnComplete)

    Usage:
    <!--language lang-cpp -->

        void AJusticeSDKDemoGameModeBase::BeginPlay()
        {
            // Login user first

            JusticeIdentity::UserLogout(FDefaultCompleteDelegate::CreateLambda(
                [](bool bSuccessful, FString ErrorStr)
                {
                    UE_LOG(LogTmp, Log, TEXT("User Logout Result: %s"), bSuccessful ? TEXT("Success") : TEXT("Failed"));
                }
            ););
        }

## PLATFORM SERVICE ##
User should be logged in first before it can use platform. Please refer to [Justice Identity](#justice-identity) to see how to log in user.
### JUSTICE CATALOG ###

* #### GetCategory

    Get category from a store in a namespace. Required parameter is **ParentPath** and optional parameter is **Language**. It'll return the TArray<[Category](../models/#category)> from the [FCategoryDefaultCompleteDelegate](../callback/#fcategorydefaultcompletedelegate) callback.
    <!--language lang-cpp -->
    
        void JusticeCatalog::GetCategory(
            FString ParentPath, 
            FString Language, 
            FCategoryDefaultCompleteDelegate OnComplete){}

    Usage:
    <!--language lang-cpp -->
    
        void AJusticeSDKDemoGameModeBase::BeginPlay()
        {
            FString ParentPath = "/game/weapon/example";
            FString Language = "en";
            JusticeCatalog::GetCategory(ParentPath, Language,
                FCategoryDefaultCompleteDelegate::CreateLambda(
                    [](bool bSuccessful, FString ErrorStr, TArray<Category> Result)
                    {
                        UE_LOG(LogTmp, Log, TEXT("Get Category Result: %s"), bSuccessful ? TEXT("Success") : TEXT("Failed"));
                        UE_LOG(LogTmp, Log, TEXT("Category Found : %d"), Result.Num());                        
                    }
                );
            );
        }

* #### GetRootCategory

    Get root categories those exists in the same namespace with user's namespace. Optional parameter is **Language**. It'll return the root TArray<[Category](../models/#category)> from the [FCategoryDefaultCompleteDelegate](../callback/#fcategorydefaultcompletedelegate) callback.
    <!--language lang-cpp -->
    
        void JusticeCatalog::GetRootCategory(FString Language, FCategoryDefaultCompleteDelegate OnComplete){}

    Usage:
    <!--language lang-cpp -->

        void AJusticeSDKDemoGameModeBase::BeginPlay()
        {
            FString Language = "en";
            JusticeCatalog::GetRootCategory(Language,
                FCategoryDefaultCompleteDelegate::CreateLambda(
                    [](bool bSuccessful, FString ErrorStr, TArray<Category> Result)
                    {
                        UE_LOG(LogTmp, Log, TEXT("Get Root Category Result: %s"), bSuccessful ? TEXT("Success") : TEXT("Failed"));
                        UE_LOG(LogTmp, Log, TEXT("Root Category Found : %d"), Result.Num());                        
                    }
                );
            );
        }

* #### GetChildCategory

    Get child categories from a category. This method requires the parent path / **CategoryPath** parameter and optional **Language** parameter. It'll return a child TArray<[Category](../models/#category)> from the [FCategoryDefaultCompleteDelegate](../callback/#fcategorydefaultcompletedelegate) callback.
    <!--language lang-cpp -->

        void JusticeCatalog::GetChildCategory(
            FString Language, 
            FString CategoryPath, 
            FCategoryDefaultCompleteDelegate OnComplete){}
    
    Usage:
    <!--language lang-cpp -->

        void AJusticeSDKDemoGameModeBase::BeginPlay()
        {
            FString Language = "en";
            FString CategoryPath = "/game/weapon";
            JusticeCatalog::GetChildCategory(Language, CategoryPath,
                FCategoryDefaultCompleteDelegate::CreateLambda(
                    [](bool bSuccessful, FString ErrorStr, TArray<Category> Result)
                    {
                        UE_LOG(LogTmp, Log, TEXT("Get Child Category Result: %s"), bSuccessful ? TEXT("Success") : TEXT("Failed"));
                        UE_LOG(LogTmp, Log, TEXT("Child Category Found : %d"), Result.Num());                        
                    }
                );
            );
        }

* #### GetDescendantCategory

    Get every descendant categories from a category. Required parameter is **CategoryPath** and optional parameter is **Language**. It'll return a descendant TArray<[Category](../models/#category)> from the [FCategoryDefaultCompleteDelegate](../callback/#fcategorydefaultcompletedelegate) callback.
    <!--language lang-cpp -->

        void JusticeCatalog::GetDescendantCategory(
            FString Language, 
            FString CategoryPath, 
            FCategoryDefaultCompleteDelegate OnComplete){}

    Usage:
    <!--language lang-cpp -->

        void AJusticeSDKDemoGameModeBase::BeginPlay()
        {
            FString Language = "en";
            FString CategoryPath = "/game/potion";
            JusticeCatalog::GetDescendantCategory(Language, CategoryPath,
                FCategoryDefaultCompleteDelegate::CreateLambda(
                    [](bool bSuccessful, FString ErrorStr, TArray<Category> Result)
                    {
                        UE_LOG(LogTmp, Log, TEXT("Get Descendant Category Result: %s"), bSuccessful ? TEXT("Success") : TEXT("Failed"));
                        UE_LOG(LogTmp, Log, TEXT("Descendant Category Found: %d"), Result.Num());                        
                    }
                );
            );
        }

* #### GetItem

    Get an item information from a store within the user's namespace. Required parameters are **ItemID** and **Region**. Optional parameter is **Language**. It'll return an [ItemInfo](../models/#iteminfo) from the [FGetItemCompleteDelegate](../callback/#fgetitemcompletedelegate) callback.
    <!--language lang-cpp -->

        void JusticeCatalog::GetItem(
            FString ItemID, 
            FString Region, 
            FString Language, 
            FGetItemCompleteDelegate OnComplete){}

    Usage:
    <!--language lang-cpp -->

        void AJusticeSDKDemoGameModeBase::BeginPlay()
        {
            FString ItemID = "123456789";
            FString Region = "US";
            FString Language = "en";
            JusticeCatalog::GetItem(ItemID, Region, Language,
                FGetItemCompleteDelegate::CreateLambda(
                    [](bool bSuccessful, FString ErrorStr, ItemInfo Result)
                    {
                        UE_LOG(LogTmp, Log, TEXT("Get Item Result: %s"), bSuccessful ? TEXT("Success") : TEXT("Failed"));
                        UE_LOG(LogTmp, Log, TEXT("Item name: %s"), *Result->Title;
                    }
                );
            );
        }

* #### GetItemByQuery

    Get an array of item with specific criteria/filter from a store within user's namespace. All paramaters is required, except the **Language** paramater. This function will return a TArray<[ItemInfo](../models/#iteminfo)> that fulfilled the criteria from the [FItemCompleteDelegate](../callback/#fitemcompletedelegate) callback.
    <!--language lang-cpp -->

        void JusticeCatalog::GetItemByQuery(
            FString Language, 
            FString Region, 
            FString CategoryPath, 
            FString ItemType, 
            FString Status, 
            int32 Page, 
            int32 Size, 
            FItemCompleteDelegate OnComplete){}

    Usage:
    <!--language lang-cpp -->

        void AJusticeSDKDemoGameModeBase::BeginPlay()
        {
            FString Language = "en";
            FString Region = "US";
            FString CategoryPath = "/game/armour";
            FString ItemType = "";
            FString Status = "ACTIVE";
            FString Page = "0";
            FString Size = "5";
            JusticeCatalog::GetItemByQuery(Language, Region, CategoryPath, ItemType, Status, Page, Size, 
                FItemCompleteDelegate::CreateLambda(
                    [](bool bSuccessful, FString ErrorStr, TArray<ItemInfo> Result)
                    {
                        UE_LOG(LogTmp, Log, TEXT("Get Item by Query Result: %s"), bSuccessful ? TEXT("Success") : TEXT("Failed"));
                        UE_LOG(LogTmp, Log, TEXT("Item found: %d"), Result.Num();
                    }
                );
            );
        }

### JUSTICE PURCHASE ###

* #### CreateNewOrder

    Create order to purchase something from store. It requires all parameters (**ItemID**, **Price**, **DiscountedPrice**, and **Currency**. It'll return [OrderInfo](../models/#orderinfo) from the [FOrderInfoCompleteDelegate](../callback/#forderinfocompletedelegate) callback.
    <!--language lang-cpp -->

        void JusticePurchase::CreateNewOrder(
            FString ItemID, 
            int32 Price, 
            int32 DiscountedPrice, 
            FString Currency, 
            FOrderInfoCompleteDelegate OnComplete){}

    Usage:
    <!--language lang-cpp -->

        void AJusticeSDKDemoGameModeBase::BeginPlay()
        {
            FString ItemID = "0123456789";
            int32 Price = 10;
            int32 DiscountedPrice = "8";
            FString Currency = "USD";
            JusticePurchase::CreateNewOrder(ItemID, Price, DiscountedPrice, Currency, 
                FOrderInfoCompleteDelegate::CreateLambda(
                    [](bool bSuccessful, FString ErrorStr, OrderInfo Result)
                    {
                        UE_LOG(LogTmp, Log, TEXT("Create Order Result: %s"), bSuccessful ? TEXT("Success") : TEXT("Failed"));
                        UE_LOG(LogTmp, Log, TEXT("Order Number: %s"), *Result->OrderNo;
                    }
                );
            );
        }

* #### GetUserOrder

    Get user's order information. It requires the order's number (**OrderNo**) as parameter. It'll return an [OrderInfo](../models/#orderinfo) from the [FOrderInfoCompleteDelegate](../callback/#forderinfocompletedelegate) callback.
    <!--language lang-cpp -->

        void JusticePurchase::GetUserOrder(
            FString OrderNo, 
            FOrderInfoCompleteDelegate OnComplete){}

    Usage:
    <!--language lang-cpp -->

        void AJusticeSDKDemoGameModeBase::BeginPlay()
        {
            FString OrderNo = "1223334444";
            JusticePurchase::GetUserOrder(OrderNo, 
                FOrderInfoCompleteDelegate::CreateLambda(
                    [](bool bSuccessful, FString ErrorStr, OrderInfo Result)
                    {
                        UE_LOG(LogTmp, Log, TEXT("Get User Order Result: %s"), bSuccessful ? TEXT("Success") : TEXT("Failed"));
                        UE_LOG(LogTmp, Log, TEXT("Order's Item ID: %s"), *Result->ItemID;
                    }
                );
            );
        }

* #### GetUserOrders

    Get all of user's order that has been created. It has two optional parameters; **Page** and **Size**. It'll return a TArray<[OrderInfo](../models/#orderinfo)> from the [FGetUserOrdersCompleteDelegate](../models/#fgetuserorderscompletedelegate) callback.
    <!--language lang-cpp -->

        void JusticePurchase::GetUserOrders(
            int32 Page, 
            int32 Size, 
            FGetUserOrdersCompleteDelegate OnComplete){}

    Usage:
    <!--language lang-cpp -->

        void AJusticeSDKDemoGameModeBase::BeginPlay()
        {
            int32 Page = "0";
            int32 Size = "20";
            JusticePurchase::GetUserOrders(Page, Size,
                FOrderInfoCompleteDelegate::CreateLambda(
                    [](bool bSuccessful, FString ErrorStr, TArray<OrderInfo> Result)
                    {
                        UE_LOG(LogTmp, Log, TEXT("Get User Orders Result: %s"), bSuccessful ? TEXT("Success") : TEXT("Failed"));
                        UE_LOG(LogTmp, Log, TEXT("User's order found : %d"), Result.Num();
                    }
                );
            );
        }

* #### GetUserOrderHistory

    Get history of an order that has been created. It requires an **OrderNo** parameter. It'll return an TArray<[OrderHistoryInfo](../models/#orderhistoryinfo)> from the [FGetUserOrderHistoryCompleteDelegate](../callback/#fgetuserorderhistorycompletedelegate) callback.

        void JusticePurchase::GetUserOrderHistory(
            FString OrderNo, 
            FGetUserOrderHistoryCompleteDelegate OnComplete){}

    Usage:
    <!--language lang-cpp -->
        
        void AJusticeSDKDemoGameModeBase::BeginPlay()
        {
            FString OrderNo = "1223334444";
            JusticePurchase::GetUserOrderHistory(OrderNo, 
                FGetUserOrderHistoryCompleteDelegate::CreateLambda(
                    [](bool bSuccessful, FString ErrorStr, TArray<OrderHistoryInfo> Result)
                    {
                        UE_LOG(LogTmp, Log, TEXT("Get User Order History Result: %s"), bSuccessful ? TEXT("Success") : TEXT("Failed"));
                        UE_LOG(LogTmp, Log, TEXT("Found %d order history."), Result.Num();
                    }
                );
            );
        }

* #### FulfillOrder

    Fulfill user's order. Required parameter is **OrderNo**. It'll return an [OrderInfo](../models/#orderinfo) from the [FOrderInfoCompleteDelegate](../callback/#forderinfocompletedelegate) callback.
    <!--language lang-cpp -->
        
        void JusticePurchase::FulfillOrder(
            FString OrderNo, 
            FOrderInfoCompleteDelegate OnComplete){}

    Usage:
    <!--language lang-cpp -->

        void AJusticeSDKDemoGameModeBase::BeginPlay()
        {
            FString OrderNo = "1223334444";
            JusticePurchase::FulfillOrder(OrderNo, 
                FOrderInfoCompleteDelegate::CreateLambda(
                    [](bool bSuccessful, FString ErrorStr, OrderInfo Result)
                    {
                        UE_LOG(LogTmp, Log, TEXT("Fulfill User Order Result: %s"), bSuccessful ? TEXT("Success") : TEXT("Failed"));
                        UE_LOG(LogTmp, Log, TEXT("Item ID that fulfilled is %s."), *Result->ItemID;
                    }
                );
            );
        }

### JUSTICE WALLET ###

* #### GetWalletBalance

    Get user's wallet information for a specific currency code. It requires **CurrencyCode** as a parameter. It'll return an **integer** number of the wallet's balance from the [FGetWalletBalanceCompleteDelegate](../callback/#fgetwalletbalancecompletedelegate)callback.
    <!--language lang-cpp -->

        void JusticeWallet::GetWalletBalance(
            FString CurrencyCode, 
            FGetWalletBalanceCompleteDelegate OnComplete){}

    Usage:
    <!--language lang-cpp -->

        void AJusticeSDKDemoGameModeBase::BeginPlay()
        {
            FString CurrencyCode = "USD";
            JusticeWallet::GetWalletBalance(CurrencyCode, 
                FGetWalletBalanceCompleteDelegate::CreateLambda(
                    [](bool bSuccessful, FString ErrorStr, int32 Balance)
                    {
                        UE_LOG(LogTmp, Log, TEXT("Get Wallet Result: %s"), bSuccessful ? TEXT("Success") : TEXT("Failed"));
                        UE_LOG(LogTmp, Log, TEXT("Wallet balance is %d."), *Balance;
                    }
                );
            );
        }

### USER PROFILE ###

* #### RequestCurrentPlayerProfile

    Get user's profile information. The profile should exists. It doesn't require any parameter. It'll return the [UserProfileInfo](../models/#userprofileinfo) from the [FReqestCurrentPlayerProfileCompleteDelegate](../callback/#freqestcurrentplayerprofilecompletedelegate) callback.
    <!--language lang-cpp -->

        void JusticePlatform::RequestCurrentPlayerProfile(
            FReqestCurrentPlayerProfileCompleteDelegate OnComplete){}

    Usage:
    <!--language lang-cpp -->

        void AJusticeSDKDemoGameModeBase::BeginPlay()
        {
            JusticePlatform::RequestCurrentPlayerProfile( 
                FReqestCurrentPlayerProfileCompleteDelegate::CreateLambda(
                    [](bool bSuccessful, FString ErrorStr, UserProfileInfo Result)
                    {
                        UE_LOG(LogTmp, Log, TEXT("Get User's Profile Result: %s"), bSuccessful ? TEXT("Success") : TEXT("Failed"));
                        UE_LOG(LogTmp, Log, TEXT("User's display name is %s."), *Result->DisplayName;
                    }
                );
            );
        }

* #### CreateDefaultPlayerProfile

    Create user's minimum profile information. It requires two parameter to be filled: **Email** and **DisplayName**. It will return [default](../callback/#fdefaultcompletedelegate) callback.
    <!--language lang-cpp -->

        void JusticePlatform::CreateDefaultPlayerProfile(
            FString Email, 
            FString DisplayName, 
            FDefaultCompleteDelegate OnComplete){}

    Usage:
    <!--language lang-cpp -->

        void AJusticeSDKDemoGameModeBase::BeginPlay()
        {
            FString Email = "johndoe@example.com";
            FString DisplayName = "JohnD03";
            JusticePlatform::CreateDefaultPlayerProfile(Email, DisplayName, 
                FDefaultCompleteDelegate::CreateLambda(
                    [](bool bSuccessful, FString ErrorStr)
                    {
                        UE_LOG(LogTmp, Log, TEXT("Create Default Profile Result: %s"), bSuccessful ? TEXT("Success") : TEXT("Failed"));
                    }
                );
            );
        }

* #### UpdatePlayerProfile

    Update user's current profile information. The profile should exists. It requires an object (**[UserProfileInfoUpdate](../models/#userprofileinfoupdate)**) as the parameter. It will return [default](../callback/#fdefaultcompletedelegate) callback.
    <!--language lang-cpp -->

        void JusticePlatform::UpdatePlayerProfile(
            UserProfileInfoUpdate newUserProfile, 
            FDefaultCompleteDelegate OnComplete)

    Usage:
    <!--language lang-cpp -->

        void AJusticeSDKDemoGameModeBase::BeginPlay()
        {
            UserProfileInfoUpdate NewUserProfile;
            NewUserProfile.Email = "johndoe@example.com";
            NewUserProfile.DisplayName = "JohnDoe";
            NewUserProfile.AvatarLargeUrl = "https://example.com/large.jpg";
            NewUserProfile.AvatarUrl = "https://example.com/medium.jpg";
            NewUserProfile.AvatarSmallUrl = "https://example.com/small.jpg";
            NewUserProfile.FirstName = "John";
            NewUserProfile.LastName = "Doe";
            NewUserProfile.Language = "en";
            NewUserProfile.Timezone = "Asia/Shanghai";
            NewUserProfile.DateOfBirth = "2002-02-02";
            
            JusticePlatform::UpdatePlayerProfile(NewUserProfile, 
                FDefaultCompleteDelegate::CreateLambda(
                    [](bool bSuccessful, FString ErrorStr)
                    {
                        UE_LOG(LogTmp, Log, TEXT("Update Player Profile Result: %s"), bSuccessful ? TEXT("Success") : TEXT("Failed"));
                    }
                );
            );
        }


<script src="https://cdn.rawgit.com/google/code-prettify/master/loader/run_prettify.js"></script>