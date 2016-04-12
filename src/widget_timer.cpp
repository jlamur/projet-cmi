#include "manager.hpp"
#include "widget_timer.hpp"

WidgetTimer::WidgetTimer(Manager& manager, bool can_change, std::function<void(int)> time_left_cb) :
    manager(manager), can_change(can_change), time_left_cb(time_left_cb),
    timer_zone(sf::Vector2f(100, 32)),
    timer_up(manager, std::bind(&WidgetTimer::addTime, this), sf::Vector2f(30, 16), WidgetButton::ARROW_UP),
    timer_down(manager, std::bind(&WidgetTimer::subtractTime, this), sf::Vector2f(30, 16), WidgetButton::ARROW_DOWN) {

    // initialisation des formes
    timer_seconds_text.setFont(manager.getResourceManager().getFont("monoid.ttf"));
    timer_seconds_text.setCharacterSize(18);
    timer_seconds_text.setColor(sf::Color::Black);

    timer_sep_text.setString(":");
    timer_sep_text.setFont(manager.getResourceManager().getFont("monoid.ttf"));
    timer_sep_text.setCharacterSize(18);
    timer_sep_text.setColor(sf::Color::Black);

    timer_minutes_text.setFont(manager.getResourceManager().getFont("monoid.ttf"));
    timer_minutes_text.setCharacterSize(18);
    timer_minutes_text.setColor(sf::Color::Black);
}

bool WidgetTimer::processEvent(const sf::Event& event) {
    // si le timer n'est pas modifiable, pas d'évent à gérer
    if (!can_change) {
        return false;
    }

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
            time_left_cb(time_left + round(event.mouseWheelScroll.delta * 10));
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
    sf::String minutes = std::to_string(time_left / 60);
    sf::String seconds = std::to_string(time_left % 60);

    // ajout d'un zéro devant les secondes si nécessaire
    if (minutes.getSize() == 1) {
        minutes = "0" + minutes;
    }

    if (seconds.getSize() == 1) {
        seconds = "0" + seconds;
    }

    timer_minutes_text.setString(minutes);
    timer_seconds_text.setString(seconds);

    float base_x = can_change ? 30 : 45;
    timer_sep_text.setPosition(position + sf::Vector2f(base_x, 6));
    timer_seconds_text.setPosition(position + sf::Vector2f(base_x + 8, 6));
    timer_minutes_text.setPosition(position + sf::Vector2f(
        base_x - 3 - floor(timer_minutes_text.getGlobalBounds().width), 6
    ));

    window.draw(timer_sep_text);
    window.draw(timer_seconds_text);
    window.draw(timer_minutes_text);

    // interface de modification du temps
    if (can_change) {
        timer_up.draw(position + sf::Vector2f(70, 0));
        timer_down.draw(position + sf::Vector2f(70, 16));
    }
}

void WidgetTimer::addTime() {
    time_left_cb(time_left + 1);
}

void WidgetTimer::subtractTime() {
    time_left_cb(time_left - 1);
}

void WidgetTimer::setTimeLeft(int set_time_left) {
    time_left = set_time_left;
}
