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
	DWORD clientAddr = reinterpret_cast<DWORD>(GetModuleHandle(L"client_panorama.dll"));
	if (clientAddr == NULL) { return; }

	for (size_t i = 1; i < 20; i++)
	{
		DWORD targetPlayerAddr = *(DWORD*)(clientAddr + hazedumper::signatures::dwEntityList + (QWORD)((i - 1) * (int)0x10));
		if (targetPlayerAddr == NULL) { return; }
		BOOL targetPlayerIsSpotted = *(BOOL*)(targetPlayerAddr + hazedumper::netvars::m_bSpotted);
		if (targetPlayerIsSpotted == FALSE)
		{
			do
			{
				*(BOOL*)(targetPlayerAddr + (QWORD)hazedumper::netvars::m_bSpotted) = TRUE;
			} while (!*(BOOL*)(targetPlayerAddr + (QWORD)hazedumper::netvars::m_bSpotted));
		}
	}
}
