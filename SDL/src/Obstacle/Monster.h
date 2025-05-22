#pragma once

#include <memory>
#include "SpriteAnimation.h"
#include "Blood/Blood.h"
enum class MonsterState
{
    Idle,
    Attack
};

class Monster
{
public:
    Monster(std::shared_ptr<SpriteAnimation> animation, Vector2 startPos, Vector2 endPos, float speed);

    void Update(float deltaTime, std::shared_ptr<SpriteAnimation> player, std::shared_ptr<Blood> blood);
    void Draw();
    SDL_Rect GetCollider();  
    std::shared_ptr<SpriteAnimation> GetAnimation();

private:
    std::shared_ptr<SpriteAnimation> m_animation;
    Vector2 m_position;
    Vector2 m_startPos;
    Vector2 m_endPos;
    float m_speed;
    MonsterState m_state;
    bool m_hasCheck = false;
    void SetState(MonsterState newState); 
};
