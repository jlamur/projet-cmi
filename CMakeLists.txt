cmake_minimum_required(VERSION 2.8)
project(skizzle)

# Inclusion des fichiers d'en-tête et de source
include_directories(include)
file(GLOB_RECURSE SOURCES "src/*.cpp")

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

# Activation du standard C++11 sur GCC
if(CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX)
    CHECK_CXX_COMPILER_FLAG("-std=c++11" COMPILER_SUPPORTS_CXX11)
    CHECK_CXX_COMPILER_FLAG("-std=c++0x" COMPILER_SUPPORTS_CXX0X)
    
    if(COMPILER_SUPPORTS_CXX11)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
    elseif(COMPILER_SUPPORTS_CXX0X)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
    else()
        message(STATUS "Votre version de gcc/g++ ne supporte pas C++11. Le projet ne compilera probablement pas.")
    endif()
endif()

# Recherche des librairies
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}" ${CMAKE_MODULE_PATH})
add_executable(skizzle ${SOURCES})

find_package(SFML 2.3.2 REQUIRED system window graphics network audio)
find_package(Boost 1.60.0 COMPONENTS system filesystem)
find_package(SFGUI 0.2.3 REQUIRED)

include_directories(${SFML_INCLUDE_DIR})
include_directories(${Boost_INCLUDE_DIRS})
include_directories(${SFGUI_INCLUDE_DIR})

target_link_libraries(skizzle ${SFML_LIBRARIES})
target_link_libraries(skizzle ${Boost_LIBRARIES})
target_link_libraries(skizzle ${SFGUI_LIBRARY})

# Installation de l'exécutable
install(TARGETS skizzle DESTINATION bin)
