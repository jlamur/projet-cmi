#ifndef __PTF_MENU_HPP__
#define __PTF_MENU_HPP__

#include <SFML/Audio.hpp>
#include <iostream>
#include "manager.hpp"
#include "game.hpp"
#include "editor.hpp"
#include "view.hpp"
#include "resource_manager.hpp"
#include <vector>

#define NB_CHOICES 4

/**
 * La classe Menu charge le menu du jeu
 * et permet de choisir entre jouer, lire les règles
 * et quitter.
 */
class Menu : public View {

public:
    Menu(Manager& manager);
    virtual ~Menu();

    /**
     * Dessine le menu
     */
    bool frame();

    /**
     * Permet de changer le choix sélectionné
     */
    void MoveUp();
    void MoveDown();
    void menu1();
    void menu2();

private:
    //repère le choix sélectionné
    int selection;

    int menu_nb;

    int positionY;
    std::vector<std::function<void()>> menu[NB_CHOICES];
    sf::Font font;

    //tableau de textes modélisant les différents
    //choix dans le menu
    sf::Text choice[NB_CHOICES];
};


#endif
