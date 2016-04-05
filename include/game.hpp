#ifndef __PTF_GAME_HPP__
#define __PTF_GAME_HPP__

#include "level.hpp"

/**
 * La classe Game gère l'affichage et les objets
 * d'une partie de jeu
 */
class Game : public Level {
private:
    sf::Time next_frame_time;

    /**
     * Met à jour les objets du jeu pour
     * qu'ils s'adaptent au nouvel état
     */
    void update();

public:
    Game(Manager& manager);
    virtual ~Game();

    /**
     * Charge un niveau de jeu depuis le fichier donné
     */
    virtual void load(std::ifstream& file);

    /**
     * Demande le passage à la frame suivante sur
     * cette vue
     */
    void frame();
};

#endif
