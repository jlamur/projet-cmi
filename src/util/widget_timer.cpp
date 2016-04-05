#include <cmath>
#include "util/widget_timer.hpp"

/**
 * Formattage du temps en m:ss
 */
std::string formatTime(int time) {
    std::string minutes = std::to_string(time / 60);
    std::string seconds = std::to_string(time % 60);

    // ajout d'un zéro devant les secondes si nécessaire
    if (seconds.size() == 1) {
        seconds = "0" + seconds;
    }

    return minutes + ":" + seconds;
}

WidgetTimer::WidgetTimer(Manager& manager, std::function<void(int)> time_left_cb) :
    manager(manager), time_left_cb(time_left_cb), timer_zone(sf::Vector2f(100, 32)),
    timer_up(manager, std::bind(&WidgetTimer::addTime, this), sf::Vector2f(30, 16), WidgetButton::ARROW_UP),
    timer_down(manager, std::bind(&WidgetTimer::subtractTime, this), sf::Vector2f(30, 16), WidgetButton::ARROW_DOWN) {

    // initialisation des formes
    timer_text.setFont(manager.getResourceManager().getFont("main_font.ttf"));
    timer_text.setCharacterSize(24);
    timer_text.setColor(sf::Color::Black);
}

bool WidgetTimer::processEvent(const sf::Event& event) {
    // gestion des boutons
    if (timer_up.processEvent(event)) {
        return true;
    }

    if (timer_down.processEvent(event)) {
        return true;
    }

    if (event.type == sf::Event::MouseButtonPressed) {
        sf::Vector2f position(event.mouseButton.x, event.mouseButton.y);

        // clic dans le widget : ne rien faire, mais empêcher le traversement
        if (timer_zone.getGlobalBounds().contains(position)) {
            return true;
        }
    }

    if (event.type == sf::Event::MouseWheelScrolled && event.mouseWheelScroll.wheel == sf::Mouse::VerticalWheel) {
        // scroll sur le timer : modification du temps alloué au niveau
        sf::Vector2f position(event.mouseWheelScroll.x, event.mouseWheelScroll.y);

        if (timer_zone.getGlobalBounds().contains(position)) {
            time_left_cb(time_left + round(event.mouseWheelScroll.delta));
            return true;
        }
    }

    return false;
}

void WidgetTimer::draw(sf::Vector2f position) {
    sf::RenderWindow& window = manager.getWindow();

    // zone de fond du timer
    timer_zone.setPosition(position);
    window.draw(timer_zone);

    // affichage du temps du niveau
    timer_text.setString(formatTime(time_left));
    timer_text.setPosition(position + sf::Vector2f(60 - round(timer_text.getGlobalBounds().width), 0));
    window.draw(timer_text);

    // affichage des boutons
    timer_up.draw(position + sf::Vector2f(70, 0));
    timer_down.draw(position + sf::Vector2f(70, 16));
}

void WidgetTimer::addTime() {
    time_left_cb(time_left + 5);
}

void WidgetTimer::subtractTime() {
    time_left_cb(time_left - 5);
}

void WidgetTimer::setTimeLeft(int set_time_left) {
    time_left = set_time_left;
}
