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

#include <Windows.h>

typedef uint64_t QWORD;

/* 二维向量类 */
struct Vec2
{
	float x;
	float y;
};

/* 三维向量类 */
struct Vec3
{
	float x;
	float y;
	float z;
};

/* 玩家类 */
class Player
{
public:
	// 身体屏幕空间坐标
	Vec2 bodyScrCoords;
	// 身体游戏空间坐标
	Vec3 bodyGameCoords;
	// 头屏幕空间坐标
	Vec2 headScrCoords;
	// 头游戏空间坐标
	Vec3 headGameCoords;

	// 地址
	/// 该玩家信息在内存中的基地址
	DWORD dwBaseAddr;
	// 是否被发现
	bool isSpotted;
	// 血量
	unsigned int health;
	// 阵营
	unsigned int team;
	// 与你之间的距离
	float distance;
	// ID
	unsigned int id;
	// 是否开镜
	bool isScoped;
	// 正在瞄准的实体ID
	int aimID;
	// 正在瞄准的实体的阵营
	int aimTeam;
};
