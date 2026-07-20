#include "Game.hpp"
#include "Config.hpp"

int main()
{
    Game game(config::window::width, config::window::height, "Space Shooter");
    game.run();

    return 0;
}
