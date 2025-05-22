#include "Monster.h"
#include "ResourceManagers.h"
#include "Sound.h"

Monster::Monster(std::shared_ptr<SpriteAnimation> animation, Vector2 startPos, Vector2 endPos, float speed)
    : m_animation(animation), m_position(startPos), m_startPos(startPos), m_endPos(endPos), m_speed(speed),
    m_state(MonsterState::Idle), m_hasCheck(false)
{
    m_animation->Set2DPosition(m_position.x, m_position.y);
}

void Monster::Update(float deltaTime, std::shared_ptr<SpriteAnimation> player, std::shared_ptr<Blood> blood)
{
    Vector2 playerPos(player->Get2DPosition().x, player->Get2DPosition().y);
    float distance = (playerPos - m_position).Length();

    if (distance <= 150.0f)
    {
        SetState(MonsterState::Attack);

        int currentFrame = m_animation->GetCurrentFrame();
        if (currentFrame >= 6 && currentFrame <= 8)
        {
            if (!m_hasCheck)
            {
                SDL_Rect monsterCollider = GetCollider();
                int playerX = player->Get2DPosition().x;
                int playerY = player->Get2DPosition().y;
                int playerWidth = player->GetWidth();
                int playerHeight = player->GetHeight();

                int offsetX = playerWidth * 0.3f;
                int offsetY = playerHeight * 0.2f;

                SDL_Rect playerCollider = {
                    playerX + offsetX,
                    playerY + 1.5f * offsetY,
                    playerWidth - offsetX * 2,
                    playerHeight - offsetY * 2
                };

                if (SDL_HasIntersection(&monsterCollider, &playerCollider))
                {
                    blood->Decrease();
                    Sound::GetInstance()->LoadSfx("monster.mp3");
                    Sound::GetInstance()->PlaySfx("monster.mp3");
                    
                }

                m_hasCheck = true;
            }
        }
        else
        {
            m_hasCheck = false;
        }

    }
    else
    {
        SetState(MonsterState::Idle);

        Vector2 direction = m_endPos - m_startPos;
        direction.Normalize();
        m_position = m_position + direction * m_speed * deltaTime;

        if (m_position.x > m_startPos.x)
        {
            m_animation->SetFlip(SDL_FLIP_NONE);
        }
        else if (m_position.x < m_startPos.x)
        {
            m_animation->SetFlip(SDL_FLIP_HORIZONTAL);
        }

        if ((m_position - m_endPos).Length() < 5.0f)
        {
            std::swap(m_startPos, m_endPos);
        }

    }

    m_animation->Set2DPosition(m_position.x, m_position.y);
    m_animation->Update(deltaTime);
}


void Monster::SetState(MonsterState newState)
{
    if (m_state == newState) return; 

    m_state = newState;

    if (m_state == MonsterState::Idle)
    {
        auto texture = ResourceManagers::GetInstance()->GetTexture("monster_movement.png");
        m_animation->SetTexture(texture);
        m_animation->SetFrameCount(10);
    }
    else if (m_state == MonsterState::Attack)
    {
        auto texture = ResourceManagers::GetInstance()->GetTexture("monster_attack.png");
        m_animation->SetTexture(texture);
        m_animation->SetFrameCount(9);

    }
}

SDL_Rect Monster::GetCollider()
{
    SDL_Rect collider;
    collider.x = m_position.x;
    collider.y = m_position.y;
    collider.w = m_animation->GetWidth();
    collider.h = m_animation->GetHeight();
    return collider;
}
std::shared_ptr<SpriteAnimation> Monster::GetAnimation() 
{
    return m_animation;
}