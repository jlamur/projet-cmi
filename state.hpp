#ifndef __PTF_STATE_HPP__
#define __PTF_STATE_HPP__

#include <vector>

// pré-déclaration de Object pour éviter
// les erreurs de compilation. Object
// est défini dans object.hpp
class Object;

/**
 * Structure qui retient l'état actuel du moteur
 * Cette structure est passée aux objets pour qu'ils se
 * mettent à jour en fonction de cet état
 */
struct State {
    std::vector<Object*> objects;
    bool goLeftKey;
    bool goRightKey;
    float delta;
};

#endif
