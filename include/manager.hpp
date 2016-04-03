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

    sf::Time elapsed_time;
    ResourceManager resource_manager;
    std::array<bool, sf::Keyboard::KeyCount> keys;

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
     * Renvoie le temps écoulé entre la frame précédente
     * et la frame actuelle
     */
    sf::Time getElapsedTime() const;

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
