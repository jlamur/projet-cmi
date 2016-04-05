#ifndef __PTF_MANAGER_HPP__
#define __PTF_MANAGER_HPP__

#include "resource_manager.hpp"
#include "view.hpp"
#include <memory>

/**
 * Gestionnaire principal de toutes les vues et
 * ressources du jeu
 */
class Manager {
private:
    sf::RenderWindow window;
    sf::Clock clock;
    sf::View window_view;
    std::string title;

    ResourceManager resource_manager;
    std::vector<sf::Event> events;

    std::shared_ptr<View> view;

public:
    Manager();

    /**
     * Démarre la boucle principale du jeu
     */
    void start();

    /**
     * Charge la vue donnée dans le jeu
     */
    void setView(std::shared_ptr<View> set_view);

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
     * Renvoie la liste des événements reçus pour la
     * précédente frame
     */
    const std::vector<sf::Event>& getEvents();

    /**
     * Renvoie la vue de la fenêtre (position centrale, taille)
     */
    sf::View getWindowView();

    /**
     * Modifie la vue de la fenêtre
     */
    void setWindowView(sf::View set_window_view);

    /**
     * Renvoie le titre actuel de la fenêtre
     */
    std::string getTitle();

    /**
     * Modifie le titre actuel de la fenêtre
     */
    void setTitle(std::string set_title);

    /**
     * Renvoie un booléen attestant de l'appui sur la
     * touche donnée ou non
     */
    bool isKeyPressed(sf::Keyboard::Key key) const;
};

#endif
