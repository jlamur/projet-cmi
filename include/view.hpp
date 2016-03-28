#ifndef __PTF_VIEW_HPP__
#define __PTF_VIEW_HPP__

#include <SFML/Graphics.hpp>

class Manager;
class Object;

/**
 * Classe abstraite pour les vues
 */
class View {
public:
    /**
     * Demande le passage à la frame suivante sur
     * cette vue
     */
    virtual void frame(Manager& manager) = 0;

    virtual void addObject(Object& object) = 0;
};

#endif
