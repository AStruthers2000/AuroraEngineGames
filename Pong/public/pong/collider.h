////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Copyright (C) 2026 AStruthers2000 - All Rights Reserved
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef PONG_COLLIDER_H
#define PONG_COLLIDER_H

#include "aurora_engine_public.h"

class Collider
{
public:
    Collider()
        : m_owner(nullptr)
    {
    }

    Collider(AuroraEngine::GameObject* owner, bool is_dynamic)
        : m_owner(owner)
        , m_is_dynamic(is_dynamic)
    {
    }

    [[nodiscard]] SDL_FRect get_collider() const
    {
        SDL_FRect extents{};
        if (m_owner)
        {
            extents = {
                    .x = m_owner->get_transform().get_position().x,
                    .y = m_owner->get_transform().get_position().y,
                    .w = m_owner->get_transform().get_scale().x,
                    .h = m_owner->get_transform().get_scale().y,
            };
        }

        return extents;
    }

    [[nodiscard]] bool is_dynamic() const { return m_is_dynamic; }

private:
    AuroraEngine::GameObject* m_owner;
    bool m_is_dynamic;
};

#endif //PONG_COLLIDER_H
