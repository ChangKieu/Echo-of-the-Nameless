#pragma once
#include "SpriteAnimation.h"
#include "Blood/Blood.h"
class Spike
{
public:
    Spike(std::shared_ptr<SpriteAnimation> animation, Vector2 position);

    void Update(float deltaTime, std::shared_ptr<SpriteAnimation> player, std::shared_ptr<Blood> blood);
    SDL_Rect GetCollider();
    std::shared_ptr<SpriteAnimation> GetAnimation();

private:
    std::shared_ptr<SpriteAnimation> m_animation;
    Vector2 m_position;  
    bool m_hasCheck = false;
};
