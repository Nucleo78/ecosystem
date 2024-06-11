#include <cstdlib>
#include <iostream>
#include "../include/Sheep.h"

Sheep::Sheep(int age, int maxAge, int hunger, int maxHunger, char gender, int x, int y, int id)
        : Animal(age, maxAge, hunger, maxHunger, gender), _isPregnant(false), _id(id) {
    setPosition(x, y);
}

void Sheep::move() {
    // The actual movement logic is now handled in Universe::moveAnimal
}

bool Sheep::eat(Grass* grass) {
    if (grass->isGrown()) {
        grass->beEaten();
        _hunger = 0;
        return true;
    } else {
        _hunger++;
        return false;
    }
}

void Sheep::die() {
    _age = _maxAge + 1;
}

void Sheep::incrementAge() {
    _age++;
    if (_age >= _maxAge || _hunger >= _maxHunger) {
        die();
    }
}

bool Sheep::canReproduceWith(Animal* other) {
    Sheep* otherSheep = dynamic_cast<Sheep*>(other);
    return otherSheep != nullptr && otherSheep->getGender() != _gender && otherSheep->isAlive() && !otherSheep->isPregnant();
}

bool Sheep::isPregnant() const {
    return _isPregnant;
}

void Sheep::setPregnant(bool pregnant) {
    _isPregnant = pregnant;
}

int Sheep::getId() const {
    return _id;
}

void Sheep::displayInfo() {
    std::cout << "ID: " << _id << std::endl;
    std::cout << "Hunger: " << _hunger << "/" << _maxHunger << std::endl;
    std::cout << "Age: " << _age << "/" << _maxAge << std::endl;
    std::cout << "Gender: " << _gender << std::endl;

}
