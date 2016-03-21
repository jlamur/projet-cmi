cmake_minimum_required(VERSION 2.8)
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
include(CheckCXXCompilerFlag)

CHECK_CXX_COMPILER_FLAG("-std=c++11" COMPILER_SUPPORTS_CXX11)
CHECK_CXX_COMPILER_FLAG("-std=c++0x" COMPILER_SUPPORTS_CXX0X)

if(COMPILER_SUPPORTS_CXX11)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
elseif(COMPILER_SUPPORTS_CXX0X)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
else()
    message(STATUS "Le compilateur actuel (${CMAKE_CXX_COMPILER}) ne supporte pas C++11. Merci d'utiliser un autre compilateur.")
endif()

# Recherche des librairies
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}" ${CMAKE_MODULE_PATH})
add_executable(ptf ${SOURCES})
find_package(SFML 2 REQUIRED system window graphics network audio)
target_link_libraries(ptf ${SFML_LIBRARIES})

# Installation de l'exécutable
install(TARGETS ptf DESTINATION bin)
