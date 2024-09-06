#pragma once
#include <raylib.h>
#include "setting.hpp"


//this class is used for both the spaceship (player) as well as the alien (enemy) lasers
class Laser
{
private:
    Vector2 position;
    int speed;
    int width;
    int height;

public:
    Laser(Vector2 position, int speed);
    void Update();
    void Draw();
    bool active;
    Rectangle getRect();
};
