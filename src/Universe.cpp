#include "../include/Universe.h"
#include <iostream>
#include <cstdlib>
#include <ctime>

Universe::Universe(int width, int height)
        : _width(width), _height(height), _grid(width, std::vector<Tile>(height)), _sheepCounter(0), _wolfCounter(0), _currentTurn(0) {}

void Universe::initialize(int numSheep, int numWolves, unsigned int seed) {
    if (seed != 0) {
        std::srand(seed);
    } else {
        std::srand(std::time(0));
    }
    placeAnimalsRandomly(numSheep, numWolves);
}

void Universe::placeAnimalsRandomly(int numSheep, int numWolves) {
    // Répartition des moutons
    for (int i = 0; i < numSheep; ++i) {
        int x, y;
        do {
            x = std::rand() % _width;
            y = std::rand() % _height;
        } while (!_grid[x][y].isEmpty());

        char gender = (i % 2 == 0) ? 'F' : 'M'; // Alternance des sexes
        auto sheep = std::make_shared<Sheep>(0, 50, 0, 5, gender, x, y, _sheepCounter++);
        _sheepList.push_back(sheep);
        _grid[x][y].addAnimal(sheep.get());
    }

    // Répartition des loups
    for (int i = 0; i < numWolves; ++i) {
        int x, y;
        do {
            x = std::rand() % _width;
            y = std::rand() % _height;
        } while (!_grid[x][y].isEmpty());

        char gender = (i % 2 == 0) ? 'F' : 'M'; // Alternance des sexes
        auto wolf = std::make_shared<Wolf>(0, 60, 0, 10, gender, x, y, _wolfCounter++); // Utilisation de std::make_shared
        _wolfList.push_back(wolf);
        _grid[x][y].addAnimal(wolf.get());
    }
}

void Universe::simulateTurn() {
    updateAnimals();
    reproduceAnimals();
    growGrass();
    updateNbSheeps();
    updateNbWolves();
    _currentTurn++;
}

void Universe::moveAnimal(Animal* animal) {
    if (animal == nullptr) return;

    int x = animal->getX();
    int y = animal->getY();

    int newX = x + (std::rand() % 3 - 1); // Mouvement aléatoire dans la grille (-1, 0, 1)
    int newY = y + (std::rand() % 3 - 1); // Mouvement aléatoire dans la grille (-1, 0, 1)

    if (newX >= 0 && newX < _width && newY >= 0 && newY < _height) {
        if (_grid[newX][newY].isEmpty()) {
            _grid[x][y].removeAnimal();
            _grid[newX][newY].addAnimal(animal);
            animal->setPosition(newX, newY);
        }
    }
}

void Universe::updateAnimals() {
    std::vector<std::string> deathLogs;
    updateSheep();
    updateWolves();
    handleDeathEvents(deathLogs);
}

void Universe::updateSheep() {
    for (auto& sheep : _sheepList) {
        if (sheep->isAlive()) {
            moveAnimal(sheep.get());
            bool grassEaten = sheep->eat(_grid[sheep->getX()][sheep->getY()].getGrass());
            if (grassEaten) {
                logEvent("Sheep " + std::to_string(sheep->getId()) + " ate grass at (" + std::to_string(sheep->getX()) + ", " + std::to_string(sheep->getY()) + ")");
            }
            sheep->incrementAge();
        }
        if (!sheep->isAlive() && !sheep->isDeathLogged()) {
            _grid[sheep->getX()][sheep->getY()].addMineralSalts(1, _currentTurn);
            logEvent("Sheep " + std::to_string(sheep->getId()) + " died of hunger or old age");
            _grid[sheep->getX()][sheep->getY()].removeAnimal();
            sheep->setDeathLogged(true);
        }
    }
}

void Universe::updateWolves() {
    for (auto& wolf : _wolfList) {
        if (wolf->isAlive()) {
            moveAnimal(wolf.get());

            bool hasEaten = false;
            for (int dx = -1; dx <= 1 && !hasEaten; ++dx) {
                for (int dy = -1; dy <= 1 && !hasEaten; ++dy) {
                    if (dx != 0 || dy != 0) {
                        int x = wolf->getX() + dx;
                        int y = wolf->getY() + dy;
                        if (x >= 0 && x < _width && y >= 0 && y < _height) {
                            Sheep* prey = dynamic_cast<Sheep*>(_grid[x][y].getAnimal());
                            if (prey != nullptr && prey->isAlive()) {
                                logEvent("Wolf " + std::to_string(wolf->getId()) + " ate Sheep " + std::to_string(prey->getId()) + " at (" + std::to_string(x) + ", " + std::to_string(y) + ")");
                                wolf->eat(prey);
                                hasEaten = true;
                            }
                        }
                    }
                }
            }

            if (!hasEaten) {
                wolf->incrementAge();
            }

            if (!wolf->isAlive() && !wolf->isDeathLogged()) {
                _grid[wolf->getX()][wolf->getY()].addMineralSalts(1, _currentTurn);
                logEvent("Wolf " + std::to_string(wolf->getId()) + " died of hunger");
                _grid[wolf->getX()][wolf->getY()].removeAnimal();
                wolf->setDeathLogged(true);
            }
        }
    }
}

void Universe::handleDeathEvents(std::vector<std::string>& deathLogs) {
    for (const auto& log : deathLogs) {
        logEvent(log);
    }

    for (auto& sheep : _sheepList) {
        if (!sheep->isAlive()) {
            sheep->setPregnant(false);
        }
    }
    for (auto& wolf : _wolfList) {
        if (!wolf->isAlive()) {
            wolf->setPregnant(false);
        }
    }
}

void Universe::growGrass() {
    for (int x = 0; x < _width; ++x) {
        for (int y = 0; y < _height; ++y) {
            Tile& tile = _grid[x][y];
            if (!tile.getGrass()->isGrown() && tile.getMineralSalts() > 0) {
                if (_currentTurn > tile.getMineralSaltsTurnAdded()) {
                    tile.getGrass()->grow();
                    tile.addMineralSalts(-1, _currentTurn); // Consommation du sel minéral
                    logEvent("Grass grew at (" + std::to_string(x) + ", " + std::to_string(y) + ")");
                }
            }
        }
    }
}

void Universe::reproduceAnimals() {
    reproduceSheep();
    reproduceWolves();
}

void Universe::reproduceSheep() {
    std::vector<std::shared_ptr<Sheep>> newSheep;
    for (auto& sheep : _sheepList) {
        if (sheep->isAlive() && !sheep->isPregnant()) {
            for (int dx = -1; dx <= 1; ++dx) {
                for (int dy = -1; dy <= 1; ++dy) {
                    if (dx != 0 || dy != 0) {
                        int x = sheep->getX() + dx;
                        int y = sheep->getY() + dy;
                        if (x >= 0 && x < _width && y >= 0 && y < _height) {
                            Sheep* other = dynamic_cast<Sheep*>(_grid[x][y].getAnimal());
                            if (sheep->canReproduceWith(other)) {
                                for (int nx = -1; nx <= 1; ++nx) {
                                    for (int ny = -1; ny <= 1; ++ny) {
                                        int newX = sheep->getX() + nx;
                                        int newY = sheep->getY() + ny;
                                        if (newX >= 0 && newX < _width && newY >= 0 && newY < _height && _grid[newX][newY].isEmpty()) {
                                            char gender = (newSheep.size() % 2 == 0) ? 'F' : 'M';
                                            auto newSheepPtr = std::make_shared<Sheep>(0, 50, 0, 5, gender, newX, newY, _sheepCounter++);
                                            newSheep.push_back(newSheepPtr);
                                            _grid[newX][newY].addAnimal(newSheepPtr.get());
                                            logEvent("Sheep " + std::to_string(newSheepPtr->getId()) + " born at (" + std::to_string(newX) + ", " + std::to_string(newY) + ")");
                                            sheep->setPregnant(true);
                                            other->setPregnant(true);
                                            goto sheep_reproduced;
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
                sheep_reproduced: ;
            }
        }
    }

    _sheepList.insert(_sheepList.end(), newSheep.begin(), newSheep.end());
}

void Universe::reproduceWolves() {
    std::vector<std::shared_ptr<Wolf>> newWolves;
    for (auto& wolf : _wolfList) {
        if (wolf->isAlive() && !wolf->isPregnant()) {
            for (int dx = -1; dx <= 1; ++dx) {
                for (int dy = -1; dy <= 1; ++dy) {
                    if (dx != 0 || dy != 0) {
                        int x = wolf->getX() + dx;
                        int y = wolf->getY() + dy;
                        if (x >= 0 && x < _width && y >= 0 && y < _height) {
                            Wolf* other = dynamic_cast<Wolf*>(_grid[x][y].getAnimal());
                            if (wolf->canReproduceWith(other)) {
                                for (int nx = -1; nx <= 1; ++nx) {
                                    for (int ny = -1; ny <= 1; ++ny) {
                                        int newX = wolf->getX() + nx;
                                        int newY = wolf->getY() + ny;
                                        if (newX >= 0 && newX < _width && newY >= 0 && newY < _height && _grid[newX][newY].isEmpty()) {
                                            char gender = (newWolves.size() % 2 == 0) ? 'F' : 'M';
                                            auto newWolfPtr = std::make_shared<Wolf>(0, 60, 0, 10, gender, newX, newY, _wolfCounter++);
                                            newWolves.push_back(newWolfPtr);
                                            _grid[newX][newY].addAnimal(newWolfPtr.get());
                                            logEvent("Wolf " + std::to_string(newWolfPtr->getId()) + " born at (" + std::to_string(newX) + ", " + std::to_string(newY) + ")");
                                            wolf->setPregnant(true);
                                            other->setPregnant(true);
                                            goto wolf_reproduced;
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
                wolf_reproduced: ;
            }
        }
    }

    _wolfList.insert(_wolfList.end(), newWolves.begin(), newWolves.end());
}

void Universe::printState() const {
    //_sheepList[9]->displayInfo();
    std::cout << "Universe state:\n";

    for (int x = 0; x < _width; ++x) {
        std::cout << "+---";
    }
    std::cout << "+\n";
    for (int y = 0; y < _height; ++y) {
        for (int x = 0; x < _width; ++x) {
            const Tile& tile = _grid[x][y];
            const Animal* animal = tile.getAnimal();
            const Grass* grass = tile.getGrass();

            std::cout << "| ";
            if (animal == nullptr) {
                if (grass->isGrown()) {
                    std::cout << "G";
                } else {
                    std::cout << " ";
                }
            } else if (dynamic_cast<const Wolf*>(animal) != nullptr) {
                std::cout << "W";
            } else if (dynamic_cast<const Sheep*>(animal) != nullptr) {
                std::cout << "S";
            } else {
                std::cout << " ";
            }
            std::cout << " ";
        }
        std::cout << "|\n";

        for (int x = 0; x < _width; ++x) {
            std::cout << "+---";
        }
        std::cout << "+\n";
    }
    std::cout << "G: Case avec de l'herbe (Grass)    W: case avec un Loup (Wolf)    S: case avec un mouton (Sheep)" << std::endl;
}

bool Universe::hasAliveAnimals() const {
    for (const auto& sheep : _sheepList) {
        if (sheep->isAlive()) return true;
    }
    for (const auto& wolf : _wolfList) {
        if (wolf->isAlive()) return true;
    }
    return false;
}

void Universe::logEvent(const std::string& event) {
    std::cout << event << std::endl;
}

void Universe::updateNbWolves() {
    int nbWolf = 0;
    for (const auto& wolf : _wolfList) {
        if (wolf->isAlive()) {
            nbWolf++;
        }
    }
    setNbWolf(nbWolf);
}

void Universe::updateNbSheeps() {
    int nbSheep = 0;
    for (const auto& sheep : _sheepList) {
        if (sheep->isAlive()) {
            nbSheep++;
        }
    }
    setNbSheep(nbSheep);
}

void Universe::setNbWolf(int nbWolf) {
    _nbWolf = nbWolf;
}

void Universe::setNbSheep(int nbSheep) {
    _nbSheep = nbSheep;
}

int Universe::getNbWolf() const {
    return _nbWolf;
}

int Universe::getNbSheep() const {
    return _nbSheep;
}
