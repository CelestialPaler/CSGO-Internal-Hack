#pragma once

#include <Windows.h>
#include <string>

#include "Logger.h"
#include "StringManipulation.h"
#include "GameData.h"
#include "GameStruct.h"
#include "GameDef.h"

namespace hazedumper {
	namespace netvars {
		constexpr ::std::ptrdiff_t m_hViewModel = 0x32F8;
		constexpr ::std::ptrdiff_t m_iViewModelIndex = 0x3220;
		constexpr ::std::ptrdiff_t m_nModelIndex = 0x258;
		constexpr ::std::ptrdiff_t precache_bayonet_ct = 89;
		constexpr ::std::ptrdiff_t precache_bayonet_t = 65;
	}
}

struct Skin
{
	Skin(int _paintKit, int _seed, int _statTrackCount, int _qualiy, std::string _name, float _wear)
	{
		this->paintKit = _paintKit;
		this->seed = _seed;
		this->statTrakCount = _statTrackCount;
		this->quality = _qualiy;
		this->name = _name;
		this->wear = _wear;
		this->itemIDHigh = 0;
		this->itemIDLow = 0;
	}

	Skin()
	{
		paintKit = 0;
		seed = 0;
		statTrakCount = 29;
		quality = 4;
		name = "My Dick";
		wear = 0.1f;
	}

	int paintKit;
	int seed;
	int statTrakCount;
	int quality;
	std::string name;
	float wear;
	int itemIDHigh;
	int itemIDLow;
};

void ForceFullUpdate(void);
void SkinChanger(void);
Skin ReadSkinInfo(void);
