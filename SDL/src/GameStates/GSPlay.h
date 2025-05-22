#pragma once
#include "GameStateBase.h"
#include <string>
#include <fstream>      
#include <sstream>     
#include <vector>       
#include <memory>      
#include "Blood/Blood.h"
#include "Obstacle/Monster.h"
#include "Obstacle/Spike.h"
#include "Obstacle/Suriken.h"
class Sprite2D;
class SpriteAnimation;
class MouseButton;
class Sound;
class GSPlay :
	public GameStateBase
{
public:
	GSPlay();
	~GSPlay();

	void	Init() override;
	void	Exit() override;

	void	Pause() override;
	void	Resume() override;

	void	HandleEvents() override;
	void	HandleKeyEvents(SDL_Event& e) override;
	void	HandleTouchEvents(SDL_Event& e) override;
	void	HandleMouseMoveEvents(int x, int y) override;
	void	MovePlayer(float deltaTime);
	void	Update(float deltaTime) override;
	void	LoadTileMap(const std::string& fileName, int tileSize);
	void	Draw(SDL_Renderer* renderer) override;
	bool	CheckCollisionWithWalls();
	void	CheckBloodCollision();
	int m_KeyPress;
	int m_tileMapWidth = 0;
	int m_tileMapHeight = 0;
	std::shared_ptr<SpriteAnimation> GetPlayer() const { return obj; }

private:
	std::shared_ptr<Sprite2D>	m_background;
	std::list<std::shared_ptr<MouseButton>>	m_listButton;
	std::list<std::shared_ptr<SpriteAnimation>>	m_listAnimation, m_bloodList;
	std::shared_ptr<SpriteAnimation> obj;
	std::shared_ptr<SpriteAnimation> key;
	std::shared_ptr<MouseButton> button;
	
	float time = 0.0f;
	float m_Velocity = 10.0f;
	std::vector<std::shared_ptr<Sprite2D>> m_tileMap, m_listWalls;
	std::vector<std::vector<int>> mapData;
	std::shared_ptr <SpriteAnimation> lastRoom;

	std::shared_ptr<Blood> m_Blood;

	enum PlayerState
	{
		IDLE,
		MOVE,
		ATTACK
	};

	std::vector<std::shared_ptr<Monster>> m_monster;
	std::vector<std::shared_ptr<Spike>> m_Spike;
	std::vector<std::shared_ptr<Suriken>> m_suriken;

	PlayerState m_PlayerState = IDLE;
	int attackFrameCount = 0; 
	bool isAttacking = false;
	bool m_hasDied = false;
	bool hasKey = false;
	bool isWin = false;
	Uint32 collisionTime = 0;

};

