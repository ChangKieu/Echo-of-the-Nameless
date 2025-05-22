#include "Spike.h"
#include "ResourceManagers.h"
#include <SDL.h>
#include "Sound.h"

Spike::Spike(std::shared_ptr<SpriteAnimation> animation, Vector2 position)
    : m_animation(animation), m_position(position), m_hasCheck(false)
{
    m_animation->Set2DPosition(m_position.x, m_position.y);
}

void Spike::Update(float deltaTime, std::shared_ptr<SpriteAnimation> player, std::shared_ptr<Blood> blood)
{
    m_animation->Set2DPosition(m_position.x, m_position.y);
    m_animation->Update(deltaTime);

    int currentFrame = m_animation->GetCurrentFrame();

    if (currentFrame >= 8 && currentFrame <= 10)
    {
        if (!m_hasCheck)
        {
            SDL_Rect spikeCollider = GetCollider();
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

            if (SDL_HasIntersection(&spikeCollider, &playerCollider))
            {
                Sound::GetInstance()->LoadSfx("hurt.mp3");
                Sound::GetInstance()->PlaySfx("hurt.mp3");
                blood->Decrease();
            }

            m_hasCheck = true; 
        }
    }
    else
    {
        m_hasCheck = false;
    }
}

SDL_Rect Spike::GetCollider()
{
    SDL_Rect collider;
    collider.x = m_position.x;
    collider.y = m_position.y;
    collider.w = m_animation->GetWidth();
    collider.h = m_animation->GetHeight();
    return collider;
}

std::shared_ptr<SpriteAnimation> Spike::GetAnimation()
{
    return m_animation;
}
