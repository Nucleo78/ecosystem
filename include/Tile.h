#ifndef TILE_H
#define TILE_H

#include "Grass.h"
#include "Animal.h"

class Tile {
private:
    Animal* _animal;
    Grass* _grass;
    int _mineralSalts;
    int _mineralSaltsTurnAdded;

public:
    Tile();
    Tile(const Tile& other); // Constructeur de copie
    Tile& operator=(const Tile& other); // Opérateur d'affectation
    ~Tile(); // Destructeur

    bool isEmpty() const;
    void addAnimal(Animal* a);
    void removeAnimal();
    Animal* getAnimal() const;
    Grass* getGrass() const;
    void addMineralSalts(int amount, int currentTurn);
    int getMineralSalts() const;
    int getMineralSaltsTurnAdded() const;
};

#endif // TILE_H