#ifndef __PTF_UTIL_WIDGET_SELECTOR_HPP__
#define __PTF_UTIL_WIDGET_SELECTOR_HPP__

#include <SFML/Graphics.hpp>
#include <functional>
#include "manager.hpp"

/**
 * Un choix possible dans une catégorie
 */
struct SelectorItem {
    sf::String name;

    sf::Sprite sprite;
    sf::Sprite select_sprite;
};

/**
 * Représente une catégorie de choix possibles
 */
struct SelectorCategory {
    sf::String name;
    std::vector<std::shared_ptr<SelectorItem>> items;

    /**
     * Ajoute un nouvel item dans cette catégorie
     */
    void addItem(sf::String name, sf::Texture& texture, sf::Texture& select_texture);
};

/**
 * Affiche une liste de sélection
 */
class WidgetSelector {
private:
    Manager& manager;

    sf::RectangleShape background;
    std::vector<std::shared_ptr<SelectorCategory>> categories;
    SelectorItem* selected;

public:
    WidgetSelector(Manager& manager);

    /**
     * Ajoute une nouvelle catégorie de choix
     */
    std::shared_ptr<SelectorCategory> addCategory(sf::String name);

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
