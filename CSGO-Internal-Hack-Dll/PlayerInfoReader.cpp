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

	DWORD localPlayerAddr = *(DWORD*)((DWORD)clientAddr + hazedumper::signatures::dwLocalPlayer);
	if (localPlayerAddr == NULL) { return; }

	localPlayer->dwBaseAddr = localPlayerAddr;
	localPlayer->health = *(INT*)(localPlayerAddr + hazedumper::netvars::m_iHealth);
	localPlayer->team = *(INT*)(localPlayerAddr + hazedumper::netvars::m_iTeamNum);
	localPlayer->id = *(INT*)(localPlayerAddr + hazedumper::netvars::m_iAccountID);
	localPlayer->bodyGameCoords.x = *(FLOAT*)(localPlayerAddr + hazedumper::netvars::m_vecOrigin + sizeof(float) * 0);
	localPlayer->bodyGameCoords.y = *(FLOAT*)(localPlayerAddr + hazedumper::netvars::m_vecOrigin + sizeof(float) * 1);
	localPlayer->bodyGameCoords.z = *(FLOAT*)(localPlayerAddr + hazedumper::netvars::m_vecOrigin + sizeof(float) * 2);

	DWORD boneMatrixAddr = *(DWORD*)(localPlayerAddr + hazedumper::netvars::m_dwBoneMatrix);

	int boneID = 8;
	localPlayer->headGameCoords.x = *(FLOAT*)(boneMatrixAddr + (0x30 * boneID) + 0x0c);
	localPlayer->headGameCoords.y = *(FLOAT*)(boneMatrixAddr + (0x30 * boneID) + 0x1c);
	localPlayer->headGameCoords.z = *(FLOAT*)(boneMatrixAddr + (0x30 * boneID) + 0x2c);

	DWORD clientStateAddr = *(DWORD*)(engineAddr + hazedumper::signatures::dwClientState);
	localPlayer->aimAngle.x = *(FLOAT*)(clientStateAddr + hazedumper::signatures::dwClientState_ViewAngles + sizeof(float) * 0);
	localPlayer->aimAngle.y = *(FLOAT*)(clientStateAddr + hazedumper::signatures::dwClientState_ViewAngles + sizeof(float) * 1);

	localPlayer->aimID = *(INT*)(localPlayerAddr + hazedumper::netvars::m_iCrosshairId);

	DWORD localPlayerActiveWeapon = *(DWORD*)(localPlayerAddr + (QWORD)hazedumper::netvars::m_hActiveWeapon);
	if (localPlayerActiveWeapon == NULL) { localPlayer->isValid = false; return; }

	DWORD localPlayerActiveWeaponAddr = *(DWORD*)(clientAddr + (QWORD)hazedumper::signatures::dwEntityList + (QWORD)(((int)(localPlayerActiveWeapon & 0xFFF) - 1) * 0x10));
	if (localPlayerActiveWeaponAddr == NULL) { localPlayer->isValid = false; return; }

	localPlayer->weaponID = *(SHORT*)(localPlayerActiveWeaponAddr + (QWORD)hazedumper::netvars::m_iItemDefinitionIndex);

	localPlayer->isValid = true;
}

void ReadOtherPlayerInfo(void)
{
	DWORD clientAddr = reinterpret_cast<DWORD>(GetModuleHandle(L"client_panorama.dll"));
	if (clientAddr == NULL) return;

	DWORD engineAddr = reinterpret_cast<DWORD>(GetModuleHandle(L"engine.dll"));
	if (engineAddr == NULL) { localPlayer->isValid = false; return; }

	// Represents the next player
	int teammateIndex = 0, enemyIndex = 0;

	for (size_t i = 0; i < teammates.size(); i++)
		teammates.at(i)->isValid = false;
	for (size_t i = 0; i < enemy.size(); i++)
		enemy.at(i)->isValid = false;

	// Traverse all the player
	// Start form 1 to exclude localplayer
	for (size_t i = 1; i < 20; i++)
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
