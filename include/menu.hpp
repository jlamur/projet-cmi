#ifndef __PTF_MENU_HPP__
#define __PTF_MENU_HPP__

#include <SFML/Audio.hpp>
#include "manager.hpp"
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
    void frame();

    /**
     *
     */
    void MoveUp();
    void MoveDown();

private:
    //repère le choix sélectionné
    int selection;

    sf::Font font;

    //tableau de textes modélisant les différents
    //choix dans le menu
    sf::Text choice[NB_CHOICES];
};


#endif
