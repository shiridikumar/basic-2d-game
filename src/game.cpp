#include "game.h"
#include "resource_manager.h"
#include "sprite_renderer.h"
#include "game_object.h"
#include "enemy_object.h"
#include <cstdlib>
#include <time.h>

#include <bits/stdc++.h>
using namespace std;
// Game-related State data
SpriteRenderer *Renderer;
vector<GameObject *> obstacles;
map< pair<float,float>,int > reserved;
GameObject *Player;
vector<EnemyObject *> enemies;
vector<GameObject *> coins;
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

}

void Game::Init()
{
    EnemyObject *Ball;
    GameObject *coin;
    this->level_no = 1;
    this->num_enemies = 5;
    this->num_coins = 5;
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
    ResourceManager::LoadTexture("/home/shirdi/spring22/cg/assign2/src/coin.png", true, "coins");
    ResourceManager::LoadTexture("/home/shirdi/spring22/cg/assign2/src/player.png", true, "player");
    // load levels
    GameLevel one;
    one.Load("/home/shirdi/spring22/cg/assign2/src/one.lvl", this->Width, this->Height);
    this->Levels.push_back(one);
    this->Level = 0;
    // configure game objects
    glm::vec2 playerPos = glm::vec2(this->Width / 2.0f, this->Height / 2.0f);
    float randomx = 0;
    float randomy = 0;
    this->num_walls=5;
    for(int i=0;i<this->num_walls;i++){
        srand(time(NULL) + i);
        float randomx = (rand() % (int)(this->Width - (2*this->Width / 14)-2*coin_size.x)) + this->Width / 14+coin_size.x;

        float randomy = (rand() % (int)( this->Height - (2*this->Height / 15)-3*coin_size.y)) + this->Height / 15+coin_size.y;
        cout << randomx << " " << randomy << endl;
        reserved[make_pair(randomx,randomy)]=1;
        obstacles.push_back(new GameObject(glm::vec2(randomx,randomy), coin_size, ResourceManager::GetTexture("block")));

        
    }

    for (int i = 0; i < this->num_enemies; i++)
    {
        srand(time(NULL) + i);

        // Get a random number
        float randomx = (rand() % this->Width);

        float randomy = (rand() % this->Height);
        cout << randomx << " " << randomy << endl;

        glm::vec2 ballPos = glm::vec2(randomx, randomy);
        enemies.push_back(new EnemyObject(ballPos, BALL_RADIUS, INITIAL_BALL_VELOCITY,
                                          ResourceManager::GetTexture("face")));
    }
    for (int i = 0; i < this->num_coins; i++)
    {
        srand(time(NULL) + i);
        float randomx = (rand() % (int)(this->Width - (2*this->Width / 14)-2*coin_size.x)) + this->Width / 14+coin_size.x;

        float randomy = (rand() % (int)( this->Height - (2*this->Height / 15)-2*coin_size.y)) + this->Height / 15+coin_size.y;
        cout << randomx << " " << randomy << endl;
        while(reserved[make_pair(randomx,randomy)]==1){
            float randomx = (rand() % (int)(this->Width - (2*this->Width / 14)-2*coin_size.x)) + this->Width / 14+coin_size.x;

            float randomy = (rand() % (int)( this->Height - (2*this->Height / 15)-2*coin_size.y)) + this->Height / 15+coin_size.y;

        
        }
        glm::vec2 coinPos = glm::vec2(randomx, randomy);
        coins.push_back(new GameObject(coinPos, coin_size, ResourceManager::GetTexture("coins")));
    }

    Player =new GameObject(glm::vec2(this->Width/2-PLAYER_SIZE.x/2,this->Height-PLAYER_SIZE.y-this->Height/15), PLAYER_SIZE, ResourceManager::GetTexture("player"));
}

void Game::Update(float dt)
{
    for (int i = 0; i < this->num_enemies; i++)
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
            for (int i = 0; i < this->num_enemies; i++)
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
        for (int i = 0; i < this->num_enemies; i++)
        {
            enemies[i]->Draw(*Renderer);
        }
        for (int i = 0; i < this->num_enemies; i++)
        {
            coins[i]->Draw(*Renderer);
        }
        Player->Draw(*Renderer);
        for (int i = 0; i < this->num_walls; i++)
        {
            obstacles[i]->Draw(*Renderer);
        }

        //Ball->Draw(*Renderer);
    }
}