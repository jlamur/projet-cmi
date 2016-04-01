#ifndef __PTF_GAME_HPP__
#define __PTF_GAME_HPP__

#include <SFML/Audio.hpp>
#include <fstream>
#include <vector>
#include "view.hpp"
#include "object.hpp"
#include "manager.hpp"
#include "resource_manager.hpp"

/**
 * La classe Game gère l'affichage et les objets
 * d'une partie de jeu
 */
class Game : public View {
private:
    std::string level_name;
    sf::Sprite background;

    float accumulator;

    std::vector<ObjectPtr> objects;
    std::vector<std::pair<float, float>> level_zone;

    /**
     * Met à jour les objets du jeu pour
     * qu'ils s'adaptent au nouvel état
     */
    void update();

    /**
     * Dessine la scène du jeu couche par couche
     */
    void draw();

public:
    Game(Manager& manager);
    virtual ~Game();

    /**
     * Charge un niveau de jeu depuis le fichier donné
     */
    void load(std::ifstream& file);

    /**
     * Sauvegarde la configuration actuelle comme un niveau
     */
    void save();

    /**
     * Demande le passage à la frame suivante sur
     * cette vue
     */
    void frame();
};

#endif
