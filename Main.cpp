#include "Game.h"

/* Size of the board
(0,0)         (800,0)
+--------------+
|              |
|              |
|              |
|              |
|              |
|              |
+--------------+
(0,600)       (800,600)
*/


Game* game = nullptr;

int main(int argc, char* argv[]) {
    game = new Game();
    game->init("Solar Sprint: Light Chaser", 800, 600);

    while (game->running()) {
        game->handleEvents();
        game->update();
        game->render();
    }

    game->clean();
    return 0;
}
