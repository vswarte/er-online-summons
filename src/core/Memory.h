#pragma once

#include <memory>
#include <windows.h>

#define MODULE_NAME "eldenring.exe"

#define OFFSET_SESSION_MANAGER_IMP (uintptr_t) 0x3c44ac0
#define OFFSET_SESSION_MANAGER_IMP_PLAYER_CONNECTION (uintptr_t) 0x78
#define OFFSET_PLAYER_CONNECTION_STEAM_ID (uintptr_t) 0x10

#define OFFSET_WORLD_CHR_MANAGER_IMP (uintptr_t) 0x3c310b8
#define OFFSET_WORLD_CHR_MANAGER_IMP_SUMMON_BUDDY_MANAGER (uintptr_t) 0x18498
#define OFFSET_FN_USE_BUDDY_GOODS (uintptr_t) 0x4af5c0
#define OFFSET_TLS_INDEX_USE_BUDDY_GOODS (uintptr_t) 0x44847D0
#define OFFSET_TLS_VALUE_USE_BUDDY_GOODS (uintptr_t) 0x3C41EC8

namespace EROnlineSummons {
    uintptr_t GetBaseAddress();
}
