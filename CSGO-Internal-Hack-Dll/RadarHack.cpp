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

#include "RadarHack.h"

void RadarHack()
{
	Process* proc = Process::GetInstance();
	QWORD clientAddr = proc->moduleBaseAddr[L"client_panorama.dll"];

	for (size_t i = 1; i < 64; i++)
	{
		DWORD targetPlayerAddr = proc->ReadDWORD(clientAddr + (QWORD)hazedumper::signatures::dwEntityList + (QWORD)(i * 0x10));
		if (targetPlayerAddr != NULL)
		{
			BOOL targetPlayerIsSpotted = proc->ReadBool(targetPlayerAddr + (QWORD)hazedumper::netvars::m_bSpotted);
			if (targetPlayerIsSpotted == FALSE)
				do
				{
					proc->WriteBool(targetPlayerAddr + (QWORD)hazedumper::netvars::m_bSpotted, TRUE);
				} while (!proc->ReadBool(targetPlayerAddr + (QWORD)hazedumper::netvars::m_bSpotted));
		}
	}
	Sleep(10);
}
