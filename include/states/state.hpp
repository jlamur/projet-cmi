#pragma once
#include <SFML/Graphics.hpp>

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
     * Active l'état
     */
    virtual void enable() = 0;

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
};
