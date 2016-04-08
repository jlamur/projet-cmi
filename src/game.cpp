#include <cmath>
#include "game.hpp"
#include "player.hpp"
#include "constants.hpp"

Game::Game(Manager& manager) : Level(manager),
    widget_timer(manager, false),
    next_frame_time(manager.getCurrentTime()),
    test_mode(false), return_view(nullptr) {}

Game::~Game() {}

void Game::load(std::ifstream& file) {
    Level::load(file);
    manager.setTitle(getName());
}

void Game::frame() {
    // traitements généraux
    Level::frame();

    sf::Time current_time = manager.getCurrentTime();

    if (current_time >= next_frame_time) {
        // si nous sommes en retard ou dans les temps
        // on replanifie la prochaine frame
        next_frame_time += Constants::PHYSICS_TIME;

        // on met à jour la physique d'un cran temporel
        update();

        // on place la caméra à la position médiane des joueurs
        // s'ils sont trop éloignés
        std::vector<ObjectPtr>& objects = getObjects();
        sf::Vector2f position_first_player;
        sf::Vector2f position_second_player;

        for (unsigned int i = 0; i < objects.size(); i++) {
            if (Player* player = dynamic_cast<Player*>(objects[i].get())) {
                if (player->getPlayerNumber() == 0) {
                    position_first_player = player->getPosition();
                }

                if (player->getPlayerNumber() == 1) {
                    position_second_player = player->getPosition();
                }
            }
        }

        sf::Vector2f diff = position_second_player - position_first_player;
        sf::View camera = getCamera();

        if (std::pow(diff.x, 2) + std::pow(diff.y, 2) > std::pow(camera.getSize().x / 3, 2)) {
            sf::Vector2f middle = (position_second_player + position_first_player) / 2.f;
            camera.setCenter(floor(middle.x), floor(middle.y));
            setCamera(camera);
        }

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

void Game::processEvent(const sf::Event& event) {
    Level::processEvent(event);

    // appui sur espace en mode test : retour à l'éditeur
    if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Space && test_mode) {
        test_mode = false;
        manager.setView(return_view);
    }
}

void Game::draw() {
    sf::Vector2i window_size = (sf::Vector2i) manager.getWindow().getSize();

    // dessin des objets du niveau
    Level::draw();

    // dessin du timer
    widget_timer.setTimeLeft(getTotalTime());
    widget_timer.draw(sf::Vector2f(window_size.x / 2 - 50, 0));
}

void Game::update() {
    std::vector<CollisionData> colliding;

    // détection des objets en collision
    for (unsigned int i = 0; i < getObjects().size(); i++) {
        ObjectPtr objA = getObjects()[i];

        for (unsigned int j = i + 1; j < getObjects().size(); j++) {
            ObjectPtr objB = getObjects()[j];
            CollisionData data(*objA, *objB);

            if (objA->detectCollision(*objB, data)) {
                colliding.push_back(data);
            }
        }
    }

    // intégration des forces dans la vitesse (seconde moitié)
    for (unsigned int i = 0; i < getObjects().size(); i++) {
        getObjects()[i]->updateVelocity(manager, getObjects(), Constants::PHYSICS_TIME.asSeconds() / 2);
    }

    // résolution des collisions détectées
    for (unsigned int i = 0; i < colliding.size(); i++) {
        CollisionData& collided = colliding[i];
        collided.objA.solveCollision(collided.objB, collided.normal);
    }

    // intégration de la vitesse dans la position
    for (unsigned int i = 0; i < getObjects().size(); i++) {
        getObjects()[i]->updatePosition(Constants::PHYSICS_TIME.asSeconds());
    }

    // application de la correction positionnelle
    for (unsigned int i = 0; i < colliding.size(); i++) {
        CollisionData& collided = colliding[i];
        collided.objA.positionalCorrection(
            collided.objB, collided.normal, collided.depth
        );
    }
}

void Game::setTestMode(std::shared_ptr<View> set_return_view) {
    return_view = set_return_view;
    test_mode = true;
}
