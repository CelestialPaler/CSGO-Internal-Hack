#pragma once

#include <string>
#include <vector>
#include <iomanip>
#include <Windows.h>
#include <mutex>

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
extern float glowColorTeammates[4];
extern float glowColorEnemy[4];
extern float glowColorWeapons[4];
extern float glowColorC4[4];

namespace FunctionEnableFlag
{
	extern bool bOverlay;
	extern bool bTriggerBot;
	extern bool bAimBot;
	extern bool bESP;
	extern bool bRadarHack;
	extern bool bBHop;
	extern bool bGlow;
	extern bool bWall;
	extern bool bMenu;
	extern bool bNULL;
	extern bool bSkinChanger;
	extern bool bReadLocalPlayerInfo;
	extern bool bReadOtherPlayerInfo;
	extern bool bReadGlowObjectInfo;
	extern bool bReadSkinInfo;
}

namespace ThreadExistFlag
{
	extern bool bGeneral;
	extern bool bOverlay;
	extern bool bTriggerBot;
	extern bool bAimBot;
	extern bool bESP;
	extern bool bRadarHack;
	extern bool bNULL;
}