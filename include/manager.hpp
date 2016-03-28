#ifndef __PTF_MANAGER_HPP__
#define __PTF_MANAGER_HPP__

#include "resource_manager.hpp"
#include "view.hpp"

/**
 * Gestionnaire principal de toutes les vues et
 * ressources du jeu
 */
class Manager {
private:
    sf::RenderWindow window;
    sf::Clock clock;

    float elapsed_time;
    ResourceManager resource_manager;
    std::array<bool, sf::Keyboard::KeyCount> keys;

public:
    // FIXME: devrait être privé
    View* view;

    Manager();

    /**
     * Démarre la boucle principale du jeu
     */
    void start();

    /**
     * Renvoie la fenêtre actuellement utilisée pour le dessin
     */
    sf::RenderWindow& getWindow();

    /**
     * Renvoie le temps écoulé entre la frame précédente
     * et la frame actuelle
     */
    float getElapsedTime() const;

    /**
     * Renvoie le gestionnaire de ressources
     */
    ResourceManager& getResourceManager();

    /**
     * Renvoie un booléen attestant de l'appui sur la
     * touche donnée ou non
     */
    bool isKeyPressed(sf::Keyboard::Key key) const;
};

#endif
