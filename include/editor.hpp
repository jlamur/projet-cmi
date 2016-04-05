#ifndef __PTF_GAME_HPP__
#define __PTF_GAME_HPP__

#include "level.hpp"

/**
 * La classe Editor permet l'édition de
 * niveaux du jeu
 */
class Editor : public Level {
private:
    ObjectPtr selected_object;

protected:
    /**
     * Dessine tous les objets, le fond et
     * l'interface de l'éditeur
     */
    virtual void draw();

public:
    Editor(Manager& manager);
    virtual ~Editor();

    /**
     * Demande le passage à la frame suivante sur
     * cette vue
     */
    void frame();
};

#endif
