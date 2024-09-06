#pragma once
#include "spaceship.hpp" 
#include "obstacle.hpp"
#include "alien.hpp"
#include "mysteryship.hpp"

class  Game
{
private:
    Spaceship spaceship;
    void DeleteInactiveLasers();

    std::vector<Obstacle> obstacles;
    std::vector<Obstacle> CreateObstacles();

    std::vector<Alien> aliens;
    std::vector<Alien> CreateAliens();
    int alienRows;
    int alienColumns;
    int aliensDirection;
    void MoveAliens();
    void MoveDownAliens(int distance);
    std::vector<Laser> alienLasers;
    void AlienShootLasers();
    static constexpr float alienLaserShootInterval = 0.35; 
    float timeLastAlienFired;

    MysteryShip mysteryShip;

    float mysteryShipSpawnInterval;
    float timeLastSeen;

    void GameOver();
    void GameWin();
    
    
    void CheckForHighScore();
    void saveHighscoreToFile(int highscore);
    int loadHighscoreFromFile();

    Sound explosionSound;

public:
    Game();
    ~ Game();
    void Draw();
    void Update();
    void HandleInput();
    void Reset();
    void InitGame(); 
    void CheckForCollisions();

    int lives;
    bool run;
    int score;
    int highscore;
    bool win;
    Music music;
};

 