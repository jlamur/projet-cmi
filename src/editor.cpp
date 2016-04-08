#include <iostream>
#include <cmath>
#include <algorithm>
#include "editor.hpp"
#include "game.hpp"
#include "block.hpp"
#include "constants.hpp"

const sf::Color SELECTION_COLOR = sf::Color(33, 33, 33, 40);
const sf::Color SELECTION_BORDER_COLOR = sf::Color(33, 33, 33, 127);
const int SCROLL_SPEED = 2;
const int SCROLL_PADDING = 8;

Editor::Editor(Manager& manager) : Level(manager), drag_mode(DragMode::NONE),
    widget_timer(manager, true, std::bind(&Editor::setTotalTime, this, std::placeholders::_1)) {}

Editor::~Editor() {}

void Editor::load(std::ifstream& file) {
    Level::load(file);
    manager.setTitle(sf::String(L"Édition de ") + getName());
}

bool Editor::frame() {
    // si le dessin de la frame a été interrompu par
    // le traitement événementiel, on arrête
    if (Level::frame()) {
        return true;
    }

    // dessin de la frame
    draw();
    sf::sleep(sf::seconds(1.f / 30));

    return true;
}

bool Editor::processEvent(const sf::Event& event) {
    if (Level::processEvent(event)) {
        return true;
    }

    // traitement des événements du widget timer
    if (widget_timer.processEvent(event)) {
        return false;
    }

    // lorsque l'on clique dans l'éditeur
    if (event.type == sf::Event::MouseButtonPressed) {
        sf::Vector2i mouse_position(event.mouseButton.x, event.mouseButton.y);
        sf::Vector2f position = convertCoords(mouse_position);
        ObjectPtr pointed_object = getObject(position);

        if (event.mouseButton.button == sf::Mouse::Left) {
            // clic + shift : sélection par rectangle de sélection
            if (manager.isKeyPressed(sf::Keyboard::LShift)) {
                drag_start = mouse_position;
                drag_end = mouse_position;
                drag_mode = DragMode::SELECT_RECT;
            }

            // clic sur un objet : démarrage de la sélection libre
            else if (pointed_object != nullptr) {
                if (manager.isKeyPressed(sf::Keyboard::LControl)) {
                    drag_start = mouse_position;
                    drag_end = mouse_position;
                    drag_mode = DragMode::SELECT_BULK;

                    select(pointed_object, SelectionMode::ADD);
                } else {
                    select(pointed_object, SelectionMode::FLIP);
                }
            }

            // clic gauche dans le vide : démarrage du placement en drag&drop
            else {
                drag_start = mouse_position;
                drag_end = mouse_position;
                drag_mode = DragMode::PLACE;

                select(addObject(position), SelectionMode::REPLACE);
            }
        }

        if (event.mouseButton.button == sf::Mouse::Right) {
            // clic droit sur un objet : démarrage de la suppression en drag&drop
            if (pointed_object != nullptr) {
                drag_start = mouse_position;
                drag_end = mouse_position;
                drag_mode = DragMode::REMOVE;

                removeObject(pointed_object);
            }
        }
    }

    // lorsqu'on déplace la souris
    if (event.type == sf::Event::MouseMoved) {
        sf::Vector2i mouse_position(event.mouseMove.x, event.mouseMove.y);
        sf::Vector2f position = convertCoords(mouse_position);
        ObjectPtr pointed_object = getObject(position);

        drag_end = mouse_position;

        // mode placement d'objets
        if (drag_mode == DragMode::PLACE && pointed_object == nullptr) {
            select(addObject(position), SelectionMode::ADD);
        }

        // mode suppression d'objets
        if (drag_mode == DragMode::REMOVE && pointed_object != nullptr) {
            removeObject(pointed_object);
        }

        // mode sélection libre : on l'objet à la sélection
        if (drag_mode == DragMode::SELECT_BULK) {
            select(position, SelectionMode::ADD);
        }
    }

    // lorsqu'on relâche un clic dans l'éditeur
    if (event.type == sf::Event::MouseButtonReleased) {
        // mode sélection rectangulaire : on applique la sélection
        if (drag_mode == DragMode::SELECT_RECT) {
            select(convertCoords(drag_start), convertCoords(drag_end));
        }

        drag_mode = DragMode::NONE;
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

            clearSelection();
        }

        // appui sur Ctrl + A : sélection de tous les objets
        if (event.key.code == sf::Keyboard::A && event.key.control) {
            selectAll();
        }

        // appui sur espace : test du niveau en cours d'édition
        if (event.key.code == sf::Keyboard::Space) {
            test();

            // demande l'interruption du dessin de la
            // frame car l'objet risque d'être détruit
            return true;
        }
    }

    return false;
}

void Editor::draw() {
    sf::RenderWindow& window = manager.getWindow();
    sf::Vector2i window_size = (sf::Vector2i) window.getSize();

    // scroll de la caméra lorsque la souris se situe sur les bords
    if (window.hasFocus()) {
        sf::View camera = getCamera();
        sf::Vector2i mouse = sf::Mouse::getPosition(window);

        if (mouse.x < SCROLL_PADDING) {
            camera.move(sf::Vector2f(-SCROLL_SPEED, 0));
        } else if (mouse.x > window_size.x - SCROLL_PADDING) {
            camera.move(sf::Vector2f(SCROLL_SPEED, 0));
        } else if (mouse.y < SCROLL_PADDING) {
            camera.move(sf::Vector2f(0, -SCROLL_SPEED));
        } else if (mouse.y > window_size.y - SCROLL_PADDING) {
            camera.move(sf::Vector2f(0, SCROLL_SPEED));
        }

        setCamera(camera);
    }

    // dessin des objets du niveau
    Level::draw();

    // dessin du rectangle de sélection
    if (drag_mode == DragMode::SELECT_RECT) {
        sf::Vector2f size = (sf::Vector2f) (drag_end - drag_start);
        sf::Vector2f pos = (sf::Vector2f) drag_start;

        sf::RectangleShape selection_rect(size);
        selection_rect.setPosition(pos);
        selection_rect.setFillColor(SELECTION_COLOR);
        selection_rect.setOutlineThickness(2.f);
        selection_rect.setOutlineColor(SELECTION_BORDER_COLOR);

        window.draw(selection_rect);
    }

    // dessin du widget timer
    widget_timer.setTimeLeft(getTotalTime());
    widget_timer.draw(sf::Vector2f(window_size.x / 2 - 50, 0));

    // menu
    sf::RectangleShape menu(sf::Vector2f(window_size.x, 64));
    menu.setPosition(sf::Vector2f(0, window_size.y - 64));

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
    ), objects.end());
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
        clearSelection();

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

    clearSelection();

    // sélection des éléments intersectant le rectangle
    for (unsigned int i = 0; i < objects.size(); i++) {
        if (objects[i]->getAABB()->intersects(selection_rect)) {
            select(objects[i], SelectionMode::ADD);
        }
    }
}

void Editor::clearSelection() {
    for (unsigned int i = 0; i < selection.size(); i++) {
        selection[i]->setSelected(false);
    }

    selection.clear();
}

void Editor::selectAll() {
    std::vector<ObjectPtr>& objects = getObjects();

    for (unsigned int i = 0; i < objects.size(); i++) {
        objects[i]->setSelected(true);
        selection.push_back(objects[i]);
    }
}

void Editor::test() {
    std::shared_ptr<Game> game = std::shared_ptr<Game>(new Game(manager));
    clearSelection();

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
