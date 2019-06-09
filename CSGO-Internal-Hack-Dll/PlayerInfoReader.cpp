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

#include "PlayerInfoReader.h"

float viewMatrix[4][4];

bool WorldProjectToScreen(Vec3& _world, Vec2& _screen)
{
	float w = 0.0f;
	_screen.x = viewMatrix[0][0] * _world.x + viewMatrix[0][1] * _world.y + viewMatrix[0][2] * _world.z + viewMatrix[0][3];
	_screen.y = viewMatrix[1][0] * _world.x + viewMatrix[1][1] * _world.y + viewMatrix[1][2] * _world.z + viewMatrix[1][3];
	w = viewMatrix[3][0] * _world.x + viewMatrix[3][1] * _world.y + viewMatrix[3][2] * _world.z + viewMatrix[3][3];
	if (w < 0.01f)
		return false;
	float invw = 1.0f / w;
	_screen.x = invw * _screen.x;
	_screen.y = invw * _screen.y;
	int width = (int)targetWndWidth;
	int height = (int)(targetWndHeight);
	float x = width / 2;
	float y = height / 2;
	x += 0.5 * _screen.x * width + 0.5;
	y -= 0.5 * _screen.y * height + 0.5;
	_screen.x = x + targetRect.left;
	_screen.y = y + targetRect.top;
	return true;
}

float CalculateDistance(Vec3 _vec1, Vec3 _vec2)
{
	return sqrt(pow(_vec1.x - _vec2.x, 2) + pow(_vec1.y - _vec2.y, 2) + pow(_vec1.z - _vec2.z, 2));
}

void ReadViewMatrix(void)
{
	DWORD clientAddr = reinterpret_cast<DWORD>(GetModuleHandle(L"client_panorama.dll"));
	if (clientAddr == NULL) { return; }

	std::memcpy(viewMatrix, (void *)(clientAddr + (QWORD)hazedumper::signatures::dwViewMatrix), sizeof(BYTE) * 4 * 4 * 4);
}

void ReadGameInfo()
{
	clientAddr = reinterpret_cast<DWORD>(GetModuleHandle(L"client_panorama.dll"));
	if (clientAddr == NULL) { return; }

	engineAddr = reinterpret_cast<DWORD>(GetModuleHandle(L"engine.dll"));
	if (engineAddr == NULL) { return; }

	localPlayerAddr = *(DWORD*)((DWORD)clientAddr + hazedumper::signatures::dwLocalPlayer);
	if (localPlayerAddr == NULL) { return; }
}

void ReadLocalPlayerInfo(void)
{
	DWORD clientAddr = reinterpret_cast<DWORD>(GetModuleHandle(L"client_panorama.dll"));
	if (clientAddr == NULL) { return; }

	DWORD engineAddr = reinterpret_cast<DWORD>(GetModuleHandle(L"engine.dll"));
	if (engineAddr == NULL) { return; }

	DWORD* localPlayerAddr = reinterpret_cast<DWORD*>((DWORD)clientAddr + hazedumper::signatures::dwLocalPlayer);
	if (localPlayerAddr == nullptr) { return; }

	localPlayer->dwBaseAddr = *localPlayerAddr;
	INT* healthAddr = reinterpret_cast<INT*>(*localPlayerAddr + hazedumper::netvars::m_iHealth);
	if (healthAddr == nullptr) { return; }
	localPlayer->health = *healthAddr;

	INT* teamAddr = reinterpret_cast<INT*>(*localPlayerAddr + hazedumper::netvars::m_iTeamNum);
	if (teamAddr == nullptr) { return; }
	localPlayer->team = *teamAddr;

	DWORD* clientStateAddr = reinterpret_cast<DWORD*>(engineAddr + hazedumper::signatures::dwClientState);
	if (clientStateAddr == nullptr) { return; }
	INT* idAddr = reinterpret_cast<INT*>(*clientStateAddr + hazedumper::signatures::dwClientState_GetLocalPlayer);
	if (idAddr == nullptr) { return; }
	localPlayer->id = *idAddr + 1;

	FLOAT* bodyGameCoordsAddr = reinterpret_cast<FLOAT*>(*localPlayerAddr + hazedumper::netvars::m_vecOrigin + sizeof(float) * 0);
	if (bodyGameCoordsAddr == nullptr) { return; }
	localPlayer->bodyGameCoords.x = *bodyGameCoordsAddr;

	bodyGameCoordsAddr = reinterpret_cast<FLOAT*>(*localPlayerAddr + hazedumper::netvars::m_vecOrigin + sizeof(float) * 1);
	if (bodyGameCoordsAddr == nullptr) { return; }
	localPlayer->bodyGameCoords.y = *bodyGameCoordsAddr;

	bodyGameCoordsAddr = reinterpret_cast<FLOAT*>(*localPlayerAddr + hazedumper::netvars::m_vecOrigin + sizeof(float) * 2);
	if (bodyGameCoordsAddr == nullptr) { return; }
	localPlayer->bodyGameCoords.z = *bodyGameCoordsAddr;

	DWORD* boneMatrixAddr = reinterpret_cast<DWORD*>(*localPlayerAddr + hazedumper::netvars::m_dwBoneMatrix);
	if (boneMatrixAddr == nullptr) { return; }

	int boneID = 8;

	FLOAT* headGameCoordsAddr = reinterpret_cast<FLOAT*>(*boneMatrixAddr + (0x30 * boneID) + 0x0c);
	if (headGameCoordsAddr == nullptr) { return; }
	localPlayer->headGameCoords.x = *headGameCoordsAddr;

	headGameCoordsAddr = reinterpret_cast<FLOAT*>(*boneMatrixAddr + (0x30 * boneID) + 0x1c);
	if (headGameCoordsAddr == nullptr) { return; }
	localPlayer->headGameCoords.y = *headGameCoordsAddr;

	headGameCoordsAddr = reinterpret_cast<FLOAT*>(*boneMatrixAddr + (0x30 * boneID) + 0x2c);
	if (headGameCoordsAddr == nullptr) { return; }
	localPlayer->headGameCoords.z = *headGameCoordsAddr;

	FLOAT* aimAngleAddr = reinterpret_cast<FLOAT*>(*clientStateAddr + hazedumper::signatures::dwClientState_ViewAngles + sizeof(float) * 0);
	if (aimAngleAddr == nullptr) { return; }
	localPlayer->aimAngle.x = *aimAngleAddr;

	aimAngleAddr = reinterpret_cast<FLOAT*>(*clientStateAddr + hazedumper::signatures::dwClientState_ViewAngles + sizeof(float) * 1);
	if (aimAngleAddr == nullptr) { return; }
	localPlayer->aimAngle.y = *aimAngleAddr;

	FLOAT* aimIDAddr = reinterpret_cast<FLOAT*>(*localPlayerAddr + hazedumper::netvars::m_iCrosshairId);
	if (aimIDAddr == nullptr) { return; }
	localPlayer->aimID = *aimIDAddr;

	DWORD* localPlayerActiveWeapon = reinterpret_cast<DWORD*>(*localPlayerAddr + (QWORD)hazedumper::netvars::m_hActiveWeapon);
	if (localPlayerActiveWeapon == nullptr) { return; }

	DWORD* localPlayerActiveWeaponAddr = reinterpret_cast<DWORD*>(clientAddr + (QWORD)hazedumper::signatures::dwEntityList + (QWORD)(((int)(*localPlayerActiveWeapon & 0xFFF) - 1) * 0x10));
	if (localPlayerActiveWeaponAddr == nullptr) { return; }

	//SHORT* weaponIDAddr = reinterpret_cast<SHORT*>(*localPlayerActiveWeaponAddr + (QWORD)hazedumper::netvars::m_iItemDefinitionIndex);
	//if (weaponIDAddr == nullptr) { return; }
	//localPlayer->weaponID = *weaponIDAddr;

	localPlayer->isValid = true;
}

void ReadOtherPlayerInfo(void)
{
	DWORD clientAddr = reinterpret_cast<DWORD>(GetModuleHandle(L"client_panorama.dll"));
	if (clientAddr == NULL) { return; }

	DWORD engineAddr = reinterpret_cast<DWORD>(GetModuleHandle(L"engine.dll"));
	if (engineAddr == NULL) { return; }

	// Represents the next player
	int teammateIndex = 0, enemyIndex = 0;

	for (size_t i = 0; i < teammates.size(); i++)
		teammates.at(i)->isValid = false;
	for (size_t i = 0; i < enemy.size(); i++)
		enemy.at(i)->isValid = false;

	// Traverse all the player
	// Start form 1 to exclude localplayer
	for (size_t i = 1; i < teammates.size() + enemy.size(); i++)
	{
		DWORD otherPlayerAddr = *(DWORD*)(clientAddr + (DWORD)hazedumper::signatures::dwEntityList + (QWORD)i * 0x10);

		// if the player entity is not here
		if (otherPlayerAddr == NULL) break;

		// Create a temp player
		std::unique_ptr<Player> tempPlayer = std::make_unique<Player>();

		// Read basic info
		tempPlayer->dwBaseAddr = otherPlayerAddr;
		tempPlayer->health = *(INT*)(otherPlayerAddr + hazedumper::netvars::m_iHealth);
		tempPlayer->team = *(INT*)(otherPlayerAddr + hazedumper::netvars::m_iTeamNum);
		tempPlayer->id = *(INT*)(otherPlayerAddr + hazedumper::netvars::m_iAccountID);
		tempPlayer->isDormant = *(BOOL*)(otherPlayerAddr + hazedumper::signatures ::m_bDormant);

		LONG* spotAddr = reinterpret_cast<LONG*>(otherPlayerAddr + hazedumper::netvars::m_bSpottedByMask);
		if (spotAddr == nullptr) { break; }
		tempPlayer->isSpotted = bool((*spotAddr & (1 << (localPlayer->id - 1))) > 0);

		// Read coords
		tempPlayer->bodyGameCoords.x = *(FLOAT*)(otherPlayerAddr + hazedumper::netvars::m_vecOrigin + sizeof(float) * 0);
		tempPlayer->bodyGameCoords.y = *(FLOAT*)(otherPlayerAddr + hazedumper::netvars::m_vecOrigin + sizeof(float) * 1);
		tempPlayer->bodyGameCoords.z = *(FLOAT*)(otherPlayerAddr + hazedumper::netvars::m_vecOrigin + sizeof(float) * 2);

		DWORD boneMatrixAddr = *(DWORD*)(otherPlayerAddr + hazedumper::netvars::m_dwBoneMatrix);

		int boneID = 8;
		tempPlayer->headGameCoords.x = *(FLOAT*)(boneMatrixAddr + (0x30 * boneID) + 0x0c);
		tempPlayer->headGameCoords.y = *(FLOAT*)(boneMatrixAddr + (0x30 * boneID) + 0x1c);
		tempPlayer->headGameCoords.z = *(FLOAT*)(boneMatrixAddr + (0x30 * boneID) + 0x2c);

		// Calculate coords
		if (WorldProjectToScreen(tempPlayer->bodyGameCoords, tempPlayer->bodyScrCoords))
		{
			tempPlayer->bodyScrCoords.x -= targetRect.left;
			tempPlayer->bodyScrCoords.y -= targetRect.top;
		}
		else
		{
			// in case of failure, set coords to 0
			tempPlayer->bodyScrCoords.x = 0;
			tempPlayer->bodyScrCoords.y = 0;
		}

		// Calculate coords
		if (WorldProjectToScreen(tempPlayer->headGameCoords, tempPlayer->headScrCoords))
		{
			tempPlayer->headGameCoords.x -= targetRect.left;
			tempPlayer->headGameCoords.y -= targetRect.top;
		}
		else
		{
			// in case of failure, set coords to 0
			tempPlayer->headScrCoords.x = 0;
			tempPlayer->headScrCoords.y = 0;
		}

		// Calculate distance
		tempPlayer->distance = CalculateDistance(tempPlayer->bodyGameCoords, localPlayer->bodyGameCoords);

		tempPlayer->isValid = true;

		// Check if it`s a teamate or not
		if (tempPlayer->team != 0 && tempPlayer->team == localPlayer->team)
			teammates.at(teammateIndex++).swap(tempPlayer);
		else
			enemy.at(enemyIndex++).swap(tempPlayer);
	}
}
