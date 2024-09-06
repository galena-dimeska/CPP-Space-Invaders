#pragma once
#include<raylib.h>
#include "setting.hpp"

class MysteryShip {
private:
    Vector2 position;
    Texture2D image;
    int speed;

public:
    MysteryShip();
    ~MysteryShip();
    void Update();
    void Draw();
    void Spawn();

    bool alive;

    Rectangle getRect();
};