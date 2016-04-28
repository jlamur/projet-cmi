#ifndef __SKIZZLE_ACTION_TOOLBAR_HPP__
#define __SKIZZLE_ACTION_TOOLBAR_HPP__

#include <SFGUI/Widgets.hpp>
#include <SFML/Graphics.hpp>
#include "../objects/object.hpp"
#include "../utility.hpp"

/**
 * Barre d'outils pour les boutons d'actions en jeu
 */
class ActionToolbar {
private:
    // widgets de la barre
    sfg::Window::Ptr toolbar_window;
    sfg::Box::Ptr toolbar_box;

    sfg::Label::Ptr timer_label;
    sfg::Box::Ptr timer_buttons_box;
    sfg::Button::Ptr timer_button_up;
    sfg::Button::Ptr timer_button_down;

    int left_buttons_count;
    int time;

    /**
     * Crée un bouton icône avec l'icône donnée
     */
    sfg::Button::Ptr createButton(std::string name);

public:
    ActionToolbar();

    /**
     * Ajoute un nouveau bouton d'action à gauche ou
     * à droite de la barre d'action d'outils
     */
    sfg::Button::Ptr addButton(
        std::string name, Utility::Direction direction,
        int position, std::function<void()> callback = std::function<void()>()
    );

    /**
     * Récupère la fenêtre de la barre d'outils
     */
    sfg::Window::Ptr getWindow();

    /**
     * Récupère la taille désirée par la barre d'outils
     */
    float getHeight();

    /**
     * Récupère le temps restant
     */
    int getTime();

    /**
     * Modifie le temps restant
     */
    void setTime(int set_time);

    /**
     * Configure les boutons de modification du temps
     */
    void setTimeEditable(
        std::function<void()> up_callback = std::function<void()>(),
        std::function<void()> down_callback = std::function<void()>()
    );
};

#endif
