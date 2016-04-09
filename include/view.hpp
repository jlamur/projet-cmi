#ifndef __PTF_VIEW_HPP__
#define __PTF_VIEW_HPP__

#include <SFML/Graphics.hpp>

class ResourceManager;
class Manager;
class Object;

/**
 * Classe abstraite pour les vues
 */
class View {
private:
    Manager& manager;

protected:
    /**
     * Traite l'événement donné
     */
    virtual void processEvent(const sf::Event& event) = 0;

public:
    View(Manager& manager);
    virtual ~View();

    /**
     * Appelé par le manager lorsque la vue commence à
     * être utilisée
     */
    virtual void begin() = 0;

    /**
     * Demande le passage à la frame suivante sur cette vue
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
