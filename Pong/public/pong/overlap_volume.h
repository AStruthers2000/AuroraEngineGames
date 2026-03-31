////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Copyright (C) 2026 AStruthers2000 - All Rights Reserved
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef PONG_OVERLAP_VOLUME_H
#define PONG_OVERLAP_VOLUME_H

#include "aurora_engine_public.h"

#include <functional>

class BetterGameObject;
class OverlapVolume;
class GameMode;
using OverlapCallback = std::function<void(OverlapVolume*)>;

class OverlapVolume : public AuroraEngine::GameObject
{
public:
    enum class EOverlapPosition
    {
        None,
        LeftSide,
        RightSide,
    };

    OverlapVolume(AuroraEngine::GameWorld& owning_world,
                  AuroraEngine::TransformComponent const& initial_transform,
                  GameMode& owning_mode,
                  EOverlapPosition overlap_side)
        : GameObject(owning_world, initial_transform)
        , m_game_mode(owning_mode)
        , m_overlap_side(overlap_side)
    {
    }

    ~OverlapVolume() override = default;

    void initialize() override;
    void update(float delta_time) override;
    void render(SDL_Renderer* renderer) override;
    void cleanup() override;

    void register_collision_response(BetterGameObject* registrant, OverlapCallback&& overlap_response);

private:
    bool check_for_overlap(BetterGameObject const* other);

    GameMode& m_game_mode;
    SDL_FRect my_volume{};
    EOverlapPosition m_overlap_side{ EOverlapPosition::None };
    std::unordered_map<BetterGameObject*, OverlapCallback> m_registered_game_objects{};
};

#endif //PONG_OVERLAP_VOLUME_H
