#ifndef __PTF_MENU_HPP__
#define __PTF_MENU_HPP__

#include "manager.hpp"
#include "view.hpp"
#include <functional>
#include <vector>

/**
 * La classe Menu charge le menu du jeu
 * et permet de choisir entre jouer, lire les règles
 * et quitter.
 */
class Menu : public View {
private:
    std::vector<sf::String> choices;
    std::vector<std::function<void(void)>> actions;
    unsigned int selection;

protected:
    /**
     * Traite l'événement donné
     */
    virtual void processEvent(const sf::Event& event);

public:
    Menu(Manager& manager);
    virtual ~Menu();

    /**
     * Appelé par le manager lorsque la vue commence à
     * être utilisée
     */
    virtual void begin();

    /**
     * Dessine le menu
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
};


#endif
