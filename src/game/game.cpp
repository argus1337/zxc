#include "game.h"
#include "../math/math.h"
#include "../window/window.hpp"
#include <iostream>
#include "../esp/esp.h"
#include "../playerInfo/PedData.h"
#include <unordered_set>
#include "../DMALibrary/Memory/Memory.h"
#include "../globals.h"



namespace FiveM {
    namespace offset {
        uintptr_t world, replay, viewport, localplayer;
        uintptr_t boneList, boneMatrix = 0x60;
        uintptr_t playerInfo, playerHealth = 0x280, playerPosition = 0x90;
        uintptr_t base;
    }
    void Setup() {
        auto process_name = g_validExecutable;
        using namespace offset;

        auto game_base = mem.GetBaseDaddy(process_name);

        if (process_name == "FiveM_b2802_GTAProcess.exe") {
            world = mem.Read<uintptr_t>(game_base + 0x2593320);
            replay = mem.Read<uintptr_t>(game_base + 0x1F58B58);
            viewport = mem.Read<uintptr_t>(game_base + 0x20019E0);
            localplayer = mem.Read<uintptr_t>(world + 0x8);
            playerInfo = 0x10A8;
        }
        if (process_name == "FiveM_b2944_GTAProcess.exe") {
            world = mem.Read<uintptr_t>(game_base + 0x257BEA0);
            replay = mem.Read<uintptr_t>(game_base + 0x1F42068);
            viewport = mem.Read<uintptr_t>(game_base + 0x1FEAAC0);
            localplayer = mem.Read<uintptr_t>(world + 0x8);
            playerInfo = 0x10A8;
        }
        if (process_name == "FiveM_b3095_GTAProcess.exe") {
            world = mem.Read<uintptr_t>(game_base + 0x2593320);
            replay = mem.Read<uintptr_t>(game_base + 0x1F58B58);
            viewport = mem.Read<uintptr_t>(game_base + 0x20019E0);
            localplayer = mem.Read<uintptr_t>(world + 0x8);
            playerInfo = 0x10A8;
        }        
    }

    void ESP::RunESP() {
        auto handle = mem.CreateScatterHandle();
        uintptr_t ped_replay_interface;
        mem.AddScatterReadRequest(handle, offset::replay + 0x18, reinterpret_cast<void*>(&ped_replay_interface), sizeof(uintptr_t));
        mem.ExecuteReadScatter(handle);
        if (!ped_replay_interface)
            return;

        uintptr_t pedListBase;
        mem.AddScatterReadRequest(handle, ped_replay_interface + 0x100, reinterpret_cast<void*>(&pedListBase), sizeof(uintptr_t));
        mem.ExecuteReadScatter(handle);
        if (!pedListBase)
            return;

        Matrix view_matrix;
        mem.AddScatterReadRequest(handle, offset::viewport + 0x24C, reinterpret_cast<void*>(&view_matrix), sizeof(Matrix));

        std::vector<uintptr_t> rawPedPointers(110);
        mem.AddScatterReadRequest(handle, pedListBase, reinterpret_cast<void*>(rawPedPointers.data()), sizeof(uintptr_t) * 110);

        mem.ExecuteReadScatter(handle);

        std::unordered_set<uintptr_t> currentFramePeds;

        for (int i = 0; i < 80; i++) {
            uintptr_t ped = rawPedPointers[i]; 
            if (!ped || ped == offset::localplayer)
                continue;

            currentFramePeds.insert(ped);
            auto& cachedPed = pedCache[ped]; 


            //mem.AddScatterReadRequest(handle, ped + offset::playerHealth, reinterpret_cast<void*>(&cachedPed.health), sizeof(float));
            mem.AddScatterReadRequest(handle, ped + offset::playerPosition, reinterpret_cast<void*>(&cachedPed.position_origin), sizeof(Vec3));
            mem.ExecuteReadScatter(handle);


            Vec2 screenPos;
            if (cachedPed.position_origin.world_to_screen(view_matrix, screenPos)) {
                esp::draw_skeleton(ped, view_matrix, offset::localplayer);

            }
        }


        for (auto it = pedCache.begin(); it != pedCache.end(); ) {
            if (currentFramePeds.find(it->first) == currentFramePeds.end())
                it = pedCache.erase(it);
            else
                ++it;
        }
    }

}