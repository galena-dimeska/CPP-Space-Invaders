#include "spaceship.hpp"
#include "setting.hpp"

Spaceship::Spaceship()
{
    image = LoadTexture("Graphics/spaceship.png");

    position.x = (GetScreenWidth() - image.width) / 2;

    position.y = GetScreenHeight() - image.height - 2*Settings::offset; 
    lastFireTime = 0.0;

    laserSound = LoadSound("Sounds/laser.ogg");
    SetSoundVolume(laserSound, 0.4f);

}

Spaceship::~Spaceship(){
    UnloadTexture(image);
    UnloadSound(laserSound);
}

void Spaceship::Draw(){
    DrawTextureV(image, position, WHITE);
}

void Spaceship::MoveLeft(){
    position.x -= speed;
    if (position.x < Settings::offset/2) 
    {   
        position.x = Settings::offset/2;
    }
    
}

void Spaceship::MoveRight(){
    position.x += speed;
    if (position.x > GetScreenWidth()-image.width - Settings::offset/2)
    {
        position.x = GetScreenWidth()-image.width - Settings::offset/2;
    }
    
}

void Spaceship::FireLaser(){
    if (GetTime() - lastFireTime >= laserFireDelay)
    {
        lasers.push_back(Laser({position.x + image.width/2 - 2 ,position.y} , laserFireSpeed));
        lastFireTime = GetTime();
        PlaySound(laserSound);
    }
    
    
}

Rectangle Spaceship::getRect()
{
    return {position.x, position.y, float(image.width), float(image.height)};
}

void Spaceship::Reset()
{
    position.x = (GetScreenWidth() - image.width) / 2.0f;
    position.y = GetScreenHeight() - image.height - 2*Settings::offset; 

    lasers.clear();
}
