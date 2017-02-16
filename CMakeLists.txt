cmake_minimum_required(VERSION 3.1)
cmake_policy(SET CMP0048 NEW)

#########################
# MÉTADONNÉES DU PROJET #
#########################

# Nom et version du projet
project(skizzle VERSION 0.1.0 LANGUAGES CXX)
set(CPACK_PACKAGE_VERSION_MAJOR ${PROJECT_VERSION_MAJOR})
set(CPACK_PACKAGE_VERSION_MINOR ${PROJECT_VERSION_MINOR})
set(CPACK_PACKAGE_VERSION_PATCH ${PROJECT_VERSION_PATCH})

# Contact des mainteneurs·ses du projet
set(CPACK_PACKAGE_CONTACT "Maëlle Beuret <maelle.beuret@etu.umontpellier.fr>" "Rémi Cérès <remi.ceres@etu.umontpellier.fr>" "Mattéo Delabre <matteo.delabre@etu.umontpellier.fr>")

# Liste des exécutables du projet
set(CPACK_PACKAGE_EXECUTABLES "skizzle;Skizzle")

# Description du projet
set(CPACK_PACKAGE_DESCRIPTION_FILE "${CMAKE_SOURCE_DIR}/README.md")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Jeu inspiré des jeux de \
plateformes, des jeux de réflexion et de coopération.")

# Icône du projet
set(CPACK_PACKAGE_ICON "${CMAKE_SOURCE_DIR}/cmake/${CMAKE_PROJECT_NAME}-128.png")

############################
# CHEMINS VERS LES SOURCES #
############################

# Liste des fichiers sources (cette liste doit être tenue à jour à chaque ajout
# ou suppression d’un fichier source dans le dossier src/. Un fichier source
# non-présent dans cette liste ne serait pas lié à l’exécutable final.)
set(SOURCES
    "src/main.cpp"
    "src/manager.cpp"
    "src/resource_manager.cpp"
    "src/utility.cpp"
    "src/gui/action_toolbar.cpp"
    "src/gui/icon_radio_button.cpp"
    "src/gui/modal.cpp"
    "src/gui/object_toolbar.cpp"
    "src/objects/block.cpp"
    "src/objects/collision.cpp"
    "src/objects/finish_block.cpp"
    "src/objects/gravity_block.cpp"
    "src/objects/kill_block.cpp"
    "src/objects/object.cpp"
    "src/objects/player.cpp"
    "src/objects/switch_block.cpp"
    "src/states/editor.cpp"
    "src/states/game.cpp"
    "src/states/level.cpp"
    "src/states/menu.cpp"
    "src/states/rules.cpp"
    "src/states/state.cpp"
)

# Chemin vers les fichiers d'en-tête
include_directories(include)

################################
# CONFIGURATION DU COMPILATEUR #
################################

# Affichage de tous les avertisements
if(MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
elseif(CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX)
    set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "-Wall -Wno-long-long -pedantic")
endif()

# Activation du standard C++11
set(CMAKE_CXX_STANDARD 11)

##############################
# GÉNÉRATION DE L’EXÉCUTABLE #
##############################

add_executable(${CMAKE_PROJECT_NAME} ${SOURCES})

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake" ${CMAKE_MODULE_PATH})

# Liaison avec la librairie SFML
find_package(SFML 2.3.2 REQUIRED system window graphics network audio)
include_directories(${SFML_INCLUDE_DIR})
target_link_libraries(${CMAKE_PROJECT_NAME} ${SFML_LIBRARIES})

# Liaison avec la librairie Boost System et Filesystem
find_package(Boost 1.58.0 COMPONENTS system filesystem)
include_directories(${Boost_INCLUDE_DIRS})
target_link_libraries(${CMAKE_PROJECT_NAME} ${Boost_LIBRARIES})

# Liaison statique avec la librairie SFGUI
set(SFGUI_STATIC_LIBRARIES YES)
find_package(SFGUI 0.2.3 REQUIRED)
include_directories(${SFGUI_INCLUDE_DIR})
target_link_libraries(${CMAKE_PROJECT_NAME} ${SFGUI_LIBRARY} ${SFGUI_DEPENDENCIES})

###########################
# CRÉATION DES PAQUETAGES #
###########################

set(CPACK_MONOLITHIC_INSTALL YES)
set(CPACK_STRIP_FILES YES)

# Configuration spécifique à Windows
if(WIN32)
    set(CPACK_GENERATOR ${CPACK_GENERATOR} "NSIS")
    # TODO: ajout des instructions pour Windows
endif()

# Configuration spécifique à Debian et dérivés
if(EXISTS /etc/debian_version)
    set(CPACK_GENERATOR ${CPACK_GENERATOR} "DEB")
    set(CPACK_DEBIAN_PACKAGE_SECTION "games")
    set(CPACK_DEBIAN_PACKAGE_SHLIBDEPS YES)

    set(BINARY_PATH games)
    set(SHARE_PATH share/${CMAKE_PROJECT_NAME})
    set(PIXMAPS_PATH share/pixmaps)
    set(ICONS_PATH share/icons/hicolor)
    set(APPS_PATH share/applications)
endif()

# Installation de l'exécutable
install(
    TARGETS ${CMAKE_PROJECT_NAME}
    DIRECTORY_PERMISSIONS OWNER_EXECUTE OWNER_READ OWNER_WRITE
                GROUP_EXECUTE GROUP_READ
                WORLD_EXECUTE WORLD_READ
    RUNTIME DESTINATION games
)

# Installation de la documentation et des ressources
install(FILES README.md DESTINATION ${SHARE_PATH})
install(DIRECTORY res docs DESTINATION ${SHARE_PATH})

# Installation des icônes
install(
    FILES cmake/${CMAKE_PROJECT_NAME}.svg
    RENAME ${CMAKE_PROJECT_NAME}.svg
    DESTINATION ${PIXMAPS_PATH}
)

install(
    FILES ${CPACK_PACKAGE_ICON}
    RENAME ${CMAKE_PROJECT_NAME}.png
    DESTINATION ${PIXMAPS_PATH}
)

foreach(ICON_SIZE 16 32 48 64 128 256 512)
    install(
        FILES cmake/${CMAKE_PROJECT_NAME}-${ICON_SIZE}.png
        RENAME ${CMAKE_PROJECT_NAME}.png
        DESTINATION ${ICONS_PATH}/${ICON_SIZE}x${ICON_SIZE}/apps
    )
endforeach(ICON_SIZE)

# Installation du fichier d'application
install(
    FILES cmake/${CMAKE_PROJECT_NAME}.desktop
    DESTINATION ${APPS_PATH}
)

include(CPack)
