#pragma once
#include<raylib.h>

class Block{
private:
    Vector2 position;
    Color color;
    int width;
    int height;
public:
    Block(Vector2 position);
    void Draw();
    Rectangle getRect();

};