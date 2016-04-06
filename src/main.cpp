#include "manager.hpp"
#include "editor.hpp"
#include "menu.hpp"
#include "game.hpp"
#include <iostream>
#include <memory>
#include <fstream>

int main() {
    Manager manager;
    std::shared_ptr<Menu> menu = std::shared_ptr<Menu>(new Menu(manager));
    std::shared_ptr<Editor> editor = std::shared_ptr<Editor>(new Editor(manager));

    try {
        // ouverture du niveau
        std::ifstream file;
        file.open("./levels/level1.dat", std::ios::binary | std::ios::in);
        editor->load(file);
        file.close();

        manager.setView(menu);
        manager.start();
    } catch (const std::exception& exception) {
        std::cerr << "Le programme a quitté après une erreur d'exécution." << std::endl;
        std::cerr << exception.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
