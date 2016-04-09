#ifndef __PTF_STATE_HPP__
#define __PTF_STATE_HPP__

#include <SFML/Graphics.hpp>

class ResourceManager;
class Manager;
class Object;

/**
 * Classe abstraite pour les états de jeu
 */
class State {
private:
    Manager& manager;

protected:
    /**
     * Traite l'événement donné
     */
    virtual void processEvent(const sf::Event& event) = 0;

public:
    State(Manager& manager);
    virtual ~State();

    /**
     * Appelé par le manager lorsque l'état commence à
     * être utilisé
     */
    virtual void begin() = 0;

    /**
     * Demande le passage à la frame suivante sur cet état
     */
    virtual void frame(const std::vector<sf::Event>& events);

    /**
     * Récupère le gestionnaire
     */
    Manager& getManager();

    /**
     * Récupère le gestionnaire de ressources
     */
    ResourceManager& getResourceManager();

    /**
     * Récupère la fenêtre
     */
    sf::RenderWindow& getWindow();

    /**
     * Récupère si une touche est pressée
     */
    bool isKeyPressed(sf::Keyboard::Key key) const;
};

#endif
