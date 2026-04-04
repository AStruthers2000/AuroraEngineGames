#include "pong/game_mode.h"
#include "pong/wall.h"
#include "pong/ball.h"

void GameMode::initialize()
{

}

void GameMode::update(float delta_time)
{
    // handle collision
    resolve_all_collision();
}

void GameMode::render(SDL_Renderer *renderer)
{

}

void GameMode::cleanup()
{

}

void GameMode::spawn_player(PlayerSpecification const& player_spec)
{
    std::shared_ptr<Player> player = std::make_unique<Player>(get_world(), player_spec.position, *this, player_spec.color, player_spec.player_num);
    player->get_transform().set_scale(player_spec.size);
    m_managed_objects.push_back(player);
    get_world().add_object(std::move(player), player_spec.update_order);
}

void GameMode::spawn_ball(BallSpecification const& ball_spec)
{
    AuroraEngine::TransformComponent spawn_location{ ball_spec.position };
    spawn_location.update_position(glm::vec2(-ball_spec.radius, -ball_spec.radius));

    std::shared_ptr<Ball> ball = std::make_unique<Ball>(get_world(), spawn_location, *this, ball_spec.radius, ball_spec.color);
    m_managed_objects.push_back(ball);
    get_world().add_object(std::move(ball), ball_spec.update_order);
}

void GameMode::spawn_wall(const WallSpecification &wall_spec)
{
    std::shared_ptr<Wall> wall = std::make_shared<Wall>(get_world(), AuroraEngine::TransformComponent(wall_spec.position), *this, wall_spec.scale, wall_spec.color, wall_spec.elasticity);
    m_managed_objects.push_back(wall);
    get_world().add_object(std::move(wall), wall_spec.update_order);
}

void GameMode::spawn_overlap(const OverlapSpecification &overlap_spec)
{
    std::shared_ptr<OverlapVolume> overlap = std::make_unique<OverlapVolume>(get_world(), AuroraEngine::TransformComponent(overlap_spec.position), *this, overlap_spec.overlap_position);
    overlap->get_transform().set_scale(overlap_spec.scale);
    m_overlap_volumes.push_back(overlap);
    get_world().add_object(std::move(overlap), overlap_spec.update_order);
}

void GameMode::resolve_all_collision()
{
    // Cache all colliders at the beginning
    std::unordered_map<BetterGameObject*, SDL_FRect> colliders;
    for (auto const& object : m_managed_objects)
    {
        if (auto object_ptr = object.lock())
        {
            if (object_ptr->get_object_state() == EGameObjectState::Active)
            {
                colliders.emplace(object_ptr.get(), object_ptr->get_collider());
            }
        }
    }

    // iterate over all dynamic objects
    for (auto& [object, collider] : colliders)
    {
        // for each object that isn't this object
        if (object->is_dynamic())
        {
            for (auto& [other, other_collider] : colliders)
            {
                if (object != other)
                {
                    SDL_FRect overlap;
                    if (SDL_GetRectIntersectionFloat(&collider, &other_collider, &overlap))
                    {
                        constexpr float tolerance = 0.001f;
                        if (abs(overlap.w) > tolerance && abs(overlap.h) > tolerance)
                        {
                            resolve_collision(object, other, overlap);
                        }
                    }
                }
            }
        }
    }
}

void GameMode::resolve_collision(BetterGameObject* dynamic, BetterGameObject* other, SDL_FRect const& overlap)
{
    AuroraEngine::TransformComponent dynamic_transform = dynamic->get_transform();
    glm::vec2 dynamic_pos = dynamic->get_transform().get_position();
    glm::vec2 dynamic_size = dynamic->get_transform().get_scale();
    glm::vec2 dynamic_center = dynamic_pos + dynamic_size * 0.5f;
    glm::vec2 overlap_center = glm::vec2{overlap.x + overlap.w * 0.5f,
                                         overlap.y + overlap.h * 0.5f};

    ECollisionDirection direction{ ECollisionDirection::None };
    glm::vec2 collision_vec = dynamic_center - overlap_center;
    if (abs(collision_vec.x) <= abs(collision_vec.y))
    {
        // Collision happened further away vertically, so this is a vertical collision
        if (collision_vec.y > 0)
        {
            // Collision happened above our center
            dynamic->get_transform().update_position(glm::vec2(0, overlap.h));
            direction = ECollisionDirection::Up;
        }
        else
        {
            // Collision happened below our center
            dynamic->get_transform().update_position(glm::vec2(0, -overlap.h));
            direction = ECollisionDirection::Down;
        }
    }
    else
    {
        // Collision happened further away horizontally, so this is a horizontal collision
        if (collision_vec.x > 0)
        {
            // Collision happened to the left of our center
            dynamic->get_transform().update_position(glm::vec2(overlap.w, 0));
            direction = ECollisionDirection::Left;
        }
        else
        {
            // Collision happened to the right of our center
            dynamic->get_transform().update_position(glm::vec2(-overlap.w, 0));
            direction = ECollisionDirection::Right;
        }
    }
    dynamic->get_transform().set_velocity(glm::vec2(0));
    // Call dynamic object's collision response function with transform before hit, other object, and direction of hit
    dynamic->collision_response(dynamic_transform, other, direction);
}
