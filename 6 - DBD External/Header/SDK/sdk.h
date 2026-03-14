#pragma once

namespace SDK
{

    // ============================================================
    // Utils
    // ============================================================

    inline D3DMATRIX MatrixMultiplication(D3DMATRIX pM1, D3DMATRIX pM2)
    {
        D3DMATRIX pOut;
        pOut._11 = pM1._11 * pM2._11 + pM1._12 * pM2._21 + pM1._13 * pM2._31 + pM1._14 * pM2._41;
        pOut._12 = pM1._11 * pM2._12 + pM1._12 * pM2._22 + pM1._13 * pM2._32 + pM1._14 * pM2._42;
        pOut._13 = pM1._11 * pM2._13 + pM1._12 * pM2._23 + pM1._13 * pM2._33 + pM1._14 * pM2._43;
        pOut._14 = pM1._11 * pM2._14 + pM1._12 * pM2._24 + pM1._13 * pM2._34 + pM1._14 * pM2._44;
        pOut._21 = pM1._21 * pM2._11 + pM1._22 * pM2._21 + pM1._23 * pM2._31 + pM1._24 * pM2._41;
        pOut._22 = pM1._21 * pM2._12 + pM1._22 * pM2._22 + pM1._23 * pM2._32 + pM1._24 * pM2._42;
        pOut._23 = pM1._21 * pM2._13 + pM1._22 * pM2._23 + pM1._23 * pM2._33 + pM1._24 * pM2._43;
        pOut._24 = pM1._21 * pM2._14 + pM1._22 * pM2._24 + pM1._23 * pM2._34 + pM1._24 * pM2._44;
        pOut._31 = pM1._31 * pM2._11 + pM1._32 * pM2._21 + pM1._33 * pM2._31 + pM1._34 * pM2._41;
        pOut._32 = pM1._31 * pM2._12 + pM1._32 * pM2._22 + pM1._33 * pM2._32 + pM1._34 * pM2._42;
        pOut._33 = pM1._31 * pM2._13 + pM1._32 * pM2._23 + pM1._33 * pM2._33 + pM1._34 * pM2._43;
        pOut._34 = pM1._31 * pM2._14 + pM1._32 * pM2._24 + pM1._33 * pM2._34 + pM1._34 * pM2._44;
        pOut._41 = pM1._41 * pM2._11 + pM1._42 * pM2._21 + pM1._43 * pM2._31 + pM1._44 * pM2._41;
        pOut._42 = pM1._41 * pM2._12 + pM1._42 * pM2._22 + pM1._43 * pM2._32 + pM1._44 * pM2._42;
        pOut._43 = pM1._41 * pM2._13 + pM1._42 * pM2._23 + pM1._43 * pM2._33 + pM1._44 * pM2._43;
        pOut._44 = pM1._41 * pM2._14 + pM1._42 * pM2._24 + pM1._43 * pM2._34 + pM1._44 * pM2._44;

        return pOut;
    }


    // ============================================================
    // Structs de base
    // ============================================================

    struct FVector
    {
        double X; // 0x0000
        double Y; // 0x0008
        double Z; // 0x0010
        FVector operator-(FVector ape)
        {
            return { X - ape.X, Y - ape.Y, Z - ape.Z };
        }
        double Dot(FVector& v)
        {
            return X * v.X + Y * v.Y + Z * v.Z;
        }
    };

    struct FRotator
    {
        double Pitch; // 0x0000
        double Yaw;   // 0x0008
        double Roll;  // 0x0010
        FRotator operator-(FRotator ape)
        {
            return { Pitch - ape.Pitch, Yaw - ape.Yaw, Roll - ape.Roll };
        }
        double Dot(FRotator& v)
        {
            return Pitch * v.Pitch + Yaw * v.Yaw + Roll * v.Roll;
        }
    };

    template<typename T>
    struct TArray
    {
        T* Data;       // 0x0000
        int32_t Count; // 0x0008
        int32_t Max;   // 0x000C
    };

    struct FMinimalViewInfo
    {
        FVector  Location; // 0x0000
        FRotator Rotation; // 0x0018
        float    FOV;      // 0x0030
        uint8_t  Pad_34[0x7DC];
    };

    struct FCameraCacheEntry
    {
        float            TimeStamp; // 0x0000
        uint8_t          Pad_4[0xC];
        FMinimalViewInfo POV;       // 0x0010
    };

    struct FQuat
    {
        double x;
        double y;
        double z;
        double w;
    };

    struct FTransform
    {
        FQuat  rot;
        FVector translation;
        float _pad1;
        FVector scale;
        float _pad2;

        D3DMATRIX ToMatrixWithScale() const {
            D3DMATRIX m;
            m._41 = translation.X;
            m._42 = translation.Y;
            m._43 = translation.Z;

            double x2 = rot.x + rot.x;
            double y2 = rot.y + rot.y;
            double z2 = rot.z + rot.z;

            double xx2 = rot.x * x2;
            double yy2 = rot.y * y2;
            double zz2 = rot.z * z2;
            m._11 = (1.0f - (yy2 + zz2)) * scale.X;
            m._22 = (1.0f - (xx2 + zz2)) * scale.Y;
            m._33 = (1.0f - (xx2 + yy2)) * scale.Z;

            double yz2 = rot.y * z2;
            double wx2 = rot.w * x2;
            m._32 = (yz2 - wx2) * scale.Z;
            m._23 = (yz2 + wx2) * scale.Y;

            double xy2 = rot.x * y2;
            double wz2 = rot.w * z2;
            m._21 = (xy2 - wz2) * scale.Y;
            m._12 = (xy2 + wz2) * scale.X;

            double xz2 = rot.x * z2;
            double wy2 = rot.w * y2;
            m._31 = (xz2 + wy2) * scale.Z;
            m._13 = (xz2 - wy2) * scale.X;

            m._14 = 0.0f;
            m._24 = 0.0f;
            m._34 = 0.0f;
            m._44 = 1.0f;

            return m;
        }
    };

    // ============================================================
    // Classes UE
    // ============================================================

    class UObject
    {
    public:
        uint8_t Pad_0[0x30];
    };

    class UActorComponent : public UObject
    {
    public:
        uint8_t Pad_30[0x88]; // → 0x00B8
    };

    class USceneComponent : public UActorComponent
    {
    public:
        uint8_t  Pad_B8[0x90];
        FVector  RelativeLocation;  // 0x0148
        FRotator RelativeRotation;  // 0x0160
        FVector  RelativeScale3D;   // 0x0178
        FVector  ComponentVelocity; // 0x0190
    };

    class ULevel;
    class AActor;
    class ULocalPlayer;
    class UGameInstance;
    class APlayerController;
    class USkeletalMesh;

    // ============================================================
    // UWorld
    // ============================================================

    class UWorld : public UObject
    {
    public:
        uint8_t         Pad_30[0x8];
        ULevel* PersistentLevel;    // 0x0038
        uint8_t         Pad_40[0x148];      // 0x0040 → 0x0188
        TArray<ULevel*> Levels;             // 0x0188
        uint8_t         Pad_198[0x50];      // 0x0198 → 0x01E8
        UGameInstance* OwningGameInstance; // 0x01E8
    };

    // ============================================================
    // UGameInstance
    // ============================================================

    class UGameInstance : public UObject
    {
    public:
        uint8_t              Pad_30[0x10];     // 0x0030 → 0x0040
        TArray<ULocalPlayer*> LocalPlayers;    // 0x0040
    };

    // ============================================================
    // UPlayer
    // ============================================================

    class UPlayer : public UObject
    {
    public:
        uint8_t           Pad_30[0x8];
        APlayerController* PlayerController;  // 0x0038
        uint8_t           Pad_40[0x10];       // → 0x0050
    };

    // ============================================================
    // ULocalPlayer
    // ============================================================

    class ULocalPlayer : public UPlayer
    {
    public:
        uint8_t Pad_50[0x270]; // → 0x02C0
    };

    // ============================================================
    // ULevel
    // ============================================================

    class ULevel : public UObject
    {
    public:
        uint8_t         Pad_30[0x78];
        TArray<AActor*> Actors;          // 0x00A8
        uint8_t         Pad_B8[0x10];
        UWorld* OwningWorld;     // 0x00C8
    };

    // ============================================================
    // AActor
    // ============================================================

    class AActor : public UObject
    {
    public:
        uint8_t          Pad_30[0x180];
        USceneComponent* RootComponent;   // 0x01B0
        uint8_t          Pad_1B8[0xF8];   // 0x01B8 → 0x02B0  ← AJOUTÉ
    };

    // ============================================================
    // APlayerCameraManager : public AActor
    // ============================================================

    class APlayerCameraManager : public AActor
    {
    public:
        uint8_t          Pad_2B0[0x8];
        uint8_t          Pad_2B8[0x8];
        uint8_t          Pad_2C0[0x90];
        uint8_t          Pad_350[0x10];
        FMinimalViewInfo CameraPOV;       // 0x0360
    };


    // ============================================================
    // APlayerState : public AActor
    // ============================================================

    class APlayerState : public AActor
    {
    public:
        float Score;
        int32_t Playerid; // 0x02B4
    };

    // ============================================================
    // UMorohTarget : public UObject
    // ============================================================

    class UMorphTarget : public UObject
    {
        USkeletalMesh* BaseSkelMesh;
        uint8_t Pad38[0x10];
    };


    // ============================================================
    // USkeletalMesh
    // ============================================================
    class USkeletalMesh
    {
    public:
        uint8_t Pad_2B8[0x1E0]; // D8->2B8
        TArray<UMorphTarget*> MorphTargets;
    };


    // ============================================================
    // USkinnedMeshComponent
    // ============================================================
    class USkinnedMeshComponent
    {
    public:
        uint8_t Pad_0[0x05C8];
        USkeletalMesh* SkeletalMesh;                          // 0x05C8
        uint8_t Pad_5D0[0x40];                                // 0x05D0 -> 0x0610
        uint8_t MeshDeformerInstances[0x20];                   // 0x0610 (FMeshDeformerInstanceSet, size 0x20)
        uint8_t Pad_630[0x10];                                 // 0x0630 -> 0x0640
        TArray<struct FTransform> ComponentSpaceTransforms;     // 0x0640 (Bone Array)
    };

    // ============================================================
    // USkeletalMeshComponent : public USkinnedMeshComponent
    // ============================================================
    class USkeletalMeshComponent : public USkinnedMeshComponent
    {
    public:
    };

    // ============================================================
    // APawn : public AActor
    // ============================================================

    class APawn : public AActor
    {
    public:
        uint8_t Pad_2D0[0x20];
        APlayerState* PlayerState; // 0x02D0
        uint8_t Pad_338[0x60]; // 0x330
    };

    // ============================================================
    // ACharacter : public APawn
    // ============================================================

    class ACharacter : public APawn
    {
    public:
        USkeletalMeshComponent* Mesh; // 0x338
    };

    // ============================================================
    // AController : public AActor
    // ============================================================

    class AController : public AActor
    {
    public:
        uint8_t Pad_2B0[0x8];
        APlayerState* PlayerState;
        uint8_t Pad_2F8[0x38];
        APawn* Pawn;
        uint8_t Pad_300[0x8];                     // 0x0300
        ACharacter* Character;                    // 0x0308
        USceneComponent* TransformComponent;      // 0x0310
        uint8_t Pad_318[0x38];                    // 0x0318 → 0x0350
    };


    // ============================================================
    // APlayerController : public AController
    // ============================================================

    class APlayerController : public AController
    {
    public:
        uint8_t               Pad_350[0x8]; // 0x02B0 → 0x0358  ← FIX (fusionné)
        UPlayer* Player;         // 0x0358
        APawn* AcknowledgedPawn; // 0x0360
        void* MyHUD;          // 0x0368
        APlayerCameraManager* PlayerCameraManager; // 0x0370
    };

    
} // namespace SDK