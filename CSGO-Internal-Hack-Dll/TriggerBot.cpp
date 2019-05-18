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
	Process* proc = Process::GetInstance();
	QWORD clientAddr = proc->moduleBaseAddr[L"client_panorama.dll"];
	DWORD localPlayerAddr = proc->ReadUint32(clientAddr + hazedumper::signatures::dwLocalPlayer);

	unsigned int aimID = proc->ReadUint32(localPlayerAddr + hazedumper::netvars::m_iCrosshairId);
	unsigned int localTeam = proc->ReadUint32(localPlayerAddr + hazedumper::netvars::m_iTeamNum);

	if (aimID >= 1 && aimID <= 64)
	{
		DWORD targetPlayerAimAddr = proc->ReadUint32(clientAddr + hazedumper::signatures::dwEntityList + (QWORD)((aimID - 1) * 0x10));
		unsigned int aimTeam = proc->ReadUint32(targetPlayerAimAddr + hazedumper::netvars::m_iTeamNum);

		if (localTeam != aimTeam)
		{
			mouse_event(MOUSEEVENTF_LEFTDOWN, NULL, NULL, NULL, NULL);
			Sleep(10);
			mouse_event(MOUSEEVENTF_LEFTUP, NULL, NULL, NULL, NULL);
		}
	}
}
