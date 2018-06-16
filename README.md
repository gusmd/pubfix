# pubfix
On many PC configurations, PUBG will crash when loading the lobby -- either on startup, or when leaving a match. Some people found that changing the process affinity to a single CPU would avoid the crash.

Tired of doing that manually, I wrote this little tool.

## Usage

For launching PUBG:

1. Open ``pubfix.exe``.
2. Launch PUBG normally.
3. Once PUBG starts loading, press **Find PID**.
4. If the process is found succesfully, press **Set To Single Core**.
5. Wait for PUBG to load the lobby.
6. Press **Set To All Cores**.
7. Play!

When leaving a match (BEFORE GOING BACK TO LOBBY)

1. Open ``pubfix.exe`` (or just never close it!)
2. Press **Find PID** (or, if you never closed it, the value is still valid)
3. Press **Set To Single Core**.
4. In PUBG, go back to lobby.
5. Press **Set To All Cores**.
6. Profit!

