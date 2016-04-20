#ifndef __SKIZZLE_MENU_HPP__
#define __SKIZZLE_MENU_HPP__

#include <functional>
#include <vector>
#include <memory>
#include "state.hpp"

class Manager;

/**
 * La classe Menu charge le menu du jeu
 * et permet de choisir entre jouer, lire les règles
 * et quitter.
 */
class Menu : public State {
private:
    sf::Sprite background;
    std::shared_ptr<sf::Font> font;

    std::vector<sf::String> choices;
    std::vector<sf::Text> labels;
    std::vector<std::function<void(void)>> actions;
    unsigned int selection;

protected:
    /**
     * Affiche le menu principal
     */
    void loadMainMenu();

    /**
     * Affiche le menu de choix des niveaux
     */
    void loadLevelMenu();

    /**
     * Affiche le menu de choix des niveaux
     */
    void loadEditorMenu();

    /**
     * Démarre le jeu avec le niveau dont le chemin
     * complet absolu est donné en paramètre
     */
    void launchGame(std::string path);

    /**
     * Démarre les règles
     */
    void launchRules();

    /**
     * Démarre l'éditeur avec le niveau dont le chemin
     * complet absolu est donné en paramètre, ou bien
     * en édition de nouveau niveau
     */
    void launchEditor(std::string path);

    /**
     * Quitte le menu
     */
    void quit();

public:
    Menu(Manager& manager);
    virtual ~Menu();

    /**
     * Active l'état
     */
    virtual void enable();

    /**
     * Traite l'événement donné
     */
    virtual void processEvent(const sf::Event& event);

    /**
     * Demande le dessin d'une frame
     */
    virtual void frame();
};


#endif
