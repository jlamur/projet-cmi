#include "manager.hpp"
#include "editor.hpp"
#include "menu.hpp"
#include "game.hpp"
#include <iostream>
#include <memory>
#include <fstream>
#include <fenv.h>

int main() {
    Manager manager;
    std::shared_ptr<Menu> menu = std::shared_ptr<Menu>(new Menu(manager));
    feenableexcept(FE_ALL_EXCEPT & ~FE_INEXACT);

    try {
        manager.setState(menu);
        manager.start();
    } catch (const std::exception& exception) {
        std::cerr << "Le programme a quitté après une erreur d'exécution." << std::endl;
        std::cerr << exception.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
