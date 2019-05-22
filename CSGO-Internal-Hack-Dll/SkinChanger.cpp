#include "SkinChanger.h"

void ForceFullUpdate(void)
{
	DWORD engineAddr = reinterpret_cast<DWORD>(GetModuleHandle(L"engine.dll"));
	if (engineAddr == NULL) { return; }

	DWORD StatePtr = *(DWORD*)((DWORD)engineAddr + (QWORD)hazedumper::signatures::dwClientState);
	if (!StatePtr) { return; }
	*(int*)(StatePtr + 0x174) = -1;
}

void ChangeWeaponSKin(DWORD _weaponEntity, int _paintkit, int _seed, int _statTrack, int _quality, int _wear, std::string _nameTag)
{
	int UID = *(INT*)(_weaponEntity + hazedumper::netvars::m_OriginalOwnerXuidLow);

	*(INT*)(_weaponEntity + (QWORD)hazedumper::netvars::m_nFallbackPaintKit) = _paintkit;
	*(INT*)(_weaponEntity + (QWORD)hazedumper::netvars::m_nFallbackSeed) = _seed;
	*(INT*)(_weaponEntity + (QWORD)hazedumper::netvars::m_nFallbackStatTrak) = _statTrack;
	*(INT*)(_weaponEntity + (QWORD)hazedumper::netvars::m_iEntityQuality) = _quality;
	*(FLOAT*)(_weaponEntity + (QWORD)hazedumper::netvars::m_flFallbackWear) = _wear;

	for (size_t i = 0; i < _nameTag.size(); i++)
		* (CHAR*)(_weaponEntity + (QWORD)hazedumper::netvars::m_szCustomName + i) = _nameTag[i];


	*(INT*)(_weaponEntity + (QWORD)hazedumper::netvars::m_iAccountID) = UID;

	*(INT*)(_weaponEntity + (QWORD)hazedumper::netvars::m_iItemIDHigh) = 1;
	*(INT*)(_weaponEntity + (QWORD)hazedumper::netvars::m_iItemIDHigh + 4) = 1;
}

void SkinChanger(void)
{
	DWORD clientAddr = reinterpret_cast<DWORD>(GetModuleHandle(L"client_panorama.dll"));
	if (clientAddr == NULL) { return; }

	DWORD localPlayerAddr = *(DWORD*)((DWORD)clientAddr + (QWORD)hazedumper::signatures::dwLocalPlayer);
	if (localPlayerAddr == NULL) { return; }

	for (size_t i = 0; i < 8; i++)
	{
		DWORD weaponIndex = *(DWORD*)(localPlayerAddr + (QWORD)hazedumper::netvars::m_hMyWeapons + ((i - 1) * 0x4));
		weaponIndex &= 0xFFF;
		DWORD weaponEntity = *(DWORD*)(clientAddr + (QWORD)hazedumper::signatures::dwEntityList + (weaponIndex - 1) * 0x10);
		if (weaponEntity <= 0) { continue; }

		int weaponID = *(SHORT*)(weaponEntity + hazedumper::netvars::m_iItemDefinitionIndex);

		switch (weaponID)
		{
		case WeaponID::AK47:
			ChangeWeaponSKin(weaponEntity, 180, 0, 999, 4, 0.1, "Celestial Tech");
			break;
		case WeaponID::AUG:
			ChangeWeaponSKin(weaponEntity, 455, 0, 999, 4, 0.1, "Celestial Tech");
			break;
		default:
			break;
		}
	}
}

Skin ReadSkinInfo(void)
{
	DWORD clientAddr = reinterpret_cast<DWORD>(GetModuleHandle(L"client_panorama.dll"));
	if (clientAddr == NULL) { return Skin(); }

	DWORD localPlayerAddr = *(DWORD*)((DWORD)clientAddr + hazedumper::signatures::dwLocalPlayer);
	if (localPlayerAddr == NULL) { return Skin(); }

	DWORD localPlayerActiveWeapon = *(DWORD*)(localPlayerAddr + (QWORD)hazedumper::netvars::m_hActiveWeapon);
	if (localPlayerActiveWeapon == NULL) { return Skin(); }

	DWORD localPlayerActiveWeaponAddr = *(DWORD*)(clientAddr + (QWORD)hazedumper::signatures::dwEntityList + (QWORD)(((int)(localPlayerActiveWeapon & 0xFFF) - 1) * 0x10));
	if (localPlayerActiveWeaponAddr == NULL) { return Skin(); }

	Skin tempSkin;
	tempSkin.itemIDHigh = *(INT*)(localPlayerActiveWeaponAddr + (QWORD)hazedumper::netvars::m_iItemIDHigh);
	tempSkin.itemIDLow = *(INT*)(localPlayerActiveWeaponAddr + (QWORD)hazedumper::netvars::m_iItemIDHigh + 0x4);
	tempSkin.paintKit = *(INT*)(localPlayerActiveWeaponAddr + (QWORD)hazedumper::netvars::m_nFallbackPaintKit);
	tempSkin.seed = *(INT*)(localPlayerActiveWeaponAddr + (QWORD)hazedumper::netvars::m_nFallbackSeed);
	tempSkin.statTrakCount = *(INT*)(localPlayerActiveWeaponAddr + (QWORD)hazedumper::netvars::m_nFallbackStatTrak);
	tempSkin.quality = *(INT*)(localPlayerActiveWeaponAddr + (QWORD)hazedumper::netvars::m_iEntityQuality);
	tempSkin.wear = *(FLOAT*)(localPlayerActiveWeaponAddr + (QWORD)hazedumper::netvars::m_flFallbackWear);
	return tempSkin;
}
