#include <cmath>
#include <algorithm>
#include "editor.hpp"
#include "game.hpp"
#include "block.hpp"
#include "constants.hpp"

const sf::Color SELECTION_COLOR = sf::Color(33, 33, 33, 40);
const sf::Color SELECTION_BORDER_COLOR = sf::Color(33, 33, 33, 127);

Editor::Editor(Manager& manager) : Level(manager), drag_mode(DragMode::NONE),
    widget_timer(manager, true, std::bind(&Editor::setTotalTime, this, std::placeholders::_1)) {}

Editor::~Editor() {}

void Editor::load(std::ifstream& file) {
    Level::load(file);
    manager.setTitle(sf::String(L"Édition de ") + getName());
}

void Editor::frame() {
    const std::vector<sf::Event>& events = manager.getEvents();

    // traitement des événements
    for (unsigned int i = 0; i < events.size(); i++) {
        processEvent(events[i]);
    }

    // dessin de la frame
    draw();
    sf::sleep(sf::seconds(1.f / 30));
}

bool Editor::processEvent(const sf::Event& event) {
    // traitement des événements du widget timer
    if (widget_timer.processEvent(event)) {
        return true;
    }

    // lorsque l'on clique dans l'éditeur
    if (event.type == sf::Event::MouseButtonPressed) {
        sf::Vector2f position(event.mouseButton.x, event.mouseButton.y);
        ObjectPtr pointed_object = getObject(position);

        if (event.mouseButton.button == sf::Mouse::Left) {
            // clic + shift : sélection par rectangle de sélection
            if (manager.isKeyPressed(sf::Keyboard::LShift)) {
                drag_start = position;
                drag_end = position;
                drag_mode = DragMode::SELECT_RECT;

                return true;
            }

            // clic sur un objet : démarrage de la sélection libre
            if (pointed_object != nullptr) {
                if (manager.isKeyPressed(sf::Keyboard::LControl)) {
                    drag_start = position;
                    drag_end = position;
                    drag_mode = DragMode::SELECT_BULK;

                    select(pointed_object, SelectionMode::ADD);
                } else {
                    select(pointed_object, SelectionMode::FLIP);
                }

                return true;
            }

            // clic gauche dans le vide : démarrage du placement
            // en drag&ndrop
            drag_start = position;
            drag_end = position;
            drag_mode = DragMode::PLACE_OBJECTS;

            select(addObject(position), SelectionMode::REPLACE);
            return true;
        }

        if (event.mouseButton.button == sf::Mouse::Right) {
            // clic droit : on supprime l'objet pointé
            removeObject(pointed_object);
            return true;
        }
    }

    // lorsqu'on déplace la souris
    if (event.type == sf::Event::MouseMoved) {
        sf::Vector2f position(event.mouseMove.x, event.mouseMove.y);
        ObjectPtr pointed_object = getObject(position);
        drag_end = position;

        // mode placement d'objets
        if (drag_mode == DragMode::PLACE_OBJECTS && pointed_object == nullptr) {
            select(addObject(position), SelectionMode::ADD);
            return true;
        }

        // mode sélection libre : on l'objet à la sélection
        if (drag_mode == DragMode::SELECT_BULK) {
            select(position, SelectionMode::ADD);
            return true;
        }
    }

    // lorsqu'on relâche un clic dans l'éditeur
    if (event.type == sf::Event::MouseButtonReleased) {
        sf::Vector2f position(event.mouseButton.x, event.mouseButton.y);

        // mode sélection rectangulaire : on applique la sélection
        if (drag_mode == DragMode::SELECT_RECT) {
            select(drag_start, drag_end);
        }

        drag_mode = DragMode::NONE;
        return true;        
    }

    // gestion des touches
    if (event.type == sf::Event::KeyPressed) {
        // appui sur suppr : suppression des blocs sélectionnés
        if (event.key.code == sf::Keyboard::Delete) {
            std::vector<ObjectPtr>& objects = getObjects();

            for (unsigned int i = 0; i < selection.size(); i++) {
                objects.erase(std::remove(
                    objects.begin(), objects.end(), selection[i]
                ), objects.end());
            }

            selection.clear();
            return true;
        }

        // appui sur espace : test du niveau en cours d'édition
        if (event.key.code == sf::Keyboard::Space) {
            test();
            return true;
        }
    }

    return false;
}

void Editor::draw() {
    Level::draw();

    sf::RenderWindow& window = manager.getWindow();
    sf::View window_view = manager.getWindowView();

    // dessin du rectangle de sélection
    if (drag_mode == DragMode::SELECT_RECT) {
        sf::RectangleShape selection_rect(drag_end - drag_start);
        selection_rect.setPosition(drag_start);
        selection_rect.setFillColor(SELECTION_COLOR);
        selection_rect.setOutlineThickness(2.f);
        selection_rect.setOutlineColor(SELECTION_BORDER_COLOR);

        window.draw(selection_rect);
    }

    // dessin du widget timer
    widget_timer.setTimeLeft(getTotalTime());
    widget_timer.draw(sf::Vector2f(window_view.getSize().x / 2 - 50, 0));

    // menu
    sf::RectangleShape menu(sf::Vector2f(window_view.getSize().x, 64));
    menu.setPosition(sf::Vector2f(0, window_view.getSize().y - 64));

    window.draw(menu);
}

ObjectPtr Editor::getObject(sf::Vector2f position) {
    std::vector<ObjectPtr>& objects = getObjects();

    for (unsigned int i = 0; i < objects.size(); i++) {
        if (objects[i]->getAABB()->contains(position)) {
            return objects[i];
        }
    }

    return nullptr;
}

ObjectPtr Editor::addObject(sf::Vector2f position) {
    std::vector<ObjectPtr>& objects = getObjects();

    // on arrondit à l'unité de grille la plus proche
    position /= Constants::GRID;
    position.x = round(position.x);
    position.y = round(position.y);
    position *= Constants::GRID;

    // TODO: ajouter un objet du type choisi, pas uniquement de bloc
    ObjectPtr object = ObjectPtr(new Block);
    object->setPosition(position);

    // avant d'ajouter l'objet, on vérifie qu'il ne soit
    // pas superposé à un autre
    float overlaps = false;

    for (unsigned int i = 0; i < objects.size(); i++) {
        if (objects[i]->getAABB()->intersects(*object->getAABB())) {
            overlaps = true;
        }
    }

    if (!overlaps) {
        objects.push_back(object);
        return object;
    }

    return nullptr;
}

void Editor::removeObject(ObjectPtr object) {
    if (object == nullptr) {
        return;
    }

    std::vector<ObjectPtr>& objects = getObjects();

    // on supprime l'objet de la sélection
    selection.erase(std::remove(
        selection.begin(), selection.end(), object
    ), selection.end());

    // on supprime l'objet de la liste d'objets
    objects.erase(std::remove(
        objects.begin(), objects.end(), object
    ), selection.end());
}

void Editor::removeObject(sf::Vector2f position) {
    removeObject(getObject(position));
}

void Editor::select(ObjectPtr object, SelectionMode mode) {
    if (object == nullptr) {
        return;
    }

    bool already_selected = std::count(selection.begin(), selection.end(), object) > 0;

    // dans les modes REPLACE et FLIP, on remplace l'ancienne sélection
    // pour REPLACE, on sélectionne forcément l'objet
    // pour FLIP, on le sélectionne s'il ne l'est pas, on le désélectionne sinon
    if (mode == SelectionMode::REPLACE || mode == SelectionMode::FLIP) {
        for (unsigned int i = 0; i < selection.size(); i++) {
            selection[i]->setSelected(false);
        }

        selection.clear();

        // on resélectionne l'objet ssi. on force la sélection
        // ou s'il n'était pas déjà sélectionné
        if (!already_selected || mode == SelectionMode::REPLACE) {
            object->setSelected(true);
            selection.push_back(object);
        }
    }

    // dans le mode ADD, on rajoute juste l'objet à la sélection
    if (mode == SelectionMode::ADD && !already_selected) {
        object->setSelected(true);
        selection.push_back(object);
    }
}

void Editor::select(sf::Vector2f position, SelectionMode mode) {
    select(getObject(position), mode);
}

void Editor::select(sf::Vector2f top_left, sf::Vector2f bottom_right) {
    std::vector<ObjectPtr>& objects = getObjects();
    sf::FloatRect selection_rect(
        std::min(top_left.x, bottom_right.x),
        std::min(top_left.y, bottom_right.y),
        std::abs(bottom_right.x - top_left.x),
        std::abs(bottom_right.y - top_left.y)
    );

    // réinitialisation de la sélectionne
    for (unsigned int i = 0; i < selection.size(); i++) {
        selection[i]->setSelected(false);
    }

    selection.clear();

    // sélection des éléments intersectant le rectangle
    for (unsigned int i = 0; i < objects.size(); i++) {
        if (objects[i]->getAABB()->intersects(selection_rect)) {
            select(objects[i], SelectionMode::ADD);
        }
    }
}

void Editor::test() {
    std::shared_ptr<Game> game = std::shared_ptr<Game>(new Game(manager));

    // copie des propriétés
    game->setName(getName());
    game->setTotalTime(getTotalTime());
    game->setBackground(getBackground());

    // copie des objets du niveau vers le jeu
    std::vector<ObjectPtr>& objects = getObjects();

    for (unsigned int i = 0; i < objects.size(); i++) {
        game->getObjects().push_back(objects[i]->clone());
    }

    // copie de la zone de jeu
    std::vector<std::pair<float, float>>& zone = getZone();

    for (unsigned int i = 0; i < zone.size(); i++) {
        game->getZone().push_back(zone[i]);
    }

    // mise en mode test
    game->setTestMode(manager.getView());
    manager.setView(game);
}
