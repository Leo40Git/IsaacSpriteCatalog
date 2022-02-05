# The Sprite Catalog
Application to mix-and-match sprite replacement mods for The Binding of Isaac: Rebirth.

### Usage instructions
0. Make sure Steam is running, and that you're logged into an account that owns The Binding of Isaac: Rebirth, plus the Afterbirth+ DLC (Repentance is optional).
1. [TODO: will be written once the app is actually ready for use]

### Build instructions
1. Set up a Qt build environment (prefer MinGW on Windows).
2. Download the Steamworks SDK from [here](https://partner.steamgames.com/downloads/steamworks_sdk.zip) - this requires a Steam account.
3. Extract the `public` and `redistributable_bin` files to `C:/SteamworksSDK`.
    - If you extract it to somewhere else, make sure to edit the `STEAMWORKS_SDK_ROOT` variable in the .pro file.
4. Build and deploy like a usual Qt application. Make sure to package the Steam runtime library file and `steam_appid.txt`!

### FAQ
#### Q: Why does Steam think I'm playing The Binding of Isaac: Rebirth while this app is running?
**A:** This app spoofs The Binding of Isaac: Rebirth to download Workshop items.
