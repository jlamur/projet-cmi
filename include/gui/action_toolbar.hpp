#ifndef __SKIZZLE_ACTION_TOOLBAR_HPP__
#define __SKIZZLE_ACTION_TOOLBAR_HPP__

#include <SFGUI/Widgets.hpp>
#include <SFML/Graphics.hpp>
#include "../objects/object.hpp"

/**
 * Barre d'outils pour les actions en jeu
 */
class ActionToolbar {
private:
    // widgets de la barre
    sfg::Window::Ptr toolbar_window;
    sfg::Box::Ptr toolbar_box;

public:
    ActionToolbar();

    /**
     * Ajoute un nouveau bouton d'action à la barre d'outils
     */
    sfg::Button::Ptr addButton(
        sf::Image image,
        std::function<void()> callback = std::function<void()>()
    );

    /**
     * Ajoute un nouvel espaceur
     */
    sfg::Alignment::Ptr addSpacer(
        float width, bool expand, bool fill
    );

    /**
     * Récupère la fenêtre de la barre d'outils
     */
    sfg::Window::Ptr getWindow();

    /**
     * Récupère la taille désirée par la barre d'outils
     */
    float getHeight();
};

#endif
