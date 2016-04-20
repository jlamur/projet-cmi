#include <boost/filesystem.hpp>
#include "resource_manager.hpp"
#include "states/editor.hpp"
#include "objects/block.hpp"
#include "objects/player.hpp"
#include "objects/switch_block.hpp"
#include "objects/gravity_block.hpp"
#include "objects/kill_block.hpp"
#include "objects/finish_block.hpp"
#include "gui/toolbar.hpp"

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

    name_entry = sfg::Entry::Create();
    path_entry = sfg::Entry::Create();

    name_entry->GetSignal(sfg::Entry::OnTextChanged).Connect(
        std::bind(&Toolbar::updateEditorName, this)
    );

    path_entry->GetSignal(sfg::Entry::OnTextChanged).Connect(
        std::bind(&Toolbar::updateEditorPath, this)
    );

    toolbar_box->PackEnd(name_entry);
    toolbar_box->PackEnd(path_entry);

    // construction des choix de fonds et musiques pour le niveau
    ResourceManager& res = ResourceManager::get();
    std::vector<boost::filesystem::path> backgrounds_list =
        res.getFiles(res.getTexturesPath() / "levels");
    std::vector<boost::filesystem::path> musics_list =
        res.getFiles(res.getMusicsPath() / "levels");

    background_combo = sfg::ComboBox::Create();
    music_combo = sfg::ComboBox::Create();

    background_combo->AppendItem("Aucun fond");
    background_combo->SelectItem(0);
    music_combo->AppendItem("Aucune musique");
    music_combo->SelectItem(0);

    for (const auto &background_path : backgrounds_list) {
        std::string choice_value = background_path.filename().string();
        background_combo->AppendItem(choice_value);
    }

    for (const auto &music_path : musics_list) {
        std::string choice_value = music_path.filename().string();
        music_combo->AppendItem(choice_value);
    }

    background_combo->GetSignal(sfg::ComboBox::OnSelect).Connect(
        std::bind(&Toolbar::updateEditorBackground, this)
    );

    music_combo->GetSignal(sfg::ComboBox::OnSelect).Connect(
        std::bind(&Toolbar::updateEditorMusic, this)
    );

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

    addCreator("block", std::bind(&Toolbar::createBlock, this));
    addCreator("movable_block", std::bind(&Toolbar::createMovableBlock, this));
    addCreator("player", std::bind(&Toolbar::createPlayer, this));
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
            *ResourceManager::get().getImage("toolbar/" + path + ".tga")
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

void Toolbar::updateEditorName() {
    editor.setName(name_entry->GetText());
}

void Toolbar::updateEditorPath() {
    editor.setPath(path_entry->GetText());
}

void Toolbar::updateEditorBackground() {
    if (background_combo->GetSelectedItem() == 0) {
        editor.setBackground("");
    } else {
        editor.setBackground(background_combo->GetSelectedText().toAnsiString());
    }
}

void Toolbar::updateEditorMusic() {
    if (music_combo->GetSelectedItem() == 0) {
        editor.setMusic("");
    } else {
        editor.setMusic(music_combo->GetSelectedText().toAnsiString());
    }
}

void Toolbar::update() {
    if (name_entry->GetText() != editor.getName()) {
        name_entry->SetText(editor.getName());
    }

    if (path_entry->GetText().toAnsiString() != editor.getPath()) {
        path_entry->SetText(editor.getPath());
    }

    for (int i = 0; i < background_combo->GetItemCount(); i++) {
        if (background_combo->GetItem(i).toAnsiString() == editor.getBackground()) {
            background_combo->SelectItem(i);
        }
    }

    for (int i = 0; i < music_combo->GetItemCount(); i++) {
        if (music_combo->GetItem(i).toAnsiString() == editor.getMusic()) {
            music_combo->SelectItem(i);
        }
    }
}

sfg::Window::Ptr Toolbar::getWindow() {
    return toolbar_window;
}

float Toolbar::getWidth() {
    return toolbar_window->GetRequisition().x;
}
