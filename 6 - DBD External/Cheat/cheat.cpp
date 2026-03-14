#include "../include.h"

using namespace global;
inline constexpr int NAME_SIZE = 1024;



std::string Cheat::GetNameById(int key)
{
    uintptr_t gNames = base_address + Offsets::GNames;
    //printf("GNames: %p\n", (void*)gNames);
    
    uint32_t chunkOffset = (key >> 16);
    uint16_t nameOffset = key & 0xFFFF;
    uintptr_t namePoolChunk = rpm<uint64_t>(gNames + 8 * (chunkOffset + 2));
    uintptr_t entryOffset = namePoolChunk + 4 * nameOffset;
    //printf("ChunkOffset: %d, NameOffset: %d, NamePoolChunk: %p, EntryOffset: %p\n", chunkOffset, nameOffset, (void*)namePoolChunk, (void*)entryOffset);

    uint16_t nameEntryHeader = rpm<uint16_t>(entryOffset + 4);
    uint16_t nameLength = nameEntryHeader >> 1;
    //printf("NameEntryHeader: %04x, NameLength: %d\n", nameEntryHeader, nameLength);
    bool isUnicode = nameEntryHeader & 1;

    if (nameLength == 0 || nameLength >= NAME_SIZE) {
        return "";
    }

    if (isUnicode) {
        wchar_t wbuffer[NAME_SIZE] = { 0 };
        driver::ReadPhysical(driver_handle , process_id,PVOID(entryOffset + 6), wbuffer, nameLength * sizeof(wchar_t));
        //printf("NAME : %s\n", wbuffer);
        std::wstring wstr(wbuffer, nameLength);
        return std::string(wstr.begin(), wstr.end()); 
    }
    else {
        char buffer[NAME_SIZE] = { 0 };
		driver::ReadPhysical(driver_handle, process_id, PVOID(entryOffset + 6), buffer, nameLength);
        //printf("NAME : %s\n", buffer);
        return std::string(buffer, nameLength);
    }
}

SDK::FTransform Cheat::GetBoneIndex(uintptr_t refDataSkeleton, int index) {

    SDK::FTransform bone = rpm<SDK::FTransform>((uintptr_t)refDataSkeleton + (index * 0x60)); //0x60 -> double

    return bone;
}

SDK::FVector Cheat::GetBoneWithRotation(uintptr_t mesh, int id, SDK::FTransform ComponentToWorld)
{
    SDK::FTransform bone = GetBoneIndex(mesh, id);

    D3DMATRIX Matrix = SDK::MatrixMultiplication(bone.ToMatrixWithScale(), ComponentToWorld.ToMatrixWithScale());

    //printf("[+] Matrix : %f %f %f\n", Matrix._41, Matrix._42, Matrix._43);

    return SDK::FVector(Matrix._41, Matrix._42, Matrix._43);
}

void Cheat::GetActorBone(uintptr_t mesh, Entity* entity)
{

    uintptr_t refDataSkeleton = rpm<uintptr_t>((uintptr_t)mesh + offsetof(SDK::USkinnedMeshComponent, ComponentSpaceTransforms)); //TArray.Data
    if (refDataSkeleton == 0) return;

    //printf("[+] refDataSkeleton %p\n", refDataSkeleton);

    int boneCount = rpm<int>((uintptr_t)mesh + offsetof(SDK::USkinnedMeshComponent, ComponentSpaceTransforms) + 0x8); //TArray.Count
    //printf("[+] BoneCount %d\n", boneCount);

    SDK::FTransform c2w = rpm<SDK::FTransform>(mesh + Offsets::ComponentToWorld);

    const int* boneIds = FindBoneMapping(entity->Name);

    for (int i = 0; i < SLOT_COUNT; i++) {
        if (boneIds[i] == -1) {
            entity->bones[i] = SDK::FVector(0, 0, 0);
            continue;
        }
        entity->bones[i] = GetBoneWithRotation(refDataSkeleton, boneIds[i], c2w);
    }
}



 void Cheat::ReadActor(int actorCount, uintptr_t actorsData, std::vector<Entity>& out) {
     out.reserve(actorCount);

     for (int i = 0; i < actorCount; i++) {
         uintptr_t currentActor = rpm<uintptr_t>(actorsData + i * sizeof(uintptr_t));
         //driver::ReadPhysical(driver_handle, process_id, (PVOID)(actorsData + i * sizeof(uintptr_t)), &currentActor, sizeof(uintptr_t));
         if (currentActor == 0) continue;

         //printf("[+] Offset Mesh %x\n", offsetof(SDK::ACharacter, Mesh));
         uintptr_t mesh = rpm<uintptr_t>((uintptr_t)currentActor + offsetof(SDK::ACharacter, Mesh));
         if (mesh == 0) continue;

         uintptr_t RootComponentData = rpm<uintptr_t>(currentActor + offsetof(SDK::AActor, RootComponent));
         //driver::ReadPhysical(driver_handle, process_id, (PVOID)(currentActor + offsetof(SDK::AActor, RootComponent)), &RootComponentData, sizeof(uintptr_t));
         if (RootComponentData == 0) continue;

         SDK::FVector pos = rpm<SDK::FVector>(RootComponentData + offsetof(SDK::USceneComponent, RelativeLocation));
         //driver::ReadPhysical(driver_handle, process_id, (PVOID)(RootComponentData + offsetof(SDK::USceneComponent, RelativeLocation)), &pos, sizeof(SDK::FVector));
         if (pos.X == 0 && pos.Y == 0 && pos.Z == 0) continue;

         int32_t objectId = rpm<int32_t>(currentActor + Offsets::ActorId);
         //driver::ReadPhysical(driver_handle, process_id, (PVOID)(currentActor + Offsets::ActorId), &objectId, sizeof(int32_t));

         uintptr_t playerState = rpm<uintptr_t>((uintptr_t)currentActor + offsetof(SDK::APawn, PlayerState));
         //Check if its my localPlayer
         if (playerState == localPlayer.playerStatePtr)
             continue;

         std::string name = GetNameById(objectId);
         if (name.empty()) continue;

         if (name.find("BP_CamperMale") != std::string::npos ||
             name.find("BP_CamperFemale") != std::string::npos ||
             name.find("BP_Slasher") != std::string::npos)
         {
             Entity entity;
             entity.actorPtr = currentActor;
             entity.rootComponent = RootComponentData;
             entity.position = pos;
             entity.objectId = objectId;
             entity.meshPtr = mesh;
             entity.playerStatePtr = playerState;

             if (name.find("BP_CamperMale") != std::string::npos)
                 entity.Name = "MaleSurvivor";
             else if (name.find("BP_CamperFemale") != std::string::npos)
                 entity.Name = "FemaleSurvivor";
             else if (name.find("BP_Slasher") != std::string::npos)
                 entity.Name = "Killer";

             GetActorBone(entity.meshPtr, &entity);

             out.push_back(entity);
         }
     }
 }

 void Cheat::RefreshPositions() {
     for (auto& entity : cachedActors) {
         uintptr_t RootComponentData = rpm<uintptr_t>(entity.actorPtr + offsetof(SDK::AActor, RootComponent));
         if (RootComponentData == 0) continue;

         uintptr_t mesh = rpm<uintptr_t>((uintptr_t)entity.actorPtr + offsetof(SDK::ACharacter, Mesh));
         if (mesh == 0) {
             continue;
         }

         GetActorBone(mesh, &entity);
         //Update actor bones

         entity.rootComponent = RootComponentData;
         entity.position = rpm<SDK::FVector>(RootComponentData + offsetof(SDK::USceneComponent, RelativeLocation));
         //driver::ReadPhysical(driver_handle, process_id, (PVOID)(RootComponentData + offsetof(SDK::USceneComponent, RelativeLocation)), &entity.position, sizeof(SDK::FVector));
     }
 }



void Cheat::RunUpdateLoop() {
    while (running) {
        Update();
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
}

void Cheat::Stop() {
    running = false;
}

LocalPlayer Cheat::GetLocalPlayer() {
    std::lock_guard<std::mutex> lock(localPlayerMutex);
    return localPlayer;
}

 void Cheat::Update() {
     uintptr_t localPlayerAPawnPtr = (uintptr_t)gclass::APlayerController->AcknowledgedPawn;
     uintptr_t PlayerCameraManagerPtr = (uintptr_t)gclass::APlayerController->PlayerCameraManager;
     {
         std::lock_guard<std::mutex> lock(localPlayerMutex);
         localPlayer.FMinimalViewInfo = rpm<SDK::FMinimalViewInfo>((uintptr_t)(PlayerCameraManagerPtr + offsetof(SDK::APlayerCameraManager, CameraPOV)));
         localPlayer.playerStatePtr = rpm<uintptr_t>((uintptr_t)(localPlayerAPawnPtr + offsetof(SDK::APawn, PlayerState)));
     }


     if (scanCounter >= FULL_SCAN_INTERVAL || cachedActors.empty()) {
         SDK::TArray<SDK::AActor*> Aactors = gclass::ULevel->Actors;
         int actorCount = Aactors.Count;
         uintptr_t actorsData = (uintptr_t)Aactors.Data;

         std::vector<Entity> fresh;
         ReadActor(actorCount, actorsData, fresh);
         cachedActors = fresh;
         scanCounter = 0;
     }
     else {
         RefreshPositions();
         scanCounter++;
     }

     {
         std::lock_guard<std::mutex> lock(actorMutex);
         actors = cachedActors;
     }
  }
