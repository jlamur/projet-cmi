#include "manager.hpp"
#include "widget_toolbar.hpp"
#include "block.hpp"
#include "player.hpp"
#include "gravity_block.hpp"
#include "switch_block.hpp"
#include "finish_block.hpp"
#include "kill_block.hpp"
#include <utility>


/**
 * Définition des variables et fonctions globales internes
 * (accessibles uniquement dans ce fichier)
 */
namespace {
    const int PADDING = 8;
}

ToolbarCategory::Ptr WidgetToolbar::addCategory(sf::String name) {
    auto cat = ToolbarCategory::Ptr(new ToolbarCategory);

    cat->name = name;
    categories.push_back(cat);
    return cat;
}

ToolbarObject::Ptr ToolbarCategory::addObject(
    sf::Texture& texture,
    std::function<Object::Ptr(void)> create_object
) {
    auto object = std::shared_ptr<ToolbarObject>(new ToolbarObject);

    object->sprite.setTexture(texture, true);
    object->create_object = create_object;
    objects.push_back(object);
    return object;
}

Object::Ptr WidgetToolbar::createBlock() {
    return Object::Ptr(new Block);
}

Object::Ptr WidgetToolbar::createMovableBlock() {
    Object::Ptr movable = Object::Ptr(new Block);
    movable->setMass(2.f);
    return movable;
}

Object::Ptr WidgetToolbar::createPlayer() {
    Object::Ptr player = Object::Ptr(new Player);
    player->setMass(1.f);
    return player;
}

Object::Ptr WidgetToolbar::createSwitchBlock() {
    return Object::Ptr(new SwitchBlock);
}

Object::Ptr WidgetToolbar::createFinishBlock() {
    return Object::Ptr(new FinishBlock);
}

Object::Ptr WidgetToolbar::createKillBlock() {
    return Object::Ptr(new KillBlock);
}

Object::Ptr WidgetToolbar::createGravityBlock(GravityDirection direction) {
    GravityBlock::Ptr gravity_block = GravityBlock::Ptr(new GravityBlock);
    gravity_block->setGravityDirection(direction);
    return std::dynamic_pointer_cast<Object>(gravity_block);
}

Object::Ptr WidgetToolbar::createObject() {
    if (selected != nullptr) {
        return selected->create_object();
    }

    return nullptr;
}

WidgetToolbar::WidgetToolbar(Manager& manager) : manager(manager), selected(nullptr) {
    // catégorie des objets plaçables de base
    ResourceManager& resources = manager.getResourceManager();
    ToolbarCategory::Ptr block_cat = addCategory("BLOCS");

    selected = block_cat->addObject(
        resources.getTexture("toolbar_block.tga"),
        std::bind(&WidgetToolbar::createBlock, this)
    );

    block_cat->addObject(
        resources.getTexture("toolbar_movable_block.tga"),
        std::bind(&WidgetToolbar::createMovableBlock, this)
    );

    // catégorie des blocs contrôlant les joueurs
    ToolbarCategory::Ptr player_cat = addCategory("JOUEURS");

    player_cat->addObject(
        resources.getTexture("toolbar_player.tga"),
        std::bind(&WidgetToolbar::createPlayer, this)
    );

    player_cat->addObject(
        resources.getTexture("toolbar_switch_block.tga"),
        std::bind(&WidgetToolbar::createSwitchBlock, this)
    );

    player_cat->addObject(
        resources.getTexture("toolbar_kill_block.tga"),
        std::bind(&WidgetToolbar::createKillBlock, this)
    );

    player_cat->addObject(
        resources.getTexture("toolbar_finish_block.tga"),
        std::bind(&WidgetToolbar::createFinishBlock, this)
    );

    // catégorie des blocs changeant la gravité
    ToolbarCategory::Ptr gravity_cat = addCategory(L"GRAVITÉ");

    gravity_cat->addObject(
        resources.getTexture("toolbar_gravity_block_north.tga"),
        std::bind(&WidgetToolbar::createGravityBlock, this, GravityDirection::NORTH)
    );

    gravity_cat->addObject(
        resources.getTexture("toolbar_gravity_block_east.tga"),
        std::bind(&WidgetToolbar::createGravityBlock, this, GravityDirection::EAST)
    );

    gravity_cat->addObject(
        resources.getTexture("toolbar_gravity_block_south.tga"),
        std::bind(&WidgetToolbar::createGravityBlock, this, GravityDirection::SOUTH)
    );

    gravity_cat->addObject(
        resources.getTexture("toolbar_gravity_block_west.tga"),
        std::bind(&WidgetToolbar::createGravityBlock, this, GravityDirection::WEST)
    );
}

bool WidgetToolbar::processEvent(const sf::Event& event) {
    if (event.type == sf::Event::MouseButtonPressed) {
        sf::Vector2f position(event.mouseButton.x, event.mouseButton.y);

        // clic gauche sur un item : on le sélectionne
        if (event.mouseButton.button == sf::Mouse::Left) {
            for (unsigned int i = 0; i < categories.size(); i++) {
                for (unsigned int j = 0; j < categories[i]->objects.size(); j++) {
                    if (categories[i]->objects[j]->sprite.getGlobalBounds().contains(position)) {
                        selected = categories[i]->objects[j];
                        return true;
                    }
                }
            }

            // ne pas laisser traverser les clics sur la barre d'outils
            if (background.getGlobalBounds().contains(position)) {
                return true;
            }
        }
    }

    return false;
}

void WidgetToolbar::draw(sf::Vector2f position, sf::Vector2f size) {
    sf::RenderWindow& window = manager.getWindow();

    background.setSize(size);
    background.setPosition(position);
    window.draw(background);

    float total_y = PADDING;

    for (unsigned int i = 0; i < categories.size(); i++) {
        std::shared_ptr<ToolbarCategory> category = categories[i];

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

        for (unsigned int j = 0; j < category->objects.size(); j++) {
            total_y += PADDING + 32;

            std::shared_ptr<ToolbarObject> object = category->objects[j];
            sf::Vector2f sprite_position = position + sf::Vector2f(
                size.x / 2 - object->sprite.getGlobalBounds().width / 2,
                total_y
            );

            if (selected == object) {
                sf::RectangleShape selection_rectangle(sf::Vector2f(32, 32));
                selection_rectangle.setPosition(sprite_position + sf::Vector2f(-4, -4));
                selection_rectangle.setFillColor(sf::Color(0, 0, 0, 20));
                window.draw(selection_rectangle);
            }

            // affichage du sprite de l'objet
            object->sprite.setPosition(sprite_position);
            window.draw(object->sprite);
        }

        total_y += 64;
    }
}
