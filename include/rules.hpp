#ifndef __PTF_RULES_HPP__
#define __PTF_RULES_HPP__

#include "state.hpp"

class Manager;

/**
 * La classe Rules affiche les règles du jeu
 */
class Rules : public State {
private:
    sf::Sprite background;

public:
    Rules(Manager& manager);
    virtual ~Rules();

    /**
     * Active l'état
     */
    virtual void enable();

    /**
     * Traite l'événement donné
     */
    virtual void processEvent(const sf::Event& event);

    /**
     * Demande le dessin d'une frame
     */
    virtual void frame();
};

#endif
