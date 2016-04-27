#include <cmath>
#include <algorithm>
#include "manager.hpp"
#include "resource_manager.hpp"
#include "states/game.hpp"
#include "states/editor.hpp"

/**
 * Définition des variables et fonctions globales internes
 * (accessibles uniquement dans ce fichier)
 */
namespace {
    const sf::Color SELECT_RECT_COLOR = sf::Color(33, 33, 33, 40);
    const sf::Color SELECT_RECT_BORDER_COLOR = sf::Color(33, 33, 33, 127);
    const sf::Color ZONE_POINT_COLOR = sf::Color(140, 15, 15, 255);
    const sf::Color ZONE_BORDER_COLOR = sf::Color(200, 15, 15, 255);

    const float WHEEL_SCROLL_SPEED = -7.f;
    const float POINTER_SCROLL_SPEED = 5.f;
    const int POINTER_SCROLL_PADDING = 10;
}

/**
 * Arrondit le vecteur donné à une position
 * sur la grille
 */
inline sf::Vector2f roundVectorToGrid(sf::Vector2f input) {
    input /= Manager::GRID;
    input.x = round(input.x);
    input.y = round(input.y);
    input *= Manager::GRID;
    return input;
}

Editor::Editor(Manager& manager) : Level(manager),
    drag_control_point(nullptr), drag_mode(Editor::DragMode::NONE) {

    // ajout des boutons dans la barre d'action
    action_toolbar.addButton(
        *ResourceManager::get().getImage("toolbar/icon_back.tga"),
        std::bind(&Manager::popState, &getManager())
    );

    mute_button = action_toolbar.addButton(
        *ResourceManager::get().getImage("toolbar/icon_music.tga"),
        []() {
            // on inverse le drapeau de muet
            ResourceManager::get().setMuted(
                !ResourceManager::get().isMuted()
            );
        }
    );

    action_toolbar.addSpacer(20, true, false);

    action_toolbar.addButton(
        *ResourceManager::get().getImage("toolbar/icon_no_music.tga")
    );

    action_toolbar.addSpacer(20, true, false);

    action_toolbar.addButton(
        *ResourceManager::get().getImage("toolbar/icon_save.tga"),
        std::bind(&Editor::save, this)
    );

    action_toolbar.addButton(
        *ResourceManager::get().getImage("toolbar/icon_test.tga"),
        std::bind(&Editor::test, this)
    );

    action_toolbar.addButton(
        *ResourceManager::get().getImage("toolbar/icon_gear.tga")
    );

    // signalement des sous-widgets
    getManager().addWidget(action_toolbar.getWindow());
    getManager().addWidget(object_toolbar.getWindow());
}

Editor::~Editor() {
    getManager().removeWidget(action_toolbar.getWindow());
    getManager().removeWidget(object_toolbar.getWindow());
}

void Editor::enable() {
    Level::enable();

    // attributs de la fenêtre
    getManager().setTitle(sf::String(L"Édition de ") + getName());
    getManager().getWindow().setFramerateLimit(Manager::FPS);

    // joue la musique de l'éditeur
    ResourceManager::get().playMusic("editor.ogg");

    // on affiche les barres d'outils
    action_toolbar.getWindow()->Show(true);
    object_toolbar.getWindow()->Show(true);
}

void Editor::processEvent(const sf::Event& event) {
    Level::processEvent(event);

    // lorsque l'on clique dans l'éditeur
    if (event.type == sf::Event::MouseButtonPressed) {
        sf::Vector2i mouse_position(event.mouseButton.x, event.mouseButton.y);
        sf::Vector2f position = pixelToCoords(mouse_position);
        Object::Ptr pointed_object = getObject(position);
        sf::Vector2f* control_point = getControlPoint(position);

        if (event.mouseButton.button == sf::Mouse::Left) {
            // clic sur un point de contrôle : déplacement du point
            if (control_point != nullptr) {
                drag_control_point = control_point;
                drag_mode = Editor::DragMode::CONTROL_POINT;
                return;
            }

            // clic + shift : sélection par rectangle de sélection
            if (getManager().isKeyPressed(Manager::Modifier::SHIFT)) {
                drag_start = mouse_position;
                drag_end = mouse_position;
                drag_mode = Editor::DragMode::SELECT_RECT;
                return;
            }

            // clic sur un objet : démarrage de la sélection libre
            if (pointed_object != nullptr) {
                if (getManager().isKeyPressed(Manager::Modifier::CONTROL)) {
                    drag_start = mouse_position;
                    drag_end = mouse_position;
                    drag_mode = Editor::DragMode::SELECT_BULK;

                    select(pointed_object, Editor::SelectionMode::ADD);
                } else {
                    select(pointed_object, Editor::SelectionMode::FLIP);
                }

                return;
            }

            // clic gauche dans le vide : démarrage du placement en drag&drop
            drag_start = mouse_position;
            drag_end = mouse_position;
            drag_mode = Editor::DragMode::PLACE;

            select(addObject(position), Editor::SelectionMode::REPLACE);
        }

        if (event.mouseButton.button == sf::Mouse::Right) {
            // clic droit sur un point de contrôle : suppression de ce point
            if (control_point != nullptr) {
                removeControlPoint(control_point);
                return;
            }

            // clic droit sur un objet : démarrage de la suppression en drag&drop
            if (pointed_object != nullptr) {
                drag_start = mouse_position;
                drag_end = mouse_position;
                drag_mode = Editor::DragMode::REMOVE;

                removeObject(pointed_object);
                return;
            }
        }
    }

    // lorsqu'on déplace la souris
    if (event.type == sf::Event::MouseMoved) {
        sf::Vector2i mouse_position(event.mouseMove.x, event.mouseMove.y);
        sf::Vector2f position = pixelToCoords(mouse_position);
        Object::Ptr pointed_object = getObject(position);

        drag_end = mouse_position;

        // mode déplacement de point de contrôle
        if (drag_mode == Editor::DragMode::CONTROL_POINT) {
            *drag_control_point = roundVectorToGrid(position);
        }

        // mode placement d'objets
        if (drag_mode == Editor::DragMode::PLACE && pointed_object == nullptr) {
            select(addObject(position), Editor::SelectionMode::ADD);
        }

        // mode suppression d'objets
        if (drag_mode == Editor::DragMode::REMOVE && pointed_object != nullptr) {
            removeObject(pointed_object);
        }

        // mode sélection libre : on ajoute l'objet à la sélection
        if (drag_mode == Editor::DragMode::SELECT_BULK) {
            select(position, Editor::SelectionMode::ADD);
        }

        return;
    }

    // lorsqu'on relâche un clic dans l'éditeur
    if (event.type == sf::Event::MouseButtonReleased) {
        // mode sélection rectangulaire : on applique la sélection
        if (drag_mode == Editor::DragMode::SELECT_RECT) {
            select(pixelToCoords(drag_start), pixelToCoords(drag_end));
        }

        drag_mode = Editor::DragMode::NONE;
    }

    // lorsqu'on scrolle on déplace la vue ou change la polarité
    if (event.type == sf::Event::MouseWheelScrolled) {
        // si on a Ctrl appuyé, on change la polarité
        if (getManager().isKeyPressed(Manager::Modifier::CONTROL)) {
            sf::Vector2i mouse_position(event.mouseWheelScroll.x, event.mouseWheelScroll.y);
            sf::Vector2f position = pixelToCoords(mouse_position);
            Object::Ptr pointed_object = getObject(position);

            if (pointed_object != nullptr) {
                float new_charge = pointed_object->getCharge() +
                    event.mouseWheelScroll.delta;

                if (new_charge != 0) {
                    new_charge /= std::abs(new_charge);
                }

                pointed_object->setCharge(new_charge);
            }
        }

        // sinon, on déplace la vue
        else {
            sf::View camera = getCamera();

            // la molette est horizontale ssi. elle l'est vraiment ou
            // si on utilise la molette verticale et shift
            bool horizontal = (
                event.mouseWheelScroll.wheel == sf::Mouse::HorizontalWheel ||
                (event.mouseWheelScroll.wheel == sf::Mouse::VerticalWheel &&
                getManager().isKeyPressed(Manager::Modifier::SHIFT))
            );

            if (!horizontal) {
                camera.move(
                    sf::Vector2f(0, event.mouseWheelScroll.delta) *
                    WHEEL_SCROLL_SPEED
                );
            } else {
                camera.move(
                    sf::Vector2f(event.mouseWheelScroll.delta, 0) *
                    WHEEL_SCROLL_SPEED
                );
            }

            setCamera(camera);
        }
    }

    // gestion des touches
    if (event.type == sf::Event::KeyPressed) {
        // appui sur suppr : suppression des blocs sélectionnés
        if (event.key.code == sf::Keyboard::Delete) {
            for (auto it = selection.begin(); it != selection.end();) {
                removeObject(*it);
            }

            clearSelection();
        }

        // appui sur Ctrl + S : sauvegarde du niveau
        if (event.key.code == sf::Keyboard::S && event.key.control) {
            save();
        }

        // appui sur Ctrl + A : sélection de tous les objets
        if (event.key.code == sf::Keyboard::A && event.key.control) {
            selectAll();
        }

        // appui sur espace : test du niveau en cours d'édition
        if (event.key.code == sf::Keyboard::Space) {
            test();
        }

        // touche retour ou échap : on quitte l'éditeur
        if (event.key.code == sf::Keyboard::BackSpace ||
            event.key.code == sf::Keyboard::Escape) {
            getManager().popState();
        }
    }
}

void Editor::frame() {
    sf::RenderWindow& window = getManager().getWindow();
    sf::Vector2i window_size = (sf::Vector2i) window.getSize();

    // dessin de la frame
    draw();

    // scroll de la caméra lorsque la souris se situe sur les bords
    if (window.hasFocus()) {
        sf::View camera = getCamera();
        sf::Vector2i mouse = sf::Mouse::getPosition(window);

        // détection du dépassement sur un des 4 bords
        if (mouse.x < POINTER_SCROLL_PADDING && mouse.x >= -POINTER_SCROLL_PADDING) {
            camera.move(sf::Vector2f(-POINTER_SCROLL_SPEED, 0));
        } else if (mouse.x >= window_size.x - POINTER_SCROLL_PADDING && mouse.x < window_size.x + POINTER_SCROLL_PADDING) {
            camera.move(sf::Vector2f(POINTER_SCROLL_SPEED, 0));
        } else if (mouse.y < POINTER_SCROLL_PADDING && mouse.y >= -POINTER_SCROLL_PADDING) {
            camera.move(sf::Vector2f(0, -POINTER_SCROLL_SPEED));
        } else if (mouse.y >= window_size.y - POINTER_SCROLL_PADDING && mouse.y < window_size.y + POINTER_SCROLL_PADDING) {
            camera.move(sf::Vector2f(0, POINTER_SCROLL_SPEED));
        }

        setCamera(camera);
    }

    // mise à jour de l'icône du mute
    sf::Image image;

    if (ResourceManager::get().isMuted()) {
        image = *ResourceManager::get().getImage("toolbar/icon_no_music.tga");
    } else {
        image = *ResourceManager::get().getImage("toolbar/icon_music.tga");
    }

    mute_button->SetImage(sfg::Image::Create(image));

    // màj du titre de la fenêtre
    getManager().setTitle(sf::String(L"Édition de ") + getName());

    // positionnement des barres d'outils au bon endroit
    action_toolbar.getWindow()->SetAllocation(sf::FloatRect(
        0, 0, window_size.x,
        action_toolbar.getHeight()
    ));

    object_toolbar.getWindow()->SetAllocation(sf::FloatRect(
        window_size.x - object_toolbar.getWidth(),
        action_toolbar.getHeight(),
        object_toolbar.getWidth(),
        window_size.y - action_toolbar.getHeight()
    ));
}

void Editor::draw() {
    sf::RenderWindow& window = getManager().getWindow();

    // dessin des objets du niveau
    Level::draw();

    // dessin de la zone de jeu
    const std::vector<sf::Vector2f>& zone = getZone();
    sf::VertexArray zone_polygon(sf::LinesStrip);
    control_points_circles.clear();

    for (auto it = zone.begin(); it != zone.end(); it++) {
        sf::CircleShape circle(5);
        sf::Vector2f position = *it;

        circle.setOrigin(sf::Vector2f(5, 5));
        circle.setFillColor(ZONE_POINT_COLOR);
        circle.setPosition(position);

        zone_polygon.append(sf::Vertex(position, ZONE_BORDER_COLOR));
        control_points_circles.push_back(circle);
    }

    zone_polygon.append(sf::Vertex(zone[0], ZONE_BORDER_COLOR));
    window.draw(zone_polygon);

    for (auto it = control_points_circles.begin(); it != control_points_circles.end(); it++) {
        window.draw(*it);
    }

    // dessin du rectangle de sélection
    if (drag_mode == Editor::DragMode::SELECT_RECT) {
        sf::Vector2f size = pixelToCoords(drag_end) - pixelToCoords(drag_start);
        sf::Vector2f pos = pixelToCoords(drag_start);

        sf::RectangleShape selection_rect(size);
        selection_rect.setPosition(pos);
        selection_rect.setFillColor(SELECT_RECT_COLOR);
        selection_rect.setOutlineThickness(2.f);
        selection_rect.setOutlineColor(SELECT_RECT_BORDER_COLOR);

        window.draw(selection_rect);
    }
}

Object::Ptr Editor::getObject(sf::Vector2f position) {
    const std::vector<Object::Ptr>& objects = getObjects();

    for (auto it = objects.begin(); it != objects.end(); it++) {
        if ((*it)->getAABB().contains(position)) {
            return *it;
        }
    }

    return nullptr;
}

sf::Vector2f* Editor::getControlPoint(sf::Vector2f position) {
    std::vector<sf::Vector2f>& zone = getZone();

    for (unsigned i = 0; i < zone.size(); i++) {
        if (control_points_circles[i].getGlobalBounds().contains(position)) {
            return &zone[i];
        }
    }

    return nullptr;
}

Object::Ptr Editor::addObject(sf::Vector2f position) {
    // on arrondit à l'unité de grille la plus proche
    position = roundVectorToGrid(position);
    Object::Ptr created_object = object_toolbar.createObject();

    if (created_object == nullptr) {
        return nullptr;
    }

    created_object->setPosition(position);

    // avant d'ajouter l'objet, on vérifie qu'il ne soit
    // pas superposé à un autre. Si c'est le cas, on annule
    // la procédure
    for (auto const &object : getObjects()) {
        if (object->getAABB().intersects(created_object->getAABB())) {
            return nullptr;
        }
    }

    // sinon, on ajoute l'objet
    return Level::addObject(created_object);
}

void Editor::removeObject(Object::Ptr object) {
    if (object == nullptr) {
        return;
    }

    // on supprime l'objet de la sélection
    selection.erase(std::remove(
        selection.begin(), selection.end(), object
    ), selection.end());

    Level::removeObject(object);

    // si c'était un joueur, il faut renuméroter
    // les autres pour plus de convenance
    if (object->getTypeId() == Player::TYPE_ID) {
        // on réattribue les numéros de joueurs
        for (unsigned int i = 0; i < getPlayers().size(); i++) {
            getPlayers()[i]->setPlayerNumber(i);
        }
    }
}

void Editor::removeControlPoint(sf::Vector2f* control_point) {
    if (control_point == nullptr) {
        return;
    }

    std::vector<sf::Vector2f>& zone = getZone();

    // on supprime le point de la liste
    zone.erase(std::remove(
        zone.begin(), zone.end(), *control_point
    ), zone.end());
}

void Editor::removeObject(sf::Vector2f position) {
    removeObject(getObject(position));
}

void Editor::select(Object::Ptr object, Editor::SelectionMode mode) {
    if (object == nullptr) {
        return;
    }

    bool already_selected = std::count(selection.begin(), selection.end(), object) > 0;

    // dans les modes REPLACE et FLIP, on remplace l'ancienne sélection
    // pour REPLACE, on sélectionne forcément l'objet
    // pour FLIP, on le sélectionne s'il ne l'est pas, on le désélectionne sinon
    if (mode == Editor::SelectionMode::REPLACE || mode == Editor::SelectionMode::FLIP) {
        clearSelection();

        // on resélectionne l'objet ssi. on force la sélection
        // ou s'il n'était pas déjà sélectionné
        if (!already_selected || mode == Editor::SelectionMode::REPLACE) {
            object->setSelected(true);
            selection.push_back(object);
        }
    }

    // dans le mode ADD, on rajoute juste l'objet à la sélection
    if (mode == Editor::SelectionMode::ADD && !already_selected) {
        object->setSelected(true);
        selection.push_back(object);
    }
}

void Editor::select(sf::Vector2f position, Editor::SelectionMode mode) {
    select(getObject(position), mode);
}

void Editor::select(sf::Vector2f top_left, sf::Vector2f bottom_right) {
    const std::vector<Object::Ptr>& objects = getObjects();
    sf::FloatRect selection_rect(
        std::min(top_left.x, bottom_right.x),
        std::min(top_left.y, bottom_right.y),
        std::abs(bottom_right.x - top_left.x),
        std::abs(bottom_right.y - top_left.y)
    );

    clearSelection();

    // sélection des éléments intersectant le rectangle
    for (auto it = objects.begin(); it != objects.end(); it++) {
        if ((*it)->getAABB().intersects(selection_rect)) {
            select(*it, Editor::SelectionMode::ADD);
        }
    }
}

void Editor::clearSelection() {
    for (auto it = selection.begin(); it != selection.end(); it++) {
        (*it)->setSelected(false);
    }

    selection.clear();
}

void Editor::selectAll() {
    const std::vector<Object::Ptr>& objects = getObjects();

    for (auto it = objects.begin(); it != objects.end(); it++) {
        (*it)->setSelected(true);
        selection.push_back(*it);
    }
}

void Editor::test() {
    auto game = std::unique_ptr<Game>(new Game(getManager()));
    clearSelection();

    // copie des propriétés
    game->setName(getName());
    game->setTotalTime(getTotalTime());
    game->setBackground(getBackground());
    game->setMusic(getMusic());

    // copie des objets du niveau vers le jeu
    std::vector<Object::Ptr>& objects = getObjects();
    game->getObjects().clear();

    for (auto it = objects.begin(); it != objects.end(); it++) {
        game->addObject((*it)->clone());
    }

    // copie de la zone de jeu
    std::vector<sf::Vector2f>& zone = getZone();
    game->getZone().clear();

    for (auto it = zone.begin(); it != zone.end(); it++) {
        game->getZone().push_back(*it);
    }

    getManager().pushState(std::move(game));
}
