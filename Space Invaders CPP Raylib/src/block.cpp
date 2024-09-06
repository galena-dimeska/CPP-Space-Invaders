#include "block.hpp"

Block::Block(Vector2 position)
{
    this->position = position;
    this->color = {243, 216, 63, 255};
    this->width = 3;
    this->height = 3;
}

void Block::Draw(){
    DrawRectangle(position.x, position.y, width, height, color);
}

Rectangle Block::getRect()
{
    Rectangle rect;
    rect.x = position.x;
    rect.y = position.y;
    rect.width = width;
    rect.height = height;
    return rect;
}
