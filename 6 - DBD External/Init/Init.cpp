#include "../include.h"

using namespace global;

bool Init::DriverInit() {
	//Replace with your own Driver Init
	//...

	pid = // findprocess(L"DeadByDaylight-Win64-Shipping.exe");
	if (pid == 0) {
		return false;
	}
	printf("[+] Pid of Dbd: %ld\n", pid);

	base_address = //FindProcessBase;
	if (base_address == 0)
		return false;

	printf("[+] BaseAddress 0x%llx\n", base_address);

	return true;
}

bool Init::InitClass() {

	// 1) Uworld
	uintptr_t worldPtr = rpm<uintptr_t>(base_address + Offsets::GWorld);

	if (worldPtr == 0) {
		return false;
	}

	// 1) struct UWorld
	*gclass::UWorld = rpm<SDK::UWorld>(worldPtr);
	//driver::ReadPhysical(driver_handle, process_id, (PVOID)(worldPtr), gclass::UWorld, sizeof(SDK::UWorld));


	// 2) ULevel
	uintptr_t levelPtr = (uintptr_t)gclass::UWorld->PersistentLevel;
	if (levelPtr == 0) return false;
	*gclass::ULevel = rpm<SDK::ULevel>(levelPtr);
	//driver::ReadPhysical(driver_handle, process_id, (PVOID)(levelPtr), gclass::ULevel, sizeof(SDK::ULevel));

	// 3) Game Instance
	uintptr_t owningGameInstancePtr = (uintptr_t)gclass::UWorld->OwningGameInstance;
	if (owningGameInstancePtr == 0) return false;
	*gclass::UGameInstance = rpm<SDK::UGameInstance>(owningGameInstancePtr);
	//driver::ReadPhysical(driver_handle, process_id, (PVOID)(owningGameInstancePtr), gclass::UGameInstance, sizeof(SDK::UGameInstance));

	uintptr_t localPlayersArray = (uintptr_t)gclass::UGameInstance->LocalPlayers.Data;
	if (localPlayersArray == 0) return false;

	uintptr_t localPlayerPtr = rpm<uintptr_t>(localPlayersArray);
	//driver::ReadPhysical(driver_handle, process_id, (PVOID)(localPlayersArray), &localPlayerPtr, sizeof(uintptr_t));
	if (localPlayerPtr == 0) return false;

	// 4) ULocalPlayer
	*gclass::ULocalPlayer = rpm<SDK::ULocalPlayer>(localPlayerPtr);
	//driver::ReadPhysical(driver_handle, process_id, (PVOID)(localPlayerPtr), gclass::ULocalPlayer, sizeof(SDK::ULocalPlayer));

	// 5) APlayerController
	uintptr_t PlayerControllerPtr = (uintptr_t)gclass::ULocalPlayer->PlayerController;
	if (PlayerControllerPtr == 0) return false;
	*gclass::APlayerController = rpm<SDK::APlayerController>(PlayerControllerPtr);
	//driver::ReadPhysical(driver_handle, process_id, (PVOID)(PlayerControllerPtr), gclass::APlayerController, sizeof(SDK::APlayerController));

	return true;
}