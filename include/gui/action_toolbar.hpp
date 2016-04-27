#ifndef __SKIZZLE_ACTION_TOOLBAR_HPP__
#define __SKIZZLE_ACTION_TOOLBAR_HPP__

#include <SFGUI/Widgets.hpp>
#include <SFML/Graphics.hpp>
#include "../objects/object.hpp"
#include "../states/level.hpp"
#include "object_button.hpp"

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
    void addButton(
        sf::Image image,
        std::function<void()> callback = std::function<void()>()
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
