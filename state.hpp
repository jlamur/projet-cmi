#ifndef PTF_STATE_HPP
#define PTF_STATE_HPP

#include <vector>
#include <memory>

/**
 * Structure qui retient l'état actuel du moteur
 * Cette structure est passée aux objets pour qu'ils se
 * mettent à jour en fonction de cet état
 */
struct State {
    std::vector<std::unique_ptr<Object>> objects;
    bool goLeftKey;
    bool goRightKey;
    float delta;
};

#endif
