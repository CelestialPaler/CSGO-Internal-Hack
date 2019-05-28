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

Vec3 Subtract(Vec3 src, Vec3 dst)
{
	Vec3 diff;
	diff.x = src.x - dst.x;
	diff.y = src.y - dst.y;
	diff.z = src.z - dst.z;
	return diff;
}

float Magnitude(Vec3 vec)
{
	return sqrtf(vec.x * vec.x + vec.y * vec.y + vec.z * vec.z);
}

float Distance(Vec3 src, Vec3 dst)
{
	Vec3 diff = Subtract(src, dst);
	return Magnitude(diff);
}

Vec3 CalcAngle(Vec3 src, Vec3 dst)
{
	Vec3 angles;
	angles.x = ((float)atan2(dst.x - src.x, dst.y - src.y)) / PI * 180.0f;
	angles.y = (asin((dst.z - src.z) / Distance(src, dst))) * 180.0f / PI;
	angles.z = 0.0f;
	return angles;
}

void AimBot(void)
{
	ReadLocalPlayerInfo();
	ReadOtherPlayerInfo();
	nearestEnemy = 0; 
	bool isTargetExist = false;

	for (size_t i = 1; i < enemy.size(); i++)
	{
		if (enemy.at(i)->isValid)
		{
			if (enemy.at(i)->health > 0)
			{
				if (enemy.at(i)->distance <= enemy.at(nearestEnemy)->distance)
					nearestEnemy = i;
				isTargetExist = true;
			}
		}
	}

	if (isTargetExist)
	{
		Vec3 angle = CalcAngle(localPlayer->headGameCoords, enemy.at(nearestEnemy)->headGameCoords);

		angleDelta.x = angle.x;
		angleDelta.y = angle.y;

		DWORD engineAddr = reinterpret_cast<DWORD>(GetModuleHandle(L"engine.dll"));
		if (engineAddr == NULL) { return; }

		DWORD clientStateAddr = *(DWORD*)(engineAddr + hazedumper::signatures::dwClientState);
		*(FLOAT*)(clientStateAddr + hazedumper::signatures::dwClientState_ViewAngles + sizeof(float) * 0) = -angle.y;
		*(FLOAT*)(clientStateAddr + hazedumper::signatures::dwClientState_ViewAngles + sizeof(float) * 1) = 90 - angle.x;
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
