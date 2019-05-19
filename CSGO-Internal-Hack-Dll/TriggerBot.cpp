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

		DWORD localPlayerAddr = *(DWORD*)((DWORD)clientAddr + hazedumper::signatures::dwLocalPlayer);
		if (localPlayerAddr == NULL) { return; }

		INT localPlayerTeam = *(INT*)(localPlayerAddr + hazedumper::netvars::m_iTeamNum);
		INT aimID = *(INT*)(localPlayerAddr + hazedumper::netvars::m_iCrosshairId);

		if (aimID >= 1 && aimID <= 64)
		{
			DWORD aimedPlayerAddr = *(DWORD*)(clientAddr + hazedumper::signatures::dwEntityList + (QWORD)((aimID - 1) * (int)0x10));
			if (aimedPlayerAddr == NULL) { return; }
			int aimedPlayerTeam = *(DWORD*)(aimedPlayerAddr + hazedumper::netvars::m_iTeamNum);
			if (localPlayerTeam != aimedPlayerTeam)
			{
				mouse_event(MOUSEEVENTF_LEFTDOWN, NULL, NULL, NULL, NULL);
				Sleep(10);
				mouse_event(MOUSEEVENTF_LEFTUP, NULL, NULL, NULL, NULL);
			}
		}
	}
}
