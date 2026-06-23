# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Build

Open `DOBAK.vcxproj` in **Visual Studio 2022**. Build with Ctrl+Shift+B or via the Build menu. Configurations available: Debug/Release × Win32/x64. There is no Makefile or CMake — MSVC is the only supported toolchain.

The project depends on the **FMOD** audio library; its headers are included in the repo. The DLL (`fmod.dll`) must be present alongside the executable at runtime.

## Project Overview

DOBAK is a Korean-language Windows console slot machine game. The player spins a 5×3 number grid, earns gold, buys items from a shop, and must meet a daily quota to progress to the next day.

## Architecture

### Game Loop (`main.cpp`, `Game.h/cpp`)

```
Init → [Update → Render → 60 FPS sync] loop → Release
```

`GameState` (defined in `GameState.h`) is the single shared mutable state struct passed through the entire loop. It holds player data, current day, quota status, shop inventory, and transition flags.

### Scene System

All scenes inherit from `AbstractScene` (`Scene.h`) with four lifecycle methods: `Init`, `Update`, `Render`, `Release`. `SceneManager` (singleton) owns scene registration and transitions.

Active scenes:
- `TitleScene` — main menu
- `ShopScene` — buy/sell items with cursor UI and NPC animations
- `SlotMachineScene` — core gameplay; 5-spin daily limit, pattern detection
- `QuotaScene` — submit gold to meet daily quota
- `NextDayScene` — day-end summary/transition
- `InfoScene` — item/equipment info

Scene transitions happen via `GameState` flags (e.g., `requestNextDay`) checked in `Game::Update`.

### Item & Command Pattern

Items (`Item.h`, `ShopItems.h/cpp`) carry an `ICommand*` effect pointer. All item effects implement `ICommand::Execute(ItemEffectContext&)` (`ICommand.h`, `Commands.h`). Concrete commands include `AddGoldCommand`, `ProbabilityCommand`, `MultiplierGoldCommand`, `SpinSpeedCommand`, `ComboBonusCommand`, `PatternBonusCommand`, and `CompositeCommand` (chains multiple commands). Equipment items apply their commands during spin resolution; consumable items apply once on use.

### Slot Machine Logic (`SlotMachineScene.h/cpp`)

State machine: `Idle → Rolling → Blinking → SixSeven` (special jackpot state for the 6-7 pattern). Spin duration starts at 1500 ms and is modified by equipped items. `Pattern.h/cpp` defines all scoring patterns — each pattern has a width, height, and gold reward; the special "67" pattern pays 6767 gold.

### Console/UI (`Console.h/cpp`, `UIAsciiObjs.h/cpp`, `AsciiArt.h/cpp`)

A thin wrapper around the Windows Console API. Key utilities: `GotoXY()` for cursor positioning, character-by-character dialogue rendering with delays, shake/transition animations, and color/font control. ASCII art objects are static and rendered with animation offsets.

### Sound (`SoundManager.h/cpp`)

Singleton wrapping FMOD. Manages BGM and SFX with independent mute/volume controls. Audio files live in `Sound/` as `a.mp3` through `j.mp3` plus named effect files.

## Key Conventions

- **Memory**: Manual `new`/`delete`; use the `SAFE_DELETE` macro (defined in `Defines.h`) for pointer cleanup.
- **Input**: Windows VK_* constants (`VK_SPACE`, `VK_UP`, `VK_DOWN`, `VK_RETURN`).
- **Timing**: `ULONGLONG` timestamps via `GetTickCount64()`.
- **Singletons**: `SceneManager` and `SoundManager` — access via `::GetInstance()`.
- **Language**: All in-game strings are Korean.
