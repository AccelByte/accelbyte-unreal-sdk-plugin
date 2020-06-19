# AccelByte - Jenkins - Agones Explanation
Agones provides a guide to develop and test locally.
It can be found here [link](https://agones.dev/site/docs/guides/client-sdks/local/)
Then we can get the binaries to simulate the local Agones.

## gameserver.yaml
This is a file that keep the game server *annotation*.
*Annotation* is like a metadata that can be obtained within the DS if it already implement the Agones.
Our Dedicated server obtain the **match-details** from the DSM controller heartbeat response.
But if we use Agones provider, we use this way (*annotation*) to pass the **match-details** information to the DS.
Our server SDK already handled the **annotation** parsing. And invoke the MatchRequestResponse delegate when the parsing success.

## sdk-server.windows.amd64.exe
The binaries should be executed if we want to run the UE4 DSM automation test.
This is the syntax to simulate agones locally.
```
sdk-server.windows.amd64.exe --local -f gameserver.yaml
```