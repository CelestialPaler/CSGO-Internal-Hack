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

#include <string>
#include <vector>
#include <thread>

#include "Logger.h"
#include "Process.h"
#include "GameStruct.h"
#include "GameData.h"

#include "TriggerBot.h"
#include "RadarHack.h"

class Manager
{
public:
	// 构造函数
	Manager() {};
	// 禁止复制初始化与赋值操作
	Manager(const Manager&) = delete;
	Manager& operator = (const Manager&) = delete;
	// 析构函数
	~Manager() {};

public:
	// 主函数
	void Run(void);

	// 初始化函数
	void Initialize(void);

private:
	static void GeneralThreadFunc(void);
	static void OverlayThreadFunc(void);
	static void TriggerBotThreadFunc(void);
	static void RadarHackThreadFunc(void);

public:
	// 键盘输入处理
	void HandleKeyBoardInput(void);
	// 鼠标输入处理
	void HandleMouseInput() {}
	// 更新配置文件
	void UpdateFromConfigFile() {}

private:
	// 开关外挂功能
	void Toggle(bool& _enable)
	{
		_enable = !_enable;
		if (true)
		{
			if (_enable)
			{
				Beep(500, 100);
				Beep(600, 100);
			}
			else
			{
				Beep(600, 100);
				Beep(500, 100);
			}
		}
	}
};