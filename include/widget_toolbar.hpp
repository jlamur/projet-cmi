#ifndef __PTF_UTIL_WIDGET_TOOLBAR_HPP__
#define __PTF_UTIL_WIDGET_TOOLBAR_HPP__

#include <SFML/Graphics.hpp>
#include <functional>
#include "object.hpp"
#include "level.hpp"
#include "manager.hpp"

/**
 * Représente un objet plaçable depuis la barre d'outils
 */
struct ToolbarObject {
    typedef std::shared_ptr<ToolbarObject> Ptr;

    // image représentant cet objet visuellement
    sf::Sprite sprite;

    // fonction appelée pour instancier un objet de ce type
    std::function<Object::Ptr(void)> create_object;
};

/**
 * Représente une catégorie d'objets plaçables dans la barre
 */
struct ToolbarCategory {
    typedef std::shared_ptr<ToolbarCategory> Ptr;

    // nom de la catégorie
    sf::String name;

    // liste des objets dans la catégorie
    std::vector<ToolbarObject::Ptr> objects;

    /**
     * Ajoute un nouvel objet dans cette catégorie
     */
    ToolbarObject::Ptr addObject(
        sf::Texture& texture,
        std::function<Object::Ptr(void)> create_object
    );
};

/**
 * Affiche une barre d'outils pour l'éditeur
 */
class WidgetToolbar {
private:
    Manager& manager;

    sf::RectangleShape background;
    std::vector<ToolbarCategory::Ptr> categories;
    ToolbarObject::Ptr selected;

    /**
     * Crée un objet de type Block
     */
    Object::Ptr createBlock();

    /**
     * Crée un objet de type Block déplaçable
     */
    Object::Ptr createMovableBlock();

    /**
     * Crée un objet de type Player
     */
    Object::Ptr createPlayer();

    /**
     * Crée un objet de type GravityBlock
     */
    Object::Ptr createGravityBlock(GravityDirection direction);

public:
    WidgetToolbar(Manager& manager);

    /**
     * Ajoute une nouvelle catégorie de choix
     */
    ToolbarCategory::Ptr addCategory(sf::String name);

    /**
     * Génère un objet du type actuellement sélectionné et le retourne
     */
    Object::Ptr createObject();

    /**
     * Traite l'événement et renvoie true si l'on s'en est servi
     */
    bool processEvent(const sf::Event& event);

    /**
     * Dessine le widget à la position donnée et avec la taille donnée
     */
    void draw(sf::Vector2f position, sf::Vector2f size);
};

#endif
