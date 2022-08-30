#pragma once

#include <windows.h>
#include "core/Memory.h"
#include "core/Logging.h"
#include "OnlineSummons.h"
#include "game/SummonBuddyManager.h"
#include "networking/SummonNetworking.h"

#define EXCEPTION_STRING_SIZE    1024

EROnlineSummons::OnlineSummons *onlineSummons = nullptr;

void attach();
void detach();

LONG WINAPI exception_handler(struct _EXCEPTION_POINTERS*);
