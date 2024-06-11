#ifndef GRASS_H
#define GRASS_H

class Grass {
private:
    bool _grown;

public:
    Grass();

    bool isGrown() const;
    void grow();
    void beEaten();
};

#endif // GRASS_H