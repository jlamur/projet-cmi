cmake_minimum_required(VERSION 3.1)
project(ptf)

# Inclusion des fichiers d'en-tête et de source
include_directories(include)
file(GLOB SOURCES "src/*.cpp")

# Affichage de tous les avertisements
if(MSVC)
    if(CMAKE_CXX_FLAGS MATCHES "/W[0-4]")
        string(REGEX REPLACE "/W[0-4]" "/W4" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
    else()
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
    endif()
elseif(CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wno-long-long -pedantic")
endif()

# Standard C++11
set(CMAKE_CXX_STANDARD 11)

# Recherche des librairies
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}" ${CMAKE_MODULE_PATH})
add_executable(ptf ${SOURCES})
find_package(SFML 2 REQUIRED system window graphics network audio)
target_link_libraries(ptf ${SFML_LIBRARIES})

# Installation de l'exécutable
install(TARGETS ptf DESTINATION bin)
