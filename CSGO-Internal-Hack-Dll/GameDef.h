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
 

#pragma once
#include <cstddef>

// 2019-05-23 14:08:47.494327500 UTC

namespace hazedumper {
	namespace netvars {
		constexpr ::std::ptrdiff_t cs_gamerules_data = 0x0;
		constexpr ::std::ptrdiff_t m_ArmorValue = 0xB340;
		constexpr ::std::ptrdiff_t m_Collision = 0x31C;
		constexpr ::std::ptrdiff_t m_CollisionGroup = 0x474;
		constexpr ::std::ptrdiff_t m_Local = 0x2FBC;
		constexpr ::std::ptrdiff_t m_MoveType = 0x25C;
		constexpr ::std::ptrdiff_t m_OriginalOwnerXuidHigh = 0x31B4;
		constexpr ::std::ptrdiff_t m_OriginalOwnerXuidLow = 0x31B0;
		constexpr ::std::ptrdiff_t m_SurvivalGameRuleDecisionTypes = 0x1320;
		constexpr ::std::ptrdiff_t m_SurvivalRules = 0xCF8;
		constexpr ::std::ptrdiff_t m_aimPunchAngle = 0x302C;
		constexpr ::std::ptrdiff_t m_aimPunchAngleVel = 0x3038;
		constexpr ::std::ptrdiff_t m_angEyeAnglesX = 0xB344;
		constexpr ::std::ptrdiff_t m_angEyeAnglesY = 0xB348;
		constexpr ::std::ptrdiff_t m_bBombPlanted = 0x99D;
		constexpr ::std::ptrdiff_t m_bFreezePeriod = 0x20;
		constexpr ::std::ptrdiff_t m_bGunGameImmunity = 0x392C;
		constexpr ::std::ptrdiff_t m_bHasDefuser = 0xB350;
		constexpr ::std::ptrdiff_t m_bHasHelmet = 0xB334;
		constexpr ::std::ptrdiff_t m_bInReload = 0x3285;
		constexpr ::std::ptrdiff_t m_bIsDefusing = 0x3918;
		constexpr ::std::ptrdiff_t m_bIsQueuedMatchmaking = 0x74;
		constexpr ::std::ptrdiff_t m_bIsScoped = 0x3910;
		constexpr ::std::ptrdiff_t m_bIsValveDS = 0x75;
		constexpr ::std::ptrdiff_t m_bSpotted = 0x93D;
		constexpr ::std::ptrdiff_t m_bSpottedByMask = 0x980;
		constexpr ::std::ptrdiff_t m_bStartedArming = 0x33D0;
		constexpr ::std::ptrdiff_t m_clrRender = 0x70;
		constexpr ::std::ptrdiff_t m_dwBoneMatrix = 0x26A8;
		constexpr ::std::ptrdiff_t m_fAccuracyPenalty = 0x3310;
		constexpr ::std::ptrdiff_t m_fFlags = 0x104;
		constexpr ::std::ptrdiff_t m_flC4Blow = 0x2990;
		constexpr ::std::ptrdiff_t m_flDefuseCountDown = 0x29AC;
		constexpr ::std::ptrdiff_t m_flDefuseLength = 0x29A8;
		constexpr ::std::ptrdiff_t m_flFallbackWear = 0x31C0;
		constexpr ::std::ptrdiff_t m_flFlashDuration = 0xA3F4;
		constexpr ::std::ptrdiff_t m_flFlashMaxAlpha = 0xA3F0;
		constexpr ::std::ptrdiff_t m_flLastBoneSetupTime = 0x2924;
		constexpr ::std::ptrdiff_t m_flLowerBodyYawTarget = 0x3A78;
		constexpr ::std::ptrdiff_t m_flNextAttack = 0x2D70;
		constexpr ::std::ptrdiff_t m_flNextPrimaryAttack = 0x3218;
		constexpr ::std::ptrdiff_t m_flSimulationTime = 0x268;
		constexpr ::std::ptrdiff_t m_flTimerLength = 0x2994;
		constexpr ::std::ptrdiff_t m_hActiveWeapon = 0x2EF8;
		constexpr ::std::ptrdiff_t m_hMyWeapons = 0x2DF8;
		constexpr ::std::ptrdiff_t m_hObserverTarget = 0x3388;
		constexpr ::std::ptrdiff_t m_hOwner = 0x29CC;
		constexpr ::std::ptrdiff_t m_hOwnerEntity = 0x14C;
		constexpr ::std::ptrdiff_t m_hViewModel = 0x32F8;
		constexpr ::std::ptrdiff_t m_iAccountID = 0x2FC8;
		constexpr ::std::ptrdiff_t m_iClip1 = 0x3244;
		constexpr ::std::ptrdiff_t m_iCompetitiveRanking = 0x1A84;
		constexpr ::std::ptrdiff_t m_iCompetitiveWins = 0x1B88;
		constexpr ::std::ptrdiff_t m_iCrosshairId = 0xB3AC;
		constexpr ::std::ptrdiff_t m_iEntityQuality = 0x2FAC;
		constexpr ::std::ptrdiff_t m_iFOV = 0x31E4;
		constexpr ::std::ptrdiff_t m_iFOVStart = 0x31E8;
		constexpr ::std::ptrdiff_t m_iGlowIndex = 0xA40C;
		constexpr ::std::ptrdiff_t m_iHealth = 0x100;
		constexpr ::std::ptrdiff_t m_iItemDefinitionIndex = 0x2FAA;
		constexpr ::std::ptrdiff_t m_iItemIDHigh = 0x2FC0;
		constexpr ::std::ptrdiff_t m_iMostRecentModelBoneCounter = 0x2690;
		constexpr ::std::ptrdiff_t m_iObserverMode = 0x3374;
		constexpr ::std::ptrdiff_t m_iShotsFired = 0xA380;
		constexpr ::std::ptrdiff_t m_iState = 0x3238;
		constexpr ::std::ptrdiff_t m_iTeamNum = 0xF4;
		constexpr ::std::ptrdiff_t m_iViewModelIndex = 0x3220;
		constexpr ::std::ptrdiff_t m_iWorldDroppedModelIndex = 0x3228;
		constexpr ::std::ptrdiff_t m_iWorldModelIndex = 0x3224;
		constexpr ::std::ptrdiff_t m_lifeState = 0x25F;
		constexpr ::std::ptrdiff_t m_nFallbackPaintKit = 0x31B8;
		constexpr ::std::ptrdiff_t m_nFallbackSeed = 0x31BC;
		constexpr ::std::ptrdiff_t m_nFallbackStatTrak = 0x31C4;
		constexpr ::std::ptrdiff_t m_nForceBone = 0x268C;
		constexpr ::std::ptrdiff_t m_nModelIndex = 0x258;
		constexpr ::std::ptrdiff_t m_nTickBase = 0x342C;
		constexpr ::std::ptrdiff_t m_rgflCoordinateFrame = 0x444;
		constexpr ::std::ptrdiff_t m_szCustomName = 0x303C;
		constexpr ::std::ptrdiff_t m_szLastPlaceName = 0x35B0;
		constexpr ::std::ptrdiff_t m_thirdPersonViewAngles = 0x31D8;
		constexpr ::std::ptrdiff_t m_vecOrigin = 0x138;
		constexpr ::std::ptrdiff_t m_vecVelocity = 0x114;
		constexpr ::std::ptrdiff_t m_vecViewOffset = 0x108;
		constexpr ::std::ptrdiff_t m_viewPunchAngle = 0x3020;
	} // namespace netvars
	namespace signatures {
		constexpr ::std::ptrdiff_t clientstate_choked_commands = 0x4D28;
		constexpr ::std::ptrdiff_t clientstate_delta_ticks = 0x174;
		constexpr ::std::ptrdiff_t clientstate_last_outgoing_command = 0x4D24;
		constexpr ::std::ptrdiff_t clientstate_net_channel = 0x9C;
		constexpr ::std::ptrdiff_t convar_name_hash_table = 0x2F0F8;
		constexpr ::std::ptrdiff_t dwClientState = 0x58BCFC;
		constexpr ::std::ptrdiff_t dwClientState_GetLocalPlayer = 0x180;
		constexpr ::std::ptrdiff_t dwClientState_IsHLTV = 0x4D40;
		constexpr ::std::ptrdiff_t dwClientState_Map = 0x28C;
		constexpr ::std::ptrdiff_t dwClientState_MapDirectory = 0x188;
		constexpr ::std::ptrdiff_t dwClientState_MaxPlayer = 0x388;
		constexpr ::std::ptrdiff_t dwClientState_PlayerInfo = 0x52B8;
		constexpr ::std::ptrdiff_t dwClientState_State = 0x108;
		constexpr ::std::ptrdiff_t dwClientState_ViewAngles = 0x4D88;
		constexpr ::std::ptrdiff_t dwEntityList = 0x4D04A94;
		constexpr ::std::ptrdiff_t dwForceAttack = 0x313618C;
		constexpr ::std::ptrdiff_t dwForceAttack2 = 0x3136198;
		constexpr ::std::ptrdiff_t dwForceBackward = 0x3136144;
		constexpr ::std::ptrdiff_t dwForceForward = 0x3136150;
		constexpr ::std::ptrdiff_t dwForceJump = 0x51A814C;
		constexpr ::std::ptrdiff_t dwForceLeft = 0x3136168;
		constexpr ::std::ptrdiff_t dwForceRight = 0x313615C;
		constexpr ::std::ptrdiff_t dwGameDir = 0x631F70;
		constexpr ::std::ptrdiff_t dwGameRulesProxy = 0x521A49C;
		constexpr ::std::ptrdiff_t dwGetAllClasses = 0xD1793C;
		constexpr ::std::ptrdiff_t dwGlobalVars = 0x58BA00;
		constexpr ::std::ptrdiff_t dwGlowObjectManager = 0x5244FD0;
		constexpr ::std::ptrdiff_t dwInput = 0x514FAE0;
		constexpr ::std::ptrdiff_t dwInterfaceLinkList = 0x8C80A4;
		constexpr ::std::ptrdiff_t dwLocalPlayer = 0xCF2A3C;
		constexpr ::std::ptrdiff_t dwMouseEnable = 0xCF8588;
		constexpr ::std::ptrdiff_t dwMouseEnablePtr = 0xCF8558;
		constexpr ::std::ptrdiff_t dwPlayerResource = 0x31344DC;
		constexpr ::std::ptrdiff_t dwRadarBase = 0x51397F4;
		constexpr ::std::ptrdiff_t dwSensitivity = 0xCF8424;
		constexpr ::std::ptrdiff_t dwSensitivityPtr = 0xCF83F8;
		constexpr ::std::ptrdiff_t dwSetClanTag = 0x895D0;
		constexpr ::std::ptrdiff_t dwViewMatrix = 0x4CF64C4;
		constexpr ::std::ptrdiff_t dwWeaponTable = 0x51505A4;
		constexpr ::std::ptrdiff_t dwWeaponTableIndex = 0x323C;
		constexpr ::std::ptrdiff_t dwYawPtr = 0xCF81E8;
		constexpr ::std::ptrdiff_t dwZoomSensitivityRatioPtr = 0xCFD428;
		constexpr ::std::ptrdiff_t dwbSendPackets = 0xD271A;
		constexpr ::std::ptrdiff_t dwppDirect3DDevice9 = 0xA6030;
		constexpr ::std::ptrdiff_t force_update_spectator_glow = 0x3932D2;
		constexpr ::std::ptrdiff_t interface_engine_cvar = 0x3E9EC;
		constexpr ::std::ptrdiff_t is_c4_owner = 0x39F5F0;
		constexpr ::std::ptrdiff_t m_bDormant = 0xED;
		constexpr ::std::ptrdiff_t m_pStudioHdr = 0x294C;
		constexpr ::std::ptrdiff_t m_pitchClassPtr = 0x5139AA0;
		constexpr ::std::ptrdiff_t m_yawClassPtr = 0xCF81E8;
		constexpr ::std::ptrdiff_t model_ambient_min = 0x58ED1C;
		constexpr ::std::ptrdiff_t set_abs_angles = 0x1CA3F0;
		constexpr ::std::ptrdiff_t set_abs_origin = 0x1CA230;
	} // namespace signatures
} // namespace hazedumper




// 所有武器的物品ID
namespace WeaponID
{
	const INT DesertEagle = 1;
	const INT DualBarettas = 2;
	const INT FiveSeven = 3;
	const INT Glock = 4;
	const INT AK47 = 7;
	const INT AUG = 8;
	const INT AWP = 9;
	const INT FAMAS = 10;
	const INT G3SG1 = 11;
	const INT GailAR = 13;
	const INT M249 = 14;
	const INT M4A4 = 16;
	const INT MAC10 = 17;
	const INT P90 = 19;
	const INT MPSSD = 23;
	const INT UMP45 = 24;
	const INT XM1014 = 25;
	const INT PPBizon = 26;
	const INT MAG7 = 27;
	const INT Negev = 28;
	const INT SawedOff = 29;
	const INT Tec9 = 30;
	const INT Zeus = 31;
	const INT P2000 = 32;
	const INT MP7 = 33;
	const INT MP9 = 34;
	const INT Nova = 35;
	const INT P250 = 36;
	const INT SCAR20 = 38;
	const INT SG553 = 39;
	const INT SSG08 = 40;
	const INT Knife1 = 41;
	const INT Knife2 = 42;
	const INT Flashbang = 43;
	const INT HE = 44;
	const INT SG = 45;
	const INT Molotov = 46;
	const INT Decoy = 47;
	const INT IG = 48;
	const INT C4 = 49;
	const INT Knife3 = 59;
	const INT M4A1S = 60;
	const INT USPS = 61;
	const INT CZ75Auto = 63;
	const INT R8Revolver = 64;
}

enum ClassID {
	CTestTraceline = 223,
	CTEWorldDecal = 224,
	CTESpriteSpray = 221,
	CTESprite = 220,
	CTESparks = 219,
	CTESmoke = 218,
	CTEShowLine = 216,
	CTEProjectedDecal = 213,
	CFEPlayerDecal = 71,
	CTEPlayerDecal = 212,
	CTEPhysicsProp = 209,
	CTEParticleSystem = 208,
	CTEMuzzleFlash = 207,
	CTELargeFunnel = 205,
	CTEKillPlayerAttachments = 204,
	CTEImpact = 203,
	CTEGlowSprite = 202,
	CTEShatterSurface = 215,
	CTEFootprintDecal = 199,
	CTEFizz = 198,
	CTEExplosion = 196,
	CTEEnergySplash = 195,
	CTEEffectDispatch = 194,
	CTEDynamicLight = 193,
	CTEDecal = 191,
	CTEClientProjectile = 190,
	CTEBubbleTrail = 189,
	CTEBubbles = 188,
	CTEBSPDecal = 187,
	CTEBreakModel = 186,
	CTEBloodStream = 185,
	CTEBloodSprite = 184,
	CTEBeamSpline = 183,
	CTEBeamRingPoint = 182,
	CTEBeamRing = 181,
	CTEBeamPoints = 180,
	CTEBeamLaser = 179,
	CTEBeamFollow = 178,
	CTEBeamEnts = 177,
	CTEBeamEntPoint = 176,
	CTEBaseBeam = 175,
	CTEArmorRicochet = 174,
	CTEMetalSparks = 206,
	CSteamJet = 167,
	CSmokeStack = 157,
	DustTrail = 275,
	CFireTrail = 74,
	SporeTrail = 281,
	SporeExplosion = 280,
	RocketTrail = 278,
	SmokeTrail = 279,
	CPropVehicleDriveable = 144,
	ParticleSmokeGrenade = 277,
	CParticleFire = 116,
	MovieExplosion = 276,
	CTEGaussExplosion = 201,
	CEnvQuadraticBeam = 66,
	CEmbers = 55,
	CEnvWind = 70,
	CPrecipitation = 137,
	CPrecipitationBlocker = 138,
	CBaseTempEntity = 18,
	NextBotCombatCharacter = 0,
	CEconWearable = 54,
	CBaseAttributableItem = 4,
	CEconEntity = 53,
	CWeaponXM1014 = 272,
	CWeaponTaser = 267,
	CTablet = 171,
	CSnowball = 158,
	CSmokeGrenade = 155,
	CWeaponShield = 265,
	CWeaponSG552 = 263,
	CSensorGrenade = 151,
	CWeaponSawedoff = 259,
	CWeaponNOVA = 255,
	CIncendiaryGrenade = 99,
	CMolotovGrenade = 112,
	CMelee = 111,
	CWeaponM3 = 247,
	CKnifeGG = 108,
	CKnife = 107,
	CHEGrenade = 96,
	CFlashbang = 77,
	CFists = 76,
	CWeaponElite = 238,
	CDecoyGrenade = 47,
	CDEagle = 46,
	CWeaponUSP = 271,
	CWeaponM249 = 246,
	CWeaponUMP45 = 270,
	CWeaponTMP = 269,
	CWeaponTec9 = 268,
	CWeaponSSG08 = 266,
	CWeaponSG556 = 264,
	CWeaponSG550 = 262,
	CWeaponScout = 261,
	CWeaponSCAR20 = 260,
	CSCAR17 = 149,
	CWeaponP90 = 258,
	CWeaponP250 = 257,
	CWeaponP228 = 256,
	CWeaponNegev = 254,
	CWeaponMP9 = 253,
	CWeaponMP7 = 252,
	CWeaponMP5Navy = 251,
	CWeaponMag7 = 250,
	CWeaponMAC10 = 249,
	CWeaponM4A1 = 248,
	CWeaponHKP2000 = 245,
	CWeaponGlock = 244,
	CWeaponGalilAR = 243,
	CWeaponGalil = 242,
	CWeaponG3SG1 = 241,
	CWeaponFiveSeven = 240,
	CWeaponFamas = 239,
	CWeaponBizon = 234,
	CWeaponAWP = 232,
	CWeaponAug = 231,
	CAK47 = 1,
	CWeaponCSBaseGun = 236,
	CWeaponCSBase = 235,
	CC4 = 34,
	CBumpMine = 32,
	CBumpMineProjectile = 33,
	CBreachCharge = 28,
	CBreachChargeProjectile = 29,
	CWeaponBaseItem = 233,
	CBaseCSGrenade = 8,
	CSnowballProjectile = 160,
	CSnowballPile = 159,
	CSmokeGrenadeProjectile = 156,
	CSensorGrenadeProjectile = 152,
	CMolotovProjectile = 113,
	CItem_Healthshot = 104,
	CItemDogtags = 106,
	CDecoyProjectile = 48,
	CPhysPropRadarJammer = 126,
	CPhysPropWeaponUpgrade = 127,
	CPhysPropAmmoBox = 124,
	CPhysPropLootCrate = 125,
	CItemCash = 105,
	CEnvGasCanister = 63,
	CDronegun = 50,
	CParadropChopper = 115,
	CSurvivalSpawnChopper = 170,
	CBRC4Target = 27,
	CInfoMapRegion = 102,
	CFireCrackerBlast = 72,
	CInferno = 100,
	CChicken = 36,
	CDrone = 49,
	CFootstepControl = 79,
	CCSGameRulesProxy = 39,
	CWeaponCubemap = 0,
	CWeaponCycler = 237,
	CTEPlantBomb = 210,
	CTEFireBullets = 197,
	CTERadioIcon = 214,
	CPlantedC4 = 128,
	CCSTeam = 43,
	CCSPlayerResource = 41,
	CCSPlayer = 40,
	CPlayerPing = 130,
	CCSRagdoll = 42,
	CTEPlayerAnimEvent = 211,
	CHostage = 97,
	CHostageCarriableProp = 98,
	CBaseCSGrenadeProjectile = 9,
	CHandleTest = 95,
	CTeamplayRoundBasedRulesProxy = 173,
	CSpriteTrail = 165,
	CSpriteOriented = 164,
	CSprite = 163,
	CRagdollPropAttached = 147,
	CRagdollProp = 146,
	CPropCounter = 141,
	CPredictedViewModel = 139,
	CPoseController = 135,
	CGrassBurn = 94,
	CGameRulesProxy = 93,
	CInfoLadderDismount = 101,
	CFuncLadder = 85,
	CTEFoundryHelpers = 200,
	CEnvDetailController = 61,
	CDangerZone = 44,
	CDangerZoneController = 45,
	CWorldVguiText = 274,
	CWorld = 273,
	CWaterLODControl = 230,
	CWaterBullet = 229,
	CVoteController = 228,
	CVGuiScreen = 227,
	CPropJeep = 143,
	CPropVehicleChoreoGeneric = 0,
	CTriggerSoundOperator = 226,
	CBaseVPhysicsTrigger = 22,
	CTriggerPlayerMovement = 225,
	CBaseTrigger = 20,
	CTest_ProxyToggle_Networkable = 222,
	CTesla = 217,
	CBaseTeamObjectiveResource = 17,
	CTeam = 172,
	CSunlightShadowControl = 169,
	CSun = 168,
	CParticlePerformanceMonitor = 117,
	CSpotlightEnd = 162,
	CSpatialEntity = 161,
	CSlideshowDisplay = 154,
	CShadowControl = 153,
	CSceneEntity = 150,
	CRopeKeyframe = 148,
	CRagdollManager = 145,
	CPhysicsPropMultiplayer = 122,
	CPhysBoxMultiplayer = 120,
	CPropDoorRotating = 142,
	CBasePropDoor = 16,
	CDynamicProp = 52,
	CProp_Hallucination = 140,
	CPostProcessController = 136,
	CPointWorldText = 134,
	CPointCommentaryNode = 133,
	CPointCamera = 132,
	CPlayerResource = 131,
	CPlasma = 129,
	CPhysMagnet = 123,
	CPhysicsProp = 121,
	CStatueProp = 166,
	CPhysBox = 119,
	CParticleSystem = 118,
	CMovieDisplay = 114,
	CMaterialModifyControl = 110,
	CLightGlow = 109,
	CItemAssaultSuitUseable = 0,
	CItem = 0,
	CInfoOverlayAccessor = 103,
	CFuncTrackTrain = 92,
	CFuncSmokeVolume = 91,
	CFuncRotating = 90,
	CFuncReflectiveGlass = 89,
	CFuncOccluder = 88,
	CFuncMoveLinear = 87,
	CFuncMonitor = 86,
	CFunc_LOD = 81,
	CTEDust = 192,
	CFunc_Dust = 80,
	CFuncConveyor = 84,
	CFuncBrush = 83,
	CBreakableSurface = 31,
	CFuncAreaPortalWindow = 82,
	CFish = 75,
	CFireSmoke = 73,
	CEnvTonemapController = 69,
	CEnvScreenEffect = 67,
	CEnvScreenOverlay = 68,
	CEnvProjectedTexture = 65,
	CEnvParticleScript = 64,
	CFogController = 78,
	CEnvDOFController = 62,
	CCascadeLight = 35,
	CEnvAmbientLight = 60,
	CEntityParticleTrail = 59,
	CEntityFreezing = 58,
	CEntityFlame = 57,
	CEntityDissolve = 56,
	CDynamicLight = 51,
	CColorCorrectionVolume = 38,
	CColorCorrection = 37,
	CBreakableProp = 30,
	CBeamSpotlight = 25,
	CBaseButton = 5,
	CBaseToggle = 19,
	CBasePlayer = 15,
	CBaseFlex = 12,
	CBaseEntity = 11,
	CBaseDoor = 10,
	CBaseCombatCharacter = 6,
	CBaseAnimatingOverlay = 3,
	CBoneFollower = 26,
	CBaseAnimating = 2,
	CAI_BaseNPC = 0,
	CBeam = 24,
	CBaseViewModel = 21,
	CBaseParticleEntity = 14,
	CBaseGrenade = 13,
	CBaseCombatWeapon = 7,
	CBaseWeaponWorldModel = 23
};

#define precache_bayonet_ct 89 // = v_knife_bayonet.mdl - v_knife_default_ct.mdl
#define precache_bayonet_t 65 // = v_knife_bayonet.mdl - v_knife_default_t.mdl

enum knifeDefinitionIndex               // id
{
	WEAPON_KNIFE = 42,
	WEAPON_KNIFE_T = 59,
	WEAPON_KNIFE_BAYONET = 500,         // 0
	WEAPON_KNIFE_FLIP = 505,            // 1
	WEAPON_KNIFE_GUT = 506,             // 2
	WEAPON_KNIFE_KARAMBIT = 507,        // 3
	WEAPON_KNIFE_M9_BAYONET = 508,      // 4
	WEAPON_KNIFE_TACTICAL = 509,        // 5
	WEAPON_KNIFE_FALCHION = 512,        // 6
	WEAPON_KNIFE_SURVIVAL_BOWIE = 514,  // 7
	WEAPON_KNIFE_BUTTERFLY = 515,       // 8
	WEAPON_KNIFE_PUSH = 516,            // 9
	WEAPON_KNIFE_URSUS = 519,           // 10
	WEAPON_KNIFE_GYPSY_JACKKNIFE = 520, // 11
	WEAPON_KNIFE_STILETTO = 522,        // 12
	WEAPON_KNIFE_WIDOWMAKER = 523       // 13
};