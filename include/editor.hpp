#ifndef __PTF_EDITOR_HPP__
#define __PTF_EDITOR_HPP__

#include <unordered_map>
#include "level.hpp"
#include "util/widget_timer.hpp"

/**
 * La classe Editor permet l'édition de
 * niveaux du jeu
 */
class Editor : public Level {
private:
    std::vector<ObjectPtr> selection;
    WidgetTimer widget_timer;

    /**
     * Ajoute un objet du type actuel à la position donnée
     */
    void addObject(sf::Vector2f position);

    /**
     * Supprime les objets passant par la position donnée
     */
    void removeObject(sf::Vector2f position);

    /**
     * Met à jour la sélection avec la position donnée :
     * - si la position correspond à un objet, si cet objet n'est
     *   pas sélectionné on le sélectionne, sinon on le désélectionne
     * - si la sélection est modifiée, renvoie true, sinon false
     */
    bool updateSelection(sf::Vector2f position);

    /**
     * Lance le test du niveau
     */
    void testLevel();

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
