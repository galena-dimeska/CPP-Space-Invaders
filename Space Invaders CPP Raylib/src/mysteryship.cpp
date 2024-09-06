#include "mysteryship.hpp"
#include "setting.hpp" 

MysteryShip::MysteryShip()
{
    image = LoadTexture("Graphics/mystery.png");
    alive = false;
}

MysteryShip::~MysteryShip()
{
    UnloadTexture(image);
}

void MysteryShip::Spawn()
{
    position.y = 90;

    //it spawns from either the left or right
    //0 - left; 1 - right;
    int side = GetRandomValue(0, 1);

    if (side == 0) //left
    {
        position.x = Settings::offset/2;
        speed = 3;
    } 
    else //right
    {
        position.x = GetScreenWidth() - image.width - Settings::offset/2;
        speed = -3;
    }

    alive = true;
    
}

Rectangle MysteryShip::getRect()
{
    //note that the mystery ship object does not get destroyed 
    //we only set its 'alive' attribute to false when we don't want it to be visible
    if (alive){
        return {position.x, position.y, float(image.width), float(image.height)};
    } else {
        return {position.x, position.y, 0,0};
    }
    
}

void MysteryShip::Update()
{
    if (alive)
    {
        position.x += speed;
        //checking if it's exited the screen
        if (position.x > GetScreenWidth()- image.width - Settings::offset/2 || position.x < Settings::offset/2)
        {
            alive = false;
        }
        
    }
    
}

void MysteryShip::Draw()
{
    if (alive)
    {
        DrawTextureV(image, position, WHITE);
    }
    
}
