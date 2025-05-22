#ifndef SURIKEN_H
#define SURIKEN_H

#include <memory>
#include "SpriteAnimation.h"
#include "Blood/Blood.h"

class Suriken
{
public:
    Suriken(std::shared_ptr<SpriteAnimation> animation, Vector2 position);

    void Update(float deltaTime, std::shared_ptr<SpriteAnimation> player, std::shared_ptr<Blood> blood);

    SDL_Rect GetCollider();

    std::shared_ptr<SpriteAnimation> GetAnimation();

private:
    std::shared_ptr<SpriteAnimation> m_animation;  
    Vector2 m_position;                           
    float m_lastCheckTime;                         
};

#endif 
