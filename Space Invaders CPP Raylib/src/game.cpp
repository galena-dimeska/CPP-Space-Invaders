#include "game.hpp"
#include <iostream>
#include "setting.hpp"
#include <fstream>


Game::Game()
{
    music = LoadMusicStream("Sounds/bubble_instrumental.ogg");
    SetMusicVolume(music, 0.15f);

    explosionSound = LoadSound("Sounds/explosion.ogg");
    SetSoundVolume(explosionSound, 0.3f);

    PlayMusicStream(music);

    InitGame();

}

Game::~Game(){
    Alien::UnloadImages();
    UnloadMusicStream(music);
    UnloadSound(explosionSound);
}

void Game::Update(){

    if (run) {

        //we call these methods every frame
        //but only while the game is running
        //if it's game over we need to freeze the game

        double currentTime = GetTime();
        if (currentTime - timeLastSeen > mysteryShipSpawnInterval)
        {
            mysteryShip.Spawn();
            timeLastSeen = GetTime();

            mysteryShipSpawnInterval = GetRandomValue(10,20);
        }
        

        for (auto& laser : spaceship.lasers){
            laser.Update();
        }

        MoveAliens();

        AlienShootLasers();
        for(auto & laser: alienLasers){
            laser.Update();
        }

        DeleteInactiveLasers();

        mysteryShip.Update();

        CheckForCollisions();
    
    } 
    
} 

void Game::Draw(){
    spaceship.Draw();

    for(auto& laser: spaceship.lasers){
        laser.Draw();
    }

    for(auto& obstacle: obstacles){
        obstacle.Draw();
    }

    for(auto& alien: aliens){
        alien.Draw();
    }

    for(auto& laser: alienLasers){
        laser.Draw();
    }

    mysteryShip.Draw();
}

void Game::HandleInput(){

    if(run) {
        if (IsKeyDown(KEY_LEFT)){
            spaceship.MoveLeft();
        } else if (IsKeyDown(KEY_RIGHT)){
            spaceship.MoveRight();
        } 
        if (IsKeyDown(KEY_SPACE)){ 
            spaceship.FireLaser();
        }
    }
    
}

void Game::DeleteInactiveLasers(){

    for(auto it = spaceship.lasers.begin(); it != spaceship.lasers.end(); ){
        if ( !it->active ){
            it = spaceship.lasers.erase(it);
        } else {
            ++it;
        }
        
    }

    for(auto it = alienLasers.begin(); it != alienLasers.end(); ){
        if (!it->active){
            it = alienLasers.erase(it);
        } else {
            ++it;
        }
        
    }
}

std::vector<Obstacle> Game::CreateObstacles()
{
    int obstacleWidth = Obstacle::grid[0].size() * 3;
    float gap = (GetScreenWidth() - (4 * obstacleWidth))/5;

    for(int i = 0; i < 4; i++) {
        float offsetX = (i + 1) * gap + i * obstacleWidth;
        obstacles.push_back(Obstacle({offsetX, float(GetScreenHeight() - 100 - 2*Settings::offset)}));
    }
    
    return obstacles;
    
}

std::vector<Alien> Game::CreateAliens()
{
    std::vector<Alien> aliens;
    for (int row = 0; row < 5; row++)
    {
        for (int column = 0; column < 11; column++)
        {
            int alienType;
            if (row ==0){
                alienType = 3;
            } else if (row == 1 || row == 2)
            {
                alienType = 2;
            } else {
                alienType = 1;
            }
            
            float x = 75 + column * Alien::cellSize;
            float y = 100 + row * Alien::cellSize;
            aliens.push_back(Alien(alienType, {x, y}));
        }
        
    }

    return aliens;
    
}

void Game::MoveAliens()
{
    //the aliens move left and right, alternating whenever they hit the edge of the window
    //they also do down towards the spaceship
    for(auto& alien: aliens){
        if (alien.position.x + alien.alienImages[alien.type-1].width > GetScreenWidth()-Settings::offset/2)
        {
            aliensDirection = -1;
            MoveDownAliens(4);
        }
        if (alien.position.x < Settings::offset/2)
        {
            aliensDirection = 1;
            MoveDownAliens(4);
        }
        
        alien.Update(aliensDirection);
    }
}

void Game::MoveDownAliens(int distance)
{
    for(auto& alien: aliens){
        alien.position.y += distance;
    }
}

void Game::AlienShootLasers()
{
    double currentTime = GetTime();
    if (currentTime - timeLastAlienFired >= alienLaserShootInterval && !aliens.empty())
    {
        int randomIndex = GetRandomValue(0, aliens.size()-1);
        Alien & alien = aliens[randomIndex];

        alienLasers.push_back(
            Laser({alien.position.x + alien.alienImages[alien.type -1].width / 2,
            alien.position.y + alien.alienImages[alien.type-1].height}, Alien::laserFireSpeed));

        timeLastAlienFired = GetTime();
    }
}

void Game::CheckForCollisions()
{
    //spaceship lasers with other game elements
    for(auto& laser: spaceship.lasers) 
    {
        //spaceship lasers with aliens
        auto it = aliens.begin(); 
        while (it!=aliens.end())
        {
            if(CheckCollisionRecs(it->getRect(), laser.getRect())){

                PlaySound(explosionSound);

                if (it->type == 1){
                    score += 100;
                } else if (it->type == 2){
                    score += 200;
                } else if (it->type == 3){
                    score += 300;
                }

                CheckForHighScore();
                
                it = aliens.erase(it);
                laser.active = false;
            } else {
                ++it;
            }
        }

        if (aliens.size()==0)
        {
            GameWin();
        }

        //spaceship lasers with obstacles
        for(auto & obstacle: obstacles)
        {
            auto it = obstacle.blocks.begin();
            while (it!=obstacle.blocks.end())
            {
                if (CheckCollisionRecs(it-> getRect(), laser.getRect()))
                {
                    //remove a block of the obstacle every time the spaceship hits it with a laser
                    it = obstacle.blocks.erase(it);
                    laser.active = false;
                } else {
                    ++it;
                }
            }
        }

        //spaceship lasers with mystery ship
        if (CheckCollisionRecs(mysteryShip.getRect(), laser.getRect()))
        {
            mysteryShip.alive = false;
            laser.active = false;
            score += 500;

            CheckForHighScore();
            PlaySound(explosionSound);
        }
        
        
    } //done with spaceship lasers

    //alien lasers
    for (auto & laser: alienLasers)
    {

        //alien lasers with spaceship
        //we don't want to destroy it bc we have 3 lives
        if (CheckCollisionRecs(laser.getRect(), spaceship.getRect()))
        {
            laser.active = false;
            lives --;
            if (lives==0)
            {
                GameOver();
            }
            
        }

        //alien lasers with obstacles
        for(auto & obstacle: obstacles)
        {
            auto it = obstacle.blocks.begin(); 
            while (it!=obstacle.blocks.end())
            {
                if (CheckCollisionRecs(it-> getRect(), laser.getRect()))
                {
                    it = obstacle.blocks.erase(it);
                    laser.active = false;
                } else {
                    ++it;
                }
            }
        }
    }

    //aliens (not alien lasers) with obstacles and spaceship
    for (auto & alien: aliens)
    {
        //aliens with obstacles
        for (auto & obstacle: obstacles){
            auto it = obstacle.blocks.begin();
            while (it != obstacle.blocks.end())
            {
                if (CheckCollisionRecs( it->getRect(), alien.getRect()))
                {
                    it = obstacle.blocks.erase(it);
                } else {
                    ++it;
                }
                
            }
        }

        //aliens with spaceship - game over
        if (CheckCollisionRecs(alien.getRect(), spaceship.getRect()))
        {
            GameOver();
        }
    }
}

void Game::GameOver()
{
    run = false;
    win = false;
}

void Game::GameWin()
{
    run = false;
    win = true;
}

void Game::Reset()
{
    spaceship.Reset();
    aliens.clear();
    alienLasers.clear();
    obstacles.clear();

}

void Game::InitGame()
{
    obstacles = CreateObstacles();

    alienColumns = 11;
    alienRows = 5;
    aliens = CreateAliens();
    aliensDirection = 1;
    timeLastAlienFired = 0.0;

    timeLastSeen = 0.0;
    mysteryShipSpawnInterval = GetRandomValue(10, 20);

    lives = 3;
    run = false;
    win = false;
    score = 0;
    highscore = loadHighscoreFromFile();

}

void Game::CheckForHighScore()
{
    if (score > highscore)
    {
        highscore = score;
        saveHighscoreToFile(highscore);
    }
}

void Game::saveHighscoreToFile(int highscore) //used in CheckForHighscore()
{
    std::ofstream highscoreFile("highscore.txt");
    if(highscoreFile.is_open()) {
        highscoreFile << highscore;
        highscoreFile.close();
    } else {
        std::cerr << "Failed to save highscore to file" << std::endl;
    }
}

int Game::loadHighscoreFromFile() //used in InitGame()
{
    int loadedHighscore = 0;
    std::ifstream highscoreFile("highscore.txt");
    if(highscoreFile.is_open()) {
        highscoreFile >> loadedHighscore;
        highscoreFile.close();
    } else {
        std::cerr << "Failed to load highscore from file." << std::endl;
    }
    return loadedHighscore;
}

