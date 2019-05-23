#include "SkinChanger.h"

void ForceFullUpdate(void)
{
	DWORD engineAddr = reinterpret_cast<DWORD>(GetModuleHandle(L"engine.dll"));
	if (engineAddr == NULL) { return; }

	DWORD StatePtr = *(DWORD*)((DWORD)engineAddr + (QWORD)hazedumper::signatures::dwClientState);
	if (!StatePtr) { return; }
	*(int*)(StatePtr + 0x174) = -1;
}

bool ChangeWeaponSKin(DWORD _weaponEntity, int _paintkit, int _seed, int _statTrack, int _quality, int _wear, std::string _nameTag)
{
	int UID = *(INT*)(_weaponEntity + hazedumper::netvars::m_OriginalOwnerXuidLow);

	*(INT*)(_weaponEntity + (QWORD)hazedumper::netvars::m_iItemIDHigh) = -1;
	// *(INT*)(_weaponEntity + (QWORD)hazedumper::netvars::m_iItemIDHigh + 4) = -1;

	if (*(INT*)(_weaponEntity + (QWORD)hazedumper::netvars::m_nFallbackPaintKit) == _paintkit) { return false; };

	*(INT*)(_weaponEntity + (QWORD)hazedumper::netvars::m_nFallbackPaintKit) = _paintkit;
	*(INT*)(_weaponEntity + (QWORD)hazedumper::netvars::m_nFallbackSeed) = _seed;
	*(INT*)(_weaponEntity + (QWORD)hazedumper::netvars::m_nFallbackStatTrak) = _statTrack;
	*(INT*)(_weaponEntity + (QWORD)hazedumper::netvars::m_iEntityQuality) = _quality;
	*(FLOAT*)(_weaponEntity + (QWORD)hazedumper::netvars::m_flFallbackWear) = _wear;

	for (size_t i = 0; i < _nameTag.size(); i++)
		* (CHAR*)(_weaponEntity + (QWORD)hazedumper::netvars::m_szCustomName + i) = _nameTag[i];

	*(INT*)(_weaponEntity + (QWORD)hazedumper::netvars::m_iAccountID) = UID;
	return true;
}

DWORD WINAPI SkinChangerWrapper(LPVOID lpParam)
{
	while (FunctionEnableFlag::bSkinChanger)
	{
		SkinChangerB();
		//std::this_thread::sleep_for(std::chrono::nanoseconds(1));
	}
	return 0;
}

DWORD cachedPlayer = 0;
DWORD modelIndex = 0;
int knifeID = 0;
short itemDef = 500;
DWORD paintKit = 415;

const int itemIDHigh = -1;
const int entityQuality = 3;
const float fallbackWear = 0.1f;
const int statTrack = 4396;

void SkinChangerA(void)
{
	int knifeIDOffset = knifeID < 10 ? 0 : 1; /* precache offset id by 1 for new knives */

	DWORD clientAddr = reinterpret_cast<DWORD>(GetModuleHandle(L"client_panorama.dll"));
	if (clientAddr == NULL) { return; }

	DWORD localPlayerAddr = *(DWORD*)(clientAddr + (DWORD)hazedumper::signatures::dwLocalPlayer);
	if (localPlayerAddr == NULL) { return; }

	if (localPlayerAddr == 0) /* localplayer is not connected to any server (unreliable method) */
	{
		modelIndex = 0;
		return;
	}
	else if (localPlayerAddr != cachedPlayer) /* localplayer changed by server switch / demo record */
	{
		modelIndex = 0;
		cachedPlayer = localPlayerAddr;
	}

	if (paintKit > 0 && modelIndex > 0)
	{
		for (int i = 0; i < 8; i++)
		{
			/* handle to weapon entity in the current slot */
			DWORD currentWeapon = *(DWORD*)(localPlayerAddr + (DWORD)hazedumper::netvars::m_hMyWeapons + i * 0x4) & 0xfff;
			currentWeapon = *(DWORD*)(clientAddr + (DWORD)hazedumper::signatures::dwEntityList + (currentWeapon - 1) * 0x10);
			if (currentWeapon == 0) { continue; }

			/* id of the weapon in the current slot */
			short weaponID = *(short*)(currentWeapon + (DWORD)hazedumper::netvars::m_iItemDefinitionIndex);
			int UID = *(int*)(currentWeapon + (DWORD)hazedumper::netvars::m_OriginalOwnerXuidLow);

			localPlayer->id = modelIndex;

			DWORD fallbackPaint = paintKit;
			if (weaponID == WeaponID::DesertEagle) { fallbackPaint = 37; } /* Desert Eagle | Blaze */
			else if (weaponID == WeaponID::Glock) { fallbackPaint = 38; } /* Glock-18 | Fade */
			else if (weaponID == WeaponID::AK47) { fallbackPaint = 675; } /* AK-47 | Fire Serpent */
			else if (weaponID == WeaponID::AWP) { fallbackPaint = 344; } /* AWP | Dragon Lore */
			else if (weaponID == WeaponID::M4A1S) { fallbackPaint = 445; } /* M4A1-S | Hot Rod */
			else if (weaponID == WeaponID::USPS) { fallbackPaint = 653; } /* USP-S | Neo-Noir */
			else if (weaponID == WeaponID::AUG) { fallbackPaint = 455; } /* USP-S | Neo-Noir */
			else if (weaponID != WEAPON_KNIFE && weaponID != WEAPON_KNIFE_T && weaponID != itemDef) { continue; }
			else
			{
				/* knife-specific memory writes */
				*(short*)(currentWeapon + (DWORD)hazedumper::netvars::m_iItemDefinitionIndex) = itemDef;
				*(DWORD*)(currentWeapon + (DWORD)hazedumper::netvars::m_nModelIndex) = modelIndex;
				*(DWORD*)(currentWeapon + (DWORD)hazedumper::netvars::m_iViewModelIndex) = modelIndex;
				*(int*)(currentWeapon + (DWORD)hazedumper::netvars::m_iEntityQuality) =entityQuality;
			}

			/* memory writes necessary for both knives and other weapons in slots */
			*(int*)(currentWeapon + (DWORD)hazedumper::netvars::m_iItemIDHigh) = itemIDHigh;
			*(DWORD*)(currentWeapon + (DWORD)hazedumper::netvars::m_nFallbackPaintKit) = fallbackPaint;
			*(float*)(currentWeapon + (DWORD)hazedumper::netvars::m_flFallbackWear) = fallbackWear;
			*(int*)(currentWeapon + (DWORD)hazedumper::netvars::m_nFallbackStatTrak) = statTrack;
			*(int*)(currentWeapon + (DWORD)hazedumper::netvars::m_OriginalOwnerXuidLow) = UID;
		}
	}

	/* handle to weapon entity we are currently holding in hands */
	DWORD activeWeapon = *(DWORD*)(localPlayerAddr + (DWORD)hazedumper::netvars::m_hActiveWeapon) & 0xfff;
	activeWeapon = *(DWORD*)(clientAddr + (DWORD)hazedumper::signatures::dwEntityList + (activeWeapon - 1) * 0x10);
	if (activeWeapon == 0) { return; }

	/* id of weapon we are currently holding in hands */
	short weaponID = *(short*)(activeWeapon + (DWORD)hazedumper::netvars::m_iItemDefinitionIndex);

	/* viewmodel id of the weapon in our hands (default ct knife should usually be around 300) */
	int weaponViewModelID = *(int*)(activeWeapon + (DWORD)hazedumper::netvars::m_iViewModelIndex);

	/* calculate the correct modelindex using the index of default knives and the precache list */
	if (weaponID == WEAPON_KNIFE && weaponViewModelID > 0)
	{
		modelIndex = weaponViewModelID + precache_bayonet_ct + 3 * knifeID + knifeIDOffset;
	}
	else if (weaponID == WEAPON_KNIFE_T && weaponViewModelID > 0)
	{
		modelIndex = weaponViewModelID + precache_bayonet_t + 3 * knifeID + knifeIDOffset;
	}
	else if (weaponID != itemDef || modelIndex == 0) { return; }

	/* handle to viewmodel entity we will use to change the knife viewmodel index */
	DWORD knifeViewModel = *(DWORD*)(localPlayerAddr + (DWORD)hazedumper::netvars::m_hViewModel) & 0xfff;
	knifeViewModel = *(DWORD*)(clientAddr + (DWORD)hazedumper::signatures::dwEntityList + (knifeViewModel - 1) * 0x10);
	if (knifeViewModel == 0) { return; }

	/* change our current viewmodel but only if localplayer is holding a knife in hands */
	*(DWORD*)(knifeViewModel + (DWORD)hazedumper::netvars::m_nModelIndex) = modelIndex;
}

void SkinChangerB(void)
{
	DWORD clientAddr = reinterpret_cast<DWORD>(GetModuleHandle(L"client_panorama.dll"));
	if (clientAddr == NULL) { return; }

	DWORD localPlayerAddr = *(DWORD*)((DWORD)clientAddr + hazedumper::signatures::dwLocalPlayer);
	if (localPlayerAddr == NULL) { return; }

	bool skinChanged = false;
	for (int i = 0; i < 8; i++)
	{
		DWORD currentWeapon = *(DWORD*)(localPlayerAddr + (DWORD)hazedumper::netvars::m_hMyWeapons + i * 0x4) & 0xfff;
		currentWeapon = *(DWORD*)(clientAddr + (DWORD)hazedumper::signatures::dwEntityList + (currentWeapon - 1) * 0x10);
		if (currentWeapon == 0) { continue; }

		SHORT weaponID = *(SHORT*)(currentWeapon + (DWORD)hazedumper::netvars::m_iItemDefinitionIndex);

		switch (weaponID)
		{
		case WeaponID::AK47: // ÑªÐÈÔË¶¯
			skinChanged = skinChanged || ChangeWeaponSKin(currentWeapon, 639, 0, 4396, 4, 0.01f, "Hammann-AK47");
			break;
		case WeaponID::AUG: // ÇïÒ¶Ô­
			skinChanged = skinChanged || ChangeWeaponSKin(currentWeapon, 455, 0, 4396, 4, 0.01f, "Hammann-AUG");
			break;
		case WeaponID::AWP: // Áú¾Ô
			skinChanged = skinChanged || ChangeWeaponSKin(currentWeapon, 344, 0, 4396, 4, 0.01f, "Hammann-AWP");
			break;
		case WeaponID::DesertEagle: // ºìÉ«´úºÅ
			skinChanged = skinChanged || ChangeWeaponSKin(currentWeapon, 711, 0, 4396, 4, 0.01f, "Hammann-Deagle");
			break;
		case WeaponID::USPS:// Ð¡½ã½ã
			skinChanged = skinChanged || ChangeWeaponSKin(currentWeapon, 653, 0, 4396, 4, 0.01f, "Hammann-USPS");
			break;
		case WeaponID::SSG08:// ºìÁú
			skinChanged = skinChanged || ChangeWeaponSKin(currentWeapon, 624, 0, 4396, 4, 0.01f, "Hammann-SSG08");
			break;
		case WeaponID::M4A1S: // CF
			skinChanged = skinChanged || ChangeWeaponSKin(currentWeapon, 548, 0, 4396, 4, 0.01f, "Hammann-M4A1S");
			break;
		case WeaponID::Glock:// ·ÏÍÁ
			skinChanged = skinChanged || ChangeWeaponSKin(currentWeapon, 586, 0, 4396, 4, 0.01f, "Hammann-Glock");
			break;
		default:
			break;
		}
	}

	if (skinChanged)
	{
		//ForceFullUpdate();
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
