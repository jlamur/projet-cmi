cmake_minimum_required(VERSION 2.8)
cmake_policy(SET CMP0048 NEW)

project(skizzle VERSION 0.1.0 LANGUAGES CXX)

# Inclusion des fichiers d'en-tête et de source
include_directories(include)
file(GLOB_RECURSE SOURCES "src/*.cpp")

# Affichage de tous les avertisements
if(MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
elseif(CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX)
    set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "-Wall -Wno-long-long -pedantic")
endif()

# Activation du standard C++11 sur le compilateur
if(CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX)
    include(CheckCXXCompilerFlag)
    CHECK_CXX_COMPILER_FLAG("-std=c++11" COMPILER_SUPPORTS_CXX11)
    CHECK_CXX_COMPILER_FLAG("-std=c++0x" COMPILER_SUPPORTS_CXX0X)

    if(COMPILER_SUPPORTS_CXX11)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
    elseif(COMPILER_SUPPORTS_CXX0X)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
    else()
        message(STATUS "Votre version de gcc ne supporte pas C++11. \
        Le projet ne compilera probablement pas. Les versions de gcc \
        compatibles sont 4.6 et supérieures.")
    endif()
elseif(MSVC)
    if(MSVC_VERSION LESS 1800)
        message(STATUS "Votre version de Visual Studio ne supporte \
        pas C++11. Le projet ne compilera probablement pas. Les versions \
        de Visual Studio compatibles sont 2012 et supérieures.")
    endif()
endif()

# Recherche des librairies
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake" ${CMAKE_MODULE_PATH})
add_executable(${CMAKE_PROJECT_NAME} ${SOURCES})

find_package(SFML 2.3.2 REQUIRED system window graphics network audio)
find_package(Boost 1.58.0 COMPONENTS system filesystem)
find_package(SFGUI 0.2.3 REQUIRED)

include_directories(${SFML_INCLUDE_DIR})
include_directories(${Boost_INCLUDE_DIRS})
include_directories(${SFGUI_INCLUDE_DIR})

target_link_libraries(${CMAKE_PROJECT_NAME} ${SFML_LIBRARIES})
target_link_libraries(${CMAKE_PROJECT_NAME} ${Boost_LIBRARIES})
target_link_libraries(${CMAKE_PROJECT_NAME} ${SFGUI_LIBRARY})

# Informations pour la création des packages
set(CPACK_PACKAGE_VERSION_MAJOR ${PROJECT_VERSION_MAJOR})
set(CPACK_PACKAGE_VERSION_MINOR ${PROJECT_VERSION_MINOR})
set(CPACK_PACKAGE_VERSION_PATCH ${PROJECT_VERSION_PATCH})
set(CPACK_PACKAGE_CONTACT "Maëlle Beuret, Rémi Cérès, Mattéo Delabre")
set(CPACK_PACKAGE_ICON "${CMAKE_SOURCE_DIR}/cmake/${CMAKE_PROJECT_NAME}-128.png")
set(CPACK_MONOLITHIC_INSTALL YES)
set(CPACK_PACKAGE_EXECUTABLES "skizzle;Skizzle")
set(CPACK_PACKAGE_DESCRIPTION_FILE "${CMAKE_SOURCE_DIR}/README.md")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Skizzle est un jeu inspiré des jeux de \
plateformes, des jeux de réflexion et de coopération.")

# Création des packages en fonction du système actuel
if(WIN32)
    set(CPACK_GENERATOR "NSIS")

    #############################################
    # TODO: ajout des instructions pour Windows #
    #############################################

elseif(UNIX AND NOT APPLE)
    set(CPACK_GENERATOR "DEB")
    set(CPACK_DEBIAN_PACKAGE_SECTION "games")

    # Installation de l'exécutable
    install(TARGETS ${CMAKE_PROJECT_NAME} DESTINATION games)

    # Installation de la documentation et des ressources
    install(FILES README.md DESTINATION share/${CMAKE_PROJECT_NAME})
    install(DIRECTORY res docs DESTINATION share/${CMAKE_PROJECT_NAME})

    # Installation des icônes
    install(FILES cmake/${CMAKE_PROJECT_NAME}.svg DESTINATION share/pixmaps)
    install(FILES ${CPACK_PACKAGE_ICON}
        RENAME ${CMAKE_PROJECT_NAME}.png
        DESTINATION share/pixmaps)

    foreach(ICON_SIZE 16 32 48 64 128 256 512)
        install(FILES cmake/${CMAKE_PROJECT_NAME}-${ICON_SIZE}.png
            RENAME ${CMAKE_PROJECT_NAME}.png
            DESTINATION share/icons/hicolor/${ICON_SIZE}x${ICON_SIZE}/apps)
    endforeach(ICON_SIZE)

    # Installation du fichier d'application
    install(FILES cmake/${CMAKE_PROJECT_NAME}.desktop
        DESTINATION share/applications)
else()
    message(STATUS "Votre plateforme n'a pas été configurée pour produire \
    des packages. La commande 'make package' ne fonctionnera pas, mais le \
    projet peut compiler et vous serez en mesure de l'exécuter.")
endif()

include(CPack)
