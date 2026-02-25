#include "pong/ball.h"
#include "pong/wall_manager.h"

#include <glm/glm.hpp>
#include <glm/gtc/random.hpp>

void Ball::initialize()
{
    glm::vec2 random_vel = glm::circularRand(1.f) * glm::linearRand(200.f, 400.f);
    get_transform().set_velocity(random_vel);
}

void Ball::update(float delta_time)
{
    glm::vec2 position = get_transform().get_position();
    glm::vec2 velocity = get_transform().get_velocity();
    glm::vec2 size     = get_transform().get_scale();

    SDL_FRect ball_rect{
        .x = position.x,
        .y = position.y,
        .w = size.x,
        .h = size.y,
    };

    for (auto const& [overlap, elasticity] : m_wall_manager->get_all_overlaps(ball_rect))
    {
        if (overlap.w >= overlap.h)
        {
            if (velocity.y < 0.f)
            {
                get_transform().update_position(glm::vec2{0.f, overlap.h});
            }
            else
            {
                get_transform().update_position(glm::vec2{0.f, -overlap.h});
            }

            get_transform().set_velocity(glm::vec2{velocity.x, -velocity.y});
        }
        else
        {
            if (velocity.x < 0.f)
            {
                get_transform().update_position(glm::vec2{overlap.w, 0.f});
            }
            else
            {
                get_transform().update_position(glm::vec2{-overlap.w, 0.f});
            }

            get_transform().set_velocity(glm::vec2{-velocity.x, velocity.y});
        }

        get_transform().set_velocity(get_transform().get_velocity() * elasticity);
    }

    clamp_velocity();
    get_transform().update_position(get_transform().get_velocity() * delta_time);
}


void Ball::render(SDL_Renderer *renderer)
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

void Ball::cleanup()
{

}

void Ball::clamp_velocity()
{
    glm::vec2 velocity = get_transform().get_velocity();
    float magnitude = glm::length(velocity);

    if (magnitude > 0.f && (magnitude < m_min_speed || magnitude > m_max_speed))
    {
        glm::vec2 direction = glm::normalize(velocity);
        float clamped_magnitude = glm::clamp(magnitude, m_min_speed, m_max_speed);
        glm::vec2 clamped_velocity = direction * clamped_magnitude;
        get_transform().set_velocity(clamped_velocity);
    }
}
