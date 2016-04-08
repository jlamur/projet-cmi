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

public:
    View(Manager& manager);
    virtual ~View();

    /**
     * Demande le passage à la frame suivante sur cette vue
     */
    virtual void frame() = 0;
};

#endif
