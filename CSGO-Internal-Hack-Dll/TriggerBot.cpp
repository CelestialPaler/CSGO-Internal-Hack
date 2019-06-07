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

#include "TriggerBot.h"

void TriggerBot(void)
{
	if (GetAsyncKeyState(VK_MBUTTON) & 0x8000)
	{
		DWORD clientAddr = reinterpret_cast<DWORD>(GetModuleHandle(L"client_panorama.dll"));
		if (clientAddr == NULL) { return; }

		DWORD* localPlayerAddr = reinterpret_cast<DWORD*>(clientAddr + hazedumper::signatures::dwLocalPlayer);
		if (localPlayerAddr == nullptr) { return; }

		INT* localPlayerTeam = reinterpret_cast<INT*>(*localPlayerAddr + hazedumper::netvars::m_iTeamNum);
		INT* aimID = reinterpret_cast<INT*>(*localPlayerAddr + hazedumper::netvars::m_iCrosshairId);

		if (*aimID >= 1 && *aimID <= 64)
		{
			DWORD* aimedPlayerAddr = reinterpret_cast<DWORD*>(clientAddr + hazedumper::signatures::dwEntityList + (*aimID - 1) * 0x10);
			if (aimedPlayerAddr == nullptr) { return; }
			INT* aimedPlayerTeam = reinterpret_cast<INT*>(*aimedPlayerAddr + hazedumper::netvars::m_iTeamNum);
			if (aimedPlayerTeam == nullptr) { return; }
			if (*localPlayerTeam != *aimedPlayerTeam)
			{
				std::this_thread::sleep_for(std::chrono::milliseconds(triggerDelay));
				mouse_event(MOUSEEVENTF_LEFTDOWN, NULL, NULL, NULL, NULL);
				std::this_thread::sleep_for(std::chrono::milliseconds(5));
				mouse_event(MOUSEEVENTF_LEFTUP, NULL, NULL, NULL, NULL);
			}
		}
	}
}
