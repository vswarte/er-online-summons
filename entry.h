#pragma once

#include <windows.h>
#include "core/Logging.h"
#include "OnlineSummons.h"
#include "SummonNetworking.h"
#include "SummonBuddyManager.h"

EROnlineSummons::OnlineSummons *onlineSummons = nullptr;

void attach();
void detach();