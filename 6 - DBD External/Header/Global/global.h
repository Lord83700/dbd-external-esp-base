#pragma once

namespace global {
	inline HWND game_hwnd;
	inline DWORD64 pid = 0;
	inline ULONG64 base_address = 0;
	inline Overlay* overlay = new Overlay();
	inline Render* render = new Render();
	inline Init* init = new Init();

	namespace gclass {
		inline SDK::UObject* UObject = new SDK::UObject();
		inline SDK::UWorld* UWorld = new SDK::UWorld();
		inline SDK::ULevel* ULevel = new SDK::ULevel();
		inline SDK::AActor* AActor = new SDK::AActor();
		inline SDK::USceneComponent* USceneComponent = new SDK::USceneComponent();
		inline SDK::UGameInstance* UGameInstance = new SDK::UGameInstance();
		inline SDK::ULocalPlayer* ULocalPlayer = new SDK::ULocalPlayer();
		inline SDK::APlayerController* APlayerController = new SDK::APlayerController();
		inline SDK::APawn* APawn = new SDK::APawn();
		inline SDK::APlayerState* APlayerState = new SDK::APlayerState();
	};
}