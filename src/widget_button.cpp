#include "widget_button.hpp"

const unsigned int WidgetButton::ARROW_UP = 0;
const unsigned int WidgetButton::ARROW_DOWN = 1;

const sf::Color ARROW_COLOR = sf::Color(33, 33, 33);
const sf::Color NORMAL_COLOR = sf::Color(230, 230, 230);
const sf::Color HOVER_COLOR = sf::Color(220, 220, 220);
const sf::Color ACTIVE_COLOR = sf::Color(190, 190, 190);

WidgetButton::WidgetButton(
    Manager& manager, std::function<void(void)> click_cb,
    sf::Vector2f size, unsigned int shape
) : manager(manager), click_cb(click_cb), shape(shape), button(size) {
    if (shape == WidgetButton::ARROW_UP || shape == WidgetButton::ARROW_DOWN) {
        button_shape.setPrimitiveType(sf::Triangles);
        button_shape.resize(3);

        button_shape[0].color = ARROW_COLOR;
        button_shape[1].color = ARROW_COLOR;
        button_shape[2].color = ARROW_COLOR;
    }
}

bool WidgetButton::processEvent(const sf::Event& event) {
    if (event.type == sf::Event::MouseButtonPressed) {
        sf::Vector2f position(event.mouseButton.x, event.mouseButton.y);

        if (event.mouseButton.button == sf::Mouse::Left) {
            // clic gauche sur le bouton : appel de la callback
            if (button.getGlobalBounds().contains(position)) {
                click_cb();
                return true;
            }
        }
    }

    return false;
}

void WidgetButton::draw(sf::Vector2f position) {
    sf::RenderWindow& window = manager.getWindow();

    // positionnement du bouton
    button.setPosition(position);
    sf::FloatRect box = button.getGlobalBounds();
    sf::Vector2f center(box.left + box.width / 2, box.top + box.height / 2);

    if (shape == WidgetButton::ARROW_UP) {
        button_shape[0].position = center + sf::Vector2f(-5, 2);
        button_shape[1].position = center + sf::Vector2f(5, 2);
        button_shape[2].position = center + sf::Vector2f(0, -2);
    }

    if (shape == WidgetButton::ARROW_DOWN) {
        button_shape[0].position = center + sf::Vector2f(-5, -2);
        button_shape[1].position = center + sf::Vector2f(5, -2);
        button_shape[2].position = center + sf::Vector2f(0, 2);
    }

    // coloration des boutons si enfoncement
    sf::Vector2f mouse_position = (sf::Vector2f) sf::Mouse::getPosition(window);
    button.setFillColor(NORMAL_COLOR);

    if (button.getGlobalBounds().contains(mouse_position)) {
        if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
            button.setFillColor(ACTIVE_COLOR);
        } else {
            button.setFillColor(HOVER_COLOR);
        }
    }

    window.draw(button);
    window.draw(button_shape);
}
