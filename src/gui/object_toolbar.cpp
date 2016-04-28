#include "resource_manager.hpp"
#include "objects/block.hpp"
#include "objects/player.hpp"
#include "objects/switch_block.hpp"
#include "objects/gravity_block.hpp"
#include "objects/kill_block.hpp"
#include "objects/finish_block.hpp"
#include "gui/object_toolbar.hpp"

const int ObjectToolbar::CREATORS_TABLE_WIDTH = 2;

ObjectToolbar::ObjectToolbar() {
    // création de la boîte de la barre d'outils
    toolbar_box = sfg::Box::Create(sfg::Box::Orientation::VERTICAL, 5.f);

    // mise en place de la liste des créateurs d'objets
    creators_group = sfg::RadioButtonGroup::Create();
    toolbar_box->PackEnd(sfg::Label::Create(L"Objets"));
    toolbar_box->PackEnd(sfg::Separator::Create());

    creators_table = sfg::Table::Create();
    creators_table_pos_x = creators_table_pos_y = 0;

    addCreator("block", std::bind(&ObjectToolbar::createBlock, this));
    addCreator("movable_block", std::bind(&ObjectToolbar::createMovableBlock, this));
    addCreator("player", std::bind(&ObjectToolbar::createPlayer, this));
    addCreator("switch_block", std::bind(&ObjectToolbar::createSwitchBlock, this));
    addCreator("finish_block", std::bind(&ObjectToolbar::createFinishBlock, this));
    addCreator("kill_block", std::bind(&ObjectToolbar::createKillBlock, this));

    addCreator("gravity_block_north", std::bind(
        &ObjectToolbar::createGravityBlock, this, Utility::Direction::NORTH
    ));

    addCreator("gravity_block_east", std::bind(
        &ObjectToolbar::createGravityBlock, this, Utility::Direction::EAST
    ));

    addCreator("gravity_block_west", std::bind(
        &ObjectToolbar::createGravityBlock, this, Utility::Direction::WEST
    ));

    addCreator("gravity_block_south", std::bind(
        &ObjectToolbar::createGravityBlock, this, Utility::Direction::SOUTH
    ));

    // attachement de la liste des créateurs à l'interface
    toolbar_box->PackEnd(creators_table);

    // on sélectionne le premier créateur par défaut
    creators.begin()->first->SetActive(true);

    // création de la fenêtre de la barre d'outils
    sfg::ScrolledWindow::Ptr scrolled_zone = sfg::ScrolledWindow::Create();
    toolbar_window = sfg::Window::Create(sfg::Window::Style::BACKGROUND);

    scrolled_zone->SetScrollbarPolicy(
        sfg::ScrolledWindow::ScrollbarPolicy::VERTICAL_AUTOMATIC |
        sfg::ScrolledWindow::ScrollbarPolicy::HORIZONTAL_NEVER
    );

    scrolled_zone->AddWithViewport(toolbar_box);
    toolbar_window->Add(scrolled_zone);

    // pour les styles
    toolbar_window->SetId("object_toolbar");
}

void ObjectToolbar::addCreator(std::string path, std::function<Object::Ptr()> creator) {
    // on crée un bouton d'objet correspondant au créateur donné
    ObjectButton::Ptr button = ObjectButton::Create(
        sfg::Image::Create(
            *ResourceManager::get().getImage("toolbar/" + path + ".tga")
        ), creators_group
    );

    creators[button] = creator;

    // on ajoute le bouton d'objet à la liste des créateurs
    creators_table->Attach(button, sf::Rect<sf::Uint32>(
        creators_table_pos_x, creators_table_pos_y, 1, 1
    ));

    creators_table_pos_x++;

    if (creators_table_pos_x >= ObjectToolbar::CREATORS_TABLE_WIDTH) {
        creators_table_pos_x = 0;
        creators_table_pos_y++;
    }
}

Object::Ptr ObjectToolbar::createBlock() {
    return Object::Ptr(new Block);
}

Object::Ptr ObjectToolbar::createMovableBlock() {
    Object::Ptr movable = Object::Ptr(new Block);
    movable->setMass(2.f);
    return movable;
}

Object::Ptr ObjectToolbar::createPlayer() {
    Object::Ptr player = Object::Ptr(new Player);
    player->setMass(1.f);
    return player;
}

Object::Ptr ObjectToolbar::createSwitchBlock() {
    return Object::Ptr(new SwitchBlock);
}

Object::Ptr ObjectToolbar::createFinishBlock() {
    return Object::Ptr(new FinishBlock);
}

Object::Ptr ObjectToolbar::createKillBlock() {
    return Object::Ptr(new KillBlock);
}

Object::Ptr ObjectToolbar::createGravityBlock(Utility::Direction direction) {
    GravityBlock::Ptr gravity_block = GravityBlock::Ptr(new GravityBlock);
    gravity_block->setGravityDirection(direction);
    return std::dynamic_pointer_cast<Object>(gravity_block);
}

Object::Ptr ObjectToolbar::createObject() {
    for (auto const &creator : creators) {
        // on recherche le bouton actif actuellement, si on
        // le trouve, on appelle le créateur associé
        if (creator.first->IsActive()) {
            return creator.second();
        }
    }

    // on n'a aucun bouton actif, on renvoie nullptr
    return nullptr;
}

sfg::Window::Ptr ObjectToolbar::getWindow() {
    return toolbar_window;
}

float ObjectToolbar::getWidth() {
    return toolbar_window->GetRequisition().x;
}
