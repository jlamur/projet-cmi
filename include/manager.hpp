#ifndef __SKIZZLE_MANAGER_HPP__
#define __SKIZZLE_MANAGER_HPP__

#include "resource_manager.hpp"
#include <SFGUI/SFGUI.hpp>
#include <SFGUI/Widgets.hpp>
#include <SFML/Graphics.hpp>
#include <memory>
#include <stack>

class State;

/**
 * Gestionnaire principal de tous les états, vues et
 * ressources du jeu
 */
class Manager {
private:
    sf::RenderWindow window;
    sf::Time previous_time;

    sfg::SFGUI sfgui;
    sfg::Desktop desktop;
    std::vector<sfg::Widget::Ptr> widgets;

    unsigned int framerate;
    ResourceManager resource_manager;

    sf::Clock clock;
    sf::View gui_view;
    sf::String title;

    State* previous_state;
    std::stack<std::unique_ptr<State>> states;

    /**
     * Détermine si l'événement donné s'est passé à l'intérieur
     * de l'interface et doit être ignoré pour la suite ou non
     */
    bool isInsideGUI(const sf::Event& event);

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

    /**
     * Taille d'une case de la grille du jeu
     */
    static const float GRID;

    Manager();
    ~Manager();

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
     * Récupère le framerate (maximal) actuel
     */
    unsigned int getFramerate();

    /**
     * Modifie le framerate (maximal) actuel
     */
    void setFramerate(unsigned int set_framerate);

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
     * @deprecated
     */
    void useGUIView();

    /**
     * Ajoute un nouveau widget à l'interface
     */
    void addWidget(sfg::Widget::Ptr widget);

    /**
     * Supprime un widget de l'interface
     */
    void removeWidget(sfg::Widget::Ptr widget);

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
