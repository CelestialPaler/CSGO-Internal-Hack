#include "GameData.h"

// 当前开发版本
const std::wstring devVersion = L"0.6b";

// 目标进程ID
DWORD targetPID;
// 目标进程名称
const std::wstring targetProcName = L"csgo.exe"; //notepad.exe csgo.exe
// 目标窗口名称
const std::wstring targetWndName = L"Counter-Strike: Global Offensive";//target.txt - Notepad Counter-Strike: Global Offensive

// 目标窗口句柄
HWND hTargetWnd;
HWND hOverlayWnd;
// 目标窗口Rect
RECT targetRect;
// 目标窗口宽度
unsigned int targetWndWidth;
// 目标窗口长度
unsigned int targetWndHeight;

// 本地玩家（天师苍邪）
std::unique_ptr<Player> localPlayer = std::make_unique<Player>();
// 所有的队友
std::vector<std::unique_ptr<Player>> teammates;
// 所有的敌人
std::vector<std::unique_ptr<Player>> enemy;

// 发光实体
std::vector<std::unique_ptr<GlowObject>> glowObjects;
int glowObjectCount;
float glowColorTeammates[4] = { 0,1,0,1 };
float glowColorEnemy[4] = { 1,0,0,1 };
float glowColorWeapons[4] = { 0,0,1,1 };
float glowColorC4[4] = { 1,1,1,1 };

namespace FunctionEnableFlag
{
	bool bOverlay = false;
	bool bTriggerBot = false;
	bool bAimBot = false;
	bool bESP = false;
	bool bRadarHack = false;
	bool bBHop = false;
	bool bGlow = false;
	bool bGlowTeammates = false;
	bool bGlowEnemy = false;
	bool bGlowWeapons = false;
	bool bWall = false;
	bool bMenu = true;
	bool bNULL = false;
	bool bSkinChanger = false;
	bool bReadLocalPlayerInfo = false;
	bool bReadOtherPlayerInfo = false;
	bool bReadGlowObjectInfo = false;
	bool bReadSkinInfo = false;
}

namespace ThreadExistFlag
{
	bool bGeneral = false;
	bool bOverlay = false;
	bool bTriggerBot = false;
	bool bAimBot = false;
	bool bESP = false;
	bool bRadarHack = false;
	bool bNULL = false;
}
