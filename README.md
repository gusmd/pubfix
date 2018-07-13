# pubfix

On many PC configurations (such as overclocked Ryzen systems), PUBG will crash when loading the lobby -- either on startup, or when leaving a match. Some people found that changing the process affinity to a single CPU would avoid the crash.

Tired of doing that manually, I wrote this little tool.

## Usage

For launching PUBG:

1. Open ``pubfix.exe``.
2. Press **Start Monitoring**. The application will start looking for the game process.
3. Launch PUBG normally.
4. Once the status changes to *Found!*, press **Set To Single Core**, or use the respective hotkey (``Ctrl+Alt+S``, by default).
5. Wait for PUBG to load the lobby.
6. Press **Set To All Cores**, or use the respective hotkey (``Ctrl+Alt+A``, by default).
7. Play!

There is no need to close ``pubfix.exe`` while the game is running. It is extremely lightweight.

When leaving a match (BEFORE GOING BACK TO LOBBY)

1. Make sure the status still reads *Found!*. Otherwise repeat steps 1 and 2 above.
3. Press **Set To Single Core**, or use the respective hotkey (``Ctrl+Alt+S``, by default).
4. In PUBG, go back to lobby.
5. Press **Set To All Cores**, or use the respective hotkey (``Ctrl+Alt+A``, by default).
6. Profit

The hotkeys work even when pubfix is minimized, such as when the game is in the foreground. You can disable the hotkeys by unchecking the corresponding checkbox. You can also change the hotkeys using the corresponding fields. Hotkey sequences using successive shortcuts are not supported. An example of a sequence using successive shortcuts is ``Ctrl+Alt+S, B``, i.e., press Ctrl+Alt+S, release, press B.

## Compiling from source

If you would rather compile this from source, you will need:
1. A recent CMake version.
2. Visual Studio 2017.
3. Qt 5.x, on your PATH (or modify the cmake invocation to pass your Qt directory).
4. A shell-like terminal (I use "Git Bash") to run the .sh scripts.

Once all that is in place, simply run ``./pack.sh`` to build and package the ZIP file.
