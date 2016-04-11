#include <cmath>
#include "game.hpp"
#include "player.hpp"
#include "constants.hpp"

Game::Game(Manager& manager) : Level(manager),
    widget_timer(manager, false),
    next_frame_time(manager.getCurrentTime()),
    test_mode(false), return_state(nullptr) {}

Game::~Game() {}

void Game::begin() {
    Level::begin();

    if (getMusic() != "") {
        getResourceManager().playMusic(getMusic());
    } else {
        getResourceManager().stopMusic();
    }

    mode = Game::Mode::NORMAL;
    getWindow().setFramerateLimit(0);
}

void Game::processEvent(const sf::Event& event) {
    Level::processEvent(event);

    if (event.type == sf::Event::KeyPressed) {
        // appui sur espace en mode test : retour à l'éditeur
        if (event.key.code == sf::Keyboard::Space && test_mode) {
            test_mode = false;

            // on s'assure que la position de la caméra reste la même
            return_state->setCamera(getCamera());

            getManager().setState(return_state);
            return_state = nullptr;
        }

        // appui sur échap : échange entre le mode pause et normal
        if (event.key.code == sf::Keyboard::Escape) {
            if (mode == Game::Mode::NORMAL) {
                mode = Game::Mode::PAUSED;
            } else if (mode == Game::Mode::PAUSED) {
                mode = Game::Mode::NORMAL;
            }
        }
    }
}

void Game::frame() {
    // titre de la fenêtre
    getManager().setTitle(getName());
    sf::Time current_time = getManager().getCurrentTime();

    if (current_time >= next_frame_time) {
        // si nous sommes en retard ou dans les temps
        // on replanifie la prochaine frame
        next_frame_time += Manager::FRAME_TIME;

        // on met à jour la physique d'un cran temporel,
        // si on est en mode normal
        if (mode == Game::Mode::NORMAL) {
            update();
        }

        // on s'assure que la caméra soit centrée sur nos joueurs
        ensureCentered();

        // si on a encore suffisamment de temps, on dessine
        if (current_time < next_frame_time) {
            draw();
        }
    } else {
        // si nous sommes en avance, on endort le processus
        // le temps nécessaire pour revenir dans les temps
        sf::sleep(next_frame_time - current_time);
    }
}

void Game::draw() {
    sf::Vector2i window_size = (sf::Vector2i) getWindow().getSize();

    // dessin des objets du niveau
    Level::draw();

    // on passe au dessin d'éléments d'interface.
    // Changement de vue sur la vue par défaut
    getManager().useGUIView();

    // dessin du timer
    widget_timer.setTimeLeft(getTotalTime());
    widget_timer.draw(sf::Vector2f(window_size.x / 2 - 50, 0));
}

void Game::ensureCentered() {
    std::vector<Player::Ptr>& players = getPlayers();

    sf::Vector2f total_position;
    sf::View camera = getCamera();
    unsigned int player_count = players.size();

    for (unsigned int i = 0; i < player_count; i++) {
        total_position += players[i]->getPosition();
    }

    if (player_count == 0) {
        // on évite la division par zéro
        camera.setCenter(sf::Vector2f(0, 0));
    } else {
        // on place la caméra à la position médiane de tous les joueurs
        camera.setCenter(total_position / (float) player_count);
    }

    setCamera(camera);
}

void Game::update() {
    std::vector<CollisionData> colliding;
    std::vector<Object::Ptr>& objects = getObjects();

    // on tue les objets qui étaient déjà planifiés pour mourir
    for (auto it = pending_kill.begin(); it != pending_kill.end(); it++) {
        removeObject(*it);
    }

    pending_kill.empty();

    // détection des objets déplaçables en dehors de la zone de jeu
    for (auto it = objects.begin(); it != objects.end(); it++) {
        if (!isInZone(*it) && (*it)->getMass() != 0) {
            // l'objet est sorti de la zone, on le signale et on
            // planifie sa mort à la prochaine frame
            (*it)->kill(*this);
            pending_kill.push_back(*it);
        }
    }

    // détection des objets en collision
    for (unsigned int i = 0; i < objects.size(); i++) {
        Object::Ptr obj_a = objects[i];

        for (unsigned int j = i + 1; j < objects.size(); j++) {
            Object::Ptr obj_b = objects[j];
            CollisionData data;

            data.obj_a = obj_a;
            data.obj_b = obj_b;

            if (obj_a->detectCollision(obj_b, data)) {
                colliding.push_back(data);
            }
        }
    }

    // intégration des forces dans la vitesse (seconde moitié)
    for (unsigned int i = 0; i < objects.size(); i++) {
        objects[i]->updateVelocity(*this);
    }

    // résolution des collisions détectées
    for (unsigned int i = 0; i < colliding.size(); i++) {
        CollisionData& collided = colliding[i];
        collided.obj_a->solveCollision(*this, collided.obj_b, collided.normal);
    }

    // intégration de la vitesse dans la position
    for (unsigned int i = 0; i < objects.size(); i++) {
        objects[i]->updatePosition();
    }

    // application de la correction positionnelle
    for (unsigned int i = 0; i < colliding.size(); i++) {
        CollisionData& collided = colliding[i];
        collided.obj_a->positionalCorrection(
            collided.obj_b, collided.normal, collided.depth
        );
    }
}

/**
 * Implémentation tirée de
 * http://stackoverflow.com/a/2922778/3452708
 */
bool Game::isInZone(Object::Ptr object) {
    std::vector<sf::Vector2f>& zone = getZone();
    sf::Vector2f pos = object->getPosition();
    unsigned int vertices = getZone().size();
    bool result = false;

    for (unsigned int i = 0, j = vertices - 1; i < vertices; j = i++) {
        if (((zone[i].y > pos.y) != (zone[j].y > pos.y)) &&
           (pos.x < (zone[j].x - zone[i].x) * (pos.y - zone[i].y) /
           (zone[j].y - zone[i].y) + zone[i].x)) {
            result = !result;
        }
    }

    return result;
}

bool Game::getTestMode() {
    return test_mode;
}

void Game::setTestMode(std::shared_ptr<Editor> set_return_state) {
    return_state = set_return_state;
    test_mode = true;
}

Game::Mode Game::getMode() {
    return mode;
}

void Game::setMode(Game::Mode set_mode) {
    mode = set_mode;
}
