#include "util/widget_selector.hpp"
#include <utility>

const int PADDING = 8;

WidgetSelector::WidgetSelector(Manager& manager) : manager(manager), selected(NULL) {
    //
}

std::shared_ptr<SelectorCategory> WidgetSelector::addCategory(sf::String name) {
    auto cat = std::shared_ptr<SelectorCategory>(new SelectorCategory);
    cat->name = name;

    categories.push_back(cat);
    return cat;
}

std::shared_ptr<SelectorItem> SelectorCategory::addItem(sf::String name, sf::Texture& texture) {
    auto item = std::shared_ptr<SelectorItem>(new SelectorItem);
    item->sprite.setTexture(texture, true);
    float width = item->sprite.getLocalBounds().width;

    // mise à l'échelle si trop grand ou trop petit
    if (width < 20 || width > 28) {
        item->sprite.scale(24 / width, 24 / width);
        width = 24;
    }

    item->name = name;
    items.push_back(item);
    return item;
}

bool WidgetSelector::processEvent(const sf::Event& event) {
    if (event.type == sf::Event::MouseButtonPressed) {
        sf::Vector2f position(event.mouseButton.x, event.mouseButton.y);

        // clic gauche sur un item : on le sélectionne
        if (event.mouseButton.button == sf::Mouse::Left) {
            for (unsigned int i = 0; i < categories.size(); i++) {
                for (unsigned int j = 0; j < categories[i]->items.size(); j++) {
                    if (categories[i]->items[j]->sprite.getGlobalBounds().contains(position)) {
                        selected = categories[i]->items[j].get();
                        return true;
                    }
                }
            }

            // ne pas laisser traverser les clics sur le sélecteur
            if (background.getGlobalBounds().contains(position)) {
                return true;
            }
        }
    }

    return false;
}

void WidgetSelector::draw(sf::Vector2f position, sf::Vector2f size) {
    sf::RenderWindow& window = manager.getWindow();

    background.setSize(size);
    background.setPosition(position);
    window.draw(background);

    float total_y = PADDING;

    for (unsigned int i = 0; i < categories.size(); i++) {
        std::shared_ptr<SelectorCategory> category = categories[i];

        // affichage du label de la catégorie
        sf::Text category_label(
            category->name,
            manager.getResourceManager().getFont("raleway.ttf"), 12
        );

        category_label.setColor(sf::Color::Black);
        category_label.setPosition(position + sf::Vector2f(
            floor(size.x / 2 - category_label.getGlobalBounds().width / 2),
            total_y
        ));

        window.draw(category_label);

        for (unsigned int j = 0; j < category->items.size(); j++) {
            total_y += PADDING + 24;

            std::shared_ptr<SelectorItem> item = category->items[j];
            sf::Vector2f sprite_position = position + sf::Vector2f(
                size.x / 2 - item->sprite.getGlobalBounds().width / 2,
                total_y
            );

            // affichage du sprite de l'item
            item->sprite.setPosition(sprite_position);
            window.draw(item->sprite);
        }

        total_y += 64;
    }
}
