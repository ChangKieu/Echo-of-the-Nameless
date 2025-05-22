#include "Suriken.h"
#include "ResourceManagers.h"
#include <SDL.h>
#include <chrono>
#include "Sound.h"

Suriken::Suriken(std::shared_ptr<SpriteAnimation> animation, Vector2 position)
    : m_animation(animation), m_position(position), m_lastCheckTime(0.0f)
{
    m_animation->Set2DPosition(m_position.x, m_position.y);
}

void Suriken::Update(float deltaTime, std::shared_ptr<SpriteAnimation> player, std::shared_ptr<Blood> blood)
{
    m_animation->Set2DPosition(m_position.x, m_position.y);
    m_animation->Update(deltaTime);

    m_lastCheckTime += deltaTime;

    SDL_Rect surikenCollider = GetCollider();
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

    if (SDL_HasIntersection(&surikenCollider, &playerCollider))
    {
        if (m_lastCheckTime >= 0.5f)
        {
			Sound::GetInstance()->LoadSfx("suriden.wav");
			Sound::GetInstance()->PlaySfx("suriden.wav", 0);
            blood->Decrease();
            m_lastCheckTime = 0.0f; 
        }
    }
    else
    {
        m_lastCheckTime = 0.0f; 
    }
}

SDL_Rect Suriken::GetCollider()
{
    SDL_Rect collider;
    collider.x = m_position.x;
    collider.y = m_position.y;
    collider.w = m_animation->GetWidth();
    collider.h = m_animation->GetHeight();
    return collider;
}

std::shared_ptr<SpriteAnimation> Suriken::GetAnimation()
{
    return m_animation;
}
