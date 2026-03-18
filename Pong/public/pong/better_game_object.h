////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Copyright (C) 2026 AStruthers2000 - All Rights Reserved
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef PONG_BETTER_GAME_OBJECT_H
#define PONG_BETTER_GAME_OBJECT_H

#include "aurora_engine_public.h"
#include "pong/collider.h"

class BetterGameObject : public AuroraEngine::GameObject
{
public:
    BetterGameObject(AuroraEngine::GameWorld& owning_world, AuroraEngine::TransformComponent const& initial_transform)
        : GameObject(owning_world, initial_transform)
        , m_collider(std::make_unique<Collider>(this))
    {
    }

    ~BetterGameObject() override = default;

    void initialize() override = 0;
    void update(float delta_time) override = 0;
    void render(SDL_Renderer* renderer) override = 0;
    void cleanup() override = 0;

    [[nodiscard]] SDL_FRect get_collider() const
    {
        SDL_FRect collider{};
        if (m_collider)
        {
            collider = m_collider->get_collider();
        }
        return collider;
    }

private:
    std::unique_ptr<Collider> m_collider;
};

#endif //PONG_BETTER_GAME_OBJECT_H
