#ifndef WOLF_H
#define WOLF_H

#include "Animal.h"
#include "Sheep.h"

class Wolf : public Animal {
private:
    bool _isPregnant;
    int _id;

public:
    Wolf(int age, int maxAge, int hunger, int maxHunger, char gender, int x, int y, int id);

    void move() override;
    bool eat(Sheep* sheep) override;
    bool eat(Grass* grass) override { return false; } // Implementation not needed for Wolf
    void die() override;
    void incrementAge() override;
    bool canReproduceWith(Animal* other) override;

    bool isPregnant() const;
    void setPregnant(bool pregnant);
    int getId() const;
};

#endif // WOLF_H