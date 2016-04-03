#include "editor.hpp"
#include "constants.hpp"

Editor::Editor(Manager& manager) : Level(manager) {
    // activation de la synchronisation verticale
    // car, dans l'éditeur, nous n'avons besoin que de dessiner
    // (pas de mise à jour physique)
    manager.getWindow().setVerticalSyncEnabled(true);
}

Editor::~Editor() {}

void Editor::frame() {
    draw();
}
