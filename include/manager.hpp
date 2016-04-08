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
    sf::View default_view;
    sf::String title;

    ResourceManager resource_manager;
    std::vector<sf::Event> events;

    std::shared_ptr<View> view;
    std::shared_ptr<View> next_view;

    bool running;

public:
    Manager();

    /**
     * Démarre la boucle principale du jeu
     */
    void start();

    /**
     * Termine la boucle principale et quitte le jeu
     */
    void quit();

    /**
     * Renvoie la vue actuelle du jeu
     */
    std::shared_ptr<View> getView();

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
     * Remet la vue par défaut (zoom 1:1, origine en haut à gauche)
     */
    void resetDefaultView();

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
};

#endif
