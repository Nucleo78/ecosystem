
#include "../include/Tile.h"

Tile::Tile()
        : _animal(nullptr), _grass(new Grass()), _mineralSalts(0), _mineralSaltsTurnAdded(-1) {
    _grass->grow();
}

Tile::Tile(const Tile& other)
        : _animal(other._animal), _grass(new Grass(*other._grass)), _mineralSalts(other._mineralSalts) {}

Tile& Tile::operator=(const Tile& other) {
    if (this != &other) {
        delete _grass; // Libérer l'ancienne herbe
        _animal = other._animal;
        _grass = new Grass(*other._grass); // Copier l'instance de Grass
        _mineralSalts = other._mineralSalts;
    }
    return *this;
}

Tile::~Tile() {
    delete _grass;
}

bool Tile::isEmpty() const {
    return _animal == nullptr;
}

void Tile::addAnimal(Animal* a) {
    _animal = a;
}

void Tile::removeAnimal() {
    _animal = nullptr;
}

Animal* Tile::getAnimal() const {
    return _animal;
}

Grass* Tile::getGrass() const {
    return _grass;
}

void Tile::addMineralSalts(int amount, int currentTurn) {
    _mineralSalts += amount;
    if (amount > 0) {
        _mineralSaltsTurnAdded = currentTurn;
    }
}

int Tile::getMineralSalts() const {
    return _mineralSalts;
}

int Tile::getMineralSaltsTurnAdded() const {
    return _mineralSaltsTurnAdded;
}
