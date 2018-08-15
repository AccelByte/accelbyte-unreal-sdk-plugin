## Integrate with Your Game:

Copy `JusticeSDK` folder and `JusticeWebBrowserWidget` folder from Plugin folder to your Game Plugins folder.

Make sure you add dependency to your .uproject file
    <pre class="prettyprint lang-json">
    {
        "FileVersion": 3,
        "EngineAssociation": "4.18",
        "Category": "",
        "Description": "",
        "Modules": [{
            "Name": "YourAwesomeGame",
            "Type": "Runtime",
            "LoadingPhase": "Default"
        }],
        "Plugins": [
            .
            .
            .
            {
                "Name": "JusticeSDK",
                "Enabled": true
            }, 
            {
                "Name": "JusticeWebBrowserWidget",
                "Enabled": true
            }
        ]
    }
    </pre>

Edit your DefaultEngine.inifile, make sure you add this configuration
<pre class="prettyprint">
[JusticeSDK]
bEnabled=true
AppId=myapp01
BaseURL="https://api.justice.accelbyte.net"
ClientID=<your client id>
ClientSecret=<your client secret>
Namespace=<your game namespace>
</pre>

Open your game build.cs file `YourAwesomeGame.Build.cs` add `JusticeSDK` to PublicDependencyModuleNames
<pre class="prettyprint lang-cs">
public class JusticeSDKDemo : ModuleRules
{
    public JusticeSDKDemo(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
        PublicDependencyModuleNames.AddRange(new string[] 
        { 
        ...
        "JusticeSDK",          
        });
    PrivateDependencyModuleNames.AddRange(new string[] {...});
    DynamicallyLoadedModuleNames.AddRange(new string[] {...});
    }
}
</pre>
<script src="https://cdn.rawgit.com/google/code-prettify/master/loader/run_prettify.js"></script>