#pragma once
#include<raylib.h>

class Alien{
private:

public:
    //since we only have 3 types of aliens
    //we can load the 3 images once and use the same images later
    static Texture2D alienImages[3];

    int type;
    Vector2 position;

    Alien(int type, Vector2 position);
    void Draw();
    void Update(int direction);
    int GetType();

    static void UnloadImages();

    static constexpr float cellSize = 55.0;
    static const int laserFireSpeed = 5; 
    static constexpr float laserFireDelay = 0.35;

    Rectangle getRect();


};
