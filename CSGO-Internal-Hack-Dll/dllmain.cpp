/*
MIT License

Copyright (c) 2018 Benjamin H鰃linger

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#include "dllmain.h"

// 
// ImGui includes
// 
#include "imgui/imgui.h"
#include "imgui_impl_dx9.h"
#include "imgui_impl_dx10.h"
#include "imgui_impl_dx11.h"
#include "imgui_impl_win32.h"

t_WindowProc OriginalDefWindowProc = nullptr;
t_WindowProc OriginalWindowProc = nullptr;
PINDICIUM_ENGINE engine = nullptr;

#define WNDPROC_HOOK

/**
 * \fn  BOOL WINAPI DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID)
 *
 * \brief   DLL main entry point. Only Indicium engine initialization or shutdown should happen
 *          here to avoid deadlocks.
 *
 * \author  Benjamin "Nefarius" H鰃linger
 * \date    16.06.2018
 *
 * \param   hInstance   The instance handle.
 * \param   dwReason    The call reason.
 * \param   parameter3  Unused.
 *
 * \return  TRUE on success, FALSE otherwise (will abort loading the library).
 */
BOOL WINAPI DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID)
{
	//
	// We don't need to get notified in thread attach- or detachments
	// 
	DisableThreadLibraryCalls(static_cast<HMODULE>(hInstance));

	INDICIUM_D3D9_EVENT_CALLBACKS d3d9;
	INDICIUM_D3D9_EVENT_CALLBACKS_INIT(&d3d9);
	d3d9.EvtIndiciumD3D9PrePresent = EvtIndiciumD3D9Present;
	d3d9.EvtIndiciumD3D9PreReset = EvtIndiciumD3D9PreReset;
	d3d9.EvtIndiciumD3D9PostReset = EvtIndiciumD3D9PostReset;
	d3d9.EvtIndiciumD3D9PrePresentEx = EvtIndiciumD3D9PresentEx;
	d3d9.EvtIndiciumD3D9PreResetEx = EvtIndiciumD3D9PreResetEx;
	d3d9.EvtIndiciumD3D9PostResetEx = EvtIndiciumD3D9PostResetEx;

	INDICIUM_D3D10_EVENT_CALLBACKS d3d10;
	INDICIUM_D3D10_EVENT_CALLBACKS_INIT(&d3d10);
	d3d10.EvtIndiciumD3D10PrePresent = EvtIndiciumD3D10Present;
	d3d10.EvtIndiciumD3D10PreResizeBuffers = EvtIndiciumD3D10PreResizeBuffers;
	d3d10.EvtIndiciumD3D10PostResizeBuffers = EvtIndiciumD3D10PostResizeBuffers;

	INDICIUM_D3D11_EVENT_CALLBACKS d3d11;
	INDICIUM_D3D11_EVENT_CALLBACKS_INIT(&d3d11);
	d3d11.EvtIndiciumD3D11PrePresent = EvtIndiciumD3D11Present;
	d3d11.EvtIndiciumD3D11PreResizeBuffers = EvtIndiciumD3D11PreResizeBuffers;
	d3d11.EvtIndiciumD3D11PostResizeBuffers = EvtIndiciumD3D11PostResizeBuffers;

	INDICIUM_ERROR err;

	switch (dwReason)
	{
	case DLL_PROCESS_ATTACH:

		if (!engine)
		{
			//
			// Get engine handle
			// 
			engine = IndiciumEngineAlloc();

			//
			// Register render pipeline callbacks
			// 
			IndiciumEngineSetD3D9EventCallbacks(engine, &d3d9);
			IndiciumEngineSetD3D10EventCallbacks(engine, &d3d10);
			IndiciumEngineSetD3D11EventCallbacks(engine, &d3d11);

			// 
			// TODO: cover failure
			//
			err = IndiciumEngineInit(engine, EvtIndiciumGameHooked);
		}

		break;
	case DLL_PROCESS_DETACH:

		if (engine)
		{
			IndiciumEngineShutdown(engine, EvtIndiciumGameUnhooked);
			IndiciumEngineFree(engine);
		}

		break;
	default:
		break;
	}

	return TRUE;
}

/**
 * \fn  void EvtIndiciumGameHooked(const INDICIUM_D3D_VERSION GameVersion)
 *
 * \brief   Gets called when the games' rendering pipeline has successfully been hooked and the
 *          rendering callbacks are about to get fired. The detected version of the used
 *          rendering objects is reported as well.
 *
 * \author  Benjamin "Nefarius" H鰃linger
 * \date    16.06.2018
 *
 * \param   GameVersion The detected DirectX/Direct3D version.
 */
void EvtIndiciumGameHooked(const INDICIUM_D3D_VERSION GameVersion)
{
    IndiciumEngineLogInfo("Loading ImGui plugin");

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;  // Enable Keyboard Controls
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;   // Enable Gamepad Controls

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsClassic();
}

/**
 * \fn  void EvtIndiciumGameUnhooked()
 *
 * \brief   Gets called when all core engine hooks have been released. At this stage it is save
 *          to remove our own additional hooks and shut down the hooking sub-system as well.
 *
 * \author  Benjamin "Nefarius" H鰃linger
 * \date    16.06.2018
 */
void EvtIndiciumGameUnhooked()
{
#ifdef WNDPROC_HOOK
	//auto& logger = Logger::get(__func__);

	if (MH_DisableHook(MH_ALL_HOOKS) != MH_OK)
	{
		//logger.fatal("Couldn't disable hooks, host process might crash");
		return;
	}

	IndiciumEngineLogInfo("Hooks disabled");

	if (MH_Uninitialize() != MH_OK)
	{
		//logger.fatal("Couldn't shut down hook engine, host process might crash");
		return;
	}
#endif
}

#pragma region D3D9(Ex)

void EvtIndiciumD3D9Present(
	LPDIRECT3DDEVICE9   pDevice,
	const RECT          *pSourceRect,
	const RECT          *pDestRect,
	HWND                hDestWindowOverride,
	const RGNDATA       *pDirtyRegion
)
{
	static auto initialized = false;
	static bool show_overlay = true;
	static bool show_console = true;
	static std::once_flag init;

	//
	// This section is only called once to initialize ImGui
	// 
	std::call_once(init, [&](LPDIRECT3DDEVICE9 pd3dDevice)
	{
		D3DDEVICE_CREATION_PARAMETERS params;

        const auto hr = pd3dDevice->GetCreationParameters(&params);
		if (FAILED(hr))
		{
            IndiciumEngineLogError("Couldn't get creation parameters from device");
			return;
		}

        ImGui_ImplWin32_Init(params.hFocusWindow);
		ImGui_ImplDX9_Init(pd3dDevice);

		IndiciumEngineLogInfo("ImGui (DX9) initialized");

		HookWindowProc(params.hFocusWindow);

		{
			for (size_t i = 0; i < 19; i++)
				teammates.push_back(std::make_unique<Player>());
			for (size_t i = 0; i < 20; i++)
				enemy.push_back(std::make_unique<Player>());
			for (size_t i = 0; i < 128; i++)
				glowObjects.push_back(std::make_unique<GlowObject>());
		}


		initialized = true;
	}, pDevice);

	if (!initialized)
		return;

	// 外挂程序
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
			SkinChanger();
		if (FunctionEnableFlag::bGlow)
			Glow();
	}

	TOGGLE_STATE(VK_F9, FunctionEnableFlag::bMenu);
	if (!FunctionEnableFlag::bMenu)
		return;

    // Start the Dear ImGui frame
    ImGui_ImplDX9_NewFrame();
    ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	RenderScene();

	ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
}

void EvtIndiciumD3D9PreReset(
	LPDIRECT3DDEVICE9       pDevice,
	D3DPRESENT_PARAMETERS   *pPresentationParameters
)
{
	ImGui_ImplDX9_InvalidateDeviceObjects();
}

void EvtIndiciumD3D9PostReset(
	LPDIRECT3DDEVICE9       pDevice,
	D3DPRESENT_PARAMETERS   *pPresentationParameters
)
{
	ImGui_ImplDX9_CreateDeviceObjects();
}

void EvtIndiciumD3D9PresentEx(
	LPDIRECT3DDEVICE9EX     pDevice,
	const RECT              *pSourceRect,
	const RECT              *pDestRect,
	HWND                    hDestWindowOverride,
	const RGNDATA           *pDirtyRegion,
	DWORD                   dwFlags
)
{
		static auto initialized = false;
		static bool show_overlay = true;
	static std::once_flag init;

	//
	// This section is only called once to initialize ImGui
	// 
	std::call_once(init, [&](LPDIRECT3DDEVICE9EX pd3dDevice)
	{
		D3DDEVICE_CREATION_PARAMETERS params;

        const auto hr = pd3dDevice->GetCreationParameters(&params);
		if (FAILED(hr))
		{
			IndiciumEngineLogError("Couldn't get creation parameters from device");
			return;
		}

        ImGui_ImplWin32_Init(params.hFocusWindow);
		ImGui_ImplDX9_Init(pd3dDevice);

		IndiciumEngineLogInfo("ImGui (DX9Ex) initialized");

		HookWindowProc(params.hFocusWindow);

		initialized = true;

	}, pDevice);

	if (!initialized)
		return;

	TOGGLE_STATE(VK_F9, show_overlay);
	if (!show_overlay) 
		return;

    // Start the Dear ImGui frame
    ImGui_ImplDX9_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();

	RenderScene();

	ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
}

void EvtIndiciumD3D9PreResetEx(
	LPDIRECT3DDEVICE9EX     pDevice,
	D3DPRESENT_PARAMETERS   *pPresentationParameters,
	D3DDISPLAYMODEEX        *pFullscreenDisplayMode
)
{
	ImGui_ImplDX9_InvalidateDeviceObjects();
}

void EvtIndiciumD3D9PostResetEx(
	LPDIRECT3DDEVICE9EX     pDevice,
	D3DPRESENT_PARAMETERS   *pPresentationParameters,
	D3DDISPLAYMODEEX        *pFullscreenDisplayMode
)
{
	ImGui_ImplDX9_CreateDeviceObjects();
}

#pragma endregion

#pragma region D3D10

void EvtIndiciumD3D10Present(
	IDXGISwapChain  *pSwapChain,
	UINT            SyncInterval,
	UINT            Flags
)
{
	static auto initialized = false;
	static bool show_overlay = true;
	static std::once_flag init;

	//
	// This section is only called once to initialize ImGui
	// 
	std::call_once(init, [&](IDXGISwapChain *pChain)
	{
		IndiciumEngineLogInfo("Grabbing device and context pointers");

		ID3D10Device *pDevice;
		if (FAILED(D3D10_DEVICE_FROM_SWAPCHAIN(pChain, &pDevice)))
		{
			IndiciumEngineLogError("Couldn't get device from swapchain");
			return;
		}

		DXGI_SWAP_CHAIN_DESC sd;
		pChain->GetDesc(&sd);

		IndiciumEngineLogInfo("Initializing ImGui");

        ImGui_ImplWin32_Init(sd.OutputWindow);
		ImGui_ImplDX10_Init(pDevice);

		IndiciumEngineLogInfo("ImGui (DX10) initialized");

		HookWindowProc(sd.OutputWindow);

		initialized = true;
	}, pSwapChain);

	if (!initialized)
		return;

	TOGGLE_STATE(VK_F9, show_overlay);
	if (!show_overlay)
		return;

	
    // Start the Dear ImGui frame
    ImGui_ImplDX10_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();

	RenderScene();

	ImGui_ImplDX10_RenderDrawData(ImGui::GetDrawData());
}

void EvtIndiciumD3D10PreResizeBuffers(
	IDXGISwapChain  *pSwapChain,
	UINT            BufferCount,
	UINT            Width,
	UINT            Height,
	DXGI_FORMAT     NewFormat,
	UINT            SwapChainFlags
)
{
	ImGui_ImplDX10_InvalidateDeviceObjects();
}

void EvtIndiciumD3D10PostResizeBuffers(
	IDXGISwapChain  *pSwapChain,
	UINT            BufferCount,
	UINT            Width,
	UINT            Height,
	DXGI_FORMAT     NewFormat,
	UINT            SwapChainFlags
)
{
	ImGui_ImplDX10_CreateDeviceObjects();
}

#pragma endregion

#pragma region D3D11

void EvtIndiciumD3D11Present(
	IDXGISwapChain  *pSwapChain,
	UINT            SyncInterval,
	UINT            Flags
)
{
	static auto initialized = false;
	static bool show_overlay = true;
	static std::once_flag init;

	static ID3D11DeviceContext *pContext;
	static ID3D11RenderTargetView *mainRenderTargetView;

	//
	// This section is only called once to initialize ImGui
	// 
	std::call_once(init, [&](IDXGISwapChain *pChain)
	{
		IndiciumEngineLogInfo("Grabbing device and context pointers");

		ID3D11Device *pDevice;
		if (FAILED(D3D11_DEVICE_CONTEXT_FROM_SWAPCHAIN(pChain, &pDevice, &pContext)))
		{
			IndiciumEngineLogError("Couldn't get device and context from swapchain");
			return;
		}

		ID3D11Texture2D* pBackBuffer;
		pChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);
		pDevice->CreateRenderTargetView(pBackBuffer, NULL, &mainRenderTargetView);
		pBackBuffer->Release();

		DXGI_SWAP_CHAIN_DESC sd;
		pChain->GetDesc(&sd);

		IndiciumEngineLogInfo("Initializing ImGui");

        ImGui_ImplWin32_Init(sd.OutputWindow);
		ImGui_ImplDX11_Init(pDevice, pContext);

		IndiciumEngineLogInfo("ImGui (DX11) initialized");

		HookWindowProc(sd.OutputWindow);

		initialized = true;

	}, pSwapChain);

	if (!initialized)
		return;

	TOGGLE_STATE(VK_F9, show_overlay);
	if (!show_overlay)
		return;

    // Start the Dear ImGui frame
    ImGui_ImplDX11_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();

	pContext->OMSetRenderTargets(1, &mainRenderTargetView, NULL);

	RenderScene();

	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}

void EvtIndiciumD3D11PreResizeBuffers(
	IDXGISwapChain  *pSwapChain,
	UINT            BufferCount,
	UINT            Width,
	UINT            Height,
	DXGI_FORMAT     NewFormat,
	UINT            SwapChainFlags
)
{
	ImGui_ImplDX11_InvalidateDeviceObjects();
}

void EvtIndiciumD3D11PostResizeBuffers(
	IDXGISwapChain  *pSwapChain,
	UINT            BufferCount,
	UINT            Width,
	UINT            Height,
	DXGI_FORMAT     NewFormat,
	UINT            SwapChainFlags
)
{
	ImGui_ImplDX11_CreateDeviceObjects();
}

#pragma endregion

#pragma region WNDPROC Hooking

void HookWindowProc(HWND hWnd)
{
#ifdef WNDPROC_HOOK
	// auto& logger = Logger::get(__func__);

	MH_STATUS ret;

	MH_Initialize();

	if ((ret = MH_CreateHook(&DefWindowProcW, &DetourDefWindowProc, reinterpret_cast<LPVOID*>(&OriginalDefWindowProc))) != MH_OK)
	{
		IndiciumEngineLogError("Couldn't create hook for DefWindowProcW: %lu", static_cast<ULONG>(ret));
		return;
	}

	if (ret == MH_OK && MH_EnableHook(&DefWindowProcW) != MH_OK)
	{
		IndiciumEngineLogError("Couldn't enable DefWindowProcW hook");
	}

	if ((ret = MH_CreateHook(
		&DefWindowProcA,
		&DetourDefWindowProc,
		reinterpret_cast<LPVOID*>(&OriginalDefWindowProc))
		) != MH_OK)
	{
		IndiciumEngineLogError("Couldn't create hook for DefWindowProcA: %lu", static_cast<ULONG>(ret));
		return;
	}

	if (ret == MH_OK && MH_EnableHook(&DefWindowProcA) != MH_OK)
	{
		IndiciumEngineLogError("Couldn't enable DefWindowProcW hook");
	}

	auto lptrWndProc = reinterpret_cast<t_WindowProc>(GetWindowLongPtr(hWnd, GWLP_WNDPROC));

	if (MH_CreateHook(lptrWndProc, &DetourWindowProc, reinterpret_cast<LPVOID*>(&OriginalWindowProc)) != MH_OK)
	{
		//logger.warning("Couldn't create hook for GWLP_WNDPROC");
		return;
	}

	if (MH_EnableHook(lptrWndProc) != MH_OK)
	{
		IndiciumEngineLogError("Couldn't enable GWLP_WNDPROC hook");
	}
#endif
}

LRESULT WINAPI DetourDefWindowProc(
	_In_ HWND hWnd,
	_In_ UINT Msg,
	_In_ WPARAM wParam,
	_In_ LPARAM lParam
)
{
	static std::once_flag flag;
	std::call_once(flag, []() { IndiciumEngineLogInfo("++ DetourDefWindowProc called"); });

	ImGui_ImplWin32_WndProcHandler(hWnd, Msg, wParam, lParam);

	return OriginalDefWindowProc(hWnd, Msg, wParam, lParam);
}

LRESULT WINAPI DetourWindowProc(
	_In_ HWND hWnd,
	_In_ UINT Msg,
	_In_ WPARAM wParam,
	_In_ LPARAM lParam
)
{
	static std::once_flag flag;
	std::call_once(flag, []() { IndiciumEngineLogInfo("++ DetourWindowProc called"); });

	ImGui_ImplWin32_WndProcHandler(hWnd, Msg, wParam, lParam);

	return OriginalWindowProc(hWnd, Msg, wParam, lParam);
}

#pragma endregion

#pragma region Main content rendering

void ShowLocalPlayerInfo()
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

void ShowOtherPlayerInfo()
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

void ShowSkinInfo()
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

	ss << "  Total Count : " << std::dec << glowObjectCount;
	ImGui::Text(ss.str().c_str());
	ss.str("");

	for (size_t i = 0; i < glowObjectCount; i++)
	{
		if (glowObjects.at(i)->dwEntityAddr == NULL) { continue; };
		ImGui::Separator();

		ss << "  Entity Addr : 0x" << std::hex << std::uppercase << glowObjects.at(i)->dwEntityAddr << std::dec;
		ImGui::Text(ss.str().c_str());
		ss.str("");

		ImGui::SameLine();
		ss << "  RGBA : (" << std::setw(3)<< glowObjects.at(i)->r << "," << glowObjects.at(i)->g << "," << glowObjects.at(i)->b << "," << glowObjects.at(i)->a << ")";
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

float menuAlpha = 0.8;
float menuAlphaPre = 0;
float colorTeammate[4] = { 0,1,0,1 };
float colorEnemy[4] = { 1,0,0,1 };
int triggerDelay = 2;
void RenderScene()
{
	static std::once_flag flag;
	std::call_once(flag, []() { IndiciumEngineLogInfo("++ RenderScene called"); });

	// 主窗口
	if (true)
	{
		ImGui::Begin("CSGO Internal Hack Demo", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
		ImGui::Text("Copyright (c) 2019 Celestial Tech All rights reserved.");

		// 窗口透明度
		if (menuAlpha != menuAlphaPre)
		{
			ImGui::GetStyle().Alpha = menuAlpha;
			menuAlphaPre = menuAlpha;
		}

		if (ImGui::CollapsingHeader("Info"))
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
			ImGui::Checkbox("  Overlay", &FunctionEnableFlag::bOverlay);
			ImGui::Checkbox("  TriggerBot", &FunctionEnableFlag::bTriggerBot);
			ImGui::Checkbox("  AimBot", &FunctionEnableFlag::bAimBot);
			ImGui::Checkbox("  ESP", &FunctionEnableFlag::bESP);
			ImGui::Checkbox("  Wall", &FunctionEnableFlag::bWall);
			ImGui::Checkbox("  RadarHack", &FunctionEnableFlag::bRadarHack);
			ImGui::Checkbox("  BHop", &FunctionEnableFlag::bBHop);
			ImGui::Checkbox("  Glow", &FunctionEnableFlag::bGlow);
			ImGui::Checkbox("  SkinChanger", &FunctionEnableFlag::bSkinChanger);
			ImGui::Checkbox("  Menu", &FunctionEnableFlag::bMenu);
		}
		if (ImGui::CollapsingHeader("Misc"))
		{
			ImGui::Separator();
			ImGui::Checkbox("  Read LocalPlayer Info", &FunctionEnableFlag::bReadLocalPlayerInfo);
			ImGui::Checkbox("  Read OtherPlayer Info", &FunctionEnableFlag::bReadOtherPlayerInfo);
			ImGui::Checkbox("  Read Skin Info", &FunctionEnableFlag::bReadSkinInfo);
			ImGui::Checkbox("  Read Glow Object Info", &FunctionEnableFlag::bReadGlowObjectInfo);
			ImGui::Separator();
			if (ImGui::Button("SkinChanger")) { SkinChanger(); }
			ImGui::SameLine();
			if (ImGui::Button("ForceFullUpdate")) { ForceFullUpdate(); }
			if (ImGui::Button("GlowOnce")) { Glow(); }
		}
		if (ImGui::CollapsingHeader("Setting"))
		{
			ImGui::Separator();
			ImGui::Text("  Menu Transparency : ");
			ImGui::SliderFloat("Alpha", &menuAlpha, 0.0f, 1.0f);

			ImGui::Separator();
			ImGui::Text("  Glow Color : ");
			ImGui::ColorEdit4("Teanmate", glowColorTeammates, ImGuiColorEditFlags_PickerHueWheel);
			ImGui::ColorEdit4("Enemy", glowColorEnemy, ImGuiColorEditFlags_PickerHueWheel);
			ImGui::ColorEdit4("Weapons", glowColorWeapons, ImGuiColorEditFlags_PickerHueWheel);
			ImGui::ColorEdit4("C4", glowColorC4, ImGuiColorEditFlags_PickerHueWheel);

			ImGui::Separator();
			ImGui::InputInt("  Trigger Delay : ", &triggerDelay);
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
	
	ImGui::Render();
}

#pragma endregion

#pragma region ImGui-specific (taken from their examples unmodified)

bool ImGui_ImplWin32_UpdateMouseCursor()
{
	ImGuiIO& io = ImGui::GetIO();
	if (io.ConfigFlags & ImGuiConfigFlags_NoMouseCursorChange)
		return false;

	ImGuiMouseCursor imgui_cursor = io.MouseDrawCursor ? ImGuiMouseCursor_None : ImGui::GetMouseCursor();
	if (imgui_cursor == ImGuiMouseCursor_None)
	{
		// Hide OS mouse cursor if imgui is drawing it or if it wants no cursor
		::SetCursor(NULL);
	}
	else
	{
		// Hardware cursor type
		LPTSTR win32_cursor = IDC_ARROW;
		switch (imgui_cursor)
		{
		case ImGuiMouseCursor_Arrow:        win32_cursor = IDC_ARROW; break;
		case ImGuiMouseCursor_TextInput:    win32_cursor = IDC_IBEAM; break;
		case ImGuiMouseCursor_ResizeAll:    win32_cursor = IDC_SIZEALL; break;
		case ImGuiMouseCursor_ResizeEW:     win32_cursor = IDC_SIZEWE; break;
		case ImGuiMouseCursor_ResizeNS:     win32_cursor = IDC_SIZENS; break;
		case ImGuiMouseCursor_ResizeNESW:   win32_cursor = IDC_SIZENESW; break;
		case ImGuiMouseCursor_ResizeNWSE:   win32_cursor = IDC_SIZENWSE; break;
		}
		::SetCursor(::LoadCursor(NULL, win32_cursor));
	}
	return true;
}

#pragma endregion
