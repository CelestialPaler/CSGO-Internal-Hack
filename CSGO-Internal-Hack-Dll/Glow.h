#pragma once

#include <math.h>

#include "Logger.h"
#include "StringManipulation.h"
#include "GameData.h"
#include "GameStruct.h"
#include "GameDef.h"

void ReadGlowObjectInfo(void)
{
	DWORD clientAddr = reinterpret_cast<DWORD>(GetModuleHandle(L"client_panorama.dll"));
	if (clientAddr == NULL) { return; }

	DWORD glowObjectArrayAddr = *(DWORD*)(clientAddr + (DWORD)hazedumper::signatures::dwGlowObjectManager);
	if (glowObjectArrayAddr == NULL) { return; }

	glowObjectCount = *(INT*)(clientAddr + (DWORD)hazedumper::signatures::dwGlowObjectManager + 0x4);

	for (size_t i = 0; i < glowObjectCount; i++)
	{
		DWORD entityBaseAddr = *(DWORD*)(glowObjectArrayAddr + i * 0x38);
		if (entityBaseAddr == NULL) { continue; }

		BOOL entityDormant = *(BOOL*)(glowObjectArrayAddr + (DWORD)hazedumper::signatures::m_bDormant);
		if (entityDormant) { continue; }

		std::unique_ptr<GlowObject> tempGlowObject = std::make_unique<GlowObject>();

		std::memcpy(tempGlowObject.get(), (void*)(glowObjectArrayAddr + (i * sizeof(GlowObject))), sizeof(GlowObject));

		glowObjects.at(i).swap(tempGlowObject);
	}
}

enum ClientClassId
{
	NextBotCombatCharacter = 0,
	CAK47 = 1,
	CBaseAnimating = 2,
	CBaseAnimatingOverlay = 3,
	CBaseAttributableItem = 4,
	CBaseButton = 5,
	CBaseCombatCharacter = 6,
	CBaseCombatWeapon = 7,
	CBaseCSGrenade = 8,
	CBaseCSGrenadeProjectile = 9,
	CBaseDoor = 10,
	CBaseEntity = 11,
	CBaseFlex = 12,
	CBaseGrenade = 13,
	CBaseParticleEntity = 14,
	CBasePlayer = 15,
	CBasePropDoor = 16,
	CBaseTeamObjectiveResource = 17,
	CBaseTempEntity = 18,
	CBaseToggle = 19,
	CBaseTrigger = 20,
	CBaseViewModel = 21,
	CBaseVPhysicsTrigger = 22,
	CBaseWeaponWorldModel = 23,
	CBeam = 24,
	CBeamSpotlight = 25,
	CBoneFollower = 26,
	CBreakableProp = 27,
	CBreakableSurface = 28,
	CC4 = 29,
	CCascadeLight = 30,
	CChicken = 31,
	CColorCorrection = 32,
	CColorCorrectionVolume = 33,
	CCSGameRulesProxy = 34,
	CCSPlayer = 35,
	CCSPlayerResource = 36,
	CCSRagdoll = 37,
	CCSTeam = 38,
	CDEagle = 39,
	CDecoyGrenade = 40,
	CDecoyProjectile = 41,
	CDynamicLight = 42,
	CDynamicProp = 43,
	CEconEntity = 44,
	CEmbers = 45,
	CEntityDissolve = 46,
	CEntityFlame = 47,
	CEntityFreezing = 48,
	CEntityParticleTrail = 49,
	CEnvAmbientLight = 50,
	CEnvDetailController = 51,
	CEnvDOFController = 52,
	CEnvParticleScript = 53,
	CEnvProjectedTexture = 54,
	CEnvQuadraticBeam = 55,
	CEnvScreenEffect = 56,
	CEnvScreenOverlay = 57,
	CEnvTonemapController = 58,
	CEnvWind = 59,
	CFireCrackerBlast = 60,
	CFireSmoke = 61,
	CFireTrail = 62,
	CFish = 63,
	CFlashbang = 64,
	CFogController = 65,
	CFootstepControl = 66,
	CFunc_Dust = 67,
	CFunc_LOD = 68,
	CFuncAreaPortalWindow = 69,
	CFuncBrush = 70,
	CFuncConveyor = 71,
	CFuncLadder = 72,
	CFuncMonitor = 73,
	CFuncMoveLinear = 74,
	CFuncOccluder = 75,
	CFuncReflectiveGlass = 76,
	CFuncRotating = 77,
	CFuncSmokeVolume = 78,
	CFuncTrackTrain = 79,
	CGameRulesProxy = 80,
	CHandleTest = 81,
	CHEGrenade = 82,
	CHostage = 83,
	CHostageCarriableProp = 84,
	CIncendiaryGrenade = 85,
	CInferno = 86,
	CInfoLadderDismount = 87,
	CInfoOverlayAccessor = 88,
	CKnife = 89,
	CKnifeGG = 90,
	CLightGlow = 91,
	CMaterialModifyControl = 92,
	CMolotovGrenade = 93,
	CMolotovProjectile = 94,
	CMovieDisplay = 95,
	CParticleFire = 96,
	CParticlePerformanceMonitor = 97,
	CParticleSystem = 98,
	CPhysBox = 99,
	CPhysBoxMultiplayer = 100,
	CPhysicsProp = 101,
	CPhysicsPropMultiplayer = 102,
	CPhysMagnet = 103,
	CPlantedC4 = 104,
	CPlasma = 105,
	CPlayerResource = 106,
	CPointCamera = 107,
	CPointCommentaryNode = 108,
	CPoseController = 109,
	CPostProcessController = 110,
	CPrecipitation = 111,
	CPrecipitationBlocker = 112,
	CPredictedViewModel = 113,
	CProp_Hallucination = 114,
	CPropDoorRotating = 115,
	CPropJeep = 116,
	CPropVehicleDriveable = 117,
	CRagdollManager = 118,
	CRagdollProp = 119,
	CRagdollPropAttached = 120,
	CRopeKeyframe = 121,
	CSCAR17 = 122,
	CSceneEntity = 123,
	CShadowControl = 124,
	CSlideshowDisplay = 125,
	CSmokeGrenade = 126,
	CSmokeGrenadeProjectile = 127,
	CSmokeStack = 128,
	CSpatialEntity = 129,
	CSpotlightEnd = 130,
	CSprite = 131,
	CSpriteOriented = 132,
	CSpriteTrail = 133,
	CStatueProp = 134,
	CSteamJet = 135,
	CSun = 136,
	CSunlightShadowControl = 137,
	CTeam = 138,
	CTeamplayRoundBasedRulesProxy = 139,
	CTEArmorRicochet = 140,
	CTEBaseBeam = 141,
	CTEBeamEntPoint = 142,
	CTEBeamEnts = 143,
	CTEBeamFollow = 144,
	CTEBeamLaser = 145,
	CTEBeamPoints = 146,
	CTEBeamRing = 147,
	CTEBeamRingPoint = 148,
	CTEBeamSpline = 149,
	CTEBloodSprite = 150,
	CTEBloodStream = 151,
	CTEBreakModel = 152,
	CTEBSPDecal = 153,
	CTEBubbles = 154,
	CTEBubbleTrail = 155,
	CTEClientProjectile = 156,
	CTEDecal = 157,
	CTEDust = 158,
	CTEDynamicLight = 159,
	CTEEffectDispatch = 160,
	CTEEnergySplash = 161,
	CTEExplosion = 162,
	CTEFireBullets = 163,
	CTEFizz = 164,
	CTEFootprintDecal = 165,
	CTEFoundryHelpers = 166,
	CTEGaussExplosion = 167,
	CTEGlowSprite = 168,
	CTEImpact = 169,
	CTEKillPlayerAttachments = 170,
	CTELargeFunnel = 171,
	CTEMetalSparks = 172,
	CTEMuzzleFlash = 173,
	CTEParticleSystem = 174,
	CTEPhysicsProp = 175,
	CTEPlantBomb = 176,
	CTEPlayerAnimEvent = 177,
	CTEPlayerDecal = 178,
	CTEProjectedDecal = 179,
	CTERadioIcon = 180,
	CTEShatterSurface = 181,
	CTEShowLine = 182,
	CTesla = 183,
	CTESmoke = 184,
	CTESparks = 185,
	CTESprite = 186,
	CTESpriteSpray = 187,
	CTest_ProxyToggle_Networkable = 188,
	CTestTraceline = 189,
	CTEWorldDecal = 190,
	CTriggerPlayerMovement = 191,
	CTriggerSoundOperator = 192,
	CVGuiScreen = 193,
	CVoteController = 194,
	CWaterBullet = 195,
	CWaterLODControl = 196,
	CWeaponAug = 197,
	CWeaponAWP = 198,
	CWeaponBizon = 199,
	CWeaponCSBase = 200,
	CWeaponCSBaseGun = 201,
	CWeaponCycler = 202,
	CWeaponElite = 203,
	CWeaponFamas = 204,
	CWeaponFiveSeven = 205,
	CWeaponG3SG1 = 206,
	CWeaponGalil = 207,
	CWeaponGalilAR = 208,
	CWeaponGlock = 209,
	CWeaponHKP2000 = 210,
	CWeaponM249 = 211,
	CWeaponM3 = 212,
	CWeaponM4A1 = 213,
	CWeaponMAC10 = 214,
	CWeaponMag7 = 215,
	CWeaponMP5Navy = 216,
	CWeaponMP7 = 217,
	CWeaponMP9 = 218,
	CWeaponNegev = 219,
	CWeaponNOVA = 220,
	CWeaponP228 = 221,
	CWeaponP250 = 222,
	CWeaponP90 = 223,
	CWeaponSawedoff = 224,
	CWeaponSCAR20 = 225,
	CWeaponScout = 226,
	CWeaponSG550 = 227,
	CWeaponSG552 = 228,
	CWeaponSG556 = 229,
	CWeaponSSG08 = 230,
	CWeaponTaser = 231,
	CWeaponTec9 = 232,
	CWeaponTMP = 233,
	CWeaponUMP45 = 234,
	CWeaponUSP = 235,
	CWeaponXM1014 = 236,
	CWorld = 237,
	DustTrail = 238,
	MovieExplosion = 239,
	ParticleSmokeGrenade = 240,
	RocketTrail = 241,
	SmokeTrail = 242,
	SporeExplosion = 243,
	SporeTrail = 244,
};

void Glow(void)
{
	if (FunctionEnableFlag::bReadLocalPlayerInfo == false) FunctionEnableFlag::bReadLocalPlayerInfo = true;

	DWORD clientAddr = reinterpret_cast<DWORD>(GetModuleHandle(L"client_panorama.dll"));
	if (clientAddr == NULL) { return; }

	DWORD glowArray = *(DWORD*)(clientAddr + (DWORD)hazedumper::signatures::dwGlowObjectManager);
	glowObjectCount = *(INT*)(clientAddr + (DWORD)hazedumper::signatures::dwGlowObjectManager + 0x4);

	GlowObject tempObj;

	for (int i = 0; i < glowObjectCount; i++)
	{
		std::memcpy(&tempObj, (void*)(glowArray + (i * sizeof(GlowObject))), sizeof(GlowObject));

		if (tempObj.dwEntityAddr == NULL) { continue; }

		DWORD vt = *(DWORD*)(tempObj.dwEntityAddr + 0x8);
		if (vt == NULL) { continue; }
		DWORD fn = *(DWORD*)(vt + 2 * 0x4);
		if (fn == NULL) { continue; }
		DWORD cls = *(DWORD*)(fn + 0x1);
		if (cls == NULL) { continue; }
		DWORD clsn = *(DWORD*)(cls + 0x8);
		if (clsn == NULL) { continue; }
		INT classID = *(INT*)(cls + 0x20);
		if (classID == NULL) { continue; }
	
		if (classID == ClientClassId::CCSPlayer)
		{
			Beep(500, 50);
			INT entityTeam = *(INT*)(tempObj.dwEntityAddr + (DWORD)hazedumper::netvars::m_iTeamNum);

			if (entityTeam == localPlayer->team)
			{
				tempObj.r = glowColorTeammates[0];
				tempObj.g = glowColorTeammates[1];
				tempObj.b = glowColorTeammates[2];
				tempObj.a = glowColorTeammates[3];
			}
			else
			{
				tempObj.r = glowColorEnemy[0];
				tempObj.g = glowColorEnemy[1];
				tempObj.b = glowColorEnemy[2];
				tempObj.a = glowColorEnemy[3];
			}
		}
		else if (classID == ClientClassId::CC4 || classID == ClientClassId::CPlantedC4)
		{
			tempObj.r = glowColorC4[0];
			tempObj.g = glowColorC4[1];
			tempObj.b = glowColorC4[2];
			tempObj.a = glowColorC4[3];
		}
		else if (classID >= ClientClassId::CWeaponAug && classID <= ClientClassId::CWeaponXM1014)
		{
			tempObj.r = glowColorWeapons[0];
			tempObj.g = glowColorWeapons[1];
			tempObj.b = glowColorWeapons[2];
			tempObj.a = glowColorWeapons[3];
		}
		else
		{
			continue;
		}

		tempObj.m_bRenderWhenOccluded = true;
		tempObj.m_bRenderWhenUnoccluded = false;

		std::memcpy((void*)(glowArray + (i * sizeof(GlowObject))), &tempObj, sizeof(GlowObject));
	}
}
