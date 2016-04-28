#include <algorithm>
#include <boost/filesystem.hpp>
#include "states/state.hpp"
#include "resource_manager.hpp"
#include "manager.hpp"

const unsigned int Manager::FPS = 60;
const sf::Time Manager::FRAME_TIME = sf::seconds(1.f / Manager::FPS);
const float Manager::GRID = 32;

Manager::Manager() : title(sf::String(L"")), previous_time(sf::seconds(0)),
    previous_state(nullptr) {

    ResourceManager& res = ResourceManager::get();

    // ajout des polices dans le gestionnaire de ressources
    // de la librairie pour l'interface
    desktop.GetEngine().GetResourceManager().AddFont(
        "overpass", ResourceManager::get().getFont("overpass.ttf")
    );

    desktop.GetEngine().GetResourceManager().AddFont(
        "overpass-bold", ResourceManager::get().getFont("overpass-bold.ttf")
    );

    desktop.GetEngine().GetResourceManager().AddFont(
        "monoid", ResourceManager::get().getFont("monoid.ttf")
    );

    // chargement du thème de l'interface
    desktop.LoadThemeFromFile("res/gui.theme");

    // préchargement des textures dans le GPU
    for (const auto &texture : res.getFiles(res.getTexturesPath())) {
        res.getTexture(texture);

        // on vide la file d'attente des événements pendant le
        // chargement des textures pour éviter que le système
        // pense que le programme a planté
        sf::Event event;
        while (window.pollEvent(event)) {}
    }

    // préchargement des images dans la RAM
    for (const auto &image : res.getFiles(res.getImagesPath())) {
        res.getImage(image);

        // mêmes raisons que ci-dessus
        sf::Event event;
        while (window.pollEvent(event)) {}
    }

    // création de la fenêtre du jeu
    window.create(
        sf::VideoMode(704, 480), "Skizzle", sf::Style::Default,
        sf::ContextSettings(0, 0, 1)
    );

    // FIXME: après avoir supprimé ::useGUIView(), supprimer ceci
    // récupération de la vue par défaut comme vue du gui
    gui_view = window.getDefaultView();
}

Manager::~Manager() {
    widgets.clear();
    desktop.RemoveAll();
}

void Manager::start() {
    while (!states.empty()) {
        sf::Event event;

        // traitement des évènements reçus
        while (window.pollEvent(event)) {
            // fermeture de la fenêtre : on dépile tous les états
            if (event.type == sf::Event::Closed) {
                while (!states.empty()) {
                    states.pop();
                }
            }

            // FIXME: après avoir supprimé ::useGUIView(), supprimer ceci
            // redimensionnement de la vue par défaut
            if (event.type == sf::Event::Resized) {
                gui_view = sf::View(sf::FloatRect(
                    0, 0, event.size.width, event.size.height
                ));
            }
            ///////////////////////////

            // événements de l'interface
            desktop.HandleEvent(event);

            // s'il n'y a plus d'état, on quitte
            if (states.empty()) {
                return;
            }

            // seulement si l'événement ne s'est pas passé dans l'interface
            // on le fait passer aux gestionnaires "normaux"
            if (!isInsideGUI(event)) {
                states.top()->processEvent(event);
            }
        }

        // s'il n'y a plus d'état, on quitte
        if (states.empty()) {
            return;
        }

        // si l'état que l'on va utiliser n'est pas le même que précédemment,
        // on l'active. Ceci permet un partage plus facile des ressources
        // globales (vue, musique)
        if (previous_state != states.top().get()) {
            previous_state = states.top().get();

            // on masque tous les widgets de l'interface
            for (const auto &widget : widgets) {
                widget->Show(false);
            }

            // on initialise le nouvel état
            previous_state->enable();
        }

        // demande à l'état actuel de dessiner une frame
        states.top()->frame();

        // dessin de l'interface
        sf::View normal_view = window.getView();

        desktop.Update((getCurrentTime() - previous_time).asSeconds());
        previous_time = getCurrentTime();
		sfgui.Display(window);

        window.setView(normal_view);

        // envoi à l'écran pour affichage
        window.display();
    }
}

void Manager::pushState(std::unique_ptr<State> set_state) {
    states.push(std::move(set_state));
}

void Manager::popState() {
    states.pop();
}

bool Manager::isInsideGUI(const sf::Event& event) {
    sf::Vector2f check_point;
    bool should_check_point = false;

    if (event.type == sf::Event::MouseButtonPressed ||
        event.type == sf::Event::MouseButtonReleased) {
        check_point.x = event.mouseButton.x;
        check_point.y = event.mouseButton.y;
        should_check_point = true;
    }

    if (event.type == sf::Event::MouseMoved) {
        check_point.x = event.mouseMove.x;
        check_point.y = event.mouseMove.y;
        should_check_point = true;
    }

    if (event.type == sf::Event::MouseWheelScrolled) {
        check_point.x = event.mouseWheelScroll.x;
        check_point.y = event.mouseWheelScroll.y;
        should_check_point = true;
    }

    // si l'événement est de type souris, on regarde que
    // le point en question ne se trouve pas dans un des widgets
    if (should_check_point) {
        for (auto const &widget : widgets) {
            if (widget->GetAllocation().contains(check_point)) {
                return true;
            }
        }
    }

    return false;
}

sf::Time Manager::getCurrentTime() const {
    return clock.getElapsedTime();
}

sf::RenderWindow& Manager::getWindow() {
    return window;
}

// FIXME: à supprimer après avoir supprimé ::useGUIView()
void Manager::useGUIView() {
    window.setView(gui_view);
}
///////////////////////////

void Manager::addWidget(sfg::Widget::Ptr widget) {
    widgets.push_back(widget);
    desktop.Add(widget);
}

void Manager::removeWidget(sfg::Widget::Ptr widget) {
    desktop.Remove(widget);
    widgets.erase(std::remove(
        widgets.begin(), widgets.end(), widget
    ), widgets.end());
}

sf::String Manager::getTitle() {
    return title;
}

void Manager::setTitle(sf::String set_title) {
    if (title != set_title) {
        title = set_title;

        // màj du titre selon le titre donné par l'état
        if (title.isEmpty()) {
            window.setTitle(sf::String(L"Skizzle"));
        } else {
            window.setTitle(sf::String(L"Skizzle ‒ ") + title);
        }
    }
}

bool Manager::isKeyPressed(sf::Keyboard::Key key) const {
    return sf::Keyboard::isKeyPressed(key) && window.hasFocus();
}

bool Manager::isKeyPressed(Manager::Modifier modifier) const {
    switch (modifier) {
    case Manager::Modifier::CONTROL:
        return isKeyPressed(sf::Keyboard::LControl) || isKeyPressed(sf::Keyboard::RControl);

    case Manager::Modifier::ALT:
        return isKeyPressed(sf::Keyboard::LAlt) || isKeyPressed(sf::Keyboard::RAlt);

    case Manager::Modifier::SYSTEM:
        return isKeyPressed(sf::Keyboard::LSystem) || isKeyPressed(sf::Keyboard::RSystem);

    case Manager::Modifier::SHIFT:
        return isKeyPressed(sf::Keyboard::LShift) || isKeyPressed(sf::Keyboard::RShift);
    }

    return false;
}
