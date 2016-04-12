#ifndef __SKIZZLE_UTIL_WIDGET_BUTTON_HPP__
#define __SKIZZLE_UTIL_WIDGET_BUTTON_HPP__

#include <SFML/Graphics.hpp>
#include <functional>

class Manager;

/**
 * Affiche un bouton pouvant être cliqué
 */
class WidgetButton {
private:
    Manager& manager;
    std::function<void(void)> click_cb;

    unsigned int shape;
    sf::RectangleShape button;
    sf::VertexArray button_shape;

public:
    static const unsigned int ARROW_UP;
    static const unsigned int ARROW_DOWN;

    WidgetButton(
        Manager& manager, std::function<void(void)> click_cb,
        sf::Vector2f size, unsigned int shape
    );

    /**
     * Process l'événement et renvoie true si
     * on s'en est servi
     */
    bool processEvent(const sf::Event& event);

    /**
     * Dessine le widget à la position (haut-gauche) donnée
     */
    void draw(sf::Vector2f position);
};

#endif
