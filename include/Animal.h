#ifndef ANIMAL_H
#define ANIMAL_H

class Grass;
class Sheep;

class Animal {
protected:
    int _age;
    int _maxAge;
    int _hunger;
    int _maxHunger;
    char _gender;
    int _x, _y; // Position de l'animal
    bool _deathLogged; // Nouveau drapeau

public:
    Animal(int age, int maxAge, int hunger, int maxHunger, char gender);
    virtual ~Animal() = default;

    virtual void move() = 0;
    virtual bool eat(Grass* grass) = 0;
    virtual bool eat(Sheep* sheep) = 0;
    virtual void die();
    virtual void incrementAge();
    virtual bool canReproduceWith(Animal* other) = 0;

    int getX() const;
    int getY() const;
    void setPosition(int x, int y);
    bool isAlive() const;
    char getGender() const;
    bool isDeathLogged() const; // Nouvelle méthode
    void setDeathLogged(bool logged); // Nouvelle méthode
};

#endif // ANIMAL_H