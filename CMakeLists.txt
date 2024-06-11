cmake_minimum_required(VERSION 3.10)

# Nom du projet
project(EcosystemSimulation)

# Définir le standard C++
set(CMAKE_CXX_STANDARD 17)

# Inclure les répertoires des en-têtes
include_directories(include)

# Ajouter les fichiers source
set(SOURCES
        src/main.cpp
        src/Universe.cpp
        src/Tile.cpp
        src/Grass.cpp
        src/Animal.cpp
        src/Sheep.cpp
        src/Wolf.cpp
)

# Définir l'exécutable
add_executable(EcosystemSimulation ${SOURCES})