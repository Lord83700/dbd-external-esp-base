#pragma once

class Entity {
public:
	uintptr_t actorPtr;
	uintptr_t rootComponent;
	uintptr_t meshPtr;
	SDK::FVector position;
    uintptr_t playerStatePtr;
    int32_t objectId;
    std::string Name;
    SDK::FVector bones[SLOT_COUNT];
};

class LocalPlayer : public Entity {
public:
    SDK::FMinimalViewInfo FMinimalViewInfo;
};

class Cheat {
private:
    void ReadActor(int actorCount, uintptr_t actorsData, std::vector<Entity>& out);
    void RefreshPositions();
    void GetActorBone(uintptr_t mesh, Entity* entity);
    SDK::FVector GetBoneWithRotation(uintptr_t mesh, int id, SDK::FTransform ComponentToWorld);
    SDK::FTransform GetBoneIndex(uintptr_t refDataSkeleton, int index);

    std::atomic<bool> running = true;
    std::vector<Entity> cachedActors;
    int scanCounter = 0;
    inline static constexpr int FULL_SCAN_INTERVAL = 500;
public:
    std::mutex actorMutex;
    std::mutex localPlayerMutex;
    LocalPlayer localPlayer;
    std::vector<Entity> actors;
    std::string GetNameById(int key);
    void Update();
    void RunUpdateLoop();
    void Stop();
    LocalPlayer GetLocalPlayer();

};

inline Cheat* cheat = new Cheat();
