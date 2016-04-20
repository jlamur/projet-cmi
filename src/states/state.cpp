#include "manager.hpp"
#include "states/state.hpp"

State::State(Manager& manager) : manager(manager) {}
State::~State() {}

Manager& State::getManager() {
    return manager;
}

const Manager& State::getManager() const {
    return manager;
}
