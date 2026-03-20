#include "esp.h"
#include "../math/math.h" // Assuming this contains definitions for Vec3, Vector3, and Matrix
#include "../game/game.h"
#include "../../ImGui/imgui.h"
#include <iostream>
#include "../playerInfo/PedData.h"
#include <cmath> // Required for std::sqrt (still useful for find_closest_player if it were active)


void esp::draw_skeleton(uintptr_t ped, Matrix viewport, uintptr_t localplayer)
{
    ImDrawList* draw_list = ImGui::GetBackgroundDrawList();

    // The head bone is typically bone ID 0. Using a constant directly.
    const int HEAD_BONE_ID = 0;

    Vec2 head_screen_pos;
    Vec3 head_world_pos = esp::get_bone_position(ped, HEAD_BONE_ID);

    if (head_world_pos.world_to_screen(viewport, head_screen_pos)) {
        // Draw a circle around the player's head
        draw_list->AddCircle(
            ImVec2(head_screen_pos.x, head_screen_pos.y),
            3.0f, // Radius of the circle, adjust this value to change its size
            IM_COL32(255, 0, 0, 255), // Red color for the head circle
            20, // Number of segments for a smoother circle
            1.5f // Thickness of the circle line
        );
    }
}


Vec3 esp::get_bone_position(uintptr_t ped, int bone_position)
{
    auto handle = mem.CreateScatterHandle();
    Matrix bone_matrix;
    // Read the bone matrix (view matrix for the skeleton)
    mem.AddScatterReadRequest(handle, ped + 0x60, reinterpret_cast<void*>(&bone_matrix), sizeof(Matrix));

    // Read the local bone offset/position from the ped's bone array
    // Assuming 0x410 is the base offset for bone data and each bone is 0x10 bytes apart.
    Vector3 Head;
    mem.AddScatterReadRequest(handle, ped + (0x410 + 0x10 * bone_position), reinterpret_cast<void*>(&Head), sizeof(Vector3));
    mem.ExecuteReadScatter(handle);

    DirectX::SimpleMath::Vector3 boneVec(Head.x, Head.y, Head.z);
    DirectX::SimpleMath::Vector3 transformedBoneVec = DirectX::XMVector3Transform(boneVec, bone_matrix);
    return Vec3(transformedBoneVec.x, transformedBoneVec.y, transformedBoneVec.z);
}


