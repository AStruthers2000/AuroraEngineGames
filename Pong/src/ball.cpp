#include "pong/ball.h"
#include "pong/wall_manager.h"
#include "pong/wall.h"

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
}

void Ball::update(float delta_time)
{
    clamp_velocity();
    get_transform().update_position(get_transform().get_velocity() * delta_time);
    // Split each frame into k_ccd_substeps equal sub-steps.
    // Within each sub-step:
    //   1. Advance position by (velocity * sub_dt)
    //   2. Resolve player collision (position separation + velocity response)
    //   3. Resolve wall collisions
    // This keeps penetration depths small regardless of speed, eliminating
    // tunnelling, corner-flip artefacts, and the multi-collision flip-flop.

//    const float sub_dt = delta_time / static_cast<float>(k_ccd_substeps);
//
//    for (int step = 0; step < k_ccd_substeps; ++step)
//    {
//        // Advance
//        get_transform().update_position(get_transform().get_velocity() * sub_dt);
//
//        // Player first so wall crush-prevention can query walls after
//
//        for (auto* player : m_players)
//        {
//            if (player)
//            {
//                SDL_FRect ball_rect{
//                        .x = get_transform().get_position().x,
//                        .y = get_transform().get_position().y,
//                        .w = get_transform().get_scale().x,
//                        .h = get_transform().get_scale().y
//                };
//
//                SDL_FRect player_rect{
//                        .x = player->get_transform().get_position().x,
//                        .y = player->get_transform().get_position().y,
//                        .w = player->get_transform().get_scale().x,
//                        .h = player->get_transform().get_scale().y,
//                };
//
//                SDL_FRect overlap;
//                if (SDL_GetRectIntersectionFloat(&ball_rect, &player_rect, &overlap))
//                {
//                    bounce(get_transform().get_velocity(), overlap, player->get_elasticity());
//                }
//            }
//        }
//
//        if (m_wall_manager)
//        {
//            SDL_FRect ball_rect{
//                .x = get_transform().get_position().x,
//                .y = get_transform().get_position().y,
//                .w = get_transform().get_scale().x,
//                .h = get_transform().get_scale().y
//            };
//
//            for (auto const &[overlap, elasticity] : m_wall_manager->get_all_overlaps(ball_rect))
//            {
//                bounce(get_transform().get_velocity(), overlap, elasticity);
//            }
//        }
//    }
//
//    clamp_velocity();
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

//void Ball::bounce(glm::vec2 const& velocity, SDL_FRect const& overlap, float elasticity)
//{
//    if (overlap.w >= overlap.h)
//    {
//        if (velocity.y < 0.f)
//        {
//            get_transform().update_position(glm::vec2{0.f, overlap.h});
//        }
//        else
//        {
//            get_transform().update_position(glm::vec2{0.f, -overlap.h});
//        }
//
//        get_transform().set_velocity(glm::vec2{velocity.x, -velocity.y});
//    }
//    else
//    {
//        if (velocity.x < 0.f)
//        {
//            get_transform().update_position(glm::vec2{overlap.w, 0.f});
//        }
//        else
//        {
//            get_transform().update_position(glm::vec2{-overlap.w, 0.f});
//        }
//
//        get_transform().set_velocity(glm::vec2{-velocity.x, velocity.y});
//    }
//
//    get_transform().set_velocity(get_transform().get_velocity() * elasticity);
//}

//glm::vec2 Ball::resolve_collision(const SDL_FRect &overlap, float elasticity)
//{
//    glm::vec2 pos      = get_transform().get_position();
//    glm::vec2 size     = get_transform().get_scale();
//    glm::vec2 velocity = get_transform().get_velocity();
//
//    // Centre of the ball and centre of the overlap rect
//    glm::vec2 ball_centre   = pos + size * 0.5f;
//    glm::vec2 overlap_centre = glm::vec2{overlap.x + overlap.w * 0.5f,
//                                         overlap.y + overlap.h * 0.5f};
//
//    glm::vec2 normal{0.f, 0.f};
//
//    if (overlap.w <= overlap.h)
//    {
//        // Shallower penetration along X  →  resolve horizontally
//        // Normal points from overlap centre toward ball centre
//        normal.x = (ball_centre.x >= overlap_centre.x) ? 1.f : -1.f;
//        get_transform().update_position(glm::vec2{normal.x * overlap.w, 0.f});
//
//        // Reflect only the X component of velocity
//        velocity.x = std::abs(velocity.x) * normal.x;
//        velocity.x *= elasticity;
//    }
//    else
//    {
//        // Shallower penetration along Y  →  resolve vertically
//        normal.y = (ball_centre.y >= overlap_centre.y) ? 1.f : -1.f;
//        get_transform().update_position(glm::vec2{0.f, normal.y * overlap.h});
//
//        // Reflect only the Y component of velocity
//        velocity.y = std::abs(velocity.y) * normal.y;
//        velocity.y *= elasticity;
//    }
//
//    get_transform().set_velocity(velocity);
//    return normal;
//}
//
//void Ball::substep_wall_collision()
//{
//    glm::vec2 pos  = get_transform().get_position();
//    glm::vec2 size = get_transform().get_scale();
//    SDL_FRect ball_rect{.x = pos.x, .y = pos.y, .w = size.x, .h = size.y};
//
//    for (auto const &[overlap, elasticity] : m_wall_manager->get_all_overlaps(ball_rect))
//    {
//        resolve_collision(overlap, elasticity);
//    }
//}
//
//void Ball::substep_player_collision()
//{
//    glm::vec2 pos  = get_transform().get_position();
//    glm::vec2 size = get_transform().get_scale();
//    SDL_FRect ball_rect{.x = pos.x, .y = pos.y, .w = size.x, .h = size.y};
//
//    SDL_FRect player_rect{
//        .x = m_player->get_transform().get_position().x,
//        .y = m_player->get_transform().get_position().y,
//        .w = m_player->get_transform().get_scale().x,
//        .h = m_player->get_transform().get_scale().y,
//    };
//
//    SDL_FRect overlap;
//    if (!SDL_GetRectIntersectionFloat(&ball_rect, &player_rect, &overlap))
//        return;
//
//    // 1. Separate & reflect using geometry-driven normal
//    glm::vec2 normal = resolve_collision(overlap, m_player->get_elasticity());
//
//    if (glm::length(normal) < glm::epsilon<float>())
//        return;
//
//    // 2. Project player velocity onto the bounce normal and add it to the ball.
//    //    This "pushes" the ball in the direction the player is moving.
//    //    Using projection avoids the sign-flip bug from glm::min/max.
//    glm::vec2 player_vel  = m_player->get_transform().get_velocity();
//    float     projection  = glm::dot(player_vel, normal);
//
//    // Only add the push when the player is actually moving into the ball
//    // (projection > 0 means player velocity has a component along the outward normal).
//    if (projection > 0.f)
//    {
//        glm::vec2 vel = get_transform().get_velocity();
//        vel += normal * projection;
//
//        float mag = glm::length(vel);
//        if (mag > m_max_speed)
//            vel = glm::normalize(vel) * m_max_speed;
//
//        get_transform().set_velocity(vel);
//    }
//
//    // 3. Cancel any velocity the player imparted back along the normal if that
//    //    velocity would drive the ball into the wall on the next sub-step.
//    //    This prevents velocity from accumulating in the squeeze direction.
//    if (m_wall_manager)
//    {
//        glm::vec2 new_pos  = get_transform().get_position();
//        SDL_FRect new_rect = {.x = new_pos.x, .y = new_pos.y, .w = size.x, .h = size.y};
//
//        if (!m_wall_manager->get_all_overlaps(new_rect).empty())
//        {
//            // Ball is already touching a wall on the same axis the player pushed.
//            // Zero the velocity component pointing back into the wall (anti-normal
//            // direction) so the squeeze cannot build up over sub-steps.
//            glm::vec2 vel = get_transform().get_velocity();
//            float into_wall = glm::dot(vel, -normal);
//            if (into_wall > 0.f)
//                vel -= (-normal) * into_wall;   // strip the wall-ward component
//            get_transform().set_velocity(vel);
//        }
//    }
//}
