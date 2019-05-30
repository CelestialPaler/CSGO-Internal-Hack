#pragma once

#include <chrono>
#include <thread>
#include <iostream>
#include <string>
#include <algorithm>
#include <set>

#include "GameData.h"
#include "GameDef.h"
#include "GameStruct.h"
#include "StringManipulation.h"

#include "TriggerBot.h"
#include "RadarHack.h"
#include "BHop.h"
#include "SkinChanger.h"
#include "PlayerInfoReader.h"
#include "Glow.h"
#include "AimBot.h"

#include "..//include/ImGUI/imgui.h"
#include <Indicium/Engine/IndiciumCore.h>
#include <Indicium/Engine/IndiciumDirect3D9.h>
#include <Indicium/Engine/IndiciumDirect3D10.h>
#include <Indicium/Engine/IndiciumDirect3D11.h>

#include <d3d9.h>

/**
 * \fn	TOGGLE_STATE(int key, bool& toggle)
 *
 * \brief	Overly complicated key toggle helper
 *
 * \author	Benjamin "Nefarius" H鰃linger-Stelzer
 * \date	7/09/2018
 *
 * \param 		  	key   	The key.
 * \param [in,out]	toggle	The value to toggle.
 */
VOID
FORCEINLINE
TOGGLE_STATE(int key, bool& toggle)
{
	static auto pressedPast = false, pressedNow = false;

	if (GetAsyncKeyState(key) & 0x8000)
	{
		pressedNow = true;
	}
	else
	{
		pressedPast = false;
		pressedNow = false;
	}

	if (!pressedPast && pressedNow)
	{
		toggle = !toggle;

		pressedPast = true;
	}
}

static void HelpMarker(const char* desc)
{
	ImGui::TextDisabled("(?)");
	if (ImGui::IsItemHovered())
	{
		ImGui::BeginTooltip();
		ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
		ImGui::TextUnformatted(desc);
		ImGui::PopTextWrapPos();
		ImGui::EndTooltip();
	}
}

#pragma region Hack Stop

void HackStop(void)
{
	FunctionEnableFlag::bAimBot = false;
	FunctionEnableFlag::bBHop = false;
	FunctionEnableFlag::bESP = false;
	FunctionEnableFlag::bGlow = false;
	FunctionEnableFlag::bMenu = false;
	FunctionEnableFlag::bOverlay = false;
	FunctionEnableFlag::bRadarHack = false;
	FunctionEnableFlag::bReadGlowObjectInfo = false;
	FunctionEnableFlag::bReadLocalPlayerInfo = false;
	FunctionEnableFlag::bReadOtherPlayerInfo = false;
	FunctionEnableFlag::bReadSkinInfo = false;
	FunctionEnableFlag::bSkinChanger = false;
	FunctionEnableFlag::bTriggerBot = false;

	std::this_thread::sleep_for(std::chrono::milliseconds(500));
}

#pragma endregion

#pragma region Game Data Init

void GameDataInit(void)
{
	for (size_t i = 0; i < 19; i++)
		teammates.push_back(std::make_unique<Player>());
	for (size_t i = 0; i < 20; i++)
		enemy.push_back(std::make_unique<Player>());
	for (size_t i = 0; i < 256; i++)
		glowObjects.push_back(std::make_unique<GlowObject>());

	std::ifstream skinIDFile("C:\\Users\\sun11\\Desktop\\SkinID.csv");
	std::string line;
	std::string weaponName;

	while (getline(skinIDFile, line))
	{
		if (line.empty()) { continue; }
		skinLoadedCount++;

		std::vector<std::string> tempCSVData;
		tempCSVData = Util::StringManipulation::SplitString(line, ",");

		if (weapons.find(tempCSVData.at(1))== weapons.end())// 不存在则添加
		{
			weapons.emplace(tempCSVData.at(1), std::vector<std::string>());
		}
		
		weapons.at(tempCSVData.at(1)).push_back(tempCSVData.at(2));

		skins.emplace(tempCSVData.at(2), std::atoi(tempCSVData.at(3).c_str()));
	}
}

#pragma endregion

#pragma region ImGui Render Funciton

void ShowLocalPlayerInfo(void)
{
	ImGui::Separator();
	ImGui::Text("LocalPlayer Info:");
	ImGui::Separator();
	std::stringstream ss;
	ss << "  BaseAddr : 0x" << std::hex << std::uppercase << localPlayer->dwBaseAddr << std::dec;
	ImGui::Text(ss.str().c_str());
	ss.str("");

	ss << "  Health : " << localPlayer->health;
	ImGui::Text(ss.str().c_str());
	ss.str("");

	ImGui::SameLine();
	ss << "  Team : " << localPlayer->team;
	ImGui::Text(ss.str().c_str());
	ss.str("");

	ImGui::SameLine();
	ss << "  ID : " << localPlayer->id;
	ImGui::Text(ss.str().c_str());
	ss.str("");

	ss << "  Body Coords : (" << std::setprecision(4) << std::fixed << (float)localPlayer->bodyGameCoords.x << "," << (float)localPlayer->bodyGameCoords.y << "," << (float)localPlayer->bodyGameCoords.z << ")";
	ImGui::Text(ss.str().c_str());
	ss.str("");

	ss << "  Head Coords : (" << std::setprecision(4) << std::fixed << (float)localPlayer->headGameCoords.x << "," << (float)localPlayer->headGameCoords.y << "," << (float)localPlayer->headGameCoords.z << ")";
	ImGui::Text(ss.str().c_str());
	ss.str("");

	ss << "  View Angle : (" << std::setprecision(4) << std::fixed << (float)localPlayer->aimAngle.x << "," << (float)localPlayer->aimAngle.y << ")";
	ImGui::Text(ss.str().c_str());
	ss.str("");

	ss << "  AimID : " << localPlayer->aimID;
	ImGui::Text(ss.str().c_str());
	ss.str("");

	ImGui::SameLine();
	ss << "  WeaponID : " << localPlayer->weaponID;
	ImGui::Text(ss.str().c_str());
	ss.str("");
}

void ShowOtherPlayerInfo(void)
{
	ImGui::Separator();
	ImGui::Text("OtherPlayer Info:");
	for (size_t i = 0; i < teammates.size(); i++)
	{
		if (!teammates.at(i)->isValid)
			continue;

		ImGui::Separator();
		std::stringstream ss;
		ss << "  BaseAddr : 0x" << std::hex << std::uppercase << teammates.at(i)->dwBaseAddr;
		ImGui::Text(ss.str().c_str());
		ss.str("");

		ImGui::SameLine();
		ss << "  Dormant : " << (teammates.at(i)->isDormant ? "True" : "False");
		ImGui::Text(ss.str().c_str());
		ss.str("");

		ss << "  Health : " << std::dec << teammates.at(i)->health;
		ImGui::Text(ss.str().c_str());
		ss.str("");

		ImGui::SameLine();
		ss << "  Team : " << teammates.at(i)->team;
		ImGui::Text(ss.str().c_str());
		ss.str("");

		ImGui::SameLine();
		ss << "  ID : " << teammates.at(i)->id;
		ImGui::Text(ss.str().c_str());
		ss.str("");

		ss << "  Body Coords : (" << std::setprecision(4) << std::fixed << (float)teammates.at(i)->bodyGameCoords.x << "," << (float)teammates.at(i)->bodyGameCoords.y << "," << (float)teammates.at(i)->bodyGameCoords.z << ")";
		ImGui::Text(ss.str().c_str());
		ss.str("");

		ss << "  Head Coords : (" << std::setprecision(4) << std::fixed << (float)teammates.at(i)->headGameCoords.x << "," << (float)teammates.at(i)->headGameCoords.y << "," << (float)teammates.at(i)->headGameCoords.z << ")";
		ImGui::Text(ss.str().c_str());
		ss.str("");

		ss << "  View Angle : (" << std::setprecision(4) << std::fixed << (float)teammates.at(i)->aimAngle.x << "," << (float)teammates.at(i)->aimAngle.y << ")";
		ImGui::Text(ss.str().c_str());
		ss.str("");

		ss << "  Delta Angle : (" << std::setprecision(4) << std::fixed << (float)teammates.at(i)->angleDelta.x << "," << (float)teammates.at(i)->angleDelta.y << ")";
		ImGui::Text(ss.str().c_str());
		ss.str("");
	}

	for (size_t i = 0; i < enemy.size(); i++)
	{
		if (!enemy.at(i)->isValid)
			continue;

		ImGui::Separator();
		std::stringstream ss;
		ss << "  BaseAddr : 0x" << std::hex << std::uppercase << enemy.at(i)->dwBaseAddr;
		ImGui::Text(ss.str().c_str());
		ss.str("");

		ImGui::SameLine();
		ss << "  Dormant : " << (enemy.at(i)->isDormant ? "True" : "False");
		ImGui::Text(ss.str().c_str());
		ss.str("");

		ss << "  Health : " << std::dec << enemy.at(i)->health;
		ImGui::Text(ss.str().c_str());
		ss.str("");

		ImGui::SameLine();
		ss << "  Team : " << enemy.at(i)->team;
		ImGui::Text(ss.str().c_str());
		ss.str("");

		ImGui::SameLine();
		ss << "  ID : " << enemy.at(i)->id;
		ImGui::Text(ss.str().c_str());
		ss.str("");

		ss << "  Body Coords : (" << std::setprecision(4) << std::fixed << (float)enemy.at(i)->bodyGameCoords.x << "," << (float)enemy.at(i)->bodyGameCoords.y << "," << (float)enemy.at(i)->bodyGameCoords.z << ")";
		ImGui::Text(ss.str().c_str());
		ss.str("");

		ss << "  Head Coords : (" << std::setprecision(4) << std::fixed << (float)enemy.at(i)->headGameCoords.x << "," << (float)enemy.at(i)->headGameCoords.y << "," << (float)enemy.at(i)->headGameCoords.z << ")";
		ImGui::Text(ss.str().c_str());
		ss.str("");

		ss << "  View Angle : (" << std::setprecision(4) << std::fixed << (float)enemy.at(i)->aimAngle.x << "," << (float)enemy.at(i)->aimAngle.y << ")";
		ImGui::Text(ss.str().c_str());
		ss.str("");

		ss << "  Delta Angle : (" << std::setprecision(4) << std::fixed << (float)enemy.at(i)->angleDelta.x << "," << (float)enemy.at(i)->angleDelta.y << ")";
		ImGui::Text(ss.str().c_str());
	}
}

void ShowAimInfo(void)
{
	ImGui::Separator();
	Skin skin = ReadSkinInfo();
	ImGui::Text("Aim Info:");
	ImGui::Separator();
	std::stringstream ss;
	ss << "  Angle Delta: (" << std::setprecision(4) << std::fixed << (float)angleDelta.x << "," << (float)angleDelta.y << ")";
	ImGui::Text(ss.str().c_str());
	ss.str("");

	ss << "  Nearest Enemy: " << nearestEnemy;
	ImGui::Text(ss.str().c_str());
	ss.str("");
}

void ShowSkinInfo(void)
{
	ImGui::Separator();
	Skin skin = ReadSkinInfo();
	ImGui::Text("Skin Info:");
	ImGui::Separator();
	std::stringstream ss;
	ss << "  HID : " << skin.itemIDHigh;
	ImGui::Text(ss.str().c_str());
	ss.str("");

	ImGui::SameLine();
	ss << "  LID : " << skin.itemIDLow;
	ImGui::Text(ss.str().c_str());
	ss.str("");

	ss << "  PaintKit : " << skin.paintKit;
	ImGui::Text(ss.str().c_str());
	ss.str("");

	ImGui::SameLine();
	ss << "  StatTrak : " << skin.statTrakCount;
	ImGui::Text(ss.str().c_str());
	ss.str("");

	ss << "  Quality : " << skin.quality;
	ImGui::Text(ss.str().c_str());
	ss.str("");

	ImGui::SameLine();
	ss << "  Wear : " << skin.wear;
	ImGui::Text(ss.str().c_str());
	ss.str("");

	ImGui::SameLine();
	ss << "  Seed : " << skin.seed;
	ImGui::Text(ss.str().c_str());
	ss.str("");
}

void ShowGlowObjectInfo(void)
{
	ImGui::Separator();
	ImGui::Text("Glow Info : ");
	ImGui::Separator();
	std::stringstream ss;

	ss << "  Count (Current / Max) : " << std::dec << glowObjectCount << " / " << glowObjectCountMax;
	ImGui::Text(ss.str().c_str());
	ss.str("");

	for (size_t i = 0; i < glowObjectCount; i++)
	{
		if (glowObjects.at(i)->dwEntityAddr == NULL) { continue; };
		ImGui::Separator();

		ss << "  Entity Addr : 0x" << std::hex << std::uppercase << (int)glowObjects.at(i)->dwEntityAddr << std::dec;
		ImGui::Text(ss.str().c_str());
		ss.str("");

		ImGui::SameLine();
		ss << "  RGBA : (" << glowObjects.at(i)->r << "," << glowObjects.at(i)->g << "," << glowObjects.at(i)->b << "," << glowObjects.at(i)->a << ")";
		ImGui::Text(ss.str().c_str());
		ss.str("");

		ss << "  Occluded : " << (glowObjects.at(i)->m_bRenderWhenOccluded ? "True" : "False");
		ImGui::Text(ss.str().c_str());
		ss.str("");

		ImGui::SameLine();
		ss << "  Unoccluded : " << (glowObjects.at(i)->m_bRenderWhenUnoccluded ? "True" : "False");
		ImGui::Text(ss.str().c_str());
		ss.str("");
	}
}

#pragma endregion 

#pragma region ImGui Render Main

float menuAlpha = 0.8;
float menuAlphaPre = 0;

// 主窗口
void ShowMainWindow(void)
{
	ImGui::Begin("CSGO Internal Hack", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
	ImGui::Text("Copyright (c) 2019 Celestial Tech  All rights reserved.");

	// 窗口透明度
	if (menuAlpha != menuAlphaPre)
	{
		ImGui::GetStyle().Alpha = menuAlpha;
		menuAlphaPre = menuAlpha;
	}

	// 信息
	if (ImGui::CollapsingHeader("Info", ImGuiTreeNodeFlags_DefaultOpen))
	{
		std::stringstream ss;

		__time64_t sysTime = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
		struct tm pTime;
		localtime_s(&pTime, &sysTime);
		char timeInString[60] = { 0 };
		sprintf_s(timeInString, "%d-%02d-%02d %02d:%02d:%02d",
			(int)pTime.tm_year + 1900,
			(int)pTime.tm_mon + 1,
			(int)pTime.tm_mday,
			(int)pTime.tm_hour,
			(int)pTime.tm_min,
			(int)pTime.tm_sec);
		ss << "  System Time : " << timeInString;
		ImGui::Text(ss.str().c_str());
		ss.str("");

		ss << "  Dev Version : " << Util::StringManipulation::WstringToString(devVersion);
		ImGui::Text(ss.str().c_str());
		ss.str("");

		ss << "  Author : " << "Celestial Paler";
		ImGui::Text(ss.str().c_str());
		ss.str("");

		ss << "  Website : " << "www.tianshicangxie.com";
		ImGui::Text(ss.str().c_str());
		ss.str("");
	}
	if (ImGui::CollapsingHeader("Hacks"))
	{
		ImGui::Checkbox("F1 - Menu", &FunctionEnableFlag::bMenu);
		ImGui::SameLine(); HelpMarker("Show the main menu.");
		ImGui::Checkbox("F2 - TriggerBot", &FunctionEnableFlag::bTriggerBot);
		ImGui::SameLine(); HelpMarker("Automatically trigger(fire) your weapon if your crosshair is pointing to an enemy.");
		ImGui::Checkbox("F3 - AimBot", &FunctionEnableFlag::bAimBot);
		ImGui::SameLine(); HelpMarker("Aim assistant such as:\n   - FOV-snap\n   - Head Lock\n   - Anti-recoil");
		ImGui::Checkbox("F4 - Glow", &FunctionEnableFlag::bGlow);
		ImGui::SameLine(); HelpMarker("All the entities in game will glow in X-ray style.");
		ImGui::Checkbox("F5 - ESP", &FunctionEnableFlag::bESP);
		ImGui::SameLine(); HelpMarker("Comming soon.");
		ImGui::Checkbox("F6 - RadarHack", &FunctionEnableFlag::bRadarHack);
		ImGui::SameLine(); HelpMarker("Show all the enemy nearby you on the in-game radar.");
		ImGui::Checkbox("F7 - BHop", &FunctionEnableFlag::bBHop);
		ImGui::SameLine(); HelpMarker("Automatically perform bunny hop when you hold space.");
		ImGui::Checkbox("F8 - SkinChanger", &FunctionEnableFlag::bSkinChanger);
		ImGui::SameLine(); HelpMarker("Change your skin as you wish without limitation.\n   - Weapon & Knife & Glove\n   - StatTrack & Sticker & Nametag\n   - Seed & Worn");
	}
	if (ImGui::CollapsingHeader("Setting"))
	{
		if (ImGui::TreeNode("Menu Setting"))
		{
			ImGui::Separator();
			ImGui::Text("  Menu Transparency : ");
			ImGui::SliderFloat("Alpha", &menuAlpha, 0.0f, 1.0f);

			ImGui::TreePop();
		}

		if (ImGui::TreeNode("Glow Setting"))
		{
			ImGui::Separator();
			ImGui::Text("Glow Color : ");
			ImGui::Separator();

			ImGui::ColorEdit4("C1", glowColorTeammates, ImGuiColorEditFlags_PickerHueWheel);
			ImGui::SameLine();
			ImGui::Checkbox("E1 Teanmate", &FunctionEnableFlag::bGlowTeammates);
			ImGui::Separator();

			ImGui::ColorEdit4("C2", glowColorEnemy, ImGuiColorEditFlags_PickerHueWheel);
			ImGui::SameLine();
			ImGui::Checkbox("E2 Enemy", &FunctionEnableFlag::bGlowEnemy);
			ImGui::Separator();

			ImGui::ColorEdit4("C3", glowColorWeapons, ImGuiColorEditFlags_PickerHueWheel);
			ImGui::SameLine();
			ImGui::Checkbox("E3 Weapons", &FunctionEnableFlag::bGlowWeapons);
			ImGui::Separator();

			ImGui::ColorEdit4("C4", glowColorC4, ImGuiColorEditFlags_PickerHueWheel);
			ImGui::SameLine();
			ImGui::Checkbox("E4 C4", &FunctionEnableFlag::bGlowC4);
			ImGui::Separator();

			ImGui::ColorEdit4("C5", glowColorDefault, ImGuiColorEditFlags_PickerHueWheel);
			ImGui::SameLine();
			ImGui::Checkbox("E5 Default", &FunctionEnableFlag::bGlowDefault);
			ImGui::Separator();

			ImGui::TreePop();
		}

		if (ImGui::TreeNode("TriggerBot Setting"))
		{
			ImGui::Separator();
			ImGui::Text("Trigger delay : ");
			ImGui::SameLine();
			ImGui::InputInt("ms", &triggerDelay);
			ImGui::Separator();

			ImGui::TreePop();
		}

		if (ImGui::TreeNode("AimBot Setting"))
		{
			ImGui::Separator();
			ImGui::Checkbox("Static FOV", &FunctionEnableFlag::bAimBotStaticFOV);
			ImGui::SliderFloat("FOV threshold", &aimLockFov, 0.0f, 180.0f);
			ImGui::Separator();
			ImGui::Checkbox("Dynamic FOV", &FunctionEnableFlag::bAimBotDynamicFOV);
			ImGui::SliderInt("Distance Base", &aimLockDistanceBase, 1, 100);
			ImGui::SliderFloat("Distance Sensitivity", &aimLockDistanceSensitivity, 0.0f, 1.0f);
			ImGui::Separator();
			if (ImGui::SliderFloat("Horizontal Sensitivity", &aimLockHorizontalSensitivity, 0.0f, 1.0f))
			{
				aimLockVerticalSensitivity = 1 - aimLockHorizontalSensitivity;
			}
			if (ImGui::SliderFloat("Vertical Sensitivity", &aimLockVerticalSensitivity, 0.0f, 1.0f))
			{
			    aimLockHorizontalSensitivity = 1 - aimLockVerticalSensitivity;
			}
			ImGui::Separator();
			ImGui::Checkbox("nmsl", &FunctionEnableFlag::bAimBotSima);

			ImGui::TreePop();
		}

		if (ImGui::TreeNode("Skin Setting"))
		{
			ImGui::Separator();
			static std::vector<std::string> tempSkinInfo;

			static const char* items_weapon[512];
			size_t i = 0;
			for (auto const& weapon : weapons)
				items_weapon[i++] = weapon.first.c_str();
			static int item_current_weapon = 0;
	
			if (ImGui::Combo("Weapon", &item_current_weapon, items_weapon, weapons.size()))
			{
				// 更换武器
				tempSkinInfo = weapons.at(items_weapon[item_current_weapon]);
			}

			static const char* items_skin[512];
			size_t j = 0;
			for (auto const& skin : tempSkinInfo)
				items_skin[j++] = skin.c_str();
			static int item_current_skin = 0;

			if (ImGui::Combo("Skin", &item_current_skin, items_skin, tempSkinInfo.size()))
			{
				// 更换皮肤
			}
			ImGui::Separator();

			static float worn = 0.1;
			static int seed = 7254798721;
			static int statTrack = 999;
			static char name[64] = "Hammann";

			ImGui::SliderFloat("Worn", &worn, 0.0f, 1.0f);
			ImGui::InputText("Nametag", name, sizeof(name));
			ImGui::InputInt("Seed", &seed, 1, 100);
			ImGui::InputInt("StatTrack", &statTrack, 1, 100);

			if (ImGui::Button("Apply")) { ; }
			ImGui::SameLine();
			if (ImGui::Button("Save to config")) { ; }

			ImGui::TreePop();
		}
	}

	if (ImGui::CollapsingHeader("Debug"))
	{
		ImGui::Separator();
		ImGui::Checkbox("  Read LocalPlayer Info", &FunctionEnableFlag::bReadLocalPlayerInfo);
		ImGui::Checkbox("  Read OtherPlayer Info", &FunctionEnableFlag::bReadOtherPlayerInfo);
		ImGui::Checkbox("  Read Skin Info", &FunctionEnableFlag::bReadSkinInfo);
		ImGui::Checkbox("  Read Glow Object Info", &FunctionEnableFlag::bReadGlowObjectInfo);

		ImGui::Checkbox("  Show Aim Info", &FunctionEnableFlag::bShowAimInfo);
		ImGui::Checkbox("  Show LocalPlayer Info", &FunctionEnableFlag::bShowLocalPlayerInfo);
		ImGui::Checkbox("  Show OtherPlayer Info", &FunctionEnableFlag::bShowOtherPlayerInfo);
		ImGui::Checkbox("  Show Skin Info", &FunctionEnableFlag::bShowSkinInfo);
		ImGui::Checkbox("  Show Glow Object Info", &FunctionEnableFlag::bShowGlowObjectInfo);

		ImGui::Separator();
		if (ImGui::Button("SkinChanger")) { SkinChangerB(); }
		ImGui::SameLine();
		if (ImGui::Button("ForceFullUpdate")) { ForceFullUpdate(); }
		ImGui::SameLine();
		if (ImGui::Button("GlowOnce")) { GlowA(); }
		ImGui::Separator();

		if (FunctionEnableFlag::bShowLocalPlayerInfo)
			ShowLocalPlayerInfo();
		if (FunctionEnableFlag::bShowOtherPlayerInfo)
			ShowOtherPlayerInfo();
		if (FunctionEnableFlag::bShowSkinInfo)
			ShowSkinInfo();
		if (FunctionEnableFlag::bShowGlowObjectInfo)
			ShowGlowObjectInfo();
		if (FunctionEnableFlag::bShowAimInfo)
			ShowAimInfo();
	}
	ImGui::End();
}

#pragma endregion

#pragma region Hack logic

// 外挂
void Hack(void)
{
	if (FunctionEnableFlag::bReadLocalPlayerInfo)
		ReadLocalPlayerInfo();
	if (FunctionEnableFlag::bReadOtherPlayerInfo)
		ReadOtherPlayerInfo();
	if (FunctionEnableFlag::bReadGlowObjectInfo)
		ReadGlowObjectInfo();

	if (FunctionEnableFlag::bBHop)
		BHop();
	if (FunctionEnableFlag::bTriggerBot)
		TriggerBot();
	if (FunctionEnableFlag::bRadarHack)
		RadarHack();

	if (FunctionEnableFlag::bSkinChanger)
	{
		if (!ThreadExistFlag::bSkinChanger)
		{
			CreateThread(NULL, 0, SkinChangerWrapper, 0, 0, NULL);
			ThreadExistFlag::bSkinChanger = true;
		}
	}

	if (FunctionEnableFlag::bGlow)
	{
		if (!ThreadExistFlag::bGlow)
		{
			CreateThread(NULL, 0, GlowWrapper, 0, 0, NULL);
			ThreadExistFlag::bGlow = true;
		}
	}

	if (FunctionEnableFlag::bAimBot)
	{
		if (!ThreadExistFlag::bAimBot)
		{
			CreateThread(NULL, 0, AimBotWrapper, 0, 0, NULL);
			ThreadExistFlag::bAimBot = true;
		}
	}

	TOGGLE_STATE(VK_F1, FunctionEnableFlag::bMenu);
	TOGGLE_STATE(VK_F2, FunctionEnableFlag::bTriggerBot);
	TOGGLE_STATE(VK_F3, FunctionEnableFlag::bAimBot);
	TOGGLE_STATE(VK_F4, FunctionEnableFlag::bGlow);
	TOGGLE_STATE(VK_F5, FunctionEnableFlag::bESP);
	TOGGLE_STATE(VK_F6, FunctionEnableFlag::bRadarHack);
	TOGGLE_STATE(VK_F7, FunctionEnableFlag::bBHop);
	TOGGLE_STATE(VK_F8, FunctionEnableFlag::bSkinChanger);
}

#pragma endregion 