#include "gui/modal.hpp"
#include <iostream>

Modal::Modal() {
    // création des fenêtres
    modal_window = sfg::Window::Create(sfg::Window::Style::BACKGROUND);
    modal_window->SetClass("modal");

    // la fenêtre interne est alignée au centre de la principale
    sfg::Alignment::Ptr inner_alignment = sfg::Alignment::Create();
    inner_alignment->SetAlignment(sf::Vector2f(.5f, .5f));
    inner_alignment->SetScale(sf::Vector2f(.0f, .0f));

    // ajout des éléments dans la fenêtre interne
    layout_box = sfg::Box::Create(sfg::Box::Orientation::VERTICAL, 10.f);
    buttons_box = sfg::Box::Create(sfg::Box::Orientation::HORIZONTAL, 10.f);
    sfg::Alignment::Ptr spacer = sfg::Alignment::Create();

    title_label = sfg::Label::Create("");
    subtitle_label = sfg::Label::Create("");
    spacer->SetRequisition(sf::Vector2f(1.f, 6.f));
    title_label->SetClass("title");

    layout_box->PackEnd(title_label);
    layout_box->PackEnd(subtitle_label);
    layout_box->PackEnd(spacer);
    layout_box->PackEnd(buttons_box);
    modal_window->Add(layout_box);
}

sfg::Window::Ptr Modal::getWindow() {
    return modal_window;
}

sfg::Box::Ptr Modal::getLayoutBox() {
    return layout_box;
}

sfg::Button::Ptr Modal::addButton(sf::String label, std::function<void()> callback) {
    sfg::Button::Ptr button = sfg::Button::Create(label);

    // liaison du bouton avec la callback, s'il y en a une
    if (callback) {
        button->GetSignal(sfg::Widget::OnLeftClick).Connect(callback);
    }

    buttons_box->PackEnd(button);
    return button;
}

void Modal::setTitle(sf::String label) {
    title_label->SetText(label);
}

void Modal::setSubtitle(sf::String label) {
    subtitle_label->SetText(label);
}

void Modal::resize(sf::Vector2u size) {
    modal_window->SetAllocation(sf::FloatRect(
        (sf::Vector2f) size / 2.f - modal_window->GetRequisition() / 2.f,
        modal_window->GetRequisition() / 2.f
    ));
}
