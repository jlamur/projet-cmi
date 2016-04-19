#include "resource_manager.hpp"
#include "editor.hpp"
#include "toolbar.hpp"
#include "block.hpp"
#include "player.hpp"
#include "switch_block.hpp"
#include "gravity_block.hpp"
#include "kill_block.hpp"
#include "finish_block.hpp"

Toolbar::Toolbar(Editor& editor) : editor(editor) {
    // création de la boîte de la barre d'outils
    toolbar_box = sfg::Box::Create(sfg::Box::Orientation::VERTICAL, 5.f);
    objects_group = sfg::RadioButtonGroup::Create();

    // catégorie des blocs de base
    toolbar_box->PackEnd(sfg::Label::Create(L"BLOCS"));
    addCreator(L"Bloc normal", std::bind(&Toolbar::createBlock, this));
    addCreator(L"Caisse", std::bind(&Toolbar::createMovableBlock, this));

    // catégorie des blocs contrôlant les joueurs
    toolbar_box->PackEnd(sfg::Label::Create(L"JOUEURS"));
    addCreator(L"Joueur", std::bind(&Toolbar::createPlayer, this));
    addCreator(L"Bloc changeur", std::bind(&Toolbar::createSwitchBlock, this));
    addCreator(L"Bloc de fin", std::bind(&Toolbar::createFinishBlock, this));
    addCreator(L"Bloc tueur", std::bind(&Toolbar::createKillBlock, this));

    // catégorie des blocs changeant la gravité
    toolbar_box->PackEnd(sfg::Label::Create(L"GRAVITÉ"));
    addCreator(L"Bloc de gravité nord", std::bind(
        &Toolbar::createGravityBlock, this, GravityDirection::NORTH
    ));

    addCreator(L"Bloc de gravité est", std::bind(
        &Toolbar::createGravityBlock, this, GravityDirection::EAST
    ));

    addCreator(L"Bloc de gravité sud", std::bind(
        &Toolbar::createGravityBlock, this, GravityDirection::SOUTH
    ));

    addCreator(L"Bloc de gravité ouest", std::bind(
        &Toolbar::createGravityBlock, this, GravityDirection::WEST
    ));

    // création de la fenêtre de la barre d'outils
    scrolled_zone = sfg::ScrolledWindow::Create();
    scrolled_zone->SetScrollbarPolicy(
        sfg::ScrolledWindow::ScrollbarPolicy::VERTICAL_AUTOMATIC |
        sfg::ScrolledWindow::ScrollbarPolicy::HORIZONTAL_NEVER
    );
    
    scrolled_zone->AddWithViewport(toolbar_box);

	toolbar_window = sfg::Window::Create(sfg::Window::Style::BACKGROUND);
    toolbar_window->Add(scrolled_zone);
    toolbar_window->SetPosition(sf::Vector2f(0, 0));
}

void Toolbar::addCreator(sf::String label, std::function<Object::Ptr()> creator) {
    sfg::RadioButton::Ptr radio = sfg::RadioButton::Create(label, objects_group);
    creators[radio] = creator;
    toolbar_box->PackEnd(radio);
}

Object::Ptr Toolbar::createBlock() {
    return Object::Ptr(new Block);
}

Object::Ptr Toolbar::createMovableBlock() {
    Object::Ptr movable = Object::Ptr(new Block);
    movable->setMass(2.f);
    return movable;
}

Object::Ptr Toolbar::createPlayer() {
    Object::Ptr player = Object::Ptr(new Player);
    player->setMass(1.f);
    return player;
}

Object::Ptr Toolbar::createSwitchBlock() {
    return Object::Ptr(new SwitchBlock);
}

Object::Ptr Toolbar::createFinishBlock() {
    return Object::Ptr(new FinishBlock);
}

Object::Ptr Toolbar::createKillBlock() {
    return Object::Ptr(new KillBlock);
}

Object::Ptr Toolbar::createGravityBlock(GravityDirection direction) {
    GravityBlock::Ptr gravity_block = GravityBlock::Ptr(new GravityBlock);
    gravity_block->setGravityDirection(direction);
    return std::dynamic_pointer_cast<Object>(gravity_block);
}

Object::Ptr Toolbar::createObject() {
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

sfg::Window::Ptr Toolbar::getWindow() {
    return toolbar_window;
}
