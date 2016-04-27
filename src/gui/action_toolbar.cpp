#include "gui/action_toolbar.hpp"

ActionToolbar::ActionToolbar() {
    // création de la boîte de la barre d'outils
    toolbar_box = sfg::Box::Create(sfg::Box::Orientation::HORIZONTAL, 5.f);

    // création de la fenêtre
    toolbar_window = sfg::Window::Create(sfg::Window::Style::BACKGROUND);
    toolbar_window->Add(toolbar_box);

    // pour les styles
    toolbar_window->SetId("action_toolbar");
}

sfg::Button::Ptr ActionToolbar::addButton(
    sf::Image image, std::function<void()> callback
) {
    // création d'un bouton avec pour image l'image passée
    sfg::Button::Ptr button = sfg::Button::Create("");
    button->SetImage(sfg::Image::Create(image));
    button->SetClass("notext");

    // liaison du bouton avec la callback, s'il y en a une
    if (callback) {
        button->GetSignal(sfg::Widget::OnLeftClick).Connect(callback);
    }

    // ajout du bouton à la barre
    toolbar_box->PackEnd(button, false, false);
    return button;
}

sfg::Alignment::Ptr ActionToolbar::addSpacer(
    float width, bool expand, bool fill
) {
    sfg::Alignment::Ptr spacer = sfg::Alignment::Create();
    spacer->SetRequisition(sf::Vector2f(width, 1.f));

    // ajout de l'espaceur à la barre
    toolbar_box->PackEnd(spacer, expand, fill);
    return spacer;
}

sfg::Window::Ptr ActionToolbar::getWindow() {
    return toolbar_window;
}

float ActionToolbar::getHeight() {
    return toolbar_window->GetRequisition().y;
}
