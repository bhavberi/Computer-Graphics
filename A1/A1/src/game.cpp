#include "game.h"
#include "resource_manager.h"
#include "sprite_renderer.h"
#include "game_object.h"
#include "text_renderer.h"

#include <stdlib.h>
#include <iostream>
#include <sstream>
#include <algorithm>

#include <irrKlang.h>
#pragma comment(lib, "irrKlang.lib") // link with irrKlang.dll

#include <ft2build.h>
using namespace irrklang;
#include FT_FREETYPE_H

// Game-related State data
SpriteRenderer* Renderer;
SpriteRenderer* glowRenderer;
SpriteRenderer* playerRenderer;
SpriteRenderer* PlayerRenderer;

GameObject* Player;
TextRenderer* Text;
ISoundEngine* SoundEngine;
ISound* back_music;
int back_sound = 100;

float backx = 0.0f;
int repeat_backgrounds = 0;

Game::Game(unsigned int width, unsigned int height, unsigned int max_lives)
    : State(GAME_ACTIVE), Keys(), Width(width), Height(height), Level(0), Maxy(0), Miny(0), lives(max_lives), max_lives(max_lives)
{
}

Game::~Game()
{
    delete Renderer;
    delete glowRenderer;
    delete playerRenderer;
    delete Player;
}

void Game::localInit()
{
    this->Maxy = (this->Height - (float)PLAYER_SIZE.y) / 2.0f + 190.0f;
    this->Miny = (this->Height - (float)PLAYER_SIZE.y) / 2.0f - 240.0f;

    this->Levels.clear();
    GameLevel one, two, three;
    this->Levels.push_back(one);
    this->Levels.push_back(two);
    this->Levels.push_back(three);
    this->Level = -1;
    this->State = GAME_MENU;
    this->Length = 0.0f;
    this->score = 0;
    this->lives = this->max_lives;

    this->Levels[0].number = 0;
    this->Levels[0].person = "person";
    this->Levels[0].Maxy = (this->Height - (float)PLAYER_SIZE.y) / 2.0f + 255.0f;
    this->Levels[0].Miny = 90.0f;
    this->Levels[0].Length = this->Width * 5;
    this->Levels[0].speed = 0.55f;
    this->Levels[0].Generate_Pipes(this->Width * 2, 400, { this->Levels[0].Miny, this->Levels[0].Maxy }, this->Levels[0].Length - this->Width, 15, 0.55f);
    this->Levels[0].Generate_Coins(this->Width, { this->Levels[0].Miny, this->Levels[0].Maxy }, this->Levels[0].Length - this->Width, 100, 0.55f);

    this->Levels[1].number = 1;
    this->Levels[1].person = "person";
    this->Levels[1].Maxy = (this->Height - (float)PLAYER_SIZE.y) / 2.0f + 190.0f;
    this->Levels[1].Miny = (this->Height - (float)PLAYER_SIZE.y) / 2.0f - 240.0f;
    this->Levels[1].Length = this->Width * 5;
    this->Levels[1].speed = 0.6f;
    this->Levels[1].Generate_Pipes(this->Width * 2, 300, { this->Levels[1].Miny, this->Levels[1].Maxy }, this->Levels[1].Length, 15, 0.75f);
    this->Levels[1].Generate_Coins(this->Width, { this->Levels[1].Miny, this->Levels[1].Maxy }, this->Levels[1].Length - this->Width, 80, 0.75f);

    this->Levels[2].number = 2;
    this->Levels[2].person = "person1";
    this->Levels[2].Maxy = (this->Height - (float)PLAYER_SIZE.y) / 2.0f + 255.0f;
    this->Levels[2].Miny = 20.0f;
    this->Levels[2].Length = this->Width * 6;
    this->Levels[2].speed = 0.75f;
    this->Levels[2].Generate_Pipes(this->Width * 2, 300, { this->Levels[2].Miny, this->Levels[2].Maxy }, this->Levels[2].Length, 20, 0.95f);
    this->Levels[2].Generate_Coins(this->Width, { this->Levels[2].Miny, this->Levels[2].Maxy }, this->Levels[2].Length - this->Width, 120, 0.95f);

    /*glm::vec2 playerPos = glm::vec2(this->Width / 2.0f - PLAYER_SIZE.x / 2.0f, this->Height - PLAYER_SIZE.y + 1.5f);*/
    glm::vec2 playerPos = glm::vec2(40.0f, this->Maxy);
    Player = new GameObject(playerPos, PLAYER_SIZE, ResourceManager::GetTexture("person"));
    Player->acceleration = 0.0f;
    Player->Color = glm::vec3(1.0f, 1.0f, 1.0f);

    PlayerRenderer = Renderer;

}

void Game::Init()
{
    SoundEngine = createIrrKlangDevice();
    if (!SoundEngine)
    {
        std::cout << "ERROR";
    }
    back_music = SoundEngine->play2D("E:/Dev/OpenGL/A1/A1/src/audio/back.mp3", true, false, true);
    back_music->setVolume(back_sound);

    // load shaders
    ResourceManager::LoadShader("E:/Dev/OpenGL/A1/A1/src/shaders/sprite.vs", "E:/Dev/OpenGL/A1/A1/src/shaders/sprite.fs", nullptr, "sprite");
    ResourceManager::LoadShader("E:/Dev/OpenGL/A1/A1/src/shaders/sprite.vs", "E:/Dev/OpenGL/A1/A1/src/shaders/sprite.frag", nullptr, "glow");
    ResourceManager::LoadShader("E:/Dev/OpenGL/A1/A1/src/shaders/sprite.vs", "E:/Dev/OpenGL/A1/A1/src/shaders/player.frag", nullptr, "player");
    // configure shaders
    glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(this->Width),
        static_cast<float>(this->Height), 0.0f, -1.0f, 1.0f);
    ResourceManager::GetShader("sprite").Use().SetInteger("image", 0);
    ResourceManager::GetShader("sprite").SetMatrix4("projection", projection);
    ResourceManager::GetShader("glow").Use().SetInteger("image", 0);
    ResourceManager::GetShader("glow").SetMatrix4("projection", projection);
    ResourceManager::GetShader("player").Use().SetInteger("image", 0);
    ResourceManager::GetShader("player").SetMatrix4("projection", projection);

    // set render-specific controls
    Shader shader = ResourceManager::GetShader("sprite");
    Renderer = new SpriteRenderer(shader);

    Shader glowshader = ResourceManager::GetShader("glow");
    glowRenderer = new SpriteRenderer(glowshader);

    Shader playershader = ResourceManager::GetShader("player");
    playerRenderer = new SpriteRenderer(playershader);

    // load textures
    ResourceManager::LoadTexture("E:/Dev/OpenGL/A1/A1/src/textures/background.png", false, "background2");
    ResourceManager::LoadTexture("E:/Dev/OpenGL/A1/A1/src/textures/background2.png", false, "background1");
    ResourceManager::LoadTexture("E:/Dev/OpenGL/A1/A1/src/textures/background3.jpg", false, "background3");
    ResourceManager::LoadTexture("E:/Dev/OpenGL/A1/A1/src/textures/result.jpg", false, "resultback");

    ResourceManager::LoadTexture("E:/Dev/OpenGL/A1/A1/src/textures/awesomeface.png", true, "person");
    ResourceManager::LoadTexture("E:/Dev/OpenGL/A1/A1/src/textures/awesomeface_angry.png", true, "person1");

    /*ResourceManager::LoadTexture("E:/Dev/OpenGL/A1/A1/src/textures/texture.png", false, "pipe1");
    ResourceManager::LoadTexture("E:/Dev/OpenGL/A1/A1/src/textures/texture.png", false, "pipe2");*/

    ResourceManager::LoadTexture("E:/Dev/OpenGL/A1/A1/src/textures/coins.png", true, "coin");

    Text = new TextRenderer(this->Width, this->Height);
    Text->Load("E:/Dev/OpenGL/A1/A1/src/fonts/OCRAEXT.TTF", 24);

    localInit();
}

void Game::Update(float dt)
{
    if (this->Level != -1)
    {
        if (this->lives > 0)
        {
            float velocity = (PLAYER_VELOCITY - 35.0f * Player->acceleration) * dt;
            Player->Position.y += velocity;

            Player->acceleration -= 0.05f;

            if (Player->Position.y >= this->Maxy)
                Player->Position.y = this->Maxy;
            if (Player->Position.y <= this->Miny)
                Player->Position.y = this->Miny;

            backx -= this->Levels[this->Level].speed;
            this->Length += this->Levels[this->Level].speed;
            this->DoCollisions();
        }
        else
        {
            this->Levels[this->Level].setVelocity(0.0f);
        }

        if (this->Length >= this->Levels[this->Level].Length)
        {
            this->Level++;
            this->Length = 0;
            repeat_backgrounds = 0;
            backx = 0;

            if (this->Level == this->Levels.size())
            {
                SoundEngine->stopAllSounds();

                this->Level = -1;
                this->State = GAME_WIN_LOSS;

                back_music = SoundEngine->play2D("E:/Dev/OpenGL/A1/A1/src/audio/success.mp3", true, false, true);
                back_sound = 100;
                back_music->setVolume(back_sound);

                return;
            }

            if (this->Level == this->Levels.size() - 2)
            {
                SoundEngine->stopAllSounds();

                back_music = SoundEngine->play2D("E:/Dev/OpenGL/A1/A1/src/audio/back1.mp3", true, false, true);
                back_music->setVolume(back_sound);
            }

            Player = new GameObject(Player->Position, PLAYER_SIZE, ResourceManager::GetTexture(this->Levels[this->Level].person));
        }

        this->Maxy = this->Levels[this->Level].Maxy;
        this->Miny = this->Levels[this->Level].Miny;

        /*if (this->lives <= 0)
        {
            this->Level = -1;
        }*/
    }
}

void Game::ProcessInput(float dt)
{
    if (this->Keys[GLFW_KEY_Q])
    {
        //SoundEngine->stopAllSounds();
        back_sound = (back_sound == 0) ? 100 : 0;
        back_music->setVolume(back_sound);
    }

    if (this->State == GAME_MENU)
    {
        if (this->Keys[GLFW_KEY_ENTER])
        {
            this->Level = 0;
            this->State = GAME_ACTIVE;

            SoundEngine->stopAllSounds();
            back_music->stop();

            back_music = SoundEngine->play2D("E:/Dev/OpenGL/A1/A1/src/audio/back.mp3", true, false, true);
            back_music->setVolume(back_sound);
        }
    }
    else if (this->State == GAME_ACTIVE && this->lives > 0)
    {
        // move playerboard
        if (this->Keys[GLFW_KEY_SPACE])
        {
            Player->acceleration = -0.05f;
            if (Player->upacceleration == 0.0f)
                Player->upacceleration = 0.2f;
            else
                Player->upacceleration += 0.015f;

            float velocity = PLAYER_VELOCITY * dt * Player->upacceleration;

            Player->Position.y -= 2 * velocity;

            if (Player->Position.y >= this->Maxy)
                Player->Position.y = this->Maxy;
            if (Player->Position.y <= this->Miny)
                Player->Position.y = this->Miny;
            
            PlayerRenderer = playerRenderer;
            Player->Color = glm::vec3(1.0f, 1.0f, 0.0f);
        }
        else
        {
            Player->upacceleration = 0.0f;
            PlayerRenderer = Renderer;
            Player->Color = glm::vec3(1.0f, 1.0f, 1.0f);
        }
    }
    else if (this->State == GAME_WIN_LOSS || this->lives <= 0)
    {
        if (this->Keys[GLFW_KEY_ENTER])
        {
            this->localInit();
            this->State = GAME_MENU;
            this->Level = -1;

            SoundEngine->stopAllSounds();

            back_music = SoundEngine->play2D("E:/Dev/OpenGL/A1/A1/src/audio/back.mp3", true, false, true);
            back_music->setVolume(back_sound);
        }
    }
}

void Game::Render()
{
    Texture2D background;
    if (this->Level == -1)
    {
        background = ResourceManager::GetTexture("resultback");
        Renderer->DrawSprite(background, glm::vec2(0.0f, 0.0f), glm::vec2(this->Width, this->Height), 0.0f);

        if (this->State == GAME_WIN_LOSS)
        {
            std::stringstream ss1;
            ss1 << this->score;

            Text->RenderText("You have completed all levels!!", (this->Width - static_cast<float>(15 * 27 * 2)) / 2, this->Height / static_cast<float>(2) - 70, 2.0f, glm::vec3(0.0, 1.0, 0.0));
            Text->RenderText("Score:" + ss1.str(), (this->Width - static_cast<float>(15 * 10)) / 2, 5.0f, 1.0f);
            Text->RenderText(
                "Press ENTER to retry or ESC to quit", (this->Width - static_cast<float>(15 * 36)) / 2, this->Height / static_cast<float>(2), 1.0, glm::vec3(1.0, 1.0, 0.0)
            );
        }
        else
        {
            Text->RenderText(
                "Press ENTER to start or ESC to quit", (this->Width - static_cast<float>(15 * 36)) / 2, this->Height / static_cast<float>(2), 1.0, glm::vec3(1.0, 1.0, 0.0)
            );
        }
        Text->RenderText(
            "Press q to play/pause back music", (this->Width - static_cast<float>(15 * 33)) / 2, this->Height / static_cast<float>(2) + 70, 1.0, glm::vec3(1.0, 1.0, 0.0)
        );
    }
    else
    {
        background = ResourceManager::GetTexture("background" + std::to_string(1 + this->Level));
        //float rotate = tan(glfwGetTime());
        Renderer->DrawSprite(background, glm::vec2(repeat_backgrounds * this->Width + backx, 0.0f), glm::vec2(this->Width, this->Height), 0.0f);
        Renderer->DrawSprite(background, glm::vec2((repeat_backgrounds + 1) * this->Width + backx, 0.0f), glm::vec2(this->Width, this->Height), 0.0f);

        Player->Draw(*PlayerRenderer);
        this->Levels[this->Level].Draw(*Renderer, *glowRenderer);

        if (-backx >= (repeat_backgrounds + 1) * this->Width)
            repeat_backgrounds++;

        std::stringstream ss1, ss2, ss3, ss4; 
        int percentage = (int)(100.0f * this->Length / (this->Levels[this->Level].Length - ((this->Level > 0) ? 1.5 : 1)*this->Width));
        ss1 << this->lives;
        ss2 << std::min(100, percentage);
        ss3 << (this->Level + 1);
        ss4 << this->score;

        Text->RenderText("Lives:" + ss1.str(), 5.0f, 5.0f, 1.0f);
        Text->RenderText("Distance:" + ss2.str() + "%", 5.0f, this->Height - 20.0f, 1.0f);
        Text->RenderText("Level:" + ss3.str(), (this->Width - static_cast<float>(15 * 10)), 5.0f, 1.0f);
        Text->RenderText("Score:" + ss4.str(), (this->Width - static_cast<float>(15 * 10)) / 2, 5.0f, 1.0f);

        if (percentage > 100)
        {
            Text->RenderText("You have completed level " + ss3.str() + "!", (this->Width - static_cast<float>(15 * 27 * 2)) / 2, this->Height / static_cast<float>(2), 2.0f);
        }

        if(this->lives <=0)
        {
            Text->RenderText("You have Lost!! :(", (this->Width - static_cast<float>(15 * 20 * 2)) / 2, this->Height / static_cast<float>(2) - 70, 2.0f, glm::vec3(0.0, 0.0, 0.0));
            Text->RenderText(
                "Press ENTER to retry or ESC to quit", (this->Width - static_cast<float>(15 * 34)) / 2, this->Height / static_cast<float>(2), 1.0, glm::vec3(1.0, 1.0, 1.0)
            );
        }
    }
}

bool Game::CheckCollision(GameObject& one, GameObject& two) // AABB - AABB collision
{
    // collision x-axis?
    bool collisionX = one.Position.x + one.Size.x >= two.Position.x &&
        two.Position.x + two.Size.x >= one.Position.x;
    // collision y-axis?
    bool collisionY = one.Position.y + one.Size.y >= two.Position.y &&
        two.Position.y + two.Size.y >= one.Position.y;
    // collision only if on both axes
    return collisionX && collisionY;
}

void Game::DoCollisions()
{
    for (GameObject& coin : this->Levels[this->Level].Coins)
    {
        if (!coin.Destroyed)
        {
            if (CheckCollision(*Player, coin))
            {
                //if (!coin.IsSolid)
                coin.Destroyed = true;
                this->score++;
                //Add score
            }
        }
    }

    for (GameObject& pipe : this->Levels[this->Level].Pipes)
    {
        if (pipe.Color != glm::vec3(1.0f, 0.0f, 1.0f))
        {
            if (CheckCollision(*Player, pipe))
            {
                //if (!coin.IsSolid)
                //pipe.Destroyed = true;
                pipe.Color = glm::vec3(1.0f, 0.0f, 1.0f);
                this->lives--;

                /*SoundSource mySpeaker;
                mySpeaker.Play(sound1);*/

                /*if (this->lives <= 0)
                    Player->Position = pipe.Position;*/

                /*std::cout << pipe.Position.x << " " << Player->Position.x << std::endl;
                std::cout << pipe.Size.x << " " << Player->Size.x << std::endl;*/

                back_music->setVolume(0);
                SoundEngine->play2D("E:/Dev/OpenGL/A1/A1/src/audio/hit.mp3", false);
                back_music->setVolume(back_sound);
            }
        }
    }

    // Add diagnol collision detection for the bars
    // - write eqn of line from one long side. Check all 4 vertices of player if any satisfies, then yes else no
    // - decrease life on above collision
}