// peddata.h
#pragma once

#include <unordered_map>
#include "../math/math.h"

// Define the structure for storing ped data
struct PedData {
    float health;
    Vec3 position_origin;
    uintptr_t playerInfo;
};

// Declare the external reference to pedCache
extern std::unordered_map<uintptr_t, PedData> pedCache;