#pragma once
#include "../gui/object_toolbar.hpp"
#include "level.hpp"
#include "game.hpp"

/**
 * La classe Editor permet l'édition de
 * niveaux du jeu
 */
class Editor : public Level {
private:
    enum class DragMode {NONE, PLACE, SELECT_RECT, SELECT_BULK, CONTROL_POINT, REMOVE};
    enum class SelectionMode {REPLACE, FLIP, ADD};

    std::vector<Object::Ptr> selection;
    std::vector<sf::CircleShape> control_points_circles;

    sf::Vector2i drag_start;
    sf::Vector2i drag_end;
    sf::Vector2f *drag_control_point;

    DragMode drag_mode;
    ObjectToolbar object_toolbar;

    /**
     * Renvoie l'objet pointé à la position donnée
     * ou nullptr si aucun
     */
    Object::Ptr getObject(sf::Vector2f position);

    /**
     * Renvoie le point de contrôle à la position donnée
     * ou nullptr si aucun
     */
    sf::Vector2f* getControlPoint(sf::Vector2f position);

    /**
     * Ajoute un objet du type actuel à la position donnée
     */
    Object::Ptr addObject(sf::Vector2f position);

    /**
     * Supprime l'objet à la position donnée ou passé par pointeur
     */
    void removeObject(Object::Ptr object);
    void removeObject(sf::Vector2f position);

    /**
     * Ajoute l'objet/les objets donné(s) (par position, entre deux
     * positions ou par pointeur) à la sélection
     *
     * - REPLACE : remplace toute sélection précédente
     * - FLIP : sélectionne l'élément s'il ne l'est pas, sinon le désélectionne
     * - ADD : rajoute à la sélection courante
     */
    void select(Object::Ptr object, SelectionMode mode);
    void select(sf::Vector2f position, SelectionMode mode);
    void select(sf::Vector2f top_left, sf::Vector2f bottom_right);

    /**
     * Vide la sélection
     */
    void clearSelection();

    /**
     * Sélectionne tout
     */
    void selectAll();

    /**
     * Lance le test du niveau
     */
    void test();

protected:
    /**
     * Demande le dessin d'une frame
     */
    void frame() override;

    /**
     * Dessine tous les objets, le fond et
     * l'interface de l'éditeur
     */
    void draw() override;

public:
    Editor(Manager& manager);
    virtual ~Editor();

    /**
     * Active l'état
     */
    void enable() override;

    /**
     * Traite l'événement donné
     */
    void processEvent(const sf::Event& event) override;
};
