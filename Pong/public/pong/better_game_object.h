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
    BetterGameObject(AuroraEngine::GameWorld& owning_world, AuroraEngine::TransformComponent const& initial_transform, bool is_dynamic_object = false)
        : GameObject(owning_world, initial_transform)
        , m_collider(std::make_unique<Collider>(this, is_dynamic_object))
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

    [[nodiscard]] bool is_dynamic() const
    {
        return m_collider->is_dynamic();
    }

    void collision_response(AuroraEngine::TransformComponent const& my_transform, BetterGameObject* other_object, ECollisionDirection direction)
    {
        m_collider->collision_response(my_transform, other_object, direction);
    }

protected:
    void register_collision_response(CollisionCallback&& collision_response)
    {
        m_collider->register_collision_response(std::move(collision_response));
    }

private:
    std::unique_ptr<Collider> m_collider;
};

#endif //PONG_BETTER_GAME_OBJECT_H
