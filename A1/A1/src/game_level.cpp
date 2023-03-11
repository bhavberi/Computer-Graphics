#include "game_level.h"

#include <fstream>
#include <sstream>
#include <stdlib.h>
#include <iostream>


void GameLevel::Load(const char* file, unsigned int levelWidth, unsigned int levelHeight)
{
    // clear old data
    this->Bricks.clear();
    // load from file
    unsigned int tileCode;
    GameLevel level;
    std::string line;
    std::ifstream fstream(file);
    std::vector<std::vector<unsigned int>> tileData;
    if (fstream)
    {
        while (std::getline(fstream, line)) // read each line from level file
        {
            std::istringstream sstream(line);
            std::vector<unsigned int> row;
            while (sstream >> tileCode) // read each word separated by spaces
                row.push_back(tileCode);
            tileData.push_back(row);
        }
        if (tileData.size() > 0)
            this->init(tileData, levelWidth, levelHeight);
    }
}

bool CheckCollision(GameObject& one, GameObject& two) // AABB - AABB collision
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

void GameLevel::Generate_Pipes(unsigned start, unsigned int distance, std::pair<unsigned, unsigned> Height, unsigned int length, unsigned int number, float velocity)
{
    for (unsigned int i = 0; i < number; i++)
    {
        float x = (rand() % ((length - start + 1))) + start;
        float y = (rand() % ((Height.second - Height.first + 1))) + Height.first;
        float Rotation = (rand() * 180.0f / RAND_MAX);
        float Rotate = (rand() % 2) * 0.5f;

        float effective_height = Rotation < 90 ? cos(glm::radians(Rotation)) * 190.0f + 5.0f : cos(glm::radians(Rotation - 90)) * 190.0f + 5.0f;

        if (y > Height.second - effective_height)
        {
            y = Height.second - effective_height;
            /*Rotation = rand() % 30;*/
        }
        else if (y < Height.first + effective_height)
        {
            y = Height.first + effective_height;
            //Rotation = rand() % 30;
        }

        if (Rotate == 1.0f && y > Height.second - 200.0f)
        {
            y = Height.second - 200.0f;
        }
        else if (Rotate == 1.0f && y < Height.second + 200.0f)
        {
            y = Height.second + 200.0f;
        }

        glm::vec2 pos = glm::vec2(x, y);
        glm::vec2 size = glm::vec2(200.0f, 50.0f);

        if ((Rotation > 45 && Rotation < 135))
            size = glm::vec2(50.0f, 200.0f);

        bool to_add = true;
        for (GameObject& pipe : this->Pipes)
        {
            GameObject new_pipe1 = GameObject(pos, size + glm::vec2(distance, distance), ResourceManager::GetTexture("pipe"));
            GameObject new_pipe2 = GameObject(pos, glm::vec2(size.y, size.x) + glm::vec2(distance, distance), ResourceManager::GetTexture("pipe"));
            GameObject checking_pipe = GameObject(pipe.Position, pipe.Size + glm::vec2(distance, distance), ResourceManager::GetTexture("pipe"));

            if(pipe.Position.x > pos.x - distance && pipe.Position.x < pos.x + distance)
            {
                i--;
                to_add = false;
                break;
            }

            if (CheckCollision(pipe, new_pipe1) || CheckCollision(pipe, new_pipe2))
            {
                i--;
                to_add = false;
                break;
            }
        }

        if (to_add)
        {
            GameObject new_pipe = GameObject(pos, size, ResourceManager::GetTexture("pipe" + std::to_string(1 + this->number)), Rotation, Rotate);
            new_pipe.Velocity = glm::vec2(velocity, 0.0f);
            new_pipe.Color = glm::vec3(1.0f, 1.0f, 0.0f);

            this->Pipes.push_back(new_pipe);
        }
    }
}

void GameLevel::Generate_Coins(unsigned start, std::pair<unsigned, unsigned> Height, unsigned int length, unsigned int number, float velocity)
{
    for (unsigned int i = 0; i < number; i++)
    {
        float x = (rand() % ((length - start + 1))) + start;
        float y = (rand() % ((Height.second - Height.first + 1))) + Height.first;

        glm::vec2 pos = glm::vec2(x, y);
        glm::vec2 size = glm::vec2(50.0f, 50.0f);

        bool to_add = true;
        GameObject new_coin = GameObject(pos, size, ResourceManager::GetTexture("coin"));
        new_coin.Velocity = glm::vec2(velocity, 0.0f);

        for (GameObject& coin : this->Coins)
        {
            if (CheckCollision(new_coin, coin))
            {
                i--;
                to_add = false;
                break;
            }
        }

        if (to_add)
        {
            for (GameObject& pipe : this->Pipes)
            {
                if (CheckCollision(new_coin, pipe))
                {
                    i--;
                    to_add = false;
                    break;
                }
            }

            if(to_add)
                this->Coins.push_back(new_coin);
        }
        
    }
}

void GameLevel::setVelocity(float velocity)
{
    for (GameObject& pipe : this->Pipes)
    {
        pipe.Velocity = glm::vec2(velocity, 0.0f);
    }
    for (GameObject& coin : this->Coins)
    {
        coin.Velocity = glm::vec2(velocity, 0.0f);
    }
}

void GameLevel::Draw(SpriteRenderer& renderer, SpriteRenderer& glowrenderer)
{
    /*for (GameObject& tile : this->Bricks)
        if (!tile.Destroyed)
            tile.Draw(renderer);*/
    for (GameObject& pipe : this->Pipes)
    {
        if (!pipe.Destroyed)
        {
            pipe.Draw(glowrenderer);
            pipe.Position -= pipe.Velocity;
        }
    }
    for (GameObject& coin : this->Coins)
    {
        if (!coin.Destroyed)
        {
            coin.Draw(renderer);
            coin.Position -= coin.Velocity;
        }
    }
}

bool GameLevel::IsCompleted()
{
    for (GameObject& tile : this->Bricks)
        if (!tile.IsSolid && !tile.Destroyed)
            return false;
    return true;
}

void GameLevel::init(std::vector<std::vector<unsigned int>> tileData, unsigned int levelWidth, unsigned int levelHeight)
{
    // calculate dimensions
    //unsigned int height = tileData.size();
    //unsigned int width = tileData[0].size(); // note we can index vector at [0] since this function is only called if height > 0
    //float unit_width = levelWidth / static_cast<float>(width), unit_height = levelHeight / height;
    //// initialize level tiles based on tileData		
    //for (unsigned int y = 0; y < height; ++y)
    //{
    //    for (unsigned int x = 0; x < width; ++x)
    //    {
    //        // check block type from level data (2D level array)
    //        if (tileData[y][x] == 1) // solid
    //        {
    //            glm::vec2 pos(unit_width * x, unit_height * y);
    //            glm::vec2 size(unit_width, unit_height);
    //            GameObject obj(pos, size, ResourceManager::GetTexture("block_solid"), glm::vec3(0.8f, 0.8f, 0.7f));
    //            obj.IsSolid = true;
    //            this->Bricks.push_back(obj);
    //        }
    //        else if (tileData[y][x] > 1)	// non-solid; now determine its color based on level data
    //        {
    //            glm::vec3 color = glm::vec3(1.0f); // original: white
    //            if (tileData[y][x] == 2)
    //                color = glm::vec3(0.2f, 0.6f, 1.0f);
    //            else if (tileData[y][x] == 3)
    //                color = glm::vec3(0.0f, 0.7f, 0.0f);
    //            else if (tileData[y][x] == 4)
    //                color = glm::vec3(0.8f, 0.8f, 0.4f);
    //            else if (tileData[y][x] == 5)
    //                color = glm::vec3(1.0f, 0.5f, 0.0f);

    //            glm::vec2 pos(unit_width * x, unit_height * y);
    //            glm::vec2 size(unit_width, unit_height);
    //            this->Bricks.push_back(GameObject(pos, size, ResourceManager::GetTexture("block"), color));
    //        }
    //    }
    //}
}
