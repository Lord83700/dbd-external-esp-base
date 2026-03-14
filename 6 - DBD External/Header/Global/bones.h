#pragma once

// Index into Entity::bones[] array — matches the order of BoneMapping entries
enum BoneSlot {
    SLOT_HEAD = 0,
    SLOT_NECK,
    SLOT_SPINE_03,
    SLOT_SPINE_02,
    SLOT_SPINE_01,
    SLOT_PELVIS,
    SLOT_CLAVICLE_L,
    SLOT_CLAVICLE_R,
    SLOT_UPPERARM_L,
    SLOT_LOWERARM_L,
    SLOT_HAND_L,
    SLOT_FINGER_L,
    SLOT_UPPERARM_R,
    SLOT_LOWERARM_R,
    SLOT_HAND_R,
    SLOT_FINGER_R,
    SLOT_THIGH_L,
    SLOT_CALF_L,
    SLOT_FOOT_L,
    SLOT_THIGH_R,
    SLOT_CALF_R,
    SLOT_FOOT_R,
    SLOT_COUNT // 22
};

inline BonePair skeletonPairs[] = {
    // Spine
    { SLOT_HEAD,       SLOT_NECK },
    { SLOT_NECK,       SLOT_SPINE_03 },
    { SLOT_SPINE_03,   SLOT_SPINE_02 },
    { SLOT_SPINE_02,   SLOT_SPINE_01 },
    { SLOT_SPINE_01,   SLOT_PELVIS },

    // Left arm
    { SLOT_SPINE_03,   SLOT_CLAVICLE_L },
    { SLOT_CLAVICLE_L, SLOT_UPPERARM_L },
    { SLOT_UPPERARM_L, SLOT_LOWERARM_L },
    { SLOT_LOWERARM_L, SLOT_HAND_L },
    { SLOT_HAND_L,     SLOT_FINGER_L },

    // Right arm
    { SLOT_SPINE_03,   SLOT_CLAVICLE_R },
    { SLOT_CLAVICLE_R, SLOT_UPPERARM_R },
    { SLOT_UPPERARM_R, SLOT_LOWERARM_R },
    { SLOT_LOWERARM_R, SLOT_HAND_R },
    { SLOT_HAND_R,     SLOT_FINGER_R },

    // Left leg
    { SLOT_PELVIS,     SLOT_THIGH_L },
    { SLOT_THIGH_L,    SLOT_CALF_L },
    { SLOT_CALF_L,     SLOT_FOOT_L },

    // Right leg
    { SLOT_PELVIS,     SLOT_THIGH_R },
    { SLOT_THIGH_R,    SLOT_CALF_R },
    { SLOT_CALF_R,     SLOT_FOOT_R },
};
