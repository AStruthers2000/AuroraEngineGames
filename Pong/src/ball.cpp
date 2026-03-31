#include "pong/ball.h"
#include "pong/wall.h"
#include "pong/game_mode.h"

#include <glm/glm.hpp>
#include <glm/gtc/random.hpp>

void Ball::initialize()
{
    get_transform().set_velocity(random_vector());

    AuroraEngine::Engine::get().get_input_subsystem().register_callback(
            AuroraEngine::InputAction(SDL_Scancode::SDL_SCANCODE_R, true),
            [this]()
            {
                get_transform().set_position(m_spawn_transform.get_position());
                get_transform().set_velocity(m_spawn_transform.get_velocity());
                get_transform().set_acceleration(m_spawn_transform.get_acceleration());
                get_transform().set_velocity(random_vector());
            });

    register_collision_response([this](AuroraEngine::TransformComponent const& my_transform, BetterGameObject* other_object, ECollisionDirection direction)
    {
        enum class HitObject
        {
            Player,
            Wall,
            Other
        };
        HitObject hit = HitObject::Other;
        if (dynamic_cast<Wall*>(other_object))
        {
            hit = HitObject::Wall;
        }
        else if (dynamic_cast<Player*>(other_object))
        {
            hit = HitObject::Player;
        }

        if (hit == HitObject::Wall || hit == HitObject::Player)
        {
            float elasticity = 1.f;
            if (hit == HitObject::Player)
            {
                elasticity = dynamic_cast<Player*>(other_object)->get_elasticity();
            }
            else
            {
                elasticity = dynamic_cast<Wall*>(other_object)->elasticity();
            }

            if (direction == ECollisionDirection::Up || direction == ECollisionDirection::Down)
            {
                get_transform().set_velocity(glm::vec2(my_transform.get_velocity().x,
                                                       -my_transform.get_velocity().y));
            }
            else
            {
                get_transform().set_velocity(glm::vec2(-my_transform.get_velocity().x,
                                                       my_transform.get_velocity().y));
            }

            get_transform().set_velocity(get_transform().get_velocity() * elasticity);
        }
    });

    auto& overlaps = get_game_mode().get_overlap_volumes();
    for (auto overlap : overlaps)
    {
        overlap->register_collision_response(this, [this](OverlapVolume* volume)
        {
            printf("Ball has been scored!!!\n");
        });
    }
}

void Ball::update(float delta_time)
{
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

    SDL_FRect collider = get_collider();
    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
    SDL_RenderRect(renderer, &collider);
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
        get_transform().set_velocity(direction * clamped_magnitude);
    }
}

glm::vec2 Ball::random_vector()
{
    glm::vec2 random_vel = glm::circularRand(1.f) * glm::linearRand(500.f, 1000.f);
//    glm::vec2 random_vel = glm::circularRand(1.f) * glm::linearRand(50.f, 100.f);
    return random_vel;
}
