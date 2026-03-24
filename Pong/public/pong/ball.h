////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Copyright (C) 2026 AStruthers2000 - All Rights Reserved
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef PONG_BALL_H
#define PONG_BALL_H

#include <utility>

#include "better_game_object.h"
#include "player.h"

class WallManager;

class Ball : public BetterGameObject
{
public:
    Ball(AuroraEngine::GameWorld& owning_world, AuroraEngine::TransformComponent const& initial_transform, float radius, SDL_Color const& color/*, WallManager* wall_manager, std::vector<Player*> players*/)
        : BetterGameObject(owning_world, initial_transform, true)
        , m_spawn_transform(initial_transform)
        , m_color(color)
//        , m_wall_manager(wall_manager)
//        , m_players(std::move(players))
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
//    WallManager* m_wall_manager;
//    std::vector<Player*> m_players;

    AuroraEngine::TransformComponent m_spawn_transform;

    float m_min_speed = 100.f;
    float m_max_speed = 50000.f;

    // Number of CCD sub-steps per frame
//    static constexpr int k_ccd_substeps = 4;

    void clamp_velocity();
    static glm::vec2 random_vector() ;

    // Returns the geometry-driven outward normal (zero if no collision).
    // Separates the ball from the overlap rect and reflects velocity.
    // elasticity scales the reflected component.
//    glm::vec2 resolve_collision(const SDL_FRect &overlap, float elasticity);
//
//    // Per-sub-step helpers
//    void substep_wall_collision();
//    void substep_player_collision();

//    void bounce(glm::vec2 const& velocity, SDL_FRect const& overlap, float elasticity);
};


#endif //PONG_BALL_H
