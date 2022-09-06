cmake_minimum_required(VERSION 3.22)

set(PROJECT er-online-summons)
project(${PROJECT} C CXX ASM)

set(CMAKE_CXX_STANDARD 23)

set(CMAKE_CXX_FLAGS "-O3 -s")
set(CMAKE_CXX_FLAGS_DEBUG "-DDEBUG=2")

set(SOURCES
    entry.cpp entry.h
    src/core/JumpHook.cpp src/core/JumpHook.h
    src/core/Logging.cpp src/core/Logging.h
    src/core/Memory.cpp src/core/Memory.h
    src/core/SteamInterfaceProvider.cpp src/core/SteamInterfaceProvider.h
    src/game/hooks/BuddySummonedHook.cpp src/game/hooks/BuddySummonedHook.h
    src/networking/SummonNetworking.cpp src/networking/SummonNetworking.h
    src/networking/SummonNetworkingMessages.h
    src/game/Offsets.h
    src/game/SessionManager.cpp src/game/SessionManager.h
    src/game/SummonBuddyManager.cpp src/game/SummonBuddyManager.h
    src/statemachine/SummonBuddyStateMachine.cpp src/statemachine/SummonBuddyStateMachine.h
    src/statemachine/states/SummonSpawnedSummonBuddyState.cpp src/statemachine/states/SummonSpawnedSummonBuddyState.h
    src/statemachine/states/NoSummonSummonBuddyState.cpp src/statemachine/states/NoSummonSummonBuddyState.h
    src/statemachine/SummonBuddyStateFactory.cpp src/statemachine/SummonBuddyStateFactory.h
    src/statemachine/states/SummonSpawnRequestedSummonBuddyState.cpp src/statemachine/states/SummonSpawnRequestedSummonBuddyState.h
    src/OnlineSummons.cpp src/OnlineSummons.h
    src/game/hooks/BuddyGoodsStateHook.cpp src/game/hooks/BuddyGoodsStateHook.h
    src/statemachine/states/SummonDespawnRequestedSummonBuddyState.cpp src/statemachine/states/SummonDespawnRequestedSummonBuddyState.h
)

include_directories(src include)

add_library(${PROJECT} SHARED ${SOURCES})
target_link_libraries(${PROJECT}
    ${CMAKE_SOURCE_DIR}/include/minhook/MinHook.x64.dll
    ${CMAKE_SOURCE_DIR}/include/steam/steam_api64.dll
)
target_link_options(${PROJECT} PRIVATE -static -Wno-multichar -Wl,--image-base,0x190000)

set_target_properties(${PROJECT} PROPERTIES PREFIX "")