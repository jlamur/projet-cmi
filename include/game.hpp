#ifndef __PTF_GAME_HPP__
#define __PTF_GAME_HPP__

#include <SFML/Audio.hpp>
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
    sf::View view;
    sf::Music music;

    float accumulator;
    std::vector<Object*> objects;

    /**
     * Met à jour les objets du jeu pour
     * qu'ils s'adaptent au nouvel état
     */
    void update(const Manager& manager);

    /**
     * Dessine la scène du jeu couche par couche
     */
    void draw(Manager& manager);

public:
    Game();
    virtual ~Game();

    /**
     * Charge un niveau de jeu depuis le fichier donné
     */
    void load();

    /**
     * Sauvegarde la configuration actuelle comme un niveau
     */
    void save();

    /**
     * Libère les ressources du niveau
     */
    void clear();

    /**
     * Demande le passage à la frame suivante sur
     * cette vue
     */
    void frame(Manager& manager);

    /**
     * Ajoute un nouvel objet à la liste des objets du jeu
     */
    void addObject(Object& object);
};

#endif
