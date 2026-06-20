/* =============================================================
	INTRODUCTION TO GAME PROGRAMMING SE102
	
	SAMPLE 05 - SCENE MANAGER

	This sample illustrates how to:

		1/ Read scene (textures, sprites, animations and objects) from files 
		2/ Handle multiple scenes in game

	Key classes/functions:
		CScene
		CPlayScene		


HOW TO INSTALL Microsoft.DXSDK.D3DX
===================================
1) Tools > NuGet package manager > Package Manager Console
2) execute command :  Install-Package Microsoft.DXSDK.D3DX


================================================================ */

#include "D:/schooling/hkiv/game dev/imgui-master/imgui-master/imgui.h"
#include "D:\schooling\hkiv\game dev\imgui-master\imgui-master\backends\imgui_impl_win32.h"
#include "D:\schooling\hkiv\game dev\imgui-master\imgui-master\backends\imgui_impl_dx10.h"

#include <fstream>
#include <iostream>



#include <windows.h>
#include <d3d10.h>
#include <d3dx10.h>
#include <list>
#include <cmath>
#include "AssetIDs.h"

#include "debug.h"
#include "Game.h"
#include "GameObject.h"
#include "Textures.h"
#include "Animation.h"
#include "Animations.h"
#include "PlayScene.h"

//#include "Mario.h"
#include "Brick.h"
#include "Goomba.h"
#include "Coin.h"
//#include "Star.h"
#include "Platform.h"


#include "SampleKeyEventHandler.h"

#include "AssetIDs.h"

#define WINDOW_CLASS_NAME L"SampleWindow"
#define MAIN_WINDOW_TITLE L"GTCTIF (Game That Celeste Took Inspiration From)"
#define WINDOW_ICON_PATH L"favicon.ico"

//#define BACKGROUND_COLOR D3DXCOLOR(200.0f/255, 200.0f/255, 255.0f/255, 0.0f)
#define BACKGROUND_COLOR D3DXCOLOR(0.0f,0.0f,0.0f,0.0f)

// #define SCREEN_WIDTH 320
// #define SCREEN_HEIGHT 240
 #define SCREEN_WIDTH 640
 #define SCREEN_HEIGHT 480


extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

bool Enable_imGUI = false;

LRESULT CALLBACK WinProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	if (ImGui_ImplWin32_WndProcHandler(hWnd, message, wParam, lParam))
		return true;

	switch (message) {
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}

	return 0;
}

/*
	Update world status for this frame
	dt: time period between beginning of last frame and beginning of this frame
*/
void Update(DWORD dt)
{
	CGame::GetInstance()->GetCurrentScene()->Update(dt);
	// --- ADD THE TOGGLE LOGIC HERE ---
	// Let's use the F1 key (VK_F1) to toggle the editor
	static bool isF1Down = false;
	CPlayScene* scene = dynamic_cast<CPlayScene*>(CGame::GetInstance()->GetCurrentScene());
	LPGAMEOBJECT player = scene->GetPlayer(); // Move player to a visible spot when toggling editor

	if (GetAsyncKeyState(VK_F1) & 0x8000)
	{
		if (!isF1Down)
		{
			Enable_imGUI = !Enable_imGUI; // Flip the switch
			isF1Down = true;            // Lock it until the key is released
			((CPlayer*)player)->infiniteJump = !((CPlayer*)player)->infiniteJump;
		
		}
	}
	else
	{
		isF1Down = false; // Unlock when the player lets go of the key
	}
}

/*
	Render a frame 
*/
void Render()
{
	CGame* g = CGame::GetInstance();

	ID3D10Device* pD3DDevice = g->GetDirect3DDevice();
	IDXGISwapChain* pSwapChain = g->GetSwapChain();
	ID3D10RenderTargetView* pRenderTargetView = g->GetRenderTargetView();
	ID3DX10Sprite* spriteHandler = g->GetSpriteHandler();

	pD3DDevice->ClearRenderTargetView(pRenderTargetView, BACKGROUND_COLOR);

	spriteHandler->Begin(D3DX10_SPRITE_SORT_TEXTURE);

	FLOAT NewBlendFactor[4] = { 0,0,0,0 };
	pD3DDevice->OMSetBlendState(g->GetAlphaBlending(), NewBlendFactor, 0xffffffff);

	CGame::GetInstance()->GetCurrentScene()->Render();

	spriteHandler->End();

	// --- ADD EVERYTHING BELOW THIS LINE ---
	// ==========================================
	// IMGUI LEVEL EDITOR WINDOW
	// ==========================================\

	if (Enable_imGUI) 
	{
		ImGui_ImplDX10_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGuiIO& io = ImGui::GetIO();

		// Tell ImGui the ratio between the actual backbuffer (320x240) and the stretched window.
		// Win32 already sets io.DisplaySize to the stretched window size, so the mouse works natively.
		if (io.DisplaySize.x > 0 && io.DisplaySize.y > 0) {
			io.DisplayFramebufferScale = ImVec2(
				(float)SCREEN_WIDTH / io.DisplaySize.x,
				(float)SCREEN_HEIGHT / io.DisplaySize.y
			);
		}


		ImGui::NewFrame();
		ImGui::SetNextWindowPos(ImVec2(10.0f, 10.0f), ImGuiCond_FirstUseEver);


		ImGui::Begin("Level Editor");

	// ==========================================


		CScene* currentScene = g->GetCurrentScene();
		string Object_name = "unknown";

		// only run this UI if we are actually in a PlayScene
		CPlayScene* playScene = dynamic_cast<CPlayScene*>(currentScene);
		if (playScene != NULL)
		{
			// --- ADD THE SAVE BUTTON HERE ---
			if (ImGui::Button("SAVE SCENE TO FILE", ImVec2(-1, 50))) // -1 makes the button stretch the full width
			{
				//	playScene->SaveScene("LevelEditor_Save.txt");
				playScene->SaveScene("D:/schooling/hkiv/game dev/projects/06-MyProject/05-SceneManager/Levels/LevelEditor_Save.txt");
			}
			ImGui::Separator();



			vector<LPGAMEOBJECT>& objects = playScene->GetObjects();
			// Grab ImGui IO to read the physical stretched window size
			ImGuiIO& io = ImGui::GetIO();
			ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.0f, 1.0f), "--- ImGui Internals ---");
			ImGui::Text("io.DisplaySize (Interactable Area): %.0f x %.0f", io.DisplaySize.x, io.DisplaySize.y);
			ImGui::Text("io.DisplayFramebufferScale: %.2f x %.2f", io.DisplayFramebufferScale.x, io.DisplayFramebufferScale.y);
			ImGui::Text("Mouse Position: (%.1f, %.1f)", io.MousePos.x, io.MousePos.y);
			// Get the exact X and Y coordinates of this ImGui window
			ImVec2 winPos = ImGui::GetWindowPos();

			// Print it directly into the UI
			ImGui::Text("Window Position: X: %.1f, Y: %.1f", winPos.x, winPos.y);


			HWND hWnd = CGame::GetInstance()->GetWindow(); // Assuming your CGame class has a way to get the window handle
			if (hWnd) {
				RECT winRect, clientRect;
				GetWindowRect(hWnd, &winRect);
				GetClientRect(hWnd, &clientRect);

				ImGui::Separator();
				ImGui::TextColored(ImVec4(0.0f, 1.0f, 1.0f, 1.0f), "--- Win32 Actuals ---");
				ImGui::Text("Outer Window Size: %ld x %ld", winRect.right - winRect.left, winRect.bottom - winRect.top);
				ImGui::Text("Inner Client Canvas: %ld x %ld", clientRect.right, clientRect.bottom);
			}
			ImGui::Separator();
				ImGui::Text("Level: %d", g->GetCurrentSceneID());

			ImGui::Separator();

			for (size_t i = 0; i < objects.size(); i++) {
				if (objects[i] == NULL) continue;

				ImGui::PushID(i);

				float x, y;
				objects[i]->GetPosition(x, y);
				//Object_name = "hellow";
				Object_name = objects[i]->Get_obj_Name();

				// TREENODE: Collapses the list so it's clean and easy to read
			//	DebugOut(L"Name: %s\n", Object_name.c_str());
				if (ImGui::TreeNode((void*)(intptr_t)i, "Object [%d]: %s", i, Object_name.c_str()))
				{
					// --- THE UNIVERSAL HIGHLIGHT BOX ---

					// 1. Get Camera Position
					float camX, camY;
					g->GetCamPos(camX, camY);

					// 2. Get the ACTUAL Bounding Box (World Coordinates)
					// This perfectly handles RowOfBrick's offsets, insets, and lengths!
					float l, t, r, b;
					objects[i]->GetBoundingBox(l, t, r, b);

					float objWidth = r - l;
					// Since your Y-axis goes down, bottom (b) is greater than top (t).
					// We use abs() just to be perfectly safe against inverted boxes.
					float objHeight = abs(b - t);

					if (objWidth <= 0) objWidth = 16.0f;
					if (objHeight <= 0) objHeight = 16.0f;

					// 3. TRANSLATE: World Space -> Game Screen Space
					// Because 'l' and 't' are ALREADY the top-left of the box in world space,
					// we completely ignore objects[i]->GetPosition() and use l/t directly!
					float gameScreenLeft = std::floor(l - camX);
					float gameScreenTop = std::floor(t - camY);

					// 4. SCALE: Game Screen Space -> ImGui Window Space
					ImGuiIO& io = ImGui::GetIO();
					float scaleX = io.DisplaySize.x / (float)SCREEN_WIDTH;
					float scaleY = io.DisplaySize.y / (float)SCREEN_HEIGHT;

					float boxLeft = gameScreenLeft * scaleX;
					float boxTop = gameScreenTop * scaleY;

					float scaledWidth = objWidth * scaleX;
					float scaledHeight = objHeight * scaleY;

					// 5. Draw the Box!
					ImGui::GetForegroundDrawList()->AddRect(
						ImVec2(boxLeft, boxTop),
						ImVec2(boxLeft + scaledWidth, boxTop + scaledHeight),
						IM_COL32(255, 255, 0, 255),
						0.0f, 0, 2.0f
					);
					// ---------------------------------

					bool posChanged = false;
					/*if (ImGui::DragFloat("X", &x, 1.0f)) posChanged = true;
					if (ImGui::DragFloat("Y", &y, 1.0f)) posChanged = true;
					if (ImGui::InputFloat("X", &x, 1.0f)) posChanged = true;
					if (ImGui::InputFloat("Y", &y, 1.0f)) posChanged = true;*/

					// Make the boxes smaller so they fit next to each other on one line
					ImGui::PushItemWidth(100.0f);

					// --- X COORDINATE ---
					// 1. The Drag Tool. We use "##" so ImGui hides the label text but keeps the ID unique.
					if (ImGui::DragFloat("##dragX", &x, 1.0f)) posChanged = true;

					ImGui::SameLine(); // Tells ImGui: "Do not go to the next line!"

					// 2. The Input Tool. This one actually prints the "X" label.
					if (ImGui::InputFloat("X", &x, 1.0f)) posChanged = true;


					// --- Y COORDINATE ---
					if (ImGui::DragFloat("##dragY", &y, 1.0f)) posChanged = true;

					ImGui::SameLine();

					if (ImGui::InputFloat("Y", &y, 1.0f)) posChanged = true;

					ImGui::PopItemWidth(); // Reset the width so you don't mess up the rest of your UI

					if (posChanged) {
						x = std::round(x);
						y = std::round(y);
						objects[i]->SetPosition(x, y);

						CBrick* brick = dynamic_cast<CBrick*>(objects[i]);
						if (brick && (brick->type == 1 || brick->type == 2)) {
							brick->SavedPositionX = x;
							brick->SavedPositionY = y;
						}
					}
					ImGui::TreePop(); // Close the TreeNode

				}

				ImGui::PopID();
			}
		}

		ImGui::End();

		// Render ImGui to the D3D10 device
		ImGui::Render();
		ImGui_ImplDX10_RenderDrawData(ImGui::GetDrawData());
}
	// ==========================================



	pSwapChain->Present(0, 0);
}

HWND CreateGameWindow(HINSTANCE hInstance, int nCmdShow, int ScreenWidth, int ScreenHeight)
{
	WNDCLASSEX wc;
	wc.cbSize = sizeof(WNDCLASSEX);

	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.hInstance = hInstance;

	wc.lpfnWndProc = (WNDPROC)WinProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hIcon = (HICON)LoadImage(hInstance, WINDOW_ICON_PATH, IMAGE_ICON, 0, 0, LR_LOADFROMFILE);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = WINDOW_CLASS_NAME;
	wc.hIconSm = NULL;

	RegisterClassEx(&wc);

	HWND hWnd =
		CreateWindow(
			WINDOW_CLASS_NAME,
			MAIN_WINDOW_TITLE,
			WS_OVERLAPPEDWINDOW, // WS_EX_TOPMOST | WS_VISIBLE | WS_POPUP,
			CW_USEDEFAULT,
			CW_USEDEFAULT,
			ScreenWidth,
			ScreenHeight,
			NULL,
			NULL,
			hInstance,
			NULL);

	if (!hWnd) 
	{
		OutputDebugString(L"[ERROR] CreateWindow failed");
		DWORD ErrCode = GetLastError();
		return FALSE;
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	return hWnd;
}

int Run()
{
	MSG msg;
	int done = 0;
	ULONGLONG frameStart = GetTickCount64();
	DWORD tickPerFrame = 1000 / MAX_FRAME_RATE;

	while (!done)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT) done = 1;

			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		ULONGLONG now = GetTickCount64();

		// dt: the time between (beginning of last frame) and now
		// this frame: the frame we are about to render
		DWORD dt = (DWORD)(now - frameStart);

		if (dt >= tickPerFrame)
		{
			frameStart = now;

			CGame::GetInstance()->ProcessKeyboard();			
			Update(dt);
			Render();

			CGame::GetInstance()->SwitchScene();
		}
		else
			Sleep(tickPerFrame - dt);	
	}

	return 1;
}

int WINAPI WinMain(
	_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPSTR lpCmdLine,
	_In_ int nCmdShow
) {
	HWND hWnd = CreateGameWindow(hInstance, nCmdShow, SCREEN_WIDTH, SCREEN_HEIGHT);

	SetDebugWindow(hWnd);

	LPGAME game = CGame::GetInstance();
	game->Init(hWnd, hInstance);
	game->InitKeyboard();


	// --- ADD THIS BLOCK ---
	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui::StyleColorsDark();

	// --- ADD THIS TO MAKE THE UI SMALLER ---
	ImGui::GetIO().FontGlobalScale = 2.5f; // Shrinks text to 80%
	ImGui::GetStyle().ScaleAllSizes(4.0f); // Shrinks sliders and padding to 80%
	// ---------------------------------------

	// Setup Platform/Renderer backends
	ImGui_ImplWin32_Init(hWnd);
	ImGui_ImplDX10_Init(game->GetDirect3DDevice());
	// --- END INSERTS ---


	//IMPORTANT: this is the only place where a hardcoded file name is allowed ! 
	
	//game->Load(L"mario-sample.txt");  
	game->Load(L"Config-sample.txt");



	SetWindowPos(hWnd, 0, 0, 0, SCREEN_WIDTH*2, SCREEN_HEIGHT*2, SWP_NOMOVE | SWP_NOOWNERZORDER | SWP_NOZORDER);

	// --- NEW WINDOW SCALING LOGIC ---
	// Calculate the exact window size needed for a perfect 2x client area
	//RECT windowRect = { 0, 0, SCREEN_WIDTH * 2, SCREEN_HEIGHT * 2 };
	//AdjustWindowRect(&windowRect, WS_OVERLAPPEDWINDOW, FALSE);

	//int exactWindowWidth = windowRect.right - windowRect.left;
	//int exactWindowHeight = windowRect.bottom - windowRect.top;

	//SetWindowPos(hWnd, 0, 0, 0, exactWindowWidth, exactWindowHeight, SWP_NOMOVE | SWP_NOOWNERZORDER | SWP_NOZORDER);
	// --------------------------------

	Run();


	return 0;
}