#ifndef __PTF_GAME_HPP__
#define __PTF_GAME_HPP__

#include "level.hpp"

/**
 * La classe Editor permet l'édition de
 * niveaux du jeu
 */
class Editor : public Level {
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
