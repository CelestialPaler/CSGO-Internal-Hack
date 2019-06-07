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

#include "Logger.h"
#include "StringManipulation.h"
#include "GameData.h"
#include "GameStruct.h"
#include "GameDef.h"

#include <algorithm>
#define PI 3.1415927f

//Vec3 Subtract(Vec3 src, Vec3 dst)
//{
//	Vec3 diff;
//	diff.x = src.x - dst.x;
//	diff.y = src.y - dst.y;
//	diff.z = src.z - dst.z;
//	return diff;
//}
//
//double GetDistanceToAngle(Vector3 AimAt, Vector3 CurrentAngle)
//{
//	AimAt.y += 180.f;
//	CurrentAngle.y += 180.f;
//	CurrentAngle.x += 90.f;
//	AimAt.x += 90.f;
//	float YDiff = max(AimAt.y, CurrentAngle.y) - min(AimAt.y, CurrentAngle.y);
//	if (YDiff > 180.f)
//		YDiff = 360.f - YDiff;
//
//	return sqrt(pow(YDiff, 2) + pow(AimAt.x - CurrentAngle.x, 2));
//}
//
//float Magnitude(Vec3 vec)
//{
//	return sqrtf(vec.x * vec.x + vec.y * vec.y + vec.z * vec.z);
//}
//
//float Distance(Vec3 src, Vec3 dst)
//{
//	Vec3 diff = Subtract(src, dst);
//	return Magnitude(diff);
//}
//
//Vec2 CalcAngle(Vec3 src, Vec3 dst)
//{
//	Vec2 angles;
//	angles.x = ((float)atan2(dst.x - src.x, dst.y - src.y)) / PI * 180.0f;
//	angles.y = (asin((dst.z - src.z) / Distance(src, dst))) * 180.0f / PI;
//	return angles;
//}

Vec2 CalcAngle(Vec3 src, Vec3 dst)
{
	Vec2 angles;
	double delta[3] = { (src.x - dst.x), (src.y - dst.y), (src.z - dst.z) };
	double hyp = sqrt(delta[0] * delta[0] + delta[1] * delta[1]);
	angles.x = (float)(asinf(delta[2] / hyp) * 57.295779513082f);
	angles.y = (float)(atanf(delta[1] / delta[0]) * 57.295779513082f);

	if (delta[0] >= 0.0)
	{
		angles.y += 180.0f;
	}

	return angles;
}

void AimBot(void)
{
	ReadLocalPlayerInfo();
	ReadOtherPlayerInfo();

	for (size_t i = 0; i < enemy.size(); i++)
	{
		if (enemy.at(i)->isValid)
		{
			Vec2 angle = CalcAngle(localPlayer->headGameCoords, enemy.at(i)->headGameCoords);
			enemy.at(i)->angleDelta.x = angle.x;
			enemy.at(i)->angleDelta.y = angle.y > 180 ? angle.y - 360 : angle.y;
		}
	}


	std::vector<int> validTargets;
	bool isTargetExist = false;

	for (size_t i = 0; i < enemy.size(); i++)
	{
		if (enemy.at(i)->isValid)
		{
			if (!enemy.at(i)->isDormant)
			{
				if (enemy.at(i)->health > 0)
				{
					validTargets.push_back(i);
				}
			}
		}
	}
	validTargetNum = validTargets.size();

	if (validTargets.size() <= 0) { return; }

	nearestEnemy = validTargets.at(0);

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
			float dfov = aimLockFov + (enemy.at(i)->distance - aimLockDistanceBase) * aimLockDistanceSensitivity;
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
	else if(FunctionEnableFlag::bAimBotSima)
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
		if (GetAsyncKeyState(VK_MBUTTON) & 0x8000)
		{
			DWORD engineAddr = reinterpret_cast<DWORD>(GetModuleHandle(L"engine.dll"));
			if (engineAddr == NULL) { return; }

			DWORD clientStateAddr = *(DWORD*)(engineAddr + hazedumper::signatures::dwClientState);
			if (FunctionEnableFlag::bAimBotSmooth)
			{
				float deltax = abs(enemy.at(nearestEnemy)->angleDelta.x - localPlayer->aimAngle.x) > 180 ? enemy.at(nearestEnemy)->angleDelta.x - localPlayer->aimAngle.x : enemy.at(nearestEnemy)->angleDelta.x - localPlayer->aimAngle.x;
				*(FLOAT*)(clientStateAddr + hazedumper::signatures::dwClientState_ViewAngles + sizeof(float) * 0) = localPlayer->aimAngle.x + deltax * aimLockSmooth;
				float deltay = abs(enemy.at(nearestEnemy)->angleDelta.y - localPlayer->aimAngle.y) > 180 ? enemy.at(nearestEnemy)->angleDelta.y - localPlayer->aimAngle.y : enemy.at(nearestEnemy)->angleDelta.y - localPlayer->aimAngle.y;
				*(FLOAT*)(clientStateAddr + hazedumper::signatures::dwClientState_ViewAngles + sizeof(float) * 1) = localPlayer->aimAngle.y + deltay * aimLockSmooth;
			}
			else
			{
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
		std::this_thread::sleep_for(std::chrono::milliseconds(5));
	}
	ThreadExistFlag::bAimBot = false;
	return 0;
}
