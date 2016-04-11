#ifndef __SKIZZLE_MANAGER_HPP__
#define __SKIZZLE_MANAGER_HPP__

#include "resource_manager.hpp"
#include "state.hpp"
#include <memory>
#include <stack>

/**
 * Gestionnaire principal de tous les états, vues et
 * ressources du jeu
 */
class Manager {
private:
    sf::RenderWindow window;
    ResourceManager resource_manager;

    sf::Clock clock;
    sf::View gui_view;
    sf::String title;

    std::stack<std::unique_ptr<State>> states;

public:
    /**
     * Énumération des modificateurs
     */
    enum class Modifier {CONTROL, ALT, SHIFT, SYSTEM};

    /**
     * Taux maximal de frames par seconde
     */
    static const unsigned int FPS;

    /**
     * Temps idéal entre deux frames
     */
    static const sf::Time FRAME_TIME;

    Manager();

    /**
     * Démarre la boucle principale du jeu
     */
    void start();

    /**
     * Empile l'état donné
     */
    void pushState(std::unique_ptr<State> set_state);

    /**
     * Dépile l'état actuel
     */
    void popState();

    /**
     * Renvoie la fenêtre actuellement utilisée pour le dessin
     */
    sf::RenderWindow& getWindow();

    /**
     * Renvoie le temps actuel du jeu
     */
    sf::Time getCurrentTime() const;

    /**
     * Renvoie le gestionnaire de ressources
     */
    ResourceManager& getResourceManager();

    /**
     * Passage en vue de l'interface
     * (coin en haut à gauche, zoom 1:1)
     */
    void useGUIView();

    /**
     * Renvoie le titre actuel de la fenêtre
     */
    sf::String getTitle();

    /**
     * Modifie le titre actuel de la fenêtre
     */
    void setTitle(sf::String set_title);

    /**
     * Renvoie un booléen attestant de l'appui sur la
     * touche donnée ou non
     */
    bool isKeyPressed(sf::Keyboard::Key key) const;

    /**
     * Renvoie un booléen attestant de l'appui sur le
     * modificateur
     */
    bool isKeyPressed(Modifier modifier) const;
};

#endif
