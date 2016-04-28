#include <iostream>
#include <cmath>
#include "manager.hpp"
#include "resource_manager.hpp"
#include "utility.hpp"
#include "objects/player.hpp"
#include "states/game.hpp"

/**
 * Définition des variables et fonctions globales internes
 * (accessibles uniquement dans ce fichier)
 */
namespace {
    // nombre maximum de frames que l'on peut sauter
    // en cas de charge CPU importante
    const unsigned int MAX_FRAME_SKIP = 5;
}

Game::Game(Manager& manager, bool test) : Level(manager),
    mode(Game::Mode::NORMAL), test(test),
    next_frame_time(manager.getCurrentTime()),
    skipped_frames(0),
    death_cause(Game::DeathCause::NONE) {

    // ajout des boutons d'action de la barre d'action
    if (!isTest()) {
        action_toolbar.addButton(
            *ResourceManager::get().getImage("toolbar/icon_restart.tga"),
            std::bind(&Game::restart, this)
        );

        action_toolbar.addButton(
            *ResourceManager::get().getImage("toolbar/icon_pause.tga"),
            std::bind(&Game::switchPause, this)
        );
    }
}

Game::~Game() {}

void Game::enable() {
    Level::enable();

    // attributs de la fenêtre
    getManager().setTitle(getName());
    getManager().getWindow().setFramerateLimit(0);

    // en dehors du mode test, on positionne la caméra directement
    // au centre des joueurs
    if (!isTest()) {
        sf::View camera = getCamera();
        camera.setCenter(getPlayerCenter());
        setCamera(camera);
    }

    // si musique il y a, on la joue
    if (getMusic() != "") {
        ResourceManager::get().playMusic("levels/" + getMusic());
    } else {
        ResourceManager::get().stopMusic();
    }
}

void Game::processEvent(const sf::Event& event) {
    Level::processEvent(event);

    if (event.type == sf::Event::KeyPressed) {
        // en mode test, retour, échap et espace pour revenir à l'éditeur
        if (isTest()) {
            if (event.key.code == sf::Keyboard::Space ||
                event.key.code == sf::Keyboard::Escape ||
                event.key.code == sf::Keyboard::BackSpace) {
                getManager().popState();
            }
        } else {
            // appui sur R : recommencer le niveau
            if (event.key.code == sf::Keyboard::R) {
                restart();
            }

            // appui sur espace : échange entre le mode pause et normal
            if (event.key.code == sf::Keyboard::Space) {
                switchPause();
            }

            // appui sur retour ou échap : sortie
            if (event.key.code == sf::Keyboard::Escape ||
                event.key.code == sf::Keyboard::BackSpace) {
                getManager().popState();
            }
        }
    }
}

void Game::frame() {
    Level::frame();
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
        setCenterGoal(getPlayerCenter());

        // si on a encore suffisamment de temps, ou si on a sauté
        // trop de frames, on dessine
        if (current_time < next_frame_time || skipped_frames >= MAX_FRAME_SKIP) {
            draw();
        }

        // sinon, on saute une frame de dessin. On stocke
        // le nombre de frames sautées pour ne pas sauter plus que
        // le maximum
        else {
            skipped_frames++;
        }
    } else {
        // si nous sommes en avance, on endort le processus
        // le temps nécessaire pour revenir dans les temps
        sf::sleep(next_frame_time - current_time);
    }
}

void Game::draw() {
    // dessin des objets du niveau
    Level::draw();
}

void Game::restart() {
    load();

    setGravityDirection(Utility::Direction::SOUTH);
    setMode(Game::Mode::NORMAL);
    setDeathCause(Game::DeathCause::NONE);
}

void Game::switchPause() {
    if (getMode() == Game::Mode::NORMAL) {
        setMode(Game::Mode::PAUSED);
    } else if (getMode() == Game::Mode::PAUSED) {
        setMode(Game::Mode::NORMAL);
    }
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
        if (obj_a->getMass() != 0 && !isInZone(obj_a)) {
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

            // si les objets ne sont pas sur la même couche,
            // ils ne peuvent pas entrer en collision
            if (obj_a->getLayer() != obj_b->getLayer()) {
                continue;
            }

            // si les deux boîtes englobantes des deux objets ne
            // s'intersectent pas, il ne risque pas d'y avoir de collision
            if (!obj_a->getAABB().intersects(obj_b->getAABB())) {
                continue;
            }

            CollisionData data;
            data.obj_a = obj_a;
            data.obj_b = obj_b;

            if (getCollisionData(data)) {
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

void Game::setDeathCause(Game::DeathCause set_death_cause) {
    death_cause = set_death_cause;
}

Game::DeathCause Game::getDeathCause() {
    return death_cause;
}

void Game::setMode(Game::Mode set_mode) {
    mode = set_mode;
}

void Game::setTotalTime(int set_total_time) {
    Level::setTotalTime(set_total_time);
    time_left = getTotalTime();
}

bool Game::isTest() {
    return test;
}
