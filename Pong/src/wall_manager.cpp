#include "pong/wall_manager.h"

#include "pong/wall.h"

#include <algorithm>
#include <cmath>

WallManager::WallManager(AuroraEngine::GameWorld &owning_world,
                         const AuroraEngine::TransformComponent &initial_transform)
    : GameObject(owning_world, initial_transform)
{
}

WallManager::~WallManager() = default;

void WallManager::add_wall(Wall* wall)
{
    m_walls.push_back(wall);
}

 std::vector<std::pair<SDL_FRect, float>> WallManager::get_all_overlaps(const SDL_FRect &rect) const
 {
     std::vector<std::pair<SDL_FRect, float>> overlaps{};
     for (const auto& wall : m_walls)
     {
         SDL_FRect wall_rect{
             .x = wall->get_transform().get_position().x,
             .y = wall->get_transform().get_position().y,
             .w = wall->get_transform().get_scale().x,
             .h = wall->get_transform().get_scale().y,
         };
         SDL_FRect overlap{};
         if (SDL_GetRectIntersectionFloat(&rect, &wall_rect, &overlap))
             overlaps.emplace_back(overlap, wall->elasticity());
     }
     return overlaps;
 }
