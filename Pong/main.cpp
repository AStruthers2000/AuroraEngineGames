#include "aurora_engine_public.h"

#include <print>

using namespace AuroraEngine;

int main()
{
    WindowSpecification window_spec
    {
        .window_size = {1920, 1080},
        .logical_size = {1920, 1080},
        .title = "Pong",
    };

    Engine app(window_spec);
    std::unique_ptr<GameWorld> world = std::make_unique<GameWorld>(app);

    app.initialize(std::move(world));
    app.run();

    return 0;
}
