#ifndef __PTF_UTIL_WIDGET_TIMER_HPP__
#define __PTF_UTIL_WIDGET_TIMER_HPP__

#include <SFML/Graphics.hpp>
#include <functional>
#include "util/widget_button.hpp"
#include "manager.hpp"

/**
 * Affiche le compteur de temps pouvant (ou non)
 * être modifié
 */
class WidgetTimer {
private:
    Manager& manager;
    std::function<void(int)> time_left_cb;
    int time_left;

    sf::RectangleShape timer_zone;
    sf::Text timer_text;

    WidgetButton timer_up;
    WidgetButton timer_down;

public:
    WidgetTimer(Manager& manager, std::function<void(int)> time_left_cb);

    /**
     * Process l'événement et renvoie true si
     * on s'en est servi
     */
    bool processEvent(const sf::Event& event);

    /**
     * Dessine le widget à la position (haut-gauche) donnée
     */
    void draw(sf::Vector2f position);

    /**
     * Augmente le temps de 5 secondes
     */
    void addTime();

    /**
     * Diminue le temps de 5 secondes
     */
    void subtractTime();

    /**
     * Modifie le temps restant
     */
    void setTimeLeft(int set_time_left);
};

#endif
