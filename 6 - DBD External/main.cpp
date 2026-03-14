#include "include.h"

using namespace global;


void Hack() {
	// Draw here
	esp->Draw();
}

int main() {
	HWND hwnd = nullptr;
	bool status = false;
	std::thread* updateThread = nullptr;

	printf("[+] Press F6 to init driver\n");
	while (!status) {
		if (GetAsyncKeyState(VK_F6)) {
			status = init->DriverInit();
			if (!status) {
				printf("[-] Driver Init Failed\n");
				goto Cleanup;
			}
		}
	}

	printf("[+] Driver Initialized\n");

	game_hwnd = FindWindowW(L"UnrealWindow",0);
	if (!game_hwnd) {
		printf("[+] Dbd window not found %p\n", game_hwnd);
	}

	//Hooking
	overlay->SetupHook(&hwnd);

	//DX11 + ImGui
	if (!render->InitDevice(hwnd)) {
		printf("[-] DX11 Init Failed\n");
		goto Cleanup;
	}
	render->InitImGui(hwnd);
	printf("[+] Render initialized\n");

	//Init Class to get pointer of each class
	if (!init->InitClass()) { printf("[+] InitClass Failed\n"); goto Cleanup; }

	//Create update thread after all init is done
	updateThread = new std::thread(&Cheat::RunUpdateLoop, cheat);

	//Main loop
	printf("[+] Press F7 to quit if wanted\n");
	while (!GetAsyncKeyState(VK_F7)) {
		overlay->UpdatePosition(&hwnd, game_hwnd);

		render->BeginFrame();
		Hack();
		//Render Menu
		render->DrawMenu(hwnd);
		

		render->EndFrame();
		_mm_pause();
	}

	goto Cleanup;

Cleanup:
	cheat->Stop();
	if (updateThread) {
		updateThread->join();
		delete updateThread;
	}
	if(hwnd)
		overlay->Unhook(hwnd);
	render->CleanupImGui();
	render->CleanupDevice();
	delete overlay;
	delete render;
	return 0;
}
