////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Copyright (C) 2026 AStruthers2000 - All Rights Reserved
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef PONG_GAME_MODE_H
#define PONG_GAME_MODE_H

#include "aurora_engine_public.h"
#include "better_game_object.h"

struct BallSpecification
{
    glm::vec2 position;
    float radius;
    SDL_Color color;
};

struct WallSpecification
{
    glm::vec2 position;
    glm::vec2 scale;
    SDL_Color color;
    float elasticity;
};

class GameMode : public AuroraEngine::GameObject
{
public:
    GameMode(AuroraEngine::GameWorld& owning_world, AuroraEngine::TransformComponent const& initial_transform)
    : GameObject(owning_world, initial_transform)
    {
    }

    ~GameMode() override = default;

    void initialize() override;
    void update(float delta_time) override;
    void render(SDL_Renderer* renderer) override;
    void cleanup() override;

    void spawn_player();
    void spawn_ball(BallSpecification const& ball_spec);
    void spawn_wall(WallSpecification const& wall_spec);

private:
    struct CollisionPair
    {
        BetterGameObject* object;
        SDL_FRect collider;
    };

    void resolve_all_collision();
    void resolve_collision(BetterGameObject* dynamic, BetterGameObject* other, SDL_FRect const& overlap);

    std::vector<BetterGameObject*> m_managed_objects;
};
#endif //PONG_GAME_MODE_H
