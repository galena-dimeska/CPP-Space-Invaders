#include "laser.hpp"
#include <iostream>
#include "setting.hpp"

Laser::Laser(Vector2 position, int speed){
    this->position = position;
    this->speed = speed;
    active = true;
    width = 4;
    height = 15;
}

void Laser::Draw(){

    if (active)
    {
        DrawRectangle(position.x, position.y, width, height, Settings::yellow);
    }
    
    
}

Rectangle Laser::getRect()
{
    Rectangle rect;
    rect.x = position.x;
    rect.y = position.y;
    rect.width = width;
    rect.height = height;
    return rect;
}

void Laser::Update(){
    //the laser beams only move in the y axis
    position.y += speed;
    //check if it has moved outside the game window
    if (active)
    {
        if ((position.y > GetScreenHeight() - 2 * Settings::offset) || (position.y < Settings::offset / 2)){
            active = false;
        }
        
    }
    
}