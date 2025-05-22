#include "blood.h"
#include "ResourceManagers.h"
#include "Sound.h"
#include "GSPlay.h"

Blood::Blood()
{
    m_MaxHealth = 5;
    m_CurrentHealth = 5;
    m_StartPos = Vector2(10, 10);
}

Blood::~Blood() {}

void Blood::Init(int maxHealth)
{
    m_MaxHealth = maxHealth;
    m_CurrentHealth = maxHealth;

    m_Hearts.clear();
    auto heartTexture = ResourceManagers::GetInstance()->GetTexture("heart.png");

    for (int i = 0; i < m_MaxHealth; ++i)
    {
        auto heart = std::make_shared<Sprite2D>(heartTexture, SDL_FLIP_NONE);
        heart->SetSize(32, 32);
        heart->Set2DPosition(m_StartPos.x + i * m_Spacing, m_StartPos.y);
        m_Hearts.push_back(heart);
    }
}

void Blood::SetPosition(float x, float y)
{
    m_StartPos = Vector2(x, y);
    for (int i = 0; i < m_Hearts.size(); ++i)
    {
        m_Hearts[i]->Set2DPosition(m_StartPos.x + i * m_Spacing, m_StartPos.y);
    }
}

void Blood::SetHealth(int health)
{
    m_CurrentHealth = std::max(0, std::min(health, m_MaxHealth));
}

int Blood::GetHealth() const
{
    return m_CurrentHealth;
}

int Blood::GetMaxHealth() const
{
    return m_MaxHealth;
}

void Blood::Decrease()
{
    if (m_CurrentHealth > 0)
    {
        m_CurrentHealth--;

    }
}

void Blood::Increase()
{
    if (m_CurrentHealth < m_MaxHealth)
        m_CurrentHealth++;
}

std::vector<std::shared_ptr<Sprite2D>> Blood::GetHeartsToDraw() const
{
    std::vector<std::shared_ptr<Sprite2D>> heartsToDraw;
    for (int i = 0; i < m_CurrentHealth; ++i)
    {
        heartsToDraw.push_back(m_Hearts[i]);
    }
    return heartsToDraw;
}
