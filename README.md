# Dead by Daylight - External ESP

An external ESP for Dead by Daylight. This was made as a **learning project** to understand UE memory structures. Since this is only my second cheese, any feedback would be greatly valued.

## Features

- **Box ESP** — Corner-style boxes around survivors and killers
- **Skeleton ESP** — Full 22-bone skeleton rendering with proper bone connections
- **Actor names** — Displays actor names above players
- **Distance lines** — Lines drawn from screen center to each player
- **ImGui menu** — Togglable menu (INSERT key) with color pickers and per-feature toggles

## What You Need to Implement

This project **will not work out of the box**. Two critical components are left out intentionally:

### 1. Kernel Driver

The cheat relies on a kernel driver for physical memory reads. You must provide your own driver.
Populate `Header/Driver/driver.h` with your own driver interface.

### 2. Window Overlay / Hook

You need to implement your own method for overlaying a transparent window on top of the game. 

## About Bone / Skeleton ESP

The skeleton rendering **works correctly** — all 22 bones are read from `USkinnedMeshComponent::ComponentSpaceTransforms` and drawn with proper connections (head → neck → spine → pelvis, arms, legs, etc.).

However, the **character-to-bone-index mapping is incomplete**. The bone indices differ per character model in DBD, and while a large mapping table exists in `Header/Global/bone_mapping.h` covering 50+ survivors and 25+ killers, **the name-matching logic to identify which character you're looking at is not finished**. A fallback mapping is used for unrecognized characters. I didn't bother completing this because I don't play the game — I only used it as a template to learn Unreal Engine internals.

## Controls

| Key | Action |
|-----|--------|
| F6 | Initialize driver + start cheat |
| F7 | Exit |
| INSERT | Toggle menu |

## Build

Built with MSVC / Visual Studio. Requires DirectX 11 SDK and ImGui (included).

## Offsets

Offsets in `Header/Global/offsets.h` are hardcoded and **will break on game updates**. You'll need to update them yourself:

- `GWorld`: `0xBA3F5B0`
- `GNames`: `0xB7B30C0`
- `GObjects`: `0xB886740`

## Thanks

Thanks to **AyPz78, UNNAMEDZ03** for the help.
