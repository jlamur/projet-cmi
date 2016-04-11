#include "manager.hpp"
#include "editor.hpp"
#include "menu.hpp"
#include "game.hpp"
#include <iostream>
#include <memory>

int main() {
    Manager manager;
    auto menu = std::unique_ptr<Menu>(new Menu(manager));

    try {
        manager.pushState(std::move(menu));
        manager.start();
    } catch (const std::exception& exception) {
        std::cerr << "Le programme a quitté après une erreur d'exécution." << std::endl;
        std::cerr << exception.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
