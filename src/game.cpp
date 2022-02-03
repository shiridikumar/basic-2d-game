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
time_t first;
time_t second = 0.0f;
vector<GameObject *> obstacles;
glm::vec3 pos = glm::vec3(0.0f, 0.0f, 0.0f);
glm::vec3 front = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
GameObject *Player;
int player_level_score = 0;
int player_total_score = 0;
int coin_value = 10;
vector<EnemyObject *> enemies;
map<pair<float, float>, int> reserved;
map<pair<float, float>, int> nextlevel;
vector<GameObject *> coins;
const glm::vec2 INITIAL_BALL_VELOCITY(0.0f, -250.0f);
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
    glm::mat4 view = glm::mat4(1.0f);
    view = glm::lookAt(pos, pos + front, up);
    ResourceManager::GetShader("sprite").SetMatrix4("view", view);
    // set render-specific controls
    Shader myshader = ResourceManager::GetShader("sprite");
    Renderer = new SpriteRenderer(myshader);
    // load textures
    ResourceManager::LoadTexture("../src/bg.png", false, "background");
    ResourceManager::LoadTexture("../src/enemy.png", true, "face");
    ResourceManager::LoadTexture("../src/walls.jpg", false, "block");
    ResourceManager::LoadTexture("../src/coin.png", true, "coins");
    ResourceManager::LoadTexture("../src/player.png", true, "player");
    // load levels
    GameLevel one;
    one.Load("../src/one.lvl", this->Width, this->Height);
    this->Levels.push_back(one);
    this->Level = 0;
    // configure game objects
    glm::vec2 playerPos = glm::vec2(this->Width / 2.0f, this->Height / 2.0f);
    float randomx = 0;
    float randomy = 0;
    this->num_walls = 5;
    reserved[make_pair(6, 0)] = 1;
    reserved[make_pair(7, 0)] = 1;
    nextlevel[make_pair(6, 0)] = 1;
    nextlevel[make_pair(7, 0)] = 1;
    for (int i = 0; i < this->num_walls; i++)
    {
        srand(time(NULL) + i);
        float randomx = (rand() % (int)(12)) + 1;

        float randomy = (rand() % (int)(12) - 2 + 1) + 2;
        reserved[make_pair(randomx, randomy)] = 1;
        obstacles.push_back(new GameObject(glm::vec2(randomx * (coin_size.x), randomy * coin_size.x), coin_size, ResourceManager::GetTexture("block")));
    }

    for (int i = 0; i < this->num_enemies; i++)
    {
        srand(time(NULL) + i);

        // Get a random number
        float randomx = (rand() % 12);

        float randomy = (rand() % 13);

        glm::vec2 ballPos = glm::vec2(randomx * coin_size.x, randomy * coin_size.x);
        enemies.push_back(new EnemyObject(ballPos, BALL_RADIUS, INITIAL_BALL_VELOCITY,
                                          ResourceManager::GetTexture("face")));
    }
    for (int i = 0; i < this->num_coins; i++)
    {
        srand(time(NULL) + i);
        float randomx = (rand() % (int)(12)) + 1;

        float randomy = (rand() % (int)(13)) + 1;
        while (reserved[make_pair(randomx, randomy)] == 1)
        {
            randomx = (rand() % (int)(12)) + 1;
            randomy = (rand() % (int)(13)) + 1;
        }
        //cout << randomx << " " << randomy << endl;
        glm::vec2 coinPos = glm::vec2(randomx * coin_size.x, randomy * coin_size.x);
        coins.push_back(new GameObject(coinPos, coin_size, ResourceManager::GetTexture("coins")));
    }

    Player = new GameObject(glm::vec2(this->Width / 2 - PLAYER_SIZE.x / 2, this->Height - PLAYER_SIZE.y - this->Height / 15), PLAYER_SIZE, ResourceManager::GetTexture("player"));
}

void Game::Update(float dt)
{
    for (int i = 0; i < this->num_enemies; i++)
    {
        enemies[i]->Move(dt, this->Width, this->Height * (this->level_no), (this->Height) * (level_no - 1), this->level_no);
    }
    for (int i = 0; i < coins.size(); i++)
    {
        if (abs(coins[i]->Position.y - Player->Position.y) < 20.0f && abs(coins[i]->Position.x - Player->Position.x) < 20.0f && time(NULL) - first / 60 > 2)
        {
            first = time(NULL);
            coins[i]->Position.x = -800;
            coins[i]->Position.y = -1000;
            coins.erase(coins.begin() + i);
            this->num_coins--;
            player_level_score += coin_value;

            break;
        }
    }
    for (int i = 0; i < enemies.size(); i++)
    {
        if (abs(enemies[i]->Position.y - Player->Position.y) < 45.0f && abs(enemies[i]->Position.x - Player->Position.x) < 45.0f)
        {
            //cout<<player_level_score<<endl;
            cout<<"GAME OVER"<<endl;
            cout<<"Levels cleared : "<<this->level_no-1<<endl;
            cout << "----------------------------------------------------" << endl;
            cout << "Total score : " << player_level_score << endl;
            exit(0);
        }
    }

    ////cout<<coins.size()<<endl;
}

void Game::level2()
{
    this->level_no++;
    int pad = this->Height * (level_no - 1);
    //cout << pad << endl;
    reserved.clear();
    glm::vec2 playerPos = glm::vec2(this->Width / 2.0f, this->Height / 2.0f);
    float randomx = 0;
    float randomy = 0;
    this->num_walls = 5 * this->level_no;
    this->num_enemies = 4 * this->level_no;
    this->num_coins = 5 * this->level_no;

    GameLevel two;
    Levels.clear();
    if (this->level_no == 2)
    {
        two.Load("../src/two.lvl", this->Width, this->Height + pad);
    }
    else if (this->level_no == 3)
    {

        two.Load("../src/three.lvl", this->Width, this->Height + pad);
    }
    else
    {
        cout<<"Game over"<<endl;
        cout<<"****Congratulations you have cleared all the levels*****"<<endl;
        cout << "----------------------------------------------------" << endl;
        cout << "Total score : " << player_level_score << endl;
        exit(0);
    }
    this->Levels.push_back(two);

    obstacles.clear();
    nextlevel.clear();
    coins.clear();
    enemies.clear();
    reserved[make_pair(6, 0 + (int)(pad / 50.0f))] = 1;
    reserved[make_pair(7, 0 + (int)(pad / 50.0f))] = 1;
    nextlevel[make_pair(7, 0 + (int)(pad / 50.0f))] = 1;
    nextlevel[make_pair(6, 0 + (int)(pad / 50.0f))] = 1;
    const glm::vec2 velocity = INITIAL_BALL_VELOCITY - glm::vec2(0.0f, this->level_no * 30);

    for (int i = 0; i < this->num_walls; i++)
    {
        srand(time(NULL) + i);
        float randomx = (rand() % (int)(12)) + 1;
        float randomy = (rand() % (int)(12 - 2 + 1)) + 2 + (int)(pad / 50.0f);
        reserved[make_pair(randomx, randomy)] = 1;
        obstacles.push_back(new GameObject(glm::vec2(randomx * (coin_size.x), randomy * coin_size.x), coin_size, ResourceManager::GetTexture("block")));
    }

    for (int i = 0; i < this->num_enemies; i++)
    {
        srand(time(NULL) + i);

        // Get a random number
        float randomx = (rand() % 12);
        float randomy = (rand() % 13) + (int)(pad / 50.0f);
        enemies.push_back(new EnemyObject(glm::vec2(randomx * coin_size.x, randomy * coin_size.x), BALL_RADIUS, velocity,
                                          ResourceManager::GetTexture("face")));
    }
    for (int i = 0; i < this->num_coins; i++)
    {
        srand(time(NULL) + i);
        float randomx = (rand() % (int)(12)) + 1;

        float randomy = (rand() % (int)(13)) + 1 + (int)(pad / 50.0f);
        while (reserved[make_pair(randomx, randomy)] == 1)
        {
            randomx = (rand() % (int)(12)) + 1;
            randomy = (rand() % (int)(13)) + 1 + (int)(pad / 50.0f);
        }
        glm::vec2 coinPos = glm::vec2(randomx * coin_size.x, randomy * coin_size.x);
        coins.push_back(new GameObject(coinPos, coin_size, ResourceManager::GetTexture("coins")));
    }

    Player = new GameObject(glm::vec2(this->Width / 2 - PLAYER_SIZE.x / 2, this->Height + pad - PLAYER_SIZE.y - this->Height / 15), PLAYER_SIZE, ResourceManager::GetTexture("player"));
}

void Game::ProcessInput(float dt)
{
    if (this->State == GAME_ACTIVE)
    {
        float velocity = PLAYER_VELOCITY * dt;

        if (this->Keys[GLFW_KEY_A])
        {
            int flag = 1;
            if (Player->Position.x >= 0.0f + 50.0f)
            {
                for (int i = 0; i < this->num_walls; i++)
                {
                    if ((Player->Position.x - obstacles[i]->Position.x) < 50.0f && Player->Position.x - obstacles[i]->Position.x > 0 && (abs(Player->Position.y - obstacles[i]->Position.y) < 45.0f))
                    {
                        flag = 0;
                        break;
                    }
                }
                if (flag == 1)
                {
                    Player->Position.x -= velocity;
                }
            }
        }
        if (this->Keys[GLFW_KEY_D])
        {
            int flag = 1;
            if (Player->Position.x <= this->Width - Player->Size.x - 50.0f)
            {
                for (int i = 0; i < this->num_walls; i++)
                {
                    if ((obstacles[i]->Position.x - Player->Position.x) < 50.0f && obstacles[i]->Position.x - Player->Position.x > 0 && (abs(Player->Position.y - obstacles[i]->Position.y) < 45.0f))
                    {
                        flag = 0;
                        break;
                    }
                }
                if (flag == 1)
                {
                    Player->Position.x += velocity;
                }
            }
        }
        if (this->Keys[GLFW_KEY_W])
        {
            int flag = 1;
            if (nextlevel[make_pair((int)Player->Position.x / 50, (int)Player->Position.y / 50)] == 1)
            {
                glm::mat4 view = glm::mat4(1.0f);
                pos = glm::vec3(0.0f, this->Height * (this->level_no), 0.0f);
                view = glm::lookAt(pos, pos + front, up);
                ResourceManager::GetShader("sprite").SetMatrix4("view", view);
                level2();
            }

            if (Player->Position.y > 50.0f + ((this->level_no - 1) * (this->Height)))
            {

                for (int i = 0; i < this->num_walls; i++)
                {
                    if ((abs(obstacles[i]->Position.x - Player->Position.x) < 45.0f) && ((Player->Position.y - obstacles[i]->Position.y) < 50.0f) && Player->Position.y - obstacles[i]->Position.y > 0.0f)
                    {
                        flag = 0;
                        break;
                    }
                }
                if (flag == 1)
                {
                    ////cout<<(int)(Player->Position.x/50.0f)<<" "<<(int)(Player->Position.y/50.0f)<<endl;
                    Player->Position.y -= velocity;
                }
            }
        }
        if (this->Keys[GLFW_KEY_S])
        {
            int flag = 1;
            if (Player->Position.y < this->Height * (this->level_no) + -2 * 50.0f)
            {
                for (int i = 0; i < this->num_walls; i++)
                {
                    if ((abs(obstacles[i]->Position.x - Player->Position.x) < 45.0f) && ((obstacles[i]->Position.y - Player->Position.y) < 50.0f) && (obstacles[i]->Position.y - Player->Position.y > 0.0f))
                    {
                        flag = 0;
                        break;
                    }
                }
                if (flag == 1)
                {
                    Player->Position.y += velocity;
                }
            }
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
            // if(this->level_no==2){
            //     //cout<<enemies[i]->Position.y<<endl;
            // }
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