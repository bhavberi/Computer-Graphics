#ifndef GAMELEVEL_H
#define GAMELEVEL_H
#include <vector>

#include <glad/glad.h>
#include <GLM/glm.hpp>

#include "game_object.h"
#include "sprite_renderer.h"
#include "resource_manager.h"


/// GameLevel holds all Tiles as part of a Breakout level and 
/// hosts functionality to Load/render levels from the harddisk.
class GameLevel
{
public:
    // level state
    std::vector<GameObject> Bricks;
    std::vector<GameObject> Coins;
    std::vector<GameObject> Pipes;

    unsigned int Miny, Maxy;
    unsigned int Length;
    unsigned int number;
    float speed;
    std::string person;

    // constructor
    GameLevel() { }
    // loads level from file
    void Load(const char* file, unsigned int levelWidth, unsigned int levelHeight);
    // Generate Pipes & Coins
    void Generate_Pipes(unsigned start, unsigned int distance, std::pair<unsigned, unsigned> Height, unsigned int length, unsigned int number, float velocity);
    void Generate_Coins(unsigned start, std::pair<unsigned, unsigned> Height, unsigned int length, unsigned int number, float velocity);
    // render level
    void Draw(SpriteRenderer& renderer, SpriteRenderer& glowrenderer);
    // check if the level is completed (all non-solid tiles are destroyed)
    bool IsCompleted();
    void setVelocity(float velocity);
private:
    // initialize level from tile data
    void init(std::vector<std::vector<unsigned int>> tileData, unsigned int levelWidth, unsigned int levelHeight);
};

#endif