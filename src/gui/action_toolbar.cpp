#include "resource_manager.hpp"
#include "gui/action_toolbar.hpp"

ActionToolbar::ActionToolbar() : left_buttons_count(0) {
    // création de la boîte de la barre d'outils
    toolbar_box = sfg::Box::Create(sfg::Box::Orientation::HORIZONTAL, 5.f);

    // création de la fenêtre
    toolbar_window = sfg::Window::Create(sfg::Window::Style::BACKGROUND);
    toolbar_window->Add(toolbar_box);

    // ajout du timer, des boutons et des espaceurs
    sfg::Alignment::Ptr left_spacer = sfg::Alignment::Create();
    sfg::Alignment::Ptr right_spacer = sfg::Alignment::Create();

    timer_buttons_box = sfg::Box::Create(sfg::Box::Orientation::VERTICAL);
    timer_button_up = createButton("arrow_up");
    timer_button_down = createButton("arrow_down");
    timer_label = sfg::Label::Create("00:00");
    timer_label->SetClass("timer");

    left_spacer->SetRequisition(sf::Vector2f(5.f, 1.f));
    right_spacer->SetRequisition(sf::Vector2f(5.f, 1.f));

    timer_buttons_box->PackEnd(timer_button_up, false, false);
    timer_buttons_box->PackEnd(timer_button_down, false, false);

    toolbar_box->PackEnd(left_spacer, true, false);
    toolbar_box->PackEnd(timer_label, false, false);
    toolbar_box->PackEnd(timer_buttons_box, false, false);
    timer_buttons_box->Show(false);
    toolbar_box->PackEnd(right_spacer, true, false);

    // pour les styles
    toolbar_window->SetId("action_toolbar");
}

sfg::Button::Ptr ActionToolbar::createButton(std::string name) {
    // création d'un bouton avec pour image l'image passée
    sfg::Button::Ptr button = sfg::Button::Create("");
    button->SetClass("icon");
    button->SetImage(ResourceManager::get().getImage(name + ".tga"));

    return button;
}

sfg::Button::Ptr ActionToolbar::addButton(
    std::string name, Utility::Direction direction,
    int position, std::function<void()> callback
) {
    sfg::Button::Ptr button = createButton(name);

    // liaison du bouton avec la callback, s'il y en a une
    if (callback) {
        button->GetSignal(sfg::Widget::OnLeftClick).Connect(callback);
    }

    // ajout du bouton à la barre selon la position donnée
    if (direction == Utility::Direction::WEST) {
        position += left_buttons_count + 4;
    } else {
        left_buttons_count++;
    }

    toolbar_box->PackEnd(button, false, false);
    toolbar_box->ReorderChild(button, position);

    return button;
}

sfg::Window::Ptr ActionToolbar::getWindow() {
    return toolbar_window;
}

float ActionToolbar::getHeight() {
    return toolbar_window->GetRequisition().y;
}

int ActionToolbar::getTime() {
    return time;
}

void ActionToolbar::setTime(int set_time) {
    time = set_time;

    // mise à jour du temps affiché sur le label
    sf::String minutes = std::to_string(time / 60);
    sf::String seconds = std::to_string(time % 60);

    // ajout d'un zéro devant les secondes si nécessaire
    if (minutes.getSize() == 1) {
        minutes = "0" + minutes;
    }

    if (seconds.getSize() == 1) {
        seconds = "0" + seconds;
    }

    timer_label->SetText(minutes + ":" + seconds);
}

void ActionToolbar::setTimeEditable(
    std::function<void()> up_callback,
    std::function<void()> down_callback
) {
    // si on passe des callbacks vides, on désactive l'édition du temps
    if (!up_callback || !down_callback) {
        timer_buttons_box->Show(false);
        return;
    }

    timer_buttons_box->Show(true);
    timer_button_up->GetSignal(sfg::Widget::OnLeftClick).Connect(up_callback);
    timer_button_down->GetSignal(sfg::Widget::OnLeftClick).Connect(down_callback);
}
