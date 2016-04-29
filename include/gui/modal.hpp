#ifndef __SKIZZLE_MODAL_HPP__
#define __SKIZZLE_MODAL_HPP__

#include <SFGUI/Widgets.hpp>
#include <SFML/Graphics.hpp>
#include <functional>

/**
 * Classe pour les dialogues d'information du jeu
 */
class Modal {
protected:
    sfg::Window::Ptr modal_window;

    sfg::Box::Ptr layout_box;
    sfg::Box::Ptr buttons_box;

    sfg::Label::Ptr title_label;
    sfg::Label::Ptr subtitle_label;

public:
    Modal();

    /**
     * Récupère la fenêtre de la modale
     */
    sfg::Window::Ptr getWindow();

    /**
     * Récupère la boîte de layout interne de la modale
     */
    sfg::Box::Ptr getLayoutBox();

    /**
     * Ajoute un nouveau bouton à la modale
     */
    sfg::Button::Ptr addButton(
        sf::String label,
        std::function<void()> callback = std::function<void()>()
    );

    /**
     * Modifie le titre de la modale
     */
    void setTitle(sf::String label);

    /**
     * Modifie le sous-titre de la modale
     */
    void setSubtitle(sf::String label);

    /**
     * Redimensionne la modale pour qu'elle prenne tout l'espace donné
     */
    void resize(sf::Vector2u size);
};

#endif
