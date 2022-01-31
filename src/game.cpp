#include "game.h"
#include "resource_manager.h"
#include "sprite_renderer.h"
#include "game_object.h"
#include "enemy_object.h"
#include <cstdlib>
#include<time.h>

#include <bits/stdc++.h>
using namespace std;
// Game-related State data
SpriteRenderer *Renderer;
GameObject *Player;
vector<EnemyObject *> enemies;
const glm::vec2 INITIAL_BALL_VELOCITY(0.0f, 350.0f);
// Radius of the ball object
const float BALL_RADIUS = 25.0f;

Game::Game(unsigned int width, unsigned int height)
    : State(GAME_ACTIVE), Keys(), Width(width), Height(height)
{
}

Game::~Game()
{
    delete Renderer;
    delete Player;
}

void Game::Init()
{
    EnemyObject *Ball;
    this->level_no = 1;
    this->num_enemies = 5;
    // load shaders
    ResourceManager::LoadShader("../src/vertex.shader", "../src/fragment.shader", nullptr, "sprite");
    // configure shaders
    glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(this->Width),
                                      static_cast<float>(this->Height), 0.0f, -1.0f, 1.0f);

    ResourceManager::GetShader("sprite").Use().SetInteger("image", 0);
    ResourceManager::GetShader("sprite").SetMatrix4("projection", projection);
    // set render-specific controls
    Shader myshader = ResourceManager::GetShader("sprite");
    Renderer = new SpriteRenderer(myshader);
    // load textures
    ResourceManager::LoadTexture("/home/shirdi/spring22/cg/assign2/src/bg.png", false, "background");
    ResourceManager::LoadTexture("/home/shirdi/spring22/cg/assign2/src/enemy.png", true, "face");
    ResourceManager::LoadTexture("/home/shirdi/spring22/cg/assign2/src/walls.jpg", false, "block");
    // load levels
    GameLevel one;
    one.Load("/home/shirdi/spring22/cg/assign2/src/one.lvl", this->Width, this->Height);
    this->Levels.push_back(one);
    this->Level = 0;
    // configure game objects
    glm::vec2 playerPos = glm::vec2(this->Width / 2.0f - PLAYER_SIZE.x / 2.0f, this->Height - PLAYER_SIZE.y);
    Player = new GameObject(playerPos, PLAYER_SIZE, ResourceManager::GetTexture("paddle"));
    float randomx=0;
    float randomy=0;
    for (int i = 0; i < 5; i++)
    {
        srand ( time(NULL) +i);

        // Get a random number
        float randomx = (rand() % this->Width);

        float randomy = (rand() % this->Height);
        cout<<randomx<<" "<<randomy<<endl;

        glm::vec2 ballPos = glm::vec2(randomx, randomy);
        enemies.push_back(new  EnemyObject(ballPos, BALL_RADIUS, INITIAL_BALL_VELOCITY,
                               ResourceManager::GetTexture("face")));
    }
}

void Game::Update(float dt)
{
    for (int i = 0; i < 5; i++)
    {
        enemies[i]->Move(dt, this->Width, this->Height);
    }
}

void Game::ProcessInput(float dt)
{
    if (this->State == GAME_ACTIVE)
    {
        float velocity = PLAYER_VELOCITY * dt;
        // move playerboard
        // if (this->Keys[GLFW_KEY_A])
        // {
        //     if (Player->Position.x >= 0.0f)
        //     {
        //         Player->Position.x -= velocity;
        //         if (Ball->Stuck)
        //             Ball->Position.x -= velocity;
        //     }
        // }
        // if (this->Keys[GLFW_KEY_D])
        // {
        //     if (Player->Position.x <= this->Width - Player->Size.x)
        //     {
        //         Player->Position.x += velocity;
        //         if (Ball->Stuck)
        //             Ball->Position.x += velocity;
        //     }
        // }
        if (this->Keys[GLFW_KEY_SPACE])
            for (int i = 0; i < 5; i++)
            {
                enemies[i]->Stuck = false;
            }
    }
}

void Game::Render()
{
    if (this->State == GAME_ACTIVE)
    {
        // draw background
        Texture2D mytexture;
        mytexture = ResourceManager::GetTexture("background");
        Renderer->DrawSprite(mytexture, glm::vec2(0.0f, 0.0f), glm::vec2(this->Width, this->Height), 0.0f);
        // draw level
        this->Levels[this->Level].Draw(*Renderer);
        for (int i = 0; i < 5; i++)
        {
            enemies[i]->Draw(*Renderer);
        }
        //Ball->Draw(*Renderer);
    }
}