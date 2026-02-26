#include "pong/player.h"
#include "pong/wall_manager.h"
#include <cmath>

void Player::initialize()
{
        AuroraEngine::Engine::get().get_input_subsystem().register_callback(
                AuroraEngine::InputAxis(AuroraEngine::InputAxis::Axis{SDL_Scancode::SDL_SCANCODE_D, SDL_Scancode::SDL_SCANCODE_A},
                                        AuroraEngine::InputAxis::Axis{SDL_Scancode::SDL_SCANCODE_W, SDL_Scancode::SDL_SCANCODE_S}),
                [this](AuroraEngine::AxisState const& axis)
                {
                    get_transform().set_acceleration(axis.get_normalized_screen_direction() * m_acceleration_speed);
                });
}

void Player::update(float delta_time)
{
    auto clamp_magnitude = [](glm::vec2 const &v, float max) -> glm::vec2 {
        float len = glm::length(v);
        if (len > max && len > 0.f)
            return (v / len) * max;
        return v;
    };

    get_transform().set_acceleration(clamp_magnitude(get_transform().get_acceleration(), m_max_acceleration));
    get_transform().update_velocity(get_transform().get_acceleration() * delta_time);
    get_transform().set_velocity(
            clamp_magnitude(get_transform().get_velocity() * std::pow(m_drag, delta_time), m_max_speed));
    get_transform().update_position(get_transform().get_velocity() * delta_time);

    // Wall collision – push out, glide, then cancel acceleration/velocity into the wall
    glm::vec2 position = get_transform().get_position();
    glm::vec2 size = get_transform().get_scale();

    SDL_FRect player_rect{
            .x = position.x,
            .y = position.y,
            .w = size.x,
            .h = size.y,
    };

    for (auto const &[overlap, elasticity]: m_wall_manager.get_all_overlaps(player_rect))
    {
        glm::vec2 velocity = get_transform().get_velocity();
        glm::vec2 accel = get_transform().get_acceleration();

        // Determine push direction from geometry: compare player centre to overlap centre
        float player_cx = player_rect.x + player_rect.w * 0.5f;
        float player_cy = player_rect.y + player_rect.h * 0.5f;
        float overlap_cx = overlap.x + overlap.w * 0.5f;
        float overlap_cy = overlap.y + overlap.h * 0.5f;

        if (overlap.w >= overlap.h)
        {
            // Horizontal wall (top/bottom) – resolve along Y
            // If player centre is above the overlap centre, push up (negative); otherwise push down (positive)
            float push = (player_cy < overlap_cy) ? -overlap.h : overlap.h;
            get_transform().update_position(glm::vec2{0.f, push});

            // Apply elasticity as continuous friction on the tangential component while touching the wall
            float tangential_x = velocity.x * std::pow(elasticity / m_wall_impact_factor, delta_time);
            get_transform().set_velocity(glm::vec2{tangential_x, 0.f});

            // Cancel acceleration pointing back into the wall
            if ((push > 0.f && accel.y < 0.f) || (push < 0.f && accel.y > 0.f))
                get_transform().set_acceleration(glm::vec2{accel.x, 0.f});
        }
        else
        {
            // Vertical wall (left/right) – resolve along X
            // If player centre is left of the overlap centre, push left (negative); otherwise push right (positive)
            float push = (player_cx < overlap_cx) ? -overlap.w : overlap.w;
            get_transform().update_position(glm::vec2{push, 0.f});

            // Apply elasticity as continuous friction on the tangential component while touching the wall
            float tangential_y = velocity.y * std::pow(elasticity / m_wall_impact_factor, delta_time);
            get_transform().set_velocity(glm::vec2{0.f, tangential_y});

            // Cancel acceleration pointing back into the wall
            if ((push > 0.f && accel.x < 0.f) || (push < 0.f && accel.x > 0.f))
                get_transform().set_acceleration(glm::vec2{0.f, accel.y});
        }
    }
}

void Player::render(SDL_Renderer* renderer)
{
    SDL_FRect dst{
        .x = get_transform().get_position().x,
        .y = get_transform().get_position().y,
        .w = get_transform().get_scale().x,
        .h = get_transform().get_scale().y,
    };

    SDL_SetRenderDrawColor(renderer, m_color.r, m_color.g, m_color.b, m_color.a);
    SDL_RenderFillRect(renderer, &dst);
}

void Player::cleanup()
{

}