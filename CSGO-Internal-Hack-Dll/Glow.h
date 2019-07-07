#pragma once

#include <math.h>

#include "Logger.h"
#include "StringManipulation.h"
#include "GameData.h"
#include "GameStruct.h"
#include "GameDef.h"

void ReadGlowObjectInfo(void)
{
	DWORD clientAddr = reinterpret_cast<DWORD>(GetModuleHandle(L"client_panorama.dll"));
	if (clientAddr == NULL) { return; }

	DWORD glowArray = *(DWORD*)(clientAddr + (DWORD)hazedumper::signatures::dwGlowObjectManager);
	if (glowArray == NULL) { return; }

	glowObjectCount = *(INT*)(clientAddr + (DWORD)hazedumper::signatures::dwGlowObjectManager + 0x4);
	glowObjectCountMax = *(INT*)(clientAddr + (DWORD)hazedumper::signatures::dwGlowObjectManager + 0x4);

	for (size_t i = 0; i < glowObjectCount; i++)
	{
		DWORD entityAddr = glowArray + (i * sizeof(GlowObject));
		if (entityAddr == NULL) { continue; }

		BOOL entityDormant = *(BOOL*)(entityAddr + (DWORD)hazedumper::signatures::m_bDormant);
		if (entityDormant) { continue; }

		std::memcpy((void*)glowObjects.at(i).get(), (void*)(entityAddr), sizeof(GlowObject));
	}
}

// This funtion is not used cause there`s some bug in it, which will crash the game.
void GlowA(void)
{
	while (FunctionEnableFlag::bReadLocalPlayerInfo == false && localPlayer->isValid == false)
	{
		FunctionEnableFlag::bReadLocalPlayerInfo = true;
		std::this_thread::sleep_for(std::chrono::milliseconds(10));
	}

	DWORD clientAddr = reinterpret_cast<DWORD>(GetModuleHandle(L"client_panorama.dll"));
	if (clientAddr == NULL) { return; }
	DWORD glowArray = *(DWORD*)(clientAddr + (DWORD)hazedumper::signatures::dwGlowObjectManager);
	if (glowArray == NULL) { return; }

	glowObjectCount = *(INT*)(clientAddr + (DWORD)hazedumper::signatures::dwGlowObjectManager + 0x4);
	glowObjectCountMax = *(INT*)(clientAddr + (DWORD)hazedumper::signatures::dwGlowObjectManager + 0x4);

	for (size_t i = 0; i < glowObjectCount; i++)
	{
		DWORD entityAddr = glowArray + (i * sizeof(GlowObject));
		if (entityAddr == NULL) { continue; }

		GlowObject tempObj;
		std::memcpy(&tempObj, (void*)(entityAddr), sizeof(GlowObject));
		if (tempObj.dwEntityAddr == NULL) { continue; }
		if (tempObj.m_bRenderWhenOccluded == true) { continue; }
		if (tempObj.r == 0 && tempObj.g == 0 && tempObj.b == 0) { continue; }

		if (*(BOOL*)(tempObj.dwEntityAddr + (QWORD)hazedumper::signatures::m_bDormant)) { continue; }

		DWORD vmt = *(DWORD*)(tempObj.dwEntityAddr + 0x8);
		if (vmt == NULL) { continue; }
		DWORD func = *(DWORD*)(vmt + 2 * 0x4);
		if (func == NULL) { continue; }
		DWORD cls = *(DWORD*)(func + 0x1);
		if (cls == NULL) { continue; }
		DWORD clsn = *(DWORD*)(cls + 0x8);
		if (clsn == NULL) { continue; }
		INT classID = *(INT*)(cls + 0x14);
		if (classID == NULL) { continue; }

		if (classID == ClassID::CCSPlayer)
		{
			INT entityTeam = *(INT*)(tempObj.dwEntityAddr + (DWORD)hazedumper::netvars::m_iTeamNum);
			tempObj.padding1[0] = classID;
			if (entityTeam == localPlayer->team)
			{
				if (!FunctionEnableFlag::bGlowTeammates) { continue; };
				tempObj.r = glowColorTeammates[0];
				tempObj.g = glowColorTeammates[1];
				tempObj.b = glowColorTeammates[2];
				tempObj.a = glowColorTeammates[3];
				tempObj.m_bRenderWhenOccluded = true;
				tempObj.m_bRenderWhenUnoccluded = false;
				tempObj.m_bFullBloom = false;
			}
			else if ((entityTeam == (localPlayer->team + 1) || entityTeam == (localPlayer->team - 1)))
			{
				if (!FunctionEnableFlag::bGlowEnemy) { continue; };
				tempObj.r = glowColorEnemy[0];
				tempObj.g = glowColorEnemy[1];
				tempObj.b = glowColorEnemy[2];
				tempObj.a = glowColorEnemy[3];
				tempObj.m_bRenderWhenOccluded = true;
				tempObj.m_bRenderWhenUnoccluded = false;
				tempObj.m_bFullBloom = false;
			}
			else
			{
				if (!FunctionEnableFlag::bGlowEnemy) { continue; };
				tempObj.r = glowColorWeapons[0];
				tempObj.g = glowColorWeapons[1];
				tempObj.b = glowColorWeapons[2];
				tempObj.a = glowColorWeapons[3];
				tempObj.m_bRenderWhenOccluded = true;
				tempObj.m_bRenderWhenUnoccluded = false;
				tempObj.m_bFullBloom = false;
			}
		}
		//else if (classID == ClassID::CC4 || classID == ClassID::CPlantedC4)
		//{
		//	if (!FunctionEnableFlag::bGlowC4) { continue; };
		//	tempObj.r = glowColorC4[0];
		//	tempObj.g = glowColorC4[1];
		//	tempObj.b = glowColorC4[2];
		//	tempObj.a = glowColorC4[3];
		//	tempObj.m_bRenderWhenOccluded = true;
		//	tempObj.m_bRenderWhenUnoccluded = false;
		//	tempObj.m_bFullBloom = false;
		//}
		//else if (classID >= ClassID::CWeaponAug && classID <= ClassID::CWeaponXM1014)
		//{
		//	if (FunctionEnableFlag::bGlowWeapons)
		//	{
		//		tempObj.r = glowColorWeapons[0];
		//		tempObj.g = glowColorWeapons[1];
		//		tempObj.b = glowColorWeapons[2];
		//		tempObj.a = glowColorWeapons[3];
		//		tempObj.m_bRenderWhenOccluded = true;
		//		tempObj.m_bRenderWhenUnoccluded = false;
		//		tempObj.m_bFullBloom = false;
		//	}
		//	else
		//	{
		//		tempObj.r = glowColorWeapons[0];
		//		tempObj.g = glowColorWeapons[1];
		//		tempObj.b = glowColorWeapons[2];
		//		tempObj.a = glowColorWeapons[3];
		//		tempObj.m_bRenderWhenOccluded = false;
		//		tempObj.m_bRenderWhenUnoccluded = false;
		//		tempObj.m_bFullBloom = false;
		//	}
		//}
		else
		{
			if (!FunctionEnableFlag::bGlowDefault) { continue; };
			tempObj.r = glowColorDefault[0];
			tempObj.g = glowColorDefault[1];
			tempObj.b = glowColorDefault[2];
			tempObj.a = glowColorDefault[3];
			tempObj.m_bRenderWhenOccluded = true;
			tempObj.m_bRenderWhenUnoccluded = false;
			tempObj.m_bFullBloom = false;
		}

		std::memcpy((void*)(entityAddr), &tempObj, sizeof(GlowObject));
	}
}

void GlowB(void)
{
	DWORD clientAddr = reinterpret_cast<DWORD>(GetModuleHandle(L"client_panorama.dll"));
	if (clientAddr == NULL) { return; }

	DWORD glowObj = *(DWORD*)(clientAddr + (DWORD)hazedumper::signatures::dwGlowObjectManager);
	if (glowObj == NULL) { return; }

	DWORD localPlayerAddr = *(DWORD*)((DWORD)clientAddr + hazedumper::signatures::dwLocalPlayer);
	if (localPlayerAddr == NULL) { return; }

	INT localPlayerTeam = *(INT*)(localPlayerAddr + hazedumper::netvars::m_iTeamNum);

	for (int x = 0; x < 32; x++)
	{
		DWORD player = *(DWORD*)(clientAddr + (DWORD)hazedumper::signatures::dwEntityList + x * 0x10);
		if (player == 0) { continue; }

		bool dormant =*(bool*)(player + (DWORD)hazedumper::signatures::m_bDormant);
		if (dormant) { continue; }

		DWORD team = *(DWORD*)(player + (DWORD)hazedumper::netvars::m_iTeamNum);
		if (team != 2 && team != 3) { continue; }

		DWORD currentGlowIndex = *(DWORD*)(player + (DWORD)hazedumper::netvars::m_iGlowIndex);

		if (team == localPlayerTeam)
		{
			if (FunctionEnableFlag::bGlowEnemy)
			{
				*(float*)(glowObj + currentGlowIndex * 0x38 + 0x4) = glowColorTeammates[0];
				*(float*)(glowObj + currentGlowIndex * 0x38 + 0x8) = glowColorTeammates[1];
				*(float*)(glowObj + currentGlowIndex * 0x38 + 0xC) = glowColorTeammates[2];
				*(float*)(glowObj + currentGlowIndex * 0x38 + 0x10) = glowColorTeammates[3];
				*(bool*)(glowObj + currentGlowIndex * 0x38 + 0x24) = true;
				*(bool*)(glowObj + currentGlowIndex * 0x38 + 0x25) = false;
			}
		}
		else if(team == localPlayerTeam + 1 || team == localPlayerTeam - 1)
		{
			if (FunctionEnableFlag::bGlowTeammates)
			{
				*(float*)(glowObj + currentGlowIndex * 0x38 + 0x4) = glowColorEnemy[0];
				*(float*)(glowObj + currentGlowIndex * 0x38 + 0x8) = glowColorEnemy[1];
				*(float*)(glowObj + currentGlowIndex * 0x38 + 0xC) = glowColorEnemy[2];
				*(float*)(glowObj + currentGlowIndex * 0x38 + 0x10) = glowColorEnemy[3];
				*(bool*)(glowObj + currentGlowIndex * 0x38 + 0x24) = true;
				*(bool*)(glowObj + currentGlowIndex * 0x38 + 0x25) = false;
			}
		}
		else
		{
			if (FunctionEnableFlag::bGlowDefault)
			{
				*(float*)(glowObj + currentGlowIndex * 0x38 + 0x4) = glowColorDefault[0];
				*(float*)(glowObj + currentGlowIndex * 0x38 + 0x8) = glowColorDefault[1];
				*(float*)(glowObj + currentGlowIndex * 0x38 + 0xC) = glowColorDefault[2];
				*(float*)(glowObj + currentGlowIndex * 0x38 + 0x10) = glowColorDefault[3];
				*(bool*)(glowObj + currentGlowIndex * 0x38 + 0x24) = true;
				*(bool*)(glowObj + currentGlowIndex * 0x38 + 0x25) = false;
			}
		}
	}
}

DWORD WINAPI GlowWrapper(LPVOID lpParam)
{
	while (FunctionEnableFlag::bGlow)
	{
		GlowB();
		std::this_thread::sleep_for(std::chrono::milliseconds(5));
	}
	ThreadExistFlag::bGlow = false;
	return 0;
}
