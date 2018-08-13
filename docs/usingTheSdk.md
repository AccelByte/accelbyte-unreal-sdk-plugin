## C++

### a. OAuth2 Login
Before you want to use other service, you must call `ClientLogin()` function first.
`ClientLogin()` function is actually OAuth2 Login to our service. It will login against **ClientID** and **ClientSecret** from `DefaultEngine.ini` file under `JusticeSDK` section.
If you build Game Client and Dedicated Server, you should call this function. There is **no** `ServerLogin` Function.

For more information about OAuth2 you can follow [this link](https://oauth.net/2/) and [this link](https://docs.microsoft.com/en-us/azure/active-directory/develop/active-directory-protocols-oauth-code).

On your start up game class, usually named `YourAwesomeGameModeBase.cpp` with class named `AYourAwesomeGameModeBase`. Add this code on `BeginPlay` Function or whatever function that will call on startup
<pre class="prettyprint lang-cpp">
void AYourAwesomeGameModeBase::BeginPlay()
{
    Super::BeginPlay();
    JusticeIdentity::ClientLogin();
}
void AYourAwesomeGameModeBase::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    Super::EndPlay(EndPlayReason);
    JusticeIdentity::ClientLogout();
}
</pre>

### b. User Login
To call login with email and password you do like this
<pre class="prettyprint lang-cpp">
void AJusticeSDKDemoGameModeBase::BeginPlay()
{
	...
    JusticeIdentity::UserLogin(TEXT("user@example.com"), 
      TEXT("userpassword"),       
      FUserLoginCompleteDelegate::CreateLambda([](bool IsSuccess, FString ErrorString, UOAuthTokenJustice* token) {
  		// this will be called when login request receive a response 
    }));
    ...
}
</pre>

### c. Device Login
If you want to login without using user and password, you can use Device Login. Device Id `FGenericPlatformMisc::GetDeviceId()` will act as identifier of the player. The example case would be player want to try our game without doing any registration.
<pre class="prettyprint lang-cpp">
void AJusticeSDKDemoGameModeBase::BeginPlay()
{
	...
    JusticeIdentity::DeviceLogin(
      FUserLoginCompleteDelegate::CreateLambda([](bool IsSuccess, FString ErrorString, UOAuthTokenJustice* token) {
  		// this will be called when login request receive a response 
    }));
    ...
}
</pre>

## Blueprint
We can call all of The Justice SDK function from blueprint. We have 2 different kind of blueprint function:

1.  Blueprint function with delegates parameter 
1.  Blueprint async function with multiple output pins

<script src="https://cdn.rawgit.com/google/code-prettify/master/loader/run_prettify.js"></script>