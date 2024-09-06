#pragma once

#include<raylib.h>
#include "laser.hpp"
#include <vector>
#include "setting.hpp"

class Spaceship{

private:
    Texture2D image;
    Vector2 position;
    double lastFireTime;
    Sound laserSound;

    static const int speed = 7;
    static const int laserFireSpeed = -6;
    static constexpr float laserFireDelay = 0.3;

    

public:
    Spaceship();
    ~Spaceship();
    void Draw();
    void MoveRight();
    void MoveLeft();
    void FireLaser();

    std::vector<Laser> lasers;

    Rectangle getRect();
    void Reset();

};