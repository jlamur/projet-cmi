#include "player.hpp"
#include "block.hpp"
#include "manager.hpp"
#include "game.hpp"
#include "constants.hpp"
#include <cstdlib>
#include <iostream>

int main() {
    Manager manager;

    Player player1(3.5f * Constants::GRID, 10 * Constants::GRID);
    player1.setPlayerNumber(1);
    Player player2(18.5f * Constants::GRID, 10 * Constants::GRID);
    player2.setPlayerNumber(2);
    Block block01(2 * Constants::GRID, 10 * Constants::GRID);
    Block block02(2 * Constants::GRID, 11 * Constants::GRID);
    Block block03(3 * Constants::GRID, 11 * Constants::GRID);
    Block block04(4 * Constants::GRID, 11 * Constants::GRID);
    Block block05(5 * Constants::GRID, 11 * Constants::GRID);
    Block block06(6 * Constants::GRID, 11 * Constants::GRID);
    Block block07(7 * Constants::GRID, 11 * Constants::GRID);
    Block block08(8 * Constants::GRID, 11 * Constants::GRID);
    Block block09(9 * Constants::GRID, 11 * Constants::GRID);
    Block block10(10 * Constants::GRID, 11 * Constants::GRID);
    Block block11(11 * Constants::GRID, 11 * Constants::GRID);
    Block block12(12 * Constants::GRID, 11 * Constants::GRID);
    Block block13(13 * Constants::GRID, 11 * Constants::GRID);
    Block block14(14 * Constants::GRID, 11 * Constants::GRID);
    Block block15(15 * Constants::GRID, 11 * Constants::GRID);
    Block block16(16 * Constants::GRID, 11 * Constants::GRID);
    Block block17(17 * Constants::GRID, 11 * Constants::GRID);
    Block block18(18 * Constants::GRID, 11 * Constants::GRID);
    Block block19(19 * Constants::GRID, 11 * Constants::GRID);
    Block block20(20 * Constants::GRID, 10 * Constants::GRID);
    Block block21(20 * Constants::GRID, 11 * Constants::GRID);
    Block block22(11 * Constants::GRID, 10 * Constants::GRID);

    player1.setCharge(-.01f);
    player2.setCharge(-.01f);
    block22.setCharge(1.f);
    block22.setMass(2);

    Game game_view;
    manager.view = &game_view;

    manager.view->addObject(player1);
    manager.view->addObject(player2);
    manager.view->addObject(block01);
    manager.view->addObject(block02);
    manager.view->addObject(block03);
    manager.view->addObject(block04);
    manager.view->addObject(block05);
    manager.view->addObject(block06);
    manager.view->addObject(block07);
    manager.view->addObject(block08);
    manager.view->addObject(block09);
    manager.view->addObject(block10);
    manager.view->addObject(block11);
    manager.view->addObject(block12);
    manager.view->addObject(block13);
    manager.view->addObject(block14);
    manager.view->addObject(block15);
    manager.view->addObject(block16);
    manager.view->addObject(block17);
    manager.view->addObject(block18);
    manager.view->addObject(block19);
    manager.view->addObject(block20);
    manager.view->addObject(block21);
    manager.view->addObject(block22);

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
