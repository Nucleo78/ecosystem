#include <cstdlib>
#include "../include/Wolf.h"

Wolf::Wolf(int age, int maxAge, int hunger, int maxHunger, char gender, int x, int y, int id)
        : Animal(age, maxAge, hunger, maxHunger, gender), _isPregnant(false), _id(id) {
    setPosition(x, y);
}

void Wolf::move() {
    // The actual movement logic is now handled in Universe::moveAnimal
}

bool Wolf::eat(Sheep* sheep) {
    if (sheep != nullptr && sheep->isAlive()) {
        sheep->die();
        _hunger = 0;
        return true;
    } else {
        _hunger++;
        return false;
    }
}

void Wolf::die() {
    _age = _maxAge;
}

void Wolf::incrementAge() {
    _age++;
    if (_age >= _maxAge || _hunger >= _maxHunger) {
        die();
    }
}

bool Wolf::canReproduceWith(Animal* other) {
    Wolf* otherWolf = dynamic_cast<Wolf*>(other);
    return otherWolf != nullptr && otherWolf->getGender() != _gender && otherWolf->isAlive() && !otherWolf->isPregnant();
}

bool Wolf::isPregnant() const {
    return _isPregnant;
}

void Wolf::setPregnant(bool pregnant) {
    _isPregnant = pregnant;
}

int Wolf::getId() const {
    return _id;
}