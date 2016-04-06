#ifndef __PTF_LVL_MENU_HPP__
#define __PTF_LVL_MENU_HPP__

#include <SFML/Audio.hpp>
#include <iostream>
#include "manager.hpp"
#include "game.hpp"
#include "editor.hpp"
#include "view.hpp"
#include "resource_manager.hpp"
#include <vector>

#define NB_LVL_CHOICES 3

/**
 * La classe Lvl_menu charge le menu permettant de
 * choisir le niveau auquel l'utilisateur souhaite
 * jouer.
 */
class Lvl_menu : public View {

public:
    Lvl_menu(Manager& manager);
    virtual ~Lvl_menu();

    /**
     * Dessine le menu de niveaux
     */
    void frame();

    /**
     * Permet de changer le choix sélectionné
     */
    void MoveUp();
    void MoveDown();

private:
    //repère le choix sélectionné
    int selection;

    sf::Font font;

    //tableau de textes modélisant les différents
    //choix dans le menu
    sf::Text choice[NB_LVL_CHOICES];
};


#endif
