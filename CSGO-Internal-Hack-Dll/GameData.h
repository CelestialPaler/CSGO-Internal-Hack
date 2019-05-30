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

#pragma once

#include <string>
#include <vector>
#include <iomanip>
#include <Windows.h>
#include <mutex>
#include <map>

#include "GameStruct.h"

// 当前开发版本
extern const std::wstring devVersion;

// 目标进程ID
extern DWORD targetPID;
// 目标进程名称
extern const std::wstring targetProcName;
// 目标窗口名称
extern const std::wstring targetWndName;

// 目标窗口句柄
extern HWND hTargetWnd;
//
extern HWND hOverlayWnd;
// 目标窗口Rect
extern RECT targetRect;
// 目标窗口宽度
extern unsigned int targetWndWidth;
// 目标窗口长度
extern unsigned int targetWndHeight;

// 本地玩家（天师苍邪）
extern std::unique_ptr<Player> localPlayer;
// 所有的队友
extern std::vector<std::unique_ptr<Player>> teammates;
// 所有的敌人
extern std::vector<std::unique_ptr<Player>> enemy;

// 发光实体
extern std::vector<std::unique_ptr<GlowObject>> glowObjects;
extern int glowObjectCount;
extern int glowObjectCountMax;
extern float glowColorTeammates[4];
extern float glowColorEnemy[4];
extern float glowColorWeapons[4];
extern float glowColorC4[4];
extern float glowColorDefault[4];

// 皮肤信息
extern std::map<std::string, std::vector<std::string>> weapons;
extern std::map<std::string, int> skins;
extern int skinLoadedCount;

extern int triggerDelay;

extern size_t nearestEnemy;
extern int validTargetNum;

extern float aimLockFov;
extern float aimLockHorizontalSensitivity;
extern float aimLockVerticalSensitivity;
extern float aimLockDistanceSensitivity;
extern int aimLockDistanceBase;

namespace FunctionEnableFlag
{
	extern bool bOverlay;
	extern bool bTriggerBot;

	extern bool bAimBot;
	extern bool bAimBotStaticFOV;
	extern bool bAimBotDynamicFOV;
	extern bool bAimBotSima;

	extern bool bESP;
	extern bool bRadarHack;
	extern bool bBHop;

	extern bool bGlow;
	extern bool bGlowTeammates;
	extern bool bGlowEnemy;
	extern bool bGlowWeapons;
	extern bool bGlowC4;
	extern bool bGlowDefault;
	
	extern bool bMenu;
	extern bool bNULL;
	extern bool bSkinChanger;

	extern bool bReadLocalPlayerInfo;
	extern bool bReadOtherPlayerInfo;
	extern bool bReadGlowObjectInfo;
	extern bool bReadSkinInfo;
	extern bool bReadAimInfo;

	extern bool bShowLocalPlayerInfo;
	extern bool bShowOtherPlayerInfo;
	extern bool bShowGlowObjectInfo;
	extern bool bShowSkinInfo;
	extern bool bShowAimInfo;
}

namespace ThreadExistFlag
{
	extern bool bSkinChanger;
	extern bool bGlow;
	extern bool bAimBot;
	extern bool bNULL;
}