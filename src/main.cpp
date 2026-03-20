#include <thread>
#include "Memory/Memory.h"
#include <iostream>
#include "game/game.h"
#include "window/window.hpp"
#include "globals.h"

void clearConsole()
{
#ifdef _WIN32
	system("cls");
#else
	system("clear");
#endif
}



// In a source file (e.g., globals.cpp or main.cpp)
std::string g_validExecutable;


// Usage in InitMemory():
void InitMemory()
{
    std::vector<std::string> executables = {
        "FiveM_b2802_GTAProcess.exe", 
        "FiveM_b2944_GTAProcess.exe", 
        "FiveM_b3095_GTAProcess.exe"
    };

    bool initialized = false;

    for (const auto& exe : executables) {
        std::cout << "Trying to initialize DMA with: " << exe << std::endl;

        if (mem.Init(exe.c_str(), true, true)) {
            std::cout << "DMA Initialized with: " << exe << "!!\n";
            g_validExecutable = exe;  // Store globally
            initialized = true;
            break;
        }
        else {
            std::cout << "Failed to initialize with: " << exe << std::endl;
        }
    }

    if (!initialized) {
        std::cout << "Failed to INIT DMA with any process... Is the game running?\n";
        return;
    }

    uint64_t base = mem.GetBaseDaddy(g_validExecutable.c_str());
    std::cout << "Base ::: 0x" << std::hex << base << "\n";
}


int main()
{



    InitMemory();
    Overlay overlay;
    overlay.SetupOverlay("not a fivem cheat");

    FiveM::Setup();


    while (overlay.shouldRun) {

    

        overlay.StartRender();

        if (overlay.RenderMenu)
            overlay.Render();
            

        overlay.EndRender();
    }
    return 0;
}

