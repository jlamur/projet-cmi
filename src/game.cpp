#include "game.hpp"
#include "constants.hpp"

Game::Game(Manager& manager) : Level(manager),
    next_frame_time(manager.getCurrentTime()),
    test_mode(false), return_view(nullptr) {

    // on s'assure que la synchronisation verticale soit
    // bien désactivée : on s'occupe de la limitation du
    // framerate manuellement dans la fonction frame pour mettre
    // une mise à jour fluide de la physique du jeu
    manager.getWindow().setVerticalSyncEnabled(false);
}

Game::~Game() {}

void Game::load(std::ifstream& file) {
    Level::load(file);
    manager.setTitle(getName());
}

void Game::frame() {
    const std::vector<sf::Event>& events = manager.getEvents();
    sf::Time current_time = manager.getCurrentTime();

    // traitement des événements
    for (unsigned int i = 0; i < events.size(); i++) {
        const sf::Event& event = events[i];

        // appui sur espace en mode test : retour à l'éditeur
        if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Space && test_mode) {
            test_mode = false;
            manager.setView(return_view);
            return; // important : ne pas dessiner la frame
            // on risque d'avoir perdu le pointeur en changeant de vue
        }
    }

    if (current_time >= next_frame_time) {
        // si nous sommes en retard ou dans les temps
        // on replanifie la prochaine frame
        next_frame_time += Constants::PHYSICS_TIME;

        // on met à jour la physique d'un cran temporel
        update();

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

    // intégration des forces dans la vitesse (première moitié)
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

    // intégration des forces dans la vitesse (seconde moitié)
    for (unsigned int i = 0; i < getObjects().size(); i++) {
        getObjects()[i]->updateVelocity(manager, getObjects(), Constants::PHYSICS_TIME.asSeconds() / 2);
    }
}

void Game::setTestMode(std::shared_ptr<View> set_return_view) {
    return_view = set_return_view;
    test_mode = true;
}
