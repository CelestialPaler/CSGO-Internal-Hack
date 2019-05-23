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

void Glow(void)
{
	if (FunctionEnableFlag::bReadLocalPlayerInfo == false)
	{
		FunctionEnableFlag::bReadLocalPlayerInfo = true;
		return;
	}

	if (localPlayer->isValid == false) { return; }

	DWORD clientAddr = reinterpret_cast<DWORD>(GetModuleHandle(L"client_panorama.dll"));
	if (clientAddr == NULL) { return; }
	DWORD glowArray = *(DWORD*)(clientAddr + (DWORD)hazedumper::signatures::dwGlowObjectManager);
	if (glowArray == NULL) { return; }

	glowObjectCount = *(INT*)(clientAddr + (DWORD)hazedumper::signatures::dwGlowObjectManager + 0x4);
	glowObjectCountMax = *(INT*)(clientAddr + (DWORD)hazedumper::signatures::dwGlowObjectManager + 0x4);

	GlowObject tempObj;

	for (size_t i = 0; i < glowObjectCount; i++)
	{
		DWORD entityAddr = glowArray + (i * sizeof(GlowObject));
		if (entityAddr == NULL) { continue; }

		std::memcpy(&tempObj, (void*)(entityAddr), sizeof(GlowObject));
		if (tempObj.dwEntityAddr == NULL) { continue; }
		if (tempObj.m_bRenderWhenOccluded == true) { continue; }
		if (tempObj.r == 0 && tempObj.g == 0 && tempObj.b == 0) { continue; }

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
			else
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
		}
		/*else if (classID == ClassID::CC4 || classID == ClassID::CPlantedC4)
		{
			if (!FunctionEnableFlag::bGlowC4) { continue; };
			tempObj.r = glowColorC4[0];
			tempObj.g = glowColorC4[1];
			tempObj.b = glowColorC4[2];
			tempObj.a = glowColorC4[3];
			tempObj.m_bRenderWhenOccluded = true;
			tempObj.m_bRenderWhenUnoccluded = false;
			tempObj.m_bFullBloom = false;
		}
		else if (classID >= ClassID::CWeaponAug && classID <= ClassID::CWeaponXM1014)
		{
			if (FunctionEnableFlag::bGlowWeapons) 
			{ 
				tempObj.r = glowColorWeapons[0];
				tempObj.g = glowColorWeapons[1];
				tempObj.b = glowColorWeapons[2];
				tempObj.a = glowColorWeapons[3];
				tempObj.m_bRenderWhenOccluded = true;
				tempObj.m_bRenderWhenUnoccluded = false;
				tempObj.m_bFullBloom = false;
			}
			else
			{
				tempObj.r = glowColorWeapons[0];
				tempObj.g = glowColorWeapons[1];
				tempObj.b = glowColorWeapons[2];
				tempObj.a = glowColorWeapons[3];
				tempObj.m_bRenderWhenOccluded = false;
				tempObj.m_bRenderWhenUnoccluded = false;
				tempObj.m_bFullBloom = false;
			}
		}
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
		}*/

		std::memcpy((void*)(entityAddr), &tempObj, sizeof(GlowObject));
	}
}
