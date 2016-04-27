#include "gui/action_toolbar.hpp"

ActionToolbar::ActionToolbar() {
    // création de la boîte de la barre d'outils
    toolbar_box = sfg::Box::Create(sfg::Box::Orientation::HORIZONTAL, 5.f);

    // création de la fenêtre
    toolbar_window = sfg::Window::Create(
        sfg::Window::Style::BACKGROUND |
        sfg::Window::Style::SHADOW
    );
    
    toolbar_window->Add(toolbar_box);

    // pour les styles
    toolbar_window->SetId("action_toolbar");
}

void ActionToolbar::addButton(sf::Image image, std::function<void()> callback) {
    // création d'un bouton avec pour image l'image passée
    ObjectButton::Ptr button = ObjectButton::Create(sfg::Image::Create(image));

    // liaison du bouton avec la callback, s'il y en a une
    if (callback) {
        button->GetSignal(sfg::Widget::OnLeftClick).Connect(callback);
    }

    // ajout du bouton à la barre
    toolbar_box->PackEnd(button, false, false);
}

sfg::Window::Ptr ActionToolbar::getWindow() {
    return toolbar_window;
}

float ActionToolbar::getHeight() {
    return toolbar_window->GetRequisition().y;
}
