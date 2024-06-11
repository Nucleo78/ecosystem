#ifndef UNIVERSE_H
#define UNIVERSE_H

#include <vector>
#include <memory>
#include "Tile.h"
#include "Sheep.h"
#include "Wolf.h"

class Universe {
private:
    int _width;
    int _height;
    std::vector<std::vector<Tile>> _grid;
    std::vector<std::shared_ptr<Sheep>> _sheepList;
    std::vector<std::shared_ptr<Wolf>> _wolfList;
    int _sheepCounter = 0;
    int _wolfCounter = 0;
    int _currentTurn;
    int _nbSheep = 0;
    int _nbWolf = 0;

public:
    Universe(int width, int height);

    void initialize(int numSheep, int numWolves, unsigned int seed = 0);
    void simulateTurn();
    void printState() const;
    bool hasAliveAnimals() const;
    void updateNbWolves();
    void updateNbSheeps();

    void moveAnimal(Animal* animal);

    int getNbWolf() const;
    int getNbSheep() const;

private:
    void placeAnimalsRandomly(int numSheep, int numWolves);
    void updateAnimals();
    void growGrass();
    void reproduceAnimals();
    void logEvent(const std::string& event);
    void updateSheep();
    void updateWolves();
    void handleDeathEvents(std::vector<std::string>& deathLogs);
    void reproduceSheep();
    void reproduceWolves();

    void setNbWolf(int nbWolf);
    void setNbSheep(int nbSheep);
};

#endif // UNIVERSE_H