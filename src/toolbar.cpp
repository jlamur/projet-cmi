#include "resource_manager.hpp"
#include "editor.hpp"
#include "toolbar.hpp"
#include "block.hpp"
#include "player.hpp"
#include "switch_block.hpp"
#include "gravity_block.hpp"
#include "kill_block.hpp"
#include "finish_block.hpp"

const int Toolbar::CREATORS_TABLE_WIDTH = 3;

Toolbar::Toolbar(Editor& editor) : editor(editor) {
    // création de la boîte de la barre d'outils
    toolbar_box = sfg::Box::Create(sfg::Box::Orientation::VERTICAL, 5.f);
    creators_group = sfg::RadioButtonGroup::Create();

    // ajout des boutons de contrôle
    sfg::Button::Ptr test_button = sfg::Button::Create(L"Tester");
    sfg::Button::Ptr save_button = sfg::Button::Create(L"Sauvegarder");

    test_button->GetSignal(sfg::Widget::OnLeftClick).Connect(
        std::bind(&Editor::test, &editor)
    );

    save_button->GetSignal(sfg::Widget::OnLeftClick).Connect(
        std::bind(&Editor::save, &editor)
    );

    toolbar_box->PackEnd(test_button);
    toolbar_box->PackEnd(save_button);

    // ajout des zones de textes pour les métadonnées
	sfg::Alignment::Ptr info_spacer = sfg::Alignment::Create();
	info_spacer->SetRequisition(sf::Vector2f(1, 5));

    toolbar_box->PackEnd(info_spacer);
    toolbar_box->PackEnd(sfg::Label::Create(L"Informations"));
    toolbar_box->PackEnd(sfg::Separator::Create());

    name_entry = sfg::Entry::Create("nom niveau test");
    path_entry = sfg::Entry::Create("chemin niveau test");

    background_combo = sfg::ComboBox::Create();
    background_combo->AppendItem("background");

    music_combo = sfg::ComboBox::Create();
    music_combo->AppendItem("music");

    toolbar_box->PackEnd(name_entry);
    toolbar_box->PackEnd(path_entry);
    toolbar_box->PackEnd(background_combo);
    toolbar_box->PackEnd(music_combo);

    // mise en place de la liste des créateurs d'objets
	sfg::Alignment::Ptr creators_spacer = sfg::Alignment::Create();
	creators_spacer->SetRequisition(sf::Vector2f(1, 5));

    toolbar_box->PackEnd(creators_spacer);
    toolbar_box->PackEnd(sfg::Label::Create(L"Choix de l'objet"));
    toolbar_box->PackEnd(sfg::Separator::Create());

    creators_table = sfg::Table::Create();
    creators_table_pos_x = creators_table_pos_y = 0;

    addCreator("player", std::bind(&Toolbar::createPlayer, this));

    addCreator("block", std::bind(&Toolbar::createBlock, this));
    addCreator("movable_block", std::bind(&Toolbar::createMovableBlock, this));
    addCreator("switch_block", std::bind(&Toolbar::createSwitchBlock, this));
    addCreator("finish_block", std::bind(&Toolbar::createFinishBlock, this));
    addCreator("kill_block", std::bind(&Toolbar::createKillBlock, this));

    addCreator("gravity_block_north", std::bind(
        &Toolbar::createGravityBlock, this, GravityDirection::NORTH
    ));

    addCreator("gravity_block_east", std::bind(
        &Toolbar::createGravityBlock, this, GravityDirection::EAST
    ));

    addCreator("gravity_block_south", std::bind(
        &Toolbar::createGravityBlock, this, GravityDirection::SOUTH
    ));

    addCreator("gravity_block_west", std::bind(
        &Toolbar::createGravityBlock, this, GravityDirection::WEST
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
}

void Toolbar::addCreator(std::string path, std::function<Object::Ptr()> creator) {
    // on crée un bouton d'objet correspondant au créateur donné
    ObjectButton::Ptr button = ObjectButton::Create(
        sfg::Image::Create(
            *editor.getResourceManager().getImage("toolbar_" + path + ".tga")
        ), creators_group
    );

    creators[button] = creator;

    // on ajoute le bouton d'objet à la liste des créateurs
    creators_table->Attach(button, sf::Rect<sf::Uint32>(
        creators_table_pos_x, creators_table_pos_y, 1, 1
    ));

    creators_table_pos_x++;

    if (creators_table_pos_x >= Toolbar::CREATORS_TABLE_WIDTH) {
        creators_table_pos_x = 0;
        creators_table_pos_y++;
    }
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

float Toolbar::getWidth() {
    return toolbar_window->GetRequisition().x;
}
