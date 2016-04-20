#include <iostream>
#include <memory>
#include <utility>
#include "manager.hpp"
#include "states/menu.hpp"

int main() {
    // le gestionnaire du jeu gère la pile d'états
    Manager manager;

    // on crée une instance de l'état "menu"
    auto menu = std::unique_ptr<Menu>(new Menu(manager));

    try {
        // on empile l'état du menu et on démarre
        manager.pushState(std::move(menu));
        manager.start();
    } catch (const std::exception& exception) {
        std::cerr << "Le programme a quitté après une erreur d'exécution." << std::endl;
        std::cerr << exception.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
