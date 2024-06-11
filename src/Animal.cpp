#include "../include/Animal.h"

Animal::Animal(int age, int maxAge, int hunger, int maxHunger, char gender)
        : _age(age), _maxAge(maxAge), _hunger(hunger), _maxHunger(maxHunger), _gender(gender), _x(0), _y(0), _deathLogged(false) {}

int Animal::getX() const {
    return _x;
}

int Animal::getY() const {
    return _y;
}

void Animal::setPosition(int x, int y) {
    _x = x;
    _y = y;
}

bool Animal::isAlive() const {
    return _age < _maxAge && _hunger < _maxHunger;
}

char Animal::getGender() const {
    return _gender;
}

void Animal::die() {
    _age = _maxAge;
}

void Animal::incrementAge() {
    _age++;
    if (_age >= _maxAge || _hunger >= _maxHunger) {
        die();
    }
}

bool Animal::isDeathLogged() const {
    return _deathLogged;
}

void Animal::setDeathLogged(bool logged) {
    _deathLogged = logged;
}