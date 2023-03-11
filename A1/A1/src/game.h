#ifndef GAME_H
#define GAME_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "game_level.h"

// Represents the current state of the game
enum GameState {
    GAME_ACTIVE,
    GAME_MENU,
    GAME_WIN_LOSS
};

// Initial size of the player
const glm::vec2 PLAYER_SIZE(100.0f, 100.0f);
// Initial velocity of the player paddle
const float PLAYER_VELOCITY(200.0f);

// Game holds all game-related state and functionality.
// Combines all game-related data into a single class for
// easy access to each of the components and manageability.
class Game
{
public:
    // game state
    GameState               State;
    bool                    Keys[1024];
    unsigned int            Width, Height;
    std::vector<GameLevel>  Levels;
    int Level;

    unsigned int lives;
    unsigned int max_lives;

    unsigned int Miny, Maxy;
    float Length;
    int score;

    // constructor/destructor
    Game(unsigned int width, unsigned int height, unsigned int max_lives);
    ~Game();
    // initialize game state (load all shaders/textures/levels)
    void Init();
    // game loop
    void ProcessInput(float dt);
    void Update(float dt);
    void Render();
    void DoCollisions();

private:
    void localInit();
    bool CheckCollision(GameObject& one, GameObject& two); // AABB - AABB collision
};

#endif