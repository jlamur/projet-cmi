#include "player.hpp"
#include "block.hpp"
#include "manager.hpp"
#include "game.hpp"
#include "constants.hpp"
#include <cstdlib>
#include <iostream>
#include <memory>

int main() {
    Manager manager;
    std::shared_ptr<Game> game = std::shared_ptr<Game>(new Game);

    manager.setView(game);

    try {
        manager.start();
    } catch (const std::exception& exception) {
        std::cerr << std::endl;
        std::cerr << "Le programme a quitté après une erreur d'exécution." << std::endl;
        std::cerr << exception.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
