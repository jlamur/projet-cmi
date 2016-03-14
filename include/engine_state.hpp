#ifndef __PTF_ENGINE_STATE_HPP__
#define __PTF_ENGINE_STATE_HPP__

#include <vector>
#include <array>

// pré-déclaration de Object pour éviter les erreurs de compilation
// Object est définie dans object.hpp
class Object;

/**
 * Structure qui retient l'état actuel du moteur
 * Cette structure est passée aux objets pour qu'ils se
 * mettent à jour en fonction de cet état
 */
struct EngineState {
    std::vector<Object*> objects;
    std::array<bool, sf::Keyboard::KeyCount> keys;
    float delta;

    EngineState() : delta(0.f) {
        // aucune touche n'est enfoncée au démarrage
        keys.fill(false);
    }
};

#endif
