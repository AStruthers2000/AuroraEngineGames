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

    get_transform().update_position(velocity * delta_time);

    SDL_FRect ball_rect{
        .x = get_transform().get_position().x,
        .y = get_transform().get_position().y,
        .w = size.x,
        .h = size.y,
    };

    for (auto const& [overlap, elasticity] : m_wall_manager->get_all_overlaps(ball_rect))
    {
        if (overlap.w >= overlap.h)
        {
            if (velocity.y < 0.f)
            {
                printf("Wall is above ball\n");
                get_transform().update_position(glm::vec2{0.f, overlap.h});
            }
            else
            {
                printf("Wall is below ball\n");
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
