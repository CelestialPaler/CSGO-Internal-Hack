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

/* Vec2 Class */
struct Vec2
{
	float x;
	float y;
};

/* Vec3 Class */
struct Vec3
{
	float x;
	float y;
	float z;
};

/* Player Class */
class Player
{
public:
	// Entity coords in the screen space
	Vec2 bodyScrCoords;
	// Entity coords in the gameworld
	Vec3 bodyGameCoords;
	// Head coords in the screen space
	Vec2 headScrCoords;
	// Head coords in the gameworld
	Vec3 headGameCoords;

	Vec2 aimAngle;
	Vec2 angleDelta;

	// Base Address of the player entity
	DWORD dwBaseAddr;
	bool isSpotted;
	unsigned int health;
	unsigned int team;
	// The distance between this player and "you" 
	float distance;
	unsigned int id;
	bool isScoped;
	int aimID;
	int aimTeam;
	short weaponID;
	bool isDormant;

	// Set to true when the data of this player is correctly read.
	// If fasle, the data is not valid.
	bool isValid;
};

struct GlowObject
{
	DWORD dwEntityAddr;
	float r;
	float g;
	float b;
	float a;
	uint8_t padding1[16];
	bool m_bRenderWhenOccluded;
	bool m_bRenderWhenUnoccluded;
	bool m_bFullBloom;
	int GlowStyle;
	uint8_t padding2[10];
};
