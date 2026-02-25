////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Copyright (C) 2026 AStruthers2000 - All Rights Reserved
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef PONG_BALL_H
#define PONG_BALL_H

#include "aurora_engine_public.h"

class WallManager;

class Ball : public AuroraEngine::GameObject
{
public:
    Ball(AuroraEngine::GameWorld& owning_world, AuroraEngine::TransformComponent const& initial_transform, float radius, SDL_Color const& color, WallManager* wall_manager)
        : GameObject(owning_world, initial_transform)
        , m_color(color)
        , m_wall_manager(wall_manager)
    {
        get_transform().set_scale(glm::vec2{radius * 2.f, radius * 2.f});
    }

    ~Ball() override = default;

    void initialize() override;
    void update(float delta_time) override;
    void render(SDL_Renderer* renderer) override;
    void cleanup() override;

private:
    SDL_Color m_color;
    WallManager* m_wall_manager;
};


#endif //PONG_BALL_H
