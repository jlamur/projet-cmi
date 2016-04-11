#ifndef __PTF_MENU_HPP__
#define __PTF_MENU_HPP__

#include "manager.hpp"
#include "state.hpp"
#include <functional>
#include <vector>

/**
 * La classe Menu charge le menu du jeu
 * et permet de choisir entre jouer, lire les règles
 * et quitter.
 */
class Menu : public State {
private:
    sf::Sprite background;

    std::vector<sf::String> choices;
    std::vector<sf::Text> labels;
    std::vector<std::function<void(void)>> actions;
    unsigned int selection;

protected:
    /**
     * Affiche les règles du jeu
     */
    void loadRules();

    /**
     * Démarre l'éditeur
     */
    void launchEditor();

    /**
     * Démarre le jeu avec le niveau dont le chemin
     * complet absolu est donné en paramètre
     */
    void launchGame(std::string path);

    /**
     * Quitte le jeu et le menu
     */
    void quit();

public:
    Menu(Manager& manager);
    virtual ~Menu();

    /**
     * Traite l'événement donné
     */
    virtual void processEvent(const sf::Event& event);

    /**
     * Demande le dessin d'une frame
     */
    virtual void frame();

    /**
     * Affiche le menu principal
     */
    void loadMainMenu();

    /**
     * Affiche le menu de choix des niveaux
     */
    void loadLevelMenu();
};


#endif
