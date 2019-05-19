#include "SkinChanger.h"

void ForceFullUpdate(void)
{
	DWORD engineAddr = reinterpret_cast<DWORD>(GetModuleHandle(L"engine.dll"));
	if (engineAddr == NULL) { return; }

	DWORD StatePtr = *(DWORD*)((DWORD)engineAddr + (QWORD)hazedumper::signatures::dwClientState);
	if (!StatePtr) { return; }
	*(int*)(StatePtr + 0x174) = -1;
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

		DWORD weaponEntity = *(DWORD*)(clientAddr + (QWORD)hazedumper::signatures::dwEntityList + (QWORD)(((int)(weaponIndex & 0xFFF) - 1) * 0x10));
		if (weaponEntity <= 0) { continue; }

		int weaponID = *(SHORT*)(weaponEntity + hazedumper::netvars::m_iItemDefinitionIndex);
		int UID = *(INT*)(weaponEntity + hazedumper::netvars::m_OriginalOwnerXuidLow);

		if (weaponID == WeaponID::AUG)
		{
			*(INT*)(weaponEntity + (QWORD)hazedumper::netvars::m_nFallbackPaintKit) = 455;
			*(INT*)(weaponEntity + (QWORD)hazedumper::netvars::m_nFallbackSeed) = 1;
			*(INT*)(weaponEntity + (QWORD)hazedumper::netvars::m_nFallbackStatTrak) = 1234;
			*(INT*)(weaponEntity + (QWORD)hazedumper::netvars::m_iEntityQuality) = 4;

			char name[] = "Celestial Paler";
			for (size_t i = 0; i < (sizeof(name) / sizeof(char)); i++)
				* (CHAR*)(weaponEntity + (QWORD)hazedumper::netvars::m_szCustomName + i) = name[i];

			*(FLOAT*)(weaponEntity + (QWORD)hazedumper::netvars::m_flFallbackWear) = 0.001f;
			*(INT*)(weaponEntity + (QWORD)hazedumper::netvars::m_iAccountID) = UID;

			*(INT*)(weaponEntity + (QWORD)hazedumper::netvars::m_iItemIDHigh) = 1;
			*(INT*)(weaponEntity + (QWORD)hazedumper::netvars::m_iItemIDHigh + 4) = 1;
		}

		if (weaponID == WeaponID::AK47)
		{
			*(INT*)(weaponEntity + (QWORD)hazedumper::netvars::m_nFallbackPaintKit) = 180;
			*(INT*)(weaponEntity + (QWORD)hazedumper::netvars::m_nFallbackSeed) = 1;
			*(INT*)(weaponEntity + (QWORD)hazedumper::netvars::m_nFallbackStatTrak) = 1234;
			*(INT*)(weaponEntity + (QWORD)hazedumper::netvars::m_iEntityQuality) = 4;

			char name[] = "Celestial Paler";
			for (size_t i = 0; i < (sizeof(name) / sizeof(char)); i++)
				* (CHAR*)(weaponEntity + (QWORD)hazedumper::netvars::m_szCustomName + i) = name[i];

			*(FLOAT*)(weaponEntity + (QWORD)hazedumper::netvars::m_flFallbackWear) = 0.001f;
			*(INT*)(weaponEntity + (QWORD)hazedumper::netvars::m_iAccountID) = UID;

			*(INT*)(weaponEntity + (QWORD)hazedumper::netvars::m_iItemIDHigh) = 1;
			*(INT*)(weaponEntity + (QWORD)hazedumper::netvars::m_iItemIDHigh + 4) = 1;
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
