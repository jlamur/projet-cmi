#ifndef __PTF_STATE_HPP__
#define __PTF_STATE_HPP__

#include <SFML/Graphics.hpp>

class ResourceManager;
class Object;
class Manager;

/**
 * Classe abstraite pour les états de jeu
 */
class State {
private:
    Manager& manager;

public:
    State(Manager& manager);
    virtual ~State();

    /**
     * Traite l'événement donné
     */
    virtual void processEvent(const sf::Event& event) = 0;

    /**
     * Demande le dessin d'une frame
     */
    virtual void frame() = 0;

    /**
     * Récupère le gestionnaire
     */
    Manager& getManager();

    /**
     * Récupère le gestionnaire (version constante)
     */
    const Manager& getManager() const;

    /**
     * Récupère le gestionnaire de ressources
     */
    ResourceManager& getResourceManager();

    /**
     * Récupère le gestionnaire de ressources (version constante)
     */
    const ResourceManager& getResourceManager() const;

    /**
     * Récupère la fenêtre
     */
    sf::RenderWindow& getWindow();

    /**
     * Récupère la fenêtre (version constante)
     */
    const sf::RenderWindow& getWindow() const;
};

#endif
