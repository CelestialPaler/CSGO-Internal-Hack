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

void Recoil(void)
{
	DWORD clientAddr = reinterpret_cast<DWORD>(GetModuleHandle(L"client_panorama.dll"));
	if (clientAddr == NULL) { return; }

	DWORD engineAddr = reinterpret_cast<DWORD>(GetModuleHandle(L"engine.dll"));
	if (engineAddr == NULL) { return; }

	DWORD* localPlayerAddr = reinterpret_cast<DWORD*>((DWORD)clientAddr + hazedumper::signatures::dwLocalPlayer);
	if (localPlayerAddr == nullptr) { return; }
	if (*localPlayerAddr == NULL) { return; }

	BOOL* isShotsFired = reinterpret_cast<BOOL*>(*localPlayerAddr + hazedumper::netvars::m_iShotsFired);
	if (isShotsFired == nullptr) { return; }

	static Vec2 oldViewAngle;

	if (*isShotsFired > 1)
	{
		DWORD* clientStateAddr = reinterpret_cast<DWORD*>(engineAddr + hazedumper::signatures::dwClientState);
		if (clientStateAddr == nullptr) { return; }

		Vec2 currentViewAngle;

		FLOAT* currentViewAngleAddr = reinterpret_cast<FLOAT*>(*clientStateAddr + hazedumper::signatures::dwClientState_ViewAngles);
		if (currentViewAngleAddr == nullptr) { return; }
		currentViewAngle.y = *currentViewAngleAddr;

		currentViewAngleAddr = reinterpret_cast<FLOAT*>(*clientStateAddr + hazedumper::signatures::dwClientState_ViewAngles + sizeof(float));
		if (currentViewAngleAddr == nullptr) { return; }
		currentViewAngle.x = *currentViewAngleAddr;

		Vec2 aimPunchAngle;

		FLOAT* aimPunchAngleAddr = reinterpret_cast<FLOAT*>(*localPlayerAddr + hazedumper::netvars::m_aimPunchAngle);
		if (aimPunchAngleAddr == nullptr) { return; }
		aimPunchAngle.y = *aimPunchAngleAddr;
		
		aimPunchAngleAddr = reinterpret_cast<FLOAT*>(*localPlayerAddr + hazedumper::netvars::m_aimPunchAngle + sizeof(float));
		if (aimPunchAngleAddr == nullptr) { return; }
		aimPunchAngle.x = *aimPunchAngleAddr;

		aimPunchAngle.x *= 2.f * rcsSensitivity;
		aimPunchAngle.y *= 2.f * rcsSensitivity;

		Vec2 newAngles;

		newAngles.x = ((currentViewAngle.x - aimPunchAngle.x) + oldViewAngle.x);
		newAngles.y = ((currentViewAngle.y - aimPunchAngle.y) + oldViewAngle.y);

		currentViewAngleAddr = reinterpret_cast<FLOAT*>(*clientStateAddr + hazedumper::signatures::dwClientState_ViewAngles);
		if (currentViewAngleAddr == nullptr) { return; }
		*currentViewAngleAddr = newAngles.y;

		currentViewAngleAddr = reinterpret_cast<FLOAT*>(*clientStateAddr + hazedumper::signatures::dwClientState_ViewAngles + sizeof(float));
		if (currentViewAngleAddr == nullptr) { return; }
		*currentViewAngleAddr = newAngles.x;

		oldViewAngle.x = aimPunchAngle.x;
		oldViewAngle.y = aimPunchAngle.y;
	}
	else
	{
		oldViewAngle.x = 0;
		oldViewAngle.y = 0;
	}
}