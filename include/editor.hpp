#ifndef __PTF_EDITOR_HPP__
#define __PTF_EDITOR_HPP__

#include <unordered_map>
#include "level.hpp"
#include "widget_timer.hpp"
#include "widget_selector.hpp"

enum class DragMode {NONE, PLACE, SELECT_RECT, SELECT_BULK, CONTROL_POINT, REMOVE};
enum class SelectionMode {REPLACE, FLIP, ADD};

/**
 * La classe Editor permet l'édition de
 * niveaux du jeu
 */
class Editor : public Level {
private:
    std::vector<Object::Ptr> selection;
    std::vector<sf::CircleShape> control_points_circles;

    sf::Vector2i drag_start;
    sf::Vector2i drag_end;
    sf::Vector2f *drag_control_point;

    DragMode drag_mode;

    WidgetTimer widget_timer;
    WidgetSelector widget_selector;

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
     * Supprime le point de contrôle passé par pointeur
     */
    void removeControlPoint(sf::Vector2f* control_point);

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
     * Dessine tous les objets, le fond et
     * l'interface de l'éditeur
     */
    virtual void draw();

public:
    Editor(Manager& manager);
    virtual ~Editor();

    /**
     * Appelé par le manager lorsque l'état est utilisé
     */
    virtual void begin();

    /**
     * Traite l'événement donné
     */
    virtual void processEvent(const sf::Event& event);

    /**
     * Demande le dessin d'une frame
     */
    virtual void frame();
};

#endif
