////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Copyright (C) 2026 AStruthers2000 - All Rights Reserved
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef PONG_PLAYER_H
#define PONG_PLAYER_H

#include "aurora_engine_public.h"

class WallManager;

class Player : public AuroraEngine::GameObject
{
public:
    Player(AuroraEngine::GameWorld& owning_world, AuroraEngine::TransformComponent const& initial_transform, SDL_Color const& color, WallManager& wall_manager, int player_num)
        : GameObject(owning_world, initial_transform)
        , m_color(color)
        , m_wall_manager(wall_manager)
        , m_player_num(player_num)
    {
    }

    ~Player() override = default;

    void initialize() override;
    void update(float delta_time) override;
    void render(SDL_Renderer* renderer) override;
    void cleanup() override;
    [[nodiscard]] float get_elasticity() const { return m_elasticity; }

private:
    SDL_Color m_color;
    WallManager& m_wall_manager;
    glm::vec2 m_move_vec{0, 0};
    float m_acceleration_speed = 1000.f;
    float m_max_acceleration = 10000.f;
    float m_max_speed = 6000.f;
    float m_drag = 0.25f;
    float m_wall_impact_factor = 75.f;
    float m_elasticity = 1.1f;
    int m_player_num = 0;
};

#endif //PONG_PLAYER_H
