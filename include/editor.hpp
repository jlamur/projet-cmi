#ifndef __PTF_EDITOR_HPP__
#define __PTF_EDITOR_HPP__

#include <unordered_map>
#include "level.hpp"
#include "util/widget_timer.hpp"

enum class DragMode {NONE, PLACE, SELECT_RECT, SELECT_BULK, REMOVE};
enum class SelectionMode {REPLACE, FLIP, ADD};

/**
 * La classe Editor permet l'édition de
 * niveaux du jeu
 */
class Editor : public Level {
private:
    std::vector<ObjectPtr> selection;
    sf::Vector2f drag_start;
    sf::Vector2f drag_end;
    DragMode drag_mode;
    WidgetTimer widget_timer;

    /**
     * Renvoie l'objet pointé à la position donnée
     * ou nullptr si aucun
     */
    ObjectPtr getObject(sf::Vector2f position);

    /**
     * Ajoute un objet du type actuel à la position donnée
     */
    ObjectPtr addObject(sf::Vector2f position);

    /**
     * Supprime l'objet à la position donnée ou passé par pointeur
     */
    void removeObject(ObjectPtr object);
    void removeObject(sf::Vector2f position);

    /**
     * Ajoute l'objet/les objets donné(s) (par position, entre deux
     * positions ou par pointeur) à la sélection
     *
     * - REPLACE : remplace toute sélection précédente
     * - FLIP : sélectionne l'élément s'il ne l'est pas, sinon le désélectionne
     * - ADD : rajoute à la sélection courante
     */
    void select(ObjectPtr object, SelectionMode mode);
    void select(sf::Vector2f position, SelectionMode mode);
    void select(sf::Vector2f top_left, sf::Vector2f bottom_right);

    /**
     * Lance le test du niveau
     */
    void test();

    /**
     * Traite l'événement et renvoie true si on s'en est servi
     */
    bool processEvent(const sf::Event& event);

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
