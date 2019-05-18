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

#include "Manager.h"

void Manager::Run(void)
{
	Initialize();


	std::thread generalThread(GeneralThreadFunc);
	std::thread overlayThread(OverlayThreadFunc);
	std::thread triggerBotThread(TriggerBotThreadFunc);
	std::thread RadarHackThread(RadarHackThreadFunc);
	//std::thread ESPThread(ESPThreadFunc);

	while (true)
	{
		HandleKeyBoardInput();
		HandleMouseInput();
		UpdateFromConfigFile();

		Sleep(10);
	}
}

void Manager::Initialize(void)
{
	// 取得Logger
	Util::Logger* logger = Util::Logger::GetInstance();

	Process* proc = Process::GetInstance();
	proc->Attach(targetProcName);

	// 日志
	{
		Util::Logger::LogDisc msg;
		msg.emPriority = Util::Logger::Priority::INFO;
		msg.szFrom = __func__;
		msg.szMsg = "Manager Initialized.";
		msg.szResult = "OK";
		logger->Write(msg);
	}
}

void Manager::GeneralThreadFunc(void)
{
	// 取得Logger
	Util::Logger* logger = Util::Logger::GetInstance();

	// 日志
	{
		Util::Logger::LogDisc msg;
		msg.emPriority = Util::Logger::Priority::INFO;
		msg.szFrom = __func__;
		msg.szMsg = "General Thread Now Runing.";
		logger->Write(msg);
	}

	while (true)
	{
		Sleep(500);
	}
}

void Manager::OverlayThreadFunc(void)
{
	// 取得Logger
	Util::Logger* logger = Util::Logger::GetInstance();

	// 日志
	{
		Util::Logger::LogDisc msg;
		msg.emPriority = Util::Logger::Priority::INFO;
		msg.szFrom = __func__;
		msg.szMsg = "Overlay Thread Now Runing.";
		logger->Write(msg);
	}
}

void Manager::TriggerBotThreadFunc(void)
{	
	// 取得Logger
	Util::Logger* logger = Util::Logger::GetInstance();

	// 日志
	{
		Util::Logger::LogDisc msg;
		msg.emPriority = Util::Logger::Priority::INFO;
		msg.szFrom = __func__;
		msg.szMsg = "TiggerBot Thread Now Runing.";
		logger->Write(msg);
	}

	while (true)
	{
		if (FunctionEnableFlag::bTriggerBot)
		{
			TriggerBot();
		}
		Sleep(1);
	}
}

void Manager::RadarHackThreadFunc(void)
{
	// 取得Logger
	Util::Logger* logger = Util::Logger::GetInstance();

	// 日志
	{
		Util::Logger::LogDisc msg;
		msg.emPriority = Util::Logger::Priority::INFO;
		msg.szFrom = __func__;
		msg.szMsg = "RadarHack Thread Now Runing.";
		logger->Write(msg);
	}

	while (true)
	{
		if (FunctionEnableFlag::bRadarHack)
		{
			RadarHack();
		}
		Sleep(10);
	}
}

void Manager::HandleKeyBoardInput(void)
{
	// 取得Logger
	Util::Logger* logger = Util::Logger::GetInstance();

	if (GetAsyncKeyState(VK_F1) & 1)
	{
		Toggle(FunctionEnableFlag::bOverlay);

		// 日志
		{
			Util::Logger::LogDisc msg;
			msg.emPriority = Util::Logger::Priority::INFO;
			msg.szFrom = __func__;
			msg.szMsg = "F1 - Overlay";
			msg.szResult = FunctionEnableFlag::bOverlay ? "Enabled." : "Disabled.";
			logger->Write(msg);
		}
	}
	if (GetAsyncKeyState(VK_F2) & 1)
	{
		Toggle(FunctionEnableFlag::bTriggerBot);

		// 日志
		{
			Util::Logger::LogDisc msg;
			msg.emPriority = Util::Logger::Priority::INFO;
			msg.szFrom = __func__;
			msg.szMsg = "F2 - TriggerBot";
			msg.szResult = FunctionEnableFlag::bTriggerBot ? "Enabled." : "Disabled.";
			logger->Write(msg);
		}
	}
	if (GetAsyncKeyState(VK_F3) & 1)
	{
		Toggle(FunctionEnableFlag::bAimBot);

		// 日志
		{
			Util::Logger::LogDisc msg;
			msg.emPriority = Util::Logger::Priority::INFO;
			msg.szFrom = __func__;
			msg.szMsg = "F3 - AimBot";
			msg.szResult = FunctionEnableFlag::bAimBot ? "Enabled." : "Disabled.";
			logger->Write(msg);
		}
	}
	if (GetAsyncKeyState(VK_F4) & 1)
	{
		Toggle(FunctionEnableFlag::bESP);

		// 日志
		{
			Util::Logger::LogDisc msg;
			msg.emPriority = Util::Logger::Priority::INFO;
			msg.szFrom = __func__;
			msg.szMsg = "F4 - ESP";
			msg.szResult = FunctionEnableFlag::bESP ? "Enabled." : "Disabled.";
			logger->Write(msg);
		}
	}
	if (GetAsyncKeyState(VK_F5) & 1)
	{
		Toggle(FunctionEnableFlag::bRadarHack);

		// 日志
		{
			Util::Logger::LogDisc msg;
			msg.emPriority = Util::Logger::Priority::INFO;
			msg.szFrom = __func__;
			msg.szMsg = "F5 - RadarHack";
			msg.szResult = FunctionEnableFlag::bRadarHack ? "Enabled." : "Disabled.";
			logger->Write(msg);
		}
	}
}