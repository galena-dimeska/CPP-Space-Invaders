#include "button.hpp"

Button::Button(const char *imagePath, float scale, Vector2 position)
{
    Image image = LoadImage(imagePath);
    int originalWidth = image.width;
    int originalHeight = image.height;
    int newWidth = static_cast<int>(originalWidth * scale);
    int newHeight = static_cast<int>(originalHeight * scale);
    ImageResize(&image, newWidth, newHeight);

    texture = LoadTextureFromImage(image);

    UnloadImage(image); 

    this->position = position;
    
}

Button::~Button()
{
    UnloadTexture(texture);
}

void Button::Draw(){
    DrawTextureV(texture, position, WHITE);
}

int Button::getButtonWidth()
{
    return texture.width;
}

int Button::getButtonHeight()
{
    return texture.height;
}

void Button::setButtonPosition(Vector2 position)
{
    this->position = position;
}

bool Button::isPressed(Vector2 mousePos, bool mousePressed)
{
    Rectangle rect = {position.x, position.y, static_cast<float>(texture.width), static_cast<float> (texture.height) };
    
    if (CheckCollisionPointRec(mousePos, rect) && mousePressed)
    {
        return true;
    }

    return false;
    
}

