#include "player.hpp"
#include "block.hpp"
#include "manager.hpp"
#include "game.hpp"
#include "constants.hpp"
#include <cstdlib>
#include <iostream>
#include <memory>
#include <fstream>

int main() {
    Manager manager;
    std::shared_ptr<Game> game = std::shared_ptr<Game>(new Game(manager));

    try {
        // ouverture du niveau
        std::ifstream file;
        file.open("./levels/level1.dat", std::ios::binary | std::ios::in);
        game->load(file);
        file.close();

        manager.setView(game);
        manager.start();
    } catch (const std::exception& exception) {
        std::cerr << "Le programme a quitté après une erreur d'exécution." << std::endl;
        std::cerr << exception.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
