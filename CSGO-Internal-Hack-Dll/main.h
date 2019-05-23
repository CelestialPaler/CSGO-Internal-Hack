#pragma once

#include <chrono>
#include <thread>

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

#include "..//include/ImGUI/imgui.h"
#include <Indicium/Engine/IndiciumCore.h>
#include <Indicium/Engine/IndiciumDirect3D9.h>
#include <Indicium/Engine/IndiciumDirect3D10.h>
#include <Indicium/Engine/IndiciumDirect3D11.h>

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

#pragma region Game Data Init

void GameDataInit(void)
{
	for (size_t i = 0; i < 19; i++)
		teammates.push_back(std::make_unique<Player>());
	for (size_t i = 0; i < 20; i++)
		enemy.push_back(std::make_unique<Player>());
	for (size_t i = 0; i < 256; i++)
		glowObjects.push_back(std::make_unique<GlowObject>());
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

	ss << "  Coords : (" << std::setw(4) << (float)localPlayer->bodyGameCoords.x << "," << (float)localPlayer->bodyGameCoords.y << "," << (float)localPlayer->bodyGameCoords.z << ")";
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

		ss << "  Coords : (" << std::setw(4) << (float)teammates.at(i)->bodyGameCoords.x << "," << (float)teammates.at(i)->bodyGameCoords.y << "," << (float)teammates.at(i)->bodyGameCoords.z << ")";
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

		ss << "  Coords : (" << std::setw(4) << (float)enemy.at(i)->bodyGameCoords.x << "," << (float)enemy.at(i)->bodyGameCoords.y << "," << (float)enemy.at(i)->bodyGameCoords.z << ")";
		ImGui::Text(ss.str().c_str());
		ss.str("");
	}
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
int triggerDelay = 2;
// 主窗口
void ShowMainWindow(void)
{
	ImGui::Begin("CSGO Internal Hack Demo", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
	ImGui::Text("Copyright (c) 2019 Celestial Tech All rights reserved.");

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
		ss << "  Dev Version : " << Util::StringManipulation::WstringToString(devVersion);
		ImGui::Text(ss.str().c_str());
		ss.str("");
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
	}
	if (ImGui::CollapsingHeader("Hacks"))
	{
		ImGui::Checkbox("F1 - Menu", &FunctionEnableFlag::bMenu);
		ImGui::Checkbox("F2 - TriggerBot", &FunctionEnableFlag::bTriggerBot);
		ImGui::Checkbox("F3 - AimBot", &FunctionEnableFlag::bAimBot);
		ImGui::Checkbox("F4 - Glow", &FunctionEnableFlag::bGlow);
		ImGui::Checkbox("F5 - ESP", &FunctionEnableFlag::bESP);
		ImGui::Checkbox("F6 - RadarHack", &FunctionEnableFlag::bRadarHack);
		ImGui::Checkbox("F7 - BHop", &FunctionEnableFlag::bBHop);
		ImGui::Checkbox("F8 - SkinChanger", &FunctionEnableFlag::bSkinChanger);
	}
	if (ImGui::CollapsingHeader("Misc"))
	{
		ImGui::Separator();
		ImGui::Checkbox("  Read LocalPlayer Info", &FunctionEnableFlag::bReadLocalPlayerInfo);
		ImGui::Checkbox("  Read OtherPlayer Info", &FunctionEnableFlag::bReadOtherPlayerInfo);
		ImGui::Checkbox("  Read Skin Info", &FunctionEnableFlag::bReadSkinInfo);
		ImGui::Checkbox("  Read Glow Object Info", &FunctionEnableFlag::bReadGlowObjectInfo);
		ImGui::Separator();
		if (ImGui::Button("SkinChanger")) { SkinChangerB(); }
		ImGui::SameLine();
		if (ImGui::Button("ForceFullUpdate")) { ForceFullUpdate(); }
		if (ImGui::Button("GlowOnce")) { Glow(); }
	}
	if (ImGui::CollapsingHeader("Setting"))
	{
		if (ImGui::TreeNode("Transparency"))
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

		if (ImGui::TreeNode("Skin Setting"))
		{
			const char* items[] = { "AAAA", "BBBB", "CCCC", "DDDD", "EEEE", "FFFF", "GGGG", "HHHH", "IIII", "JJJJ", "KKKK", "LLLLLLL", "MMMM", "OOOOOOO" };
			static int item_current = 0;
			ImGui::Combo("combo", &item_current, items, IM_ARRAYSIZE(items));
			ImGui::TreePop();
		}
	}
	if (ImGui::CollapsingHeader("Debug"))
	{
		if (FunctionEnableFlag::bReadLocalPlayerInfo)
			ShowLocalPlayerInfo();
		if (FunctionEnableFlag::bReadSkinInfo)
			ShowSkinInfo();
		if (FunctionEnableFlag::bReadOtherPlayerInfo)
			ShowOtherPlayerInfo();
		if (FunctionEnableFlag::bReadGlowObjectInfo)
			ShowGlowObjectInfo();
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
		Glow();

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