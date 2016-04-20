#ifndef __SKIZZLE_TOOLBAR_HPP__
#define __SKIZZLE_TOOLBAR_HPP__

#include <SFGUI/Widgets.hpp>
#include <SFML/Graphics.hpp>
#include <map>
#include <functional>
#include "../objects/object.hpp"
#include "../states/level.hpp"
#include "object_button.hpp"

class Editor;

/**
 * Barre d'outils pour l'éditeur
 */
class Toolbar {
private:
    Editor& editor;

    // widgets de la barre
    sfg::Window::Ptr toolbar_window;
    sfg::Box::Ptr toolbar_box;

    // zones de texte pour les métadonnées
    sfg::Entry::Ptr name_entry;
    sfg::Entry::Ptr path_entry;
    sfg::ComboBox::Ptr background_combo;
    sfg::ComboBox::Ptr music_combo;

    // types d'objets de la barre d'outils
    sfg::RadioButtonGroup::Ptr creators_group;
    sfg::Table::Ptr creators_table;
    std::map<ObjectButton::Ptr, std::function<Object::Ptr()>> creators;

    int creators_table_pos_x, creators_table_pos_y;

    /**
     * Nombre d'objets en largeur dans la liste des
     * créateurs
     */
    static const int CREATORS_TABLE_WIDTH;

    /**
     * Ajoute un nouveau créateur d'objet au dictionnaire
     * des créateurs et ajoute son image à l'interface
     */
    void addCreator(std::string path, std::function<Object::Ptr()> creator);

    Object::Ptr createBlock();
    Object::Ptr createMovableBlock();
    Object::Ptr createPlayer();
    Object::Ptr createSwitchBlock();
    Object::Ptr createFinishBlock();
    Object::Ptr createKillBlock();
    Object::Ptr createGravityBlock(GravityDirection direction);

public:
    Toolbar(Editor& editor);

    /**
     * Crée un nouvel objet du type actuellement sélectionné
     */
    Object::Ptr createObject();

    /**
     * Récupère la fenêtre de la barre d'outils
     */
    sfg::Window::Ptr getWindow();

    /**
     * Récupère la taille désirée par la barre d'outils
     */
    float getWidth();
};

#endif
