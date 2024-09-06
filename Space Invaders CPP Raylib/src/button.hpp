#pragma once

#include<raylib.h>

class Button{
private:
    Texture2D texture;
    Vector2 position;
public:
    Button(const char* imagePath, float scale = 1.0, Vector2 position = {0,0});
    ~Button();
    void Draw();
    void Update();
    int getButtonWidth();
    int getButtonHeight();
    void setButtonPosition(Vector2 position);
    bool isPressed(Vector2 mousePos, bool mousePressed);
};