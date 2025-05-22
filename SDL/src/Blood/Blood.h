#pragma once
#include "Sprite2D.h"
#include <vector>

class Blood
{
public:
    Blood();
    ~Blood();

    void Init(int maxHealth = 5);
    void SetPosition(float x, float y);
    void SetHealth(int health);
    int GetHealth() const;
    int GetMaxHealth() const;

    void Decrease(); 
    void Increase();

    std::vector<std::shared_ptr<Sprite2D>> GetHeartsToDraw() const;

private:
    int m_MaxHealth;
    int m_CurrentHealth;
    std::vector<std::shared_ptr<Sprite2D>> m_Hearts;
    Vector2 m_StartPos;
    float m_Spacing = 36.0f;
};
