#include "game.h"
#include "resource_manager.h"
#include "sprite_renderer.h"
#include "game_object.h"

// Game-related State data
SpriteRenderer  *Renderer;
GameObject      *Player;

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
    // load shaders
    ResourceManager::LoadShader("../src/vertex.shader", "../src/fragment.shader", nullptr, "sprite");
    // configure shaders
    glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(this->Width), 
        static_cast<float>(this->Height), 0.0f, -1.0f, 1.0f);
        
    ResourceManager::GetShader("sprite").Use().SetInteger("image", 0);
    ResourceManager::GetShader("sprite").SetMatrix4("projection", projection);
    // set render-specific controls
    Shader myshader=ResourceManager::GetShader("sprite");
    Renderer = new SpriteRenderer(myshader);
    // load textures
    ResourceManager::LoadTexture("/home/shirdi/spring22/cg/assign2/src/bg.png", false, "background");
    ResourceManager::LoadTexture("/home/shirdi/spring22/cg/assign2/src/sample.png", true, "face");
    ResourceManager::LoadTexture("/home/shirdi/spring22/cg/assign2/src/walls.jpg", false, "block");
    ResourceManager::LoadTexture("/home/shirdi/spring22/cg/assign2/src/walls.jpg", false, "block_solid");
    ResourceManager::LoadTexture("/home/shirdi/spring22/cg/assign2/src/paddle.png", true, "paddle");
    // load levels
    GameLevel one; one.Load("/home/shirdi/spring22/cg/assign2/src/one.lvl", this->Width, this->Height);
    GameLevel two; two.Load("/home/shirdi/spring22/cg/assign2/src/two.lvl", this->Width, this->Height / 2);
    GameLevel three; three.Load("/home/shirdi/spring22/cg/assign2/src/three.lvl", this->Width, this->Height / 2);
    GameLevel four; four.Load("/home/shirdi/spring22/cg/assign2/src/four.lvl", this->Width, this->Height / 2);
    this->Levels.push_back(one);
    this->Levels.push_back(two);
    this->Levels.push_back(three);
    this->Levels.push_back(four);
    this->Level = 0;
    // configure game objects
    glm::vec2 playerPos = glm::vec2(this->Width / 2.0f - PLAYER_SIZE.x / 2.0f, this->Height - PLAYER_SIZE.y);
    Player = new GameObject(playerPos, PLAYER_SIZE, ResourceManager::GetTexture("paddle"));
}

void Game::Update(float dt)
{
    
}

void Game::ProcessInput(float dt)
{
    if (this->State == GAME_ACTIVE)
    {
        float velocity = PLAYER_VELOCITY * dt;
        // move playerboard
        if (this->Keys[GLFW_KEY_A])
        {
            if (Player->Position.x >= 0.0f)
                Player->Position.x -= velocity;
        }
        if (this->Keys[GLFW_KEY_D])
        {
            if (Player->Position.x <= this->Width - Player->Size.x)
                Player->Position.x += velocity;
        }
    }
}

void Game::Render()
{
    if(this->State == GAME_ACTIVE)
    {
        // draw background
        Texture2D mytexture;
        mytexture=ResourceManager::GetTexture("background");
        Renderer->DrawSprite(mytexture, glm::vec2(0.0f, 0.0f), glm::vec2(this->Width, this->Height), 0.0f);
        // draw level
        this->Levels[this->Level].Draw(*Renderer);
        // draw player
        Player->Draw(*Renderer);
    }
}