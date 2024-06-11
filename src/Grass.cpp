#include "../include/Grass.h"

Grass::Grass()
        : _grown(false) {}

bool Grass::isGrown() const {
    return _grown;
}

void Grass::grow() {
    _grown = true;
}

void Grass::beEaten() {
    _grown = false;
}