# ᐳ GPS Icon with distance text for GTA:SA 1.08

# ᐳ How to use?
1) Add files to `jni/game/` folder

2) Find and open `hooks.cpp`

3) Add code:
> void InstallHooks()
```
SetUpHook(g_libGTASA+0x3DAF84, (uintptr_t)CRadar__SetCoordBlip_hook, (uintptr_t*)&CRadar__SetCoordBlip);
SetUpHook(g_libGTASA+0x3DC27C, (uintptr_t)CRadar__ClearBlip_hook, (uintptr_t*)&CRadar__ClearBlip);
```
> Free space in hooks.cpp
```
#include "CGPS.h"
int (*CRadar__SetCoordBlip)(int r0, float X, float Y, float Z, int r4, int r5, char* name);
int CRadar__SetCoordBlip_hook(int r0, float X, float Y, float Z, int r4, int r5, char* name)
{
	if(pNetGame && !strncmp(name, "CODEWAY", 7))
	{
		float findZ = (( float (*)(float, float))(g_libGTASA+0x3C3DD8+1))(X, Y);
		findZ += 1.5f;

        CGPS::SetTarget({ X, Y, findZ });

		RakNet::BitStream bsSend;
		bsSend.Write(X);
		bsSend.Write(Y);
		bsSend.Write(findZ);
		pNetGame->GetRakClient()->RPC(&RPC_MapMarker, &bsSend, HIGH_PRIORITY, RELIABLE, 0, false, UNASSIGNED_NETWORK_ID, nullptr);
	}

	return CRadar__SetCoordBlip(r0, X, Y, Z, r4, r5, name);
}

void (*CRadar__ClearBlip)(void *thiz, int blipId);
void CRadar__ClearBlip_hook(void *thiz, int blipId) {
    uintptr_t gMobileMenu = (uintptr_t)(g_libGTASA + 0x63E048);
    if(*(int*)(gMobileMenu + 0x18) == blipId) {
        CGPS::Clear();
    }

    CRadar__ClearBlip(thiz, blipId);
}
```

4) Nice, now we need to add render and init for our gps system
> Write to such code in gui.cpp
```
#include "../game/CGPS.h"
CGUI::CGUI()
{
    CGPS::Init();
}

void CGUI::Render()
{
    CGPS::Render();
}
```

5) Okay, last thing it there in server logic!
> Find and open game.cpp, then find code below and replace it
```
#include "CGPS.h"
uint32_t CGame::CreateRadarMarkerIcon(int iMarkerType, float fX, float fY, float fZ, int iColor, int iStyle) {
	uint32_t dwMarkerID = 0;

	if(iStyle == 1) 
		ScriptCommand(&create_marker_icon, fX, fY, fZ, iMarkerType, &dwMarkerID);
	else if(iStyle == 2) 
		ScriptCommand(&create_radar_marker_icon, fX, fY, fZ, iMarkerType, &dwMarkerID);
	else if(iStyle == 3) {
		ScriptCommand(&create_icon_marker_sphere, fX, fY, fZ, iMarkerType, &dwMarkerID);
        CGPS::SetTarget({ fX, fY, fZ });
	} else
		ScriptCommand(&create_radar_marker_without_sphere, fX, fY, fZ, iMarkerType, &dwMarkerID);

	if(iMarkerType == 0) {
		if(iColor >= 1004) {
			ScriptCommand(&set_marker_color, dwMarkerID, iColor);
			ScriptCommand(&show_on_radar, dwMarkerID, 3);
		} else {
			ScriptCommand(&set_marker_color, dwMarkerID, iColor);
			ScriptCommand(&show_on_radar, dwMarkerID, 2);
		}
	}

	return dwMarkerID;
}

void CGame::DisableMarker(uint32_t dwMarkerID) {
	ScriptCommand(&disable_marker, dwMarkerID);
    CGPS::Clear();
}
```

6) End! Good luck!

