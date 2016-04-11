#include <iostream>
#include <cmath>
#include "game.hpp"
#include "player.hpp"
#include "constants.hpp"

Game::Game(Manager& manager) : Level(manager),
    widget_timer(manager, false),
    next_frame_time(manager.getCurrentTime()) {

    if (getMusic() != "") {
        getResourceManager().playMusic(getMusic());
    } else {
        getResourceManager().stopMusic();
    }

    mode = Game::Mode::NORMAL;
    death_cause = Game::DeathCause::NONE;

    time_left = getTotalTime();
    getWindow().setFramerateLimit(0);
}

Game::~Game() {}

void Game::processEvent(const sf::Event& event) {
    Level::processEvent(event);

    if (event.type == sf::Event::KeyPressed) {
        // appui sur espace : retour
        if (event.key.code == sf::Keyboard::Space) {
            getManager().popState();
        }

        // appui sur échap : échange entre le mode pause et normal
        if (event.key.code == sf::Keyboard::Escape) {
            if (getMode() == Game::Mode::NORMAL) {
                setMode(Game::Mode::PAUSED);
            } else if (getMode() == Game::Mode::PAUSED) {
                setMode(Game::Mode::NORMAL);
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
        if (getMode() == Game::Mode::NORMAL) {
            update();
        } else {
            // TODO: pour le débogage affichage du mode actuel
            switch (getMode()) {
            case Game::Mode::NORMAL:
                std::cout << "<< Reprise >>" << std::endl;
                break;
            case Game::Mode::PAUSED:
                std::cout << "<< En pause >>" << std::endl;
                break;
            case Game::Mode::WON:
                std::cout << "<< Gagné ! >>" << std::endl;
                break;
            case Game::Mode::LOST:
                std::cout << "<< Perdu : ";

                switch (getDeathCause()) {
                case Game::DeathCause::OUT_OF_BOUNDS:
                    std::cout << "sortie du cadre";
                    break;
                case Game::DeathCause::KILLED:
                    std::cout << "tué par bloc";
                    break;
                case Game::DeathCause::TIME_OUT:
                    std::cout << "temps écoulé";
                    break;
                case Game::DeathCause::NONE:
                    std::cout << "sans aucune raison";
                    break;
                }

                std::cout << " ! >>" << std::endl;
                break;
            }
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
    widget_timer.setTimeLeft(
        std::max(std::floor(time_left), 0.f)
    );

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

    // on décrémente le temps, si le temps est nul,
    // on a perdu
    if (time_left <= 0) {
        setMode(Game::Mode::LOST);
        setDeathCause(Game::DeathCause::TIME_OUT);
        return;
    } else {
        time_left -= Manager::FRAME_TIME.asSeconds();
    }

    // on tue les objets qui étaient déjà planifiés pour mourir
    for (auto it = pending_kill.begin(); it != pending_kill.end(); it++) {
        removeObject(*it);
    }

    pending_kill.empty();

    // détection des objets en collision
    for (auto it = objects.begin(); it != objects.end(); it++) {
        Object::Ptr obj_a = *it;

        // l'objet est sorti de la zone, on le signale et on
        // planifie sa mort à la prochaine frame
        if (!isInZone(obj_a) && obj_a->getMass() != 0) {
            kill(obj_a);

            // si c'était un joueur, on a perdu
            if (obj_a->getTypeId() == Player::TYPE_ID) {
                setMode(Game::Mode::LOST);
                setDeathCause(Game::DeathCause::OUT_OF_BOUNDS);
            }
        }

        // on regarde s'il est en collision avec
        // d'autres objets
        for (auto jt = it + 1; jt != objects.end(); jt++) {
            Object::Ptr obj_b = *jt;
            CollisionData data;

            data.obj_a = obj_a;
            data.obj_b = obj_b;

            if (obj_a->detectCollision(obj_b, data)) {
                colliding.push_back(data);
            }
        }
    }

    // intégration des forces dans la vitesse
    for (auto it = objects.begin(); it != objects.end(); it++) {
        (*it)->updateVelocity(*this);
    }

    // résolution des collisions détectées
    for (auto it = colliding.begin(); it != colliding.end(); it++) {
        it->obj_a->solveCollision(*this, it->obj_b, it->normal);
    }

    // intégration de la vitesse dans la position
    for (auto it = objects.begin(); it != objects.end(); it++) {
        (*it)->updatePosition();
    }

    // application de la correction positionnelle
    for (auto it = colliding.begin(); it != colliding.end(); it++) {
        it->obj_a->positionalCorrection(it->obj_b, it->normal, it->depth);
    }
}

void Game::kill(Object::Ptr object) {
    pending_kill.push_back(object);
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

Game::Mode Game::getMode() {
    return mode;
}

Game::DeathCause Game::getDeathCause() {
    return death_cause;
}

void Game::setMode(Game::Mode set_mode) {
    mode = set_mode;
}

void Game::setDeathCause(Game::DeathCause set_death_cause) {
    death_cause = set_death_cause;
}
