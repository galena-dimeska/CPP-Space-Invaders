#include <raylib.h>
#include "game.hpp"
#include <string>
#include "button.hpp"

std::string FormatWithLeadingZeros(int number, int width){ //padding with 0s
    std::string numberText = std::to_string(number);
    int leadingZeros = width - numberText.length();

    return numberText = std::string(leadingZeros, '0') + numberText;
}

int main()
{
    Color grey = Settings::grey;
    Color yellow = Settings::yellow;

    int offset = Settings::offset; 
    int screenWidth = Settings::screenWidth;
    int screenHeight = Settings::screenHeight;

    bool menu = true;

    InitWindow(screenWidth + offset, screenHeight + 2*offset, "C++ Space Invaders");
    InitAudioDevice();

    Font font = LoadFontEx("Font/monogram.ttf", 64, 0, 0);

    //for depicting the remaining lives of the spaceship
    Texture2D spaceshipImage = LoadTexture("Graphics/spaceship.png");

    //for the menu
    Texture2D menuBackground = LoadTexture("Graphics/menu_background.png");
    Button startButton("Graphics/start_button.png", 0.6);
    startButton.setButtonPosition({float(Settings::screenWidth/2 - startButton.getButtonWidth()/2 + Settings::offset/2), float(Settings::screenHeight/2 - startButton.getButtonHeight() + Settings::offset - 10)});
    Button exitButton("Graphics/exit_button.png", 0.6);
    exitButton.setButtonPosition({float(Settings::screenWidth/2 - startButton.getButtonWidth()/2 + Settings::offset/2), float(Settings::screenHeight/2 + Settings::offset + 20)});
    Button retryButton("Graphics/retry_button.png", 0.6);
    retryButton.setButtonPosition({float(Settings::screenWidth/2 - retryButton.getButtonWidth()/2 + Settings::offset/2), float(Settings::screenHeight/2 - retryButton.getButtonHeight() + Settings::offset - 10)});

    SetTargetFPS(60);

    Game game;

    while (!WindowShouldClose())
    {
        UpdateMusicStream(game.music);

        Vector2 mousePosition = GetMousePosition();
        bool mousePressed = IsMouseButtonPressed(MOUSE_BUTTON_LEFT);

        BeginDrawing();

        ClearBackground(grey);

        if (menu)
        {
            DrawTextureV(menuBackground, {0,0}, WHITE);
            startButton.Draw();
            exitButton.Draw();

            if (startButton.isPressed(mousePosition, mousePressed))
            {
                game.run = true;
                menu = false;
            } else if (exitButton.isPressed(mousePosition, mousePressed)) 
            {
                break;
            }
            
        } else {
            DrawRectangleRoundedLines({10,10,780,780}, 0.18f, 20, 2, yellow);
            DrawLineEx({25, 730}, {775, 730}, 3, yellow);
            
            game.HandleInput();
            game.Update();
            game.Draw();

            float x = 70.0;
            for (int i = 0; i < game.lives; i++)
            {
                DrawTextureV(spaceshipImage, {x, 745}, WHITE);
                x += 50;
            }

            DrawTextEx(font, "SCORE", {50, 15}, 34, 2, yellow);
            std::string scoreText = FormatWithLeadingZeros(game.score, 5);
            DrawTextEx(font, scoreText.c_str(), {50, 45}, 34, 2, yellow);
            
            DrawTextEx(font, "HIGH-SCORE", {570, 15}, 34, 2, yellow);
            std::string highscoreText = FormatWithLeadingZeros(game.highscore, 5);
            DrawTextEx(font, highscoreText.c_str(), {659, 45}, 34, 2, yellow);

            if (game.run)
            {
                DrawTextEx(font, "NOW PLAYING", {536, 742}, 34, 2, yellow);
            } else {
                if (game.win)
                {
                    DrawTextEx(font, "YOU WIN", {638, 742}, 34, 2, yellow);
                } else {
                    DrawTextEx(font, "GAME OVER", {570, 742}, 34, 2, yellow);
                }
                
                exitButton.Draw();
                retryButton.Draw();
                if (retryButton.isPressed(mousePosition, mousePressed))
                {
                    game.Reset();
                    game.InitGame();
                    game.run = true;
                } else if (exitButton.isPressed(mousePosition, mousePressed)) 
                {
                    break;
                }

            } 

        }

        EndDrawing();
    }

    CloseWindow();
    CloseAudioDevice();
    return 0;
}