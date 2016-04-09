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
     * Traite l'événement donné
     */
    virtual void processEvent(const sf::Event& event);

    /**
     * Affiche les règles du jeu
     */
    void loadRules();

    /**
     * Démarre l'éditeur
     */
    void launchEditor();

    /**
     * Démarre le jeu avec le niveau donné
     */
    void launchGame(std::string name);

    /**
     * Quitte le jeu et le menu
     */
    void quit();

public:
    Menu(Manager& manager);
    virtual ~Menu();

    /**
     * Appelé par le manager lorsque le menu est affiché
     */
    virtual void begin();

    /**
     * Dessine la prochaine frame du menu
     */
    virtual void frame(const std::vector<sf::Event>& events);

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
