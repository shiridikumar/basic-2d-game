#ifndef EnemyObject_H
#define EnemyObject_H

#include <glad/glad.h>
#include <glm/glm.hpp>

#include "game_object.h"
#include "texture.h"



class EnemyObject : public GameObject
{
public:
    // ball state	
    float   Radius;
    bool    Stuck;
    // constructor(s)
    EnemyObject();
    EnemyObject(glm::vec2 pos, float radius, glm::vec2 velocity, Texture2D sprite);
    // moves the ball, keeping it constrained within the window bounds (except bottom edge); returns new position
    glm::vec2 Move(float dt, unsigned int window_width,unsigned int window_height);
    // resets the ball to original state with given position and velocity
    void      Reset(glm::vec2 position, glm::vec2 velocity);
};

#endif