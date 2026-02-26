////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Copyright (C) 2026 AStruthers2000 - All Rights Reserved
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef PONG_PLAYER_H
#define PONG_PLAYER_H

#include "aurora_engine_public.h"

class Player : public AuroraEngine::GameObject
{
public:
    Player(AuroraEngine::GameWorld& owning_world, AuroraEngine::TransformComponent const& initial_transform, SDL_Color const& color)
        : GameObject(owning_world, initial_transform)
        , m_color(color)
    {
    }

    ~Player() override = default;

    void initialize() override;
    void update(float delta_time) override;
    void render(SDL_Renderer* renderer) override;
    void cleanup() override;

private:
    SDL_Color m_color;
    glm::vec2 m_move_vec{0, 0};
    float m_acceleration_speed = 400.f;
    float m_max_acceleration = 400.f;
    float m_max_speed = 600.f;
    float m_drag = 0.85f;
};

#endif //PONG_PLAYER_H
