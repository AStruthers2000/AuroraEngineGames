////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Copyright (C) 2026 AStruthers2000 - All Rights Reserved
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef PONG_WALL_MANAGER_H
#define PONG_WALL_MANAGER_H

#include "aurora_engine_public.h"

#include <vector>
#include <utility>
#include <tuple>

#include <glm/glm.hpp>

class Wall;

class WallManager : public AuroraEngine::GameObject
{
public:
    WallManager(AuroraEngine::GameWorld& owning_world, AuroraEngine::TransformComponent const& initial_transform);
    ~WallManager() override;

    void initialize() override {}
    void update(float delta_time) override {}
    void render(SDL_Renderer* renderer) override {}
    void cleanup() override {}

    void add_wall(Wall* wall);
     [[nodiscard]] std::vector<std::pair<SDL_FRect, float>> get_all_overlaps(SDL_FRect const& rect) const;

private:
    std::vector<Wall*> m_walls;
};


#endif //PONG_WALL_MANAGER_H
