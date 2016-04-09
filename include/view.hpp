#ifndef __PTF_VIEW_HPP__
#define __PTF_VIEW_HPP__

#include <SFML/Graphics.hpp>

class Manager;
class Object;

/**
 * Classe abstraite pour les vues
 */
class View {
protected:
    Manager& manager;

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
};

#endif
