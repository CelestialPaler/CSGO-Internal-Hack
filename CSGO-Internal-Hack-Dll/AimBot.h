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

#include "StringManipulation.h"
#include "GameData.h"
#include "GameStruct.h"
#include "GameDef.h"
#include "PlayerInfoReader.h"

#include <algorithm>
#define PI 3.1415927f

Vec2 CalcAngle(const Vec3 & src, const Vec3 & dst)
{
	Vec2 angles;
	double delta[3] = { (src.x - dst.x), (src.y - dst.y), (src.z - dst.z) };
	double hyp = sqrt(delta[0] * delta[0] + delta[1] * delta[1]);
	angles.x = (float)(asinf(delta[2] / hyp) * 57.295779513082f);
	angles.y = (float)(atanf(delta[1] / delta[0]) * 57.295779513082f);
	if (delta[0] >= 0.0) angles.y += 180.0f;
	return angles;
}

void AimBot(void)
{
	ReadLocalPlayerInfo();
	ReadOtherPlayerInfo();

	// 计算与玩家的角度
	for (std::unique_ptr<Player>& player : enemy)
	{
		if (player->isValid)
		{
			Vec2 angle = CalcAngle(localPlayer->headGameCoords, player->headGameCoords);
			player->angleDelta.x = angle.x;
			player->angleDelta.y = angle.y > 180 ? angle.y - 360 : angle.y;
		}
	}

	// 有效目标
	std::vector<size_t> validTargets;
	bool isTargetExist = false;
	for (size_t i = 0; i < enemy.size(); i++)
	{
		if (enemy.at(i)->isValid)
		{
			if (!enemy.at(i)->isDormant)
			{
				if (enemy.at(i)->isSpotted)
				{
					if (enemy.at(i)->health > 0)
					{
						validTargets.push_back(i);
					}
				}
			}
		}
	}

	if (validTargets.size() <= 0) { return; }
	size_t nearestEnemy = validTargets.front();

	if (FunctionEnableFlag::bAimBotStaticFOV)
	{
		for (auto i : validTargets)
		{
			if ((pow(enemy.at(i)->angleDelta.x - localPlayer->aimAngle.x, 2) * aimLockHorizontalSensitivity + pow(enemy.at(i)->angleDelta.y - localPlayer->aimAngle.y, 2) * aimLockVerticalSensitivity) <= pow(aimLockFov, 2))
			{
				if (enemy.at(i)->distance <= enemy.at(nearestEnemy)->distance)
				{
					nearestEnemy = i;
					isTargetExist = true;
				}
			}
		}
	}
	else if (FunctionEnableFlag::bAimBotDynamicFOV)
	{
		for (auto i : validTargets)
		{
			float dfov = aimLockFov + (enemy.at(i)->distance - aimLockDistanceBase * 10) * aimLockDistanceSensitivity;
			if ((pow(enemy.at(i)->angleDelta.x - localPlayer->aimAngle.x, 2) * aimLockHorizontalSensitivity + pow(enemy.at(i)->angleDelta.y - localPlayer->aimAngle.y, 2) * aimLockVerticalSensitivity) <= pow(dfov, 2))
			{
				if (enemy.at(i)->distance <= enemy.at(nearestEnemy)->distance)
				{
					nearestEnemy = i;
					isTargetExist = true;
				}
			}
		}
	}
	else if (FunctionEnableFlag::bAimBotSima)
	{
		for (auto i : validTargets)
		{
			if (enemy.at(i)->distance <= enemy.at(nearestEnemy)->distance)
			{
				nearestEnemy = i;
				isTargetExist = true;
			}
		}
	}

	if (isTargetExist)
	{
		if (GetAsyncKeyState(VK_F2) & 0x8000)
		{
			DWORD engineAddr = reinterpret_cast<DWORD>(GetModuleHandle(L"engine.dll"));
			if (engineAddr == NULL) { return; }
			if (FunctionEnableFlag::bAimBotSmooth)
			{
				DWORD clientStateAddr = *(DWORD*)(engineAddr + hazedumper::signatures::dwClientState);
				float deltax = abs(enemy.at(nearestEnemy)->angleDelta.x - localPlayer->aimAngle.x) > 180 ? enemy.at(nearestEnemy)->angleDelta.x - localPlayer->aimAngle.x : enemy.at(nearestEnemy)->angleDelta.x - localPlayer->aimAngle.x;
				*(FLOAT*)(clientStateAddr + hazedumper::signatures::dwClientState_ViewAngles + sizeof(float) * 0) = localPlayer->aimAngle.x + deltax * aimLockSmooth;
				float deltay = abs(enemy.at(nearestEnemy)->angleDelta.y - localPlayer->aimAngle.y) > 180 ? enemy.at(nearestEnemy)->angleDelta.y - localPlayer->aimAngle.y : enemy.at(nearestEnemy)->angleDelta.y - localPlayer->aimAngle.y;
				*(FLOAT*)(clientStateAddr + hazedumper::signatures::dwClientState_ViewAngles + sizeof(float) * 1) = localPlayer->aimAngle.y + deltay * aimLockSmooth;
			}
			else
			{
				DWORD clientStateAddr = *(DWORD*)(engineAddr + hazedumper::signatures::dwClientState);
				*(FLOAT*)(clientStateAddr + hazedumper::signatures::dwClientState_ViewAngles + sizeof(float) * 0) = enemy.at(nearestEnemy)->angleDelta.x;
				*(FLOAT*)(clientStateAddr + hazedumper::signatures::dwClientState_ViewAngles + sizeof(float) * 1) = enemy.at(nearestEnemy)->angleDelta.y;
			}
		}
	}
}

DWORD WINAPI AimBotWrapper(LPVOID lpParam)
{
	while (FunctionEnableFlag::bAimBot)
	{
		AimBot();
		std::this_thread::sleep_for(std::chrono::milliseconds(10));
	}
	ThreadExistFlag::bAimBot = false;
	return 0;
}
