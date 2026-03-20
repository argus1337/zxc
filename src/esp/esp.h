#pragma once
#include <cstdint>
#include "../math/math.h"  // Ensure this is included if needed
#include "../game/game.h"  // Ensure this is included if needed
#include "../../ImGui/imgui.h"  // Ensure this is included if needed

namespace esp {
    void draw_skeleton(uintptr_t ped, Matrix viewport, uintptr_t localplayer);
    Vec3 get_bone_position(uintptr_t ped, int bone_position);
}