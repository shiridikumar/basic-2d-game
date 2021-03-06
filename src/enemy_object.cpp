#include "enemy_object.h"
#include "game.h"
#include<bits/stdc++.h>
using namespace std;
extern map< pair<float,float>,int > reserved;

EnemyObject::EnemyObject() 
    : GameObject(), Radius(12.5f), Stuck(true) { }

EnemyObject::EnemyObject(glm::vec2 pos, float radius, glm::vec2 velocity, Texture2D sprite)
    : GameObject(pos, glm::vec2(radius * 2.0f, radius * 2.0f), sprite, glm::vec3(1.0f), velocity), Radius(radius), Stuck(true) { }

glm::vec2 EnemyObject::Move(float dt, unsigned int window_width,unsigned int window_height,int base,int level)
{
    // if not stuck to player board
   
    
        // move the ball
        if(this->Velocity.y > 0){
            if(reserved[make_pair((int)(this->Position.x/50.0f),(int)(this->Position.y/50.0f)+1)]){
                this->Velocity.y=-this->Velocity.y;
            }
            else{
                this->Position += this->Velocity * dt;
            }
        }
        else{
            if(reserved[make_pair((int)(this->Position.x/50.0f),(int)(this->Position.y/50.0f))]){
                this->Velocity.y=-this->Velocity.y;
            }
            else{
                this->Position += this->Velocity * dt;
            }

        }
        
        // then check if outside window bounds and if so, reverse velocity and restore at correct position
        if (this->Position.x <= window_width/14)
        {
            this->Velocity.x = -this->Velocity.x;
            this->Position.x = window_width/14;
        }
        else if (this->Position.x + this->Size.x >= (window_width-window_width/14))
        {
            this->Velocity.x = -this->Velocity.x;
            this->Position.x = window_width - this->Size.x-window_width/14;
        }
        if (this->Position.y <=(window_height/15)+base-(level-1)*50.0f)
        {
            this->Velocity.y = -this->Velocity.y;
            this->Position.y = window_height/15+base-(level-1)*50.0f;
        }
        else if (this->Position.y + this->Size.y >= window_height-window_height/15+(level-1)*50.0f)
        {
            this->Velocity.y = -this->Velocity.y;
            this->Position.y = window_height - this->Size.y-window_height/15+(level-1)*50.0f;
        }
    return this->Position;
}

// resets the ball to initial Stuck Position (if ball is outside window bounds)
void EnemyObject::Reset(glm::vec2 position, glm::vec2 velocity)
{
    this->Position = position;
    this->Velocity = velocity;
    this->Stuck = true;
}