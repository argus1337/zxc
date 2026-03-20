#include <cstdint>
#include <vector>
#include "../DMALibrary/Memory/Memory.h"
#include "../../Utils/SimpleMath.h"  
#include "../esp/esp.h"


namespace FiveM {
    namespace offset {
        extern uintptr_t world, replay, viewport, localplayer;
        extern uintptr_t boneList, boneMatrix;
        extern uintptr_t playerInfo, playerHealth, playerPosition;
        extern uintptr_t base;
    }

    namespace ESP {
        void RunESP();
    }

    void Setup();

}

