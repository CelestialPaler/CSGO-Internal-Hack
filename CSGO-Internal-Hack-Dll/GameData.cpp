// CTHackFramework 														      
//	A framework for general game hacking								      
// Copyright © 2019 Celestial Tech All rights reserved.
//
// The MIT License (MIT)
// Copyright (c) 2019 Celestial Tech
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this softwareand associated documentation files(the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and /or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions :
// 
// The above copyright noticeand this permission notice shall be included in all
// copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#include "GameData.h"

// Dev Version
const std::wstring devVersion = L"0.8a";

// Target process id
DWORD targetPID;
// Target process name
const std::wstring targetProcName = L"csgo.exe";
// Target window name
const std::wstring targetWndName = L"Counter-Strike: Global Offensive";

// Target window handle
HWND hTargetWnd;

// Target window Rect
RECT targetRect;
// Target window width
unsigned int targetWndWidth;
// Target window height
unsigned int targetWndHeight;

// localpalyer
std::unique_ptr<Player> localPlayer = std::make_unique<Player>();
// all the teamates
std::vector<std::unique_ptr<Player>> teammates;
// all the enemies
std::vector<std::unique_ptr<Player>> enemy;

// Glow objects
std::vector<std::unique_ptr<GlowObject>> glowObjects;
int glowObjectCount;
int glowObjectCountMax;
float glowColorTeammates[4] = { 0,1,0,1 };
float glowColorEnemy[4] = { 1,0,0,1 };
float glowColorWeapons[4] = { 0,0,1,1 };
float glowColorC4[4] = { 1,0,1,1 };
float glowColorDefault[4] = { 1,1,1,1 };

// Skin info
std::map<std::string, std::vector<std::string>> weapons;
std::map<std::string, int> skins;
int skinLoadedCount = 0;

// Trigger bot
int triggerDelay = 2;

// Aim bot
size_t nearestEnemy;
int validTargetNum;

float aimLockFov = 22;
float aimLockHorizontalSensitivity = 0.9;
float aimLockVerticalSensitivity = 0.1;
float aimLockDistanceSensitivity = 0.5;
int aimLockDistanceBase = 10;

namespace FunctionEnableFlag
{
	bool bOverlay = false;
	bool bTriggerBot = false;

	bool bAimBot = false;
	bool bAimBotStaticFOV = true;
	bool bAimBotDynamicFOV = false;
	bool bAimBotSima = false;

	bool bESP = false;
	bool bRadarHack = false;
	bool bBHop = false;

	bool bGlow = false;
	bool bGlowTeammates = true;
	bool bGlowEnemy = true;
	bool bGlowWeapons = false;
	bool bGlowDefault = false;
	bool bGlowC4 = false;

	bool bMenu = true;
	bool bNULL = false;
	bool bSkinChanger = false;

	bool bReadLocalPlayerInfo = false;
	bool bReadOtherPlayerInfo = false;
	bool bReadGlowObjectInfo = false;
	bool bReadSkinInfo = false;
	bool bReadAimInfo = false;

	bool bShowLocalPlayerInfo = false;
	bool bShowOtherPlayerInfo = false;
	bool bShowGlowObjectInfo = false;
	bool bShowSkinInfo = false;
	bool bShowAimInfo = false;
}

namespace ThreadExistFlag
{
	bool bSkinChanger = false;
	bool bGlow = false;
	bool bAimBot = false;
	bool bNULL = false;
}
