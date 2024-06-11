#ifndef SHEEP_H
#define SHEEP_H

#include "Animal.h"
#include "Grass.h"

class Sheep : public Animal {
private:
    bool _isPregnant;
    int _id;

public:
    Sheep(int age, int maxAge, int hunger, int maxHunger, char gender, int x, int y, int id);

    void move() override;
    bool eat(Grass* grass) override;
    bool eat(Sheep* sheep) override { return false; } // Implementation not needed for Sheep
    void die() override;
    void incrementAge() override;
    bool canReproduceWith(Animal* other) override;

    bool isPregnant() const;
    void setPregnant(bool pregnant);
    int getId() const;

    void displayInfo();
};

#endif // SHEEP_H