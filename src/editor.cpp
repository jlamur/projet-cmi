#include "editor.hpp"
#include "game.hpp"
#include "block.hpp"
#include "constants.hpp"
#include <cmath>
#include <algorithm>

const sf::Color SELECT_RECT_COLOR = sf::Color(33, 33, 33, 40);
const sf::Color SELECT_RECT_BORDER_COLOR = sf::Color(33, 33, 33, 127);

const float WHEEL_SCROLL_SPEED = -8.f;
const float POINTER_SCROLL_SPEED = 8.f;
const float POINTER_SCROLL_MAX_SPEED = 10.f;
const float POINTER_SCROLL_STEP = 64.f;
const float ASSUME_TITLEBAR = 32.f;
const int POINTER_SCROLL_PADDING = 5;

Editor::Editor(Manager& manager) : Level(manager), drag_mode(DragMode::NONE),
    widget_timer(manager, true, std::bind(&Editor::setTotalTime, this, std::placeholders::_1)),
    widget_selector(manager) {

    setName("Nouveau niveau");
    setTotalTime(30);

    ResourceManager& resources = getResourceManager();

    std::shared_ptr<SelectorCategory> basic_cat = widget_selector.addCategory("BASE");
    basic_cat->addItem("Block", resources.getTexture("block.tga"));
    basic_cat->addItem("Player", resources.getTexture("player.tga"));

    std::shared_ptr<SelectorCategory> gravity_cat = widget_selector.addCategory(sf::String(L"GRAVITÉ"));
    gravity_cat->addItem("GravityN", resources.getTexture("gravity_block_north.tga"));
    gravity_cat->addItem("GravityE", resources.getTexture("gravity_block_east.tga"));
    gravity_cat->addItem("GravityS", resources.getTexture("gravity_block_south.tga"));
    gravity_cat->addItem("GravityW", resources.getTexture("gravity_block_west.tga"));
}

Editor::~Editor() {}

void Editor::begin() {
    Level::begin();

    getResourceManager().playMusic("editor.ogg");
    getWindow().setFramerateLimit(60);
}

void Editor::processEvent(const sf::Event& event) {
    Level::processEvent(event);

    // traitement des événements du widget timer
    if (widget_timer.processEvent(event)) {
        return;
    }

    // traitement des événements du widget selector
    if (widget_selector.processEvent(event)) {
        return;
    }

    // lorsque l'on clique dans l'éditeur
    if (event.type == sf::Event::MouseButtonPressed) {
        sf::Vector2i mouse_position(event.mouseButton.x, event.mouseButton.y);
        sf::Vector2f position = pixelToCoords(mouse_position);
        ObjectPtr pointed_object = getObject(position);

        if (event.mouseButton.button == sf::Mouse::Left) {
            // clic + shift : sélection par rectangle de sélection
            if (getManager().isKeyPressed(Manager::Modifier::SHIFT)) {
                drag_start = mouse_position;
                drag_end = mouse_position;
                drag_mode = DragMode::SELECT_RECT;
            }

            // clic sur un objet : démarrage de la sélection libre
            else if (pointed_object != nullptr) {
                if (getManager().isKeyPressed(Manager::Modifier::CONTROL)) {
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
        sf::Vector2f position = pixelToCoords(mouse_position);
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
            select(pixelToCoords(drag_start), pixelToCoords(drag_end));
        }

        drag_mode = DragMode::NONE;
    }

    // lorsqu'on scrolle on déplace la vue
    if (event.type == sf::Event::MouseWheelScrolled) {
        sf::View camera = getCamera();

        if (event.mouseWheelScroll.wheel == sf::Mouse::HorizontalWheel) {
            camera.move(sf::Vector2f(event.mouseWheelScroll.delta, 0) * WHEEL_SCROLL_SPEED);
        } else if (event.mouseWheelScroll.wheel == sf::Mouse::VerticalWheel) {
            camera.move(sf::Vector2f(0, event.mouseWheelScroll.delta) * WHEEL_SCROLL_SPEED);
        }

        setCamera(camera);
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
        }
    }
}

void Editor::frame() {
    // titre de la fenêtre
    getManager().setTitle(sf::String(L"Édition de ") + getName());

    // dessin de la frame
    draw();
}

void Editor::draw() {
    sf::RenderWindow& window = getWindow();
    sf::Vector2i window_size = (sf::Vector2i) window.getSize();

    // scroll de la caméra lorsque la souris se situe sur les bords
    if (window.hasFocus()) {
        sf::View camera = getCamera();
        sf::Vector2i mouse = sf::Mouse::getPosition(window);

        sf::Vector2f change;
        float absolute_diff;

        // détection du dépassement sur un des 4 bords
        if (mouse.x < POINTER_SCROLL_PADDING) {
            absolute_diff = POINTER_SCROLL_PADDING - mouse.x;
            change = sf::Vector2f(-POINTER_SCROLL_SPEED, 0);
        } else if (mouse.x > window_size.x - POINTER_SCROLL_PADDING) {
            absolute_diff = mouse.x - window_size.x + POINTER_SCROLL_PADDING;
            change = sf::Vector2f(POINTER_SCROLL_SPEED, 0);
        } else if (mouse.y < POINTER_SCROLL_PADDING) {
            // on évite de scroller sur la barre de titre (c'est chiant)
            if (mouse.y >= 0 || mouse.y < -ASSUME_TITLEBAR) {
                absolute_diff = POINTER_SCROLL_PADDING - mouse.y;
                change = sf::Vector2f(0, -POINTER_SCROLL_SPEED);
            }
        } else if (mouse.y > window_size.y - POINTER_SCROLL_PADDING) {
            absolute_diff = mouse.y - window_size.y + POINTER_SCROLL_PADDING;
            change = sf::Vector2f(0, POINTER_SCROLL_SPEED);
        }

        camera.move(change * std::min(
            1 + absolute_diff / POINTER_SCROLL_STEP,
            POINTER_SCROLL_MAX_SPEED
        ));

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
        selection_rect.setFillColor(SELECT_RECT_COLOR);
        selection_rect.setOutlineThickness(2.f);
        selection_rect.setOutlineColor(SELECT_RECT_BORDER_COLOR);

        window.draw(selection_rect);
    }

    // dessin du widget timer
    widget_timer.setTimeLeft(getTotalTime());
    widget_timer.draw(sf::Vector2f(window_size.x / 2 - 50, 0));

    // sélectionneur d'objet
    widget_selector.draw(
        sf::Vector2f(window_size.x - 64, 0),
        sf::Vector2f(64, window_size.y)
    );
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
    std::shared_ptr<Game> game = std::shared_ptr<Game>(new Game(getManager()));
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
    game->setTestMode(getManager().getState());
    getManager().setState(game);
}
