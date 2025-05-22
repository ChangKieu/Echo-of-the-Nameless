#include "GSPlay.h"
#include "ResourceManagers.h"
#include "GameObject/MouseButton.h"
#include "GameObject/Sprite2D.h"
#include "GameObject/SpriteAnimation.h"
#include "GameObject/Camera.h"
#include "KeyState.h"
#include "Sound.h"
#include "Map/RandomMap.h"
#include "Obstacle/Obstacle.h"
#include "Map/GameData.h"
GSPlay::GSPlay()
{
}


GSPlay::~GSPlay()
{
}


void GSPlay::Init()
{
	auto texture = ResourceManagers::GetInstance()->GetTexture("bg_play1.tga");
	RandomMap map;
	auto& data = GameData::GetInstance();

	if (!data.isRetry) {
		map.generateRooms();
		data.roomList = map.getRoomList();
	}
	else {
		map.setRoomList(data.roomList);
		data.isRetry = false; 
	}
	LoadTileMap("random_map.txt", 100);

	// button close
	texture = ResourceManagers::GetInstance()->GetTexture("btn_Close.png");
	std::shared_ptr<MouseButton> button = std::make_shared<MouseButton>(texture, SDL_FLIP_NONE);
	//btnClose = std::make_shared<MouseButton>(texture);
	button->SetSize(60, 60);
	button->Set2DPosition(SCREEN_WIDTH - button->GetWidth() - 10, 20);
	button->SetOnClick([this]() {
		GameStateMachine::GetInstance()->ChangeState(StateType::STATE_MENU);
		});
	m_listButton.push_back(button);
	
	m_Blood = std::make_shared<Blood>();
	m_Blood->Init(5); 
	m_Blood->SetPosition(20, 20);

	m_KeyPress = 0;
}

void GSPlay::Exit()
{
}


void GSPlay::Pause()
{

}
void GSPlay::Resume()
{
}


void GSPlay::HandleEvents()
{
}

void GSPlay::HandleKeyEvents(SDL_Event& e)
{
	//If a key was pressed
	if (e.type == SDL_KEYDOWN )//&& e.key.repeat == 0) //For e.key.repeat it's because key repeat is enabled by default and if you press and hold a key it will report multiple key presses. That means we have to check if the key press is the first one because we only care when the key was first pressed.
	{
		//Adjust the velocity
		switch (e.key.keysym.sym)                                                                                            
		{                                                                                                                     
		case SDLK_LEFT:
			m_KeyPress |= 1;
			break;
		case SDLK_DOWN:
			m_KeyPress |= 1 << 1;
			break;
		case SDLK_RIGHT:
			m_KeyPress |= 1 << 2;
			break;
		case SDLK_UP:
			m_KeyPress |= 1 << 3;
			break;
		default:
			break;
		}
	}
	////Key Up
	else if (e.type == SDL_KEYUP )//&& e.key.repeat == 0)
	{
		//Adjust the velocity
		switch (e.key.keysym.sym)
		{
		case SDLK_LEFT:
			m_KeyPress ^= 1;
			break;
		case SDLK_DOWN:
			m_KeyPress ^= 1 << 1;
			break;
		case SDLK_RIGHT:
			m_KeyPress ^= 1 << 2;
			break;
		case SDLK_UP:
			m_KeyPress ^= 1 << 3;
			break;
		default:
			break;
		}
	}
	/*else if (e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_LEFT && m_PlayerState != ATTACK)
	{
		m_PlayerState = ATTACK;
		auto attackTexture = ResourceManagers::GetInstance()->GetTexture("attack_test.png");
		obj->SetFrameCount(12);
		obj->SetTexture(attackTexture);
	}*/
}

void GSPlay::HandleTouchEvents(SDL_Event& e)
{
	for (auto button : m_listButton)
	{
		if (button->HandleTouchEvent(&e))
		{
			break;
		}
	}
}

void GSPlay::HandleMouseMoveEvents(int x, int y)
{
}
void GSPlay::MovePlayer(float deltaTime)
{
	if (m_PlayerState == ATTACK)
	{
		if (obj->GetCurrentFrame() == obj->GetFrameCount() - 1) 
		{
			m_PlayerState = IDLE;
			auto idleTexture = ResourceManagers::GetInstance()->GetTexture("idle.png"); 
			obj->SetFrameCount(6); 
			obj->SetTexture(idleTexture);

		}
		return;
	}
	float dx = 0.0f;
	float dy = 0.0f;
	float speed = 350.0f; 

	if (m_KeyPress & (1 << 0)) dx -= 1.0f; // LEFT
	if (m_KeyPress & (1 << 2)) dx += 1.0f; // RIGHT
	if (m_KeyPress & (1 << 1)) dy += 1.0f; // DOWN
	if (m_KeyPress & (1 << 3)) dy -= 1.0f; // UP

	if (dx != 0 && dy != 0)
	{
		float norm = 1.0f / sqrtf(2.0f);
		dx *= norm;
		dy *= norm;
	}

	if (dx != 0 || dy != 0)
	{

		Vector2 pos = obj->Get2DPosition();
		Vector2 newPos = pos;
		newPos.x += dx * speed * deltaTime;
		newPos.y += dy * speed * deltaTime;

		obj->Set2DPosition(newPos.x, newPos.y);

		if (CheckCollisionWithWalls()) 
		{
			obj->Set2DPosition(pos.x, pos.y);
		}
		else
		{
			pos = newPos;
			obj->Set2DPosition(pos.x, pos.y);
		}

		auto texture = ResourceManagers::GetInstance()->GetTexture("movement.png");
		obj->SetFrameCount(8);
		obj->SetTexture(texture);

		if (dx < 0)
		{
			obj->SetFlip(SDL_FLIP_HORIZONTAL); 
		}
		else if (dx > 0)
		{
			obj->SetFlip(SDL_FLIP_NONE); 
		}
	}
	else
	{
		auto texture = ResourceManagers::GetInstance()->GetTexture("idle.png");
		obj->SetFrameCount(6);
		obj->SetTexture(texture);
	}
	CheckBloodCollision();
}

void GSPlay::Update(float deltaTime)
{
	switch (m_KeyPress)//Handle Key event start
	{
	default:
		break;
	}
	if (!m_hasDied && m_Blood->GetHealth() <= 0)
	{
		collisionTime = SDL_GetTicks();
		auto texture = ResourceManagers::GetInstance()->GetTexture("death.png");
		obj->SetFrameCount(14);
		obj->SetTexture(texture);
		obj->PlayOnce();
		Sound::GetInstance()->LoadSfx("lose.mp3");
		Sound::GetInstance()->PlaySfx("lose.mp3");
		m_hasDied = true;
	}
	if (m_hasDied) 
	{
		Uint32 currentTime = SDL_GetTicks();
		if (currentTime - collisionTime >= 910)
		{
			GameData::GetInstance().isRetry = true;
			GameStateMachine::GetInstance()->ChangeState(StateType::STATE_GAMEOVER);
		}
	}
	if (isWin)
	{
		Uint32 currentTime = SDL_GetTicks();
		if (currentTime - collisionTime >= 2000)
		{
			GameData::GetInstance().isRetry = false;
			GameStateMachine::GetInstance()->ChangeState(StateType::STATE_GAMEOVER);
		}
	}

	Camera::GetInstance()->Update(deltaTime);
	// Key State event end
	for (auto it : m_listAnimation)
	{
		it->Update(deltaTime);
	}
	if (m_hasDied)
	{
		return;
	}
	MovePlayer(deltaTime);

	for (auto it : m_listButton)
	{
		it->Update(deltaTime);
	}
	for (auto it : m_bloodList)
	{
		it->Update(deltaTime);
	}
	for (auto it : m_Spike)
	{
		it->Update(deltaTime,obj,m_Blood);
	}
	for (auto it : m_suriken)
	{
		it->Update(deltaTime, obj, m_Blood);
	}
	for (auto it : m_monster)
	{
		it->Update(deltaTime, obj, m_Blood);
	}
	lastRoom->Update(deltaTime);
}
bool GSPlay::CheckCollisionWithWalls()
{
	int objX = obj->Get2DPosition().x;
	int objY = obj->Get2DPosition().y;
	int objWidth = obj->GetWidth();
	int objHeight = obj->GetHeight();
	int offsetX = objWidth * 0.3f;   
	int offsetY = objHeight * 0.2f;

	for (auto tile : m_listWalls)
	{
		SDL_Rect objCollider = {
			objX + offsetX,
			objY + 1.5f * offsetY,
			objWidth - offsetX * 2,
			objHeight - offsetY * 2
		};

		int tileX = tile->Get2DPosition().x;
		int tileY = tile->Get2DPosition().y;
		int tileWidth = tile->GetWidth();
		int tileHeight = tile->GetHeight();

		SDL_Rect wallCollider = { tileX,tileY,tileWidth,tileHeight };


		if (SDL_HasIntersection(&objCollider, &wallCollider)) {
			return true;
		}
	}

	return false;
}
void GSPlay::CheckBloodCollision()
{
	int objX = obj->Get2DPosition().x;
	int objY = obj->Get2DPosition().y;
	int objWidth = obj->GetWidth();
	int objHeight = obj->GetHeight();
	int offsetX = objWidth * 0.3f;
	int offsetY = objHeight * 0.2f;
	SDL_Rect playerRect = {
		objX + offsetX,
		objY + 1.5f * offsetY,
		objWidth - offsetX * 2,
		objHeight - offsetY * 2
	};

	for (auto it = m_bloodList.begin(); it != m_bloodList.end(); )
	{
		auto blood = *it;

		SDL_Rect bloodRect = {
			static_cast<int>(blood->Get2DPosition().x),
			static_cast<int>(blood->Get2DPosition().y),
			blood->GetWidth(),
			blood->GetHeight()
		};

		if (SDL_HasIntersection(&playerRect, &bloodRect))
		{
			Sound::GetInstance()->LoadSfx("collect.mp3");
			Sound::GetInstance()->PlaySfx("collect.mp3");
			m_Blood->Increase(); 
			it = m_bloodList.erase(it); 
			continue;
		}

		++it;
	}
	if (key) 
	{
		SDL_Rect keyRect = {
			static_cast<int>(key->Get2DPosition().x),
			static_cast<int>(key->Get2DPosition().y),
			key->GetWidth(),
			key->GetHeight()
		};

		if (SDL_HasIntersection(&playerRect, &keyRect))
		{
			Sound::GetInstance()->LoadSfx("key.mp3");
			Sound::GetInstance()->PlaySfx("key.mp3");
			hasKey = true;
			auto it = std::find(m_listAnimation.begin(), m_listAnimation.end(), key);
			if (it != m_listAnimation.end())
			{
				m_listAnimation.erase(it);
			}

			key.reset();
		}
	}
	SDL_Rect doorRect = {
			static_cast<int>(lastRoom->Get2DPosition().x),
			static_cast<int>(lastRoom->Get2DPosition().y),
			lastRoom->GetWidth(),
			lastRoom->GetHeight()
	};
	if (SDL_HasIntersection(&playerRect, &doorRect) && hasKey)
	{
		if (!isWin)
		{
			isWin = true;
			collisionTime = SDL_GetTicks(); 
			Sound::GetInstance()->LoadSfx("win.mp3");
			Sound::GetInstance()->PlaySfx("win.mp3");
			lastRoom->PlayOnce();
		}
	}

	

}


void GSPlay::LoadTileMap(const std::string& fileName, int tileSize)
{
	

	ObstacleManager obstacleManager;
	std::vector<RoomLayout> roomLayouts = obstacleManager.loadLayoutsFromFile("obstacle.txt");

	auto wallTexture = ResourceManagers::GetInstance()->GetTexture("wall.png");
	auto floorTexture = ResourceManagers::GetInstance()->GetTexture("floor.png");
	auto bloodTexture = ResourceManagers::GetInstance()->GetTexture("blood.png");
	auto keyTexture = ResourceManagers::GetInstance()->GetTexture("key.png");
	auto doorTexture = ResourceManagers::GetInstance()->GetTexture("door.png");
	auto monsterTexture = ResourceManagers::GetInstance()->GetTexture("monster.png");
	auto fireSpikeTexture = ResourceManagers::GetInstance()->GetTexture("Fire_Trap.png");
	auto trapSpikeTexture = ResourceManagers::GetInstance()->GetTexture("Spike_Trap.png");
	auto surikenTexture = ResourceManagers::GetInstance()->GetTexture("Suriken.png");


	if (!wallTexture)
	{
		printf("Failed to load wall.png texture!\n");
		return;
	}

	std::shared_ptr<Sprite2D> playerSprite = nullptr;

	std::ifstream file(fileName);
	if (!file.is_open())
	{
		std::cerr << "Cannot open map file: " << fileName << std::endl;
	}

	mapData.clear();
	std::string line;
	while (std::getline(file, line))
	{
		std::istringstream iss(line);
		std::vector<int> rowData;
		int tile;
		while (iss >> tile)
		{
			rowData.push_back(tile);
		}
		mapData.push_back(rowData);
	}

	file.close();

	int numRows = mapData.size();
	int numCols = numRows > 0 ? mapData[0].size() : 0;
	for (int row = 0; row < numRows; ++row)
	{
		for (int col = 0; col < mapData[row].size(); ++col)
		{
			int tile = mapData[row][col];
			if (tile == 1 || tile == 6)
			{
				auto tileSprite = std::make_shared<Sprite2D>(wallTexture, SDL_FLIP_NONE);
				tileSprite->SetSize(tileSize, tileSize);
				tileSprite->Set2DPosition(col * tileSize, row * tileSize);

				m_tileMap.push_back(tileSprite);
				m_listWalls.push_back(tileSprite);

				if (tile == 6)
				{
					auto torchTexture = ResourceManagers::GetInstance()->GetTexture("torch.png");
					auto torchAnim = std::make_shared<SpriteAnimation>(torchTexture, 1, 4, 1, 0.1f);
					torchAnim->SetSize(64, 64);
					torchAnim->Set2DPosition(col * tileSize + 16 , row * tileSize);
					m_listAnimation.push_back(torchAnim);
				}
			}
			if (tile == 2)
			{
				auto tileSprite = std::make_shared<Sprite2D>(floorTexture, SDL_FLIP_NONE);
				tileSprite->SetSize(tileSize, tileSize);
				tileSprite->Set2DPosition(col * tileSize, row * tileSize);
				
				m_tileMap.push_back(tileSprite);
			}
			else if (tile == 3)
			{
				auto tileSprite = std::make_shared<Sprite2D>(floorTexture, SDL_FLIP_NONE);
				tileSprite->SetSize(tileSize, tileSize);
				tileSprite->Set2DPosition(col * tileSize, row * tileSize);

				m_tileMap.push_back(tileSprite);

			
				auto texture = ResourceManagers::GetInstance()->GetTexture("idle.png");
				obj = std::make_shared<SpriteAnimation>(texture, 1, 6, 1, 0.068f);
				obj->SetFlip(SDL_FLIP_HORIZONTAL);
				obj->SetSize(200, 250);
				obj->Set2DPosition(col * tileSize, row * tileSize);

				m_listAnimation.push_back(obj);


			}
			else if (tile == 4)
			{
				auto tileSprite = std::make_shared<Sprite2D>(floorTexture, SDL_FLIP_NONE);
				tileSprite->SetSize(tileSize, tileSize);
				tileSprite->Set2DPosition(col* tileSize, row* tileSize);

				m_tileMap.push_back(tileSprite);
				lastRoom = std::make_shared<SpriteAnimation>(doorTexture, 1, 10, 1, 0.1f);
				lastRoom->SetSize(128, 128);
				lastRoom->Set2DPosition(col* tileSize, row* tileSize);
				lastRoom->Reset();     
				lastRoom->Stop();



			}
			else if (tile == 5 || tile == 7)
			{
				auto tileSprite = std::make_shared<Sprite2D>(floorTexture, SDL_FLIP_NONE);
				tileSprite->SetSize(tileSize, tileSize);
				tileSprite->Set2DPosition(col * tileSize, row * tileSize);

				m_tileMap.push_back(tileSprite);

				if (tile == 7)
				{
					key = std::make_shared<SpriteAnimation>(keyTexture, 1, 4, 1, 0.1f);
					key->SetSize(64, 64);
					key->Set2DPosition(col* tileSize, row* tileSize);
					m_listAnimation.push_back(key);
				}

				if (rand() % 4 != 0)
				{
					int randomIndex = std::rand() % roomLayouts.size();
					RoomLayout selectedLayout = roomLayouts[randomIndex];
					for (const Obstacle& obstacle : selectedLayout.obstacles) {
						int actualX = obstacle.x * tileSize + col * tileSize;
						int actualY = obstacle.y * tileSize + row * tileSize;
						int mapValue = 0;
						if (obstacle.y + row >= 0 && obstacle.y + row < numRows &&
							obstacle.x + col >= 0 && obstacle.x + col < numCols) {

							mapValue = mapData[obstacle.y + row][obstacle.x + col];
						}
						auto idleTexture = ResourceManagers::GetInstance()->GetTexture("monster_movement.png");
						auto monsterAnim = std::make_shared<SpriteAnimation>(idleTexture, 1, 10, 1, 0.1f);
						if (mapValue == 2 || mapValue == 5)
						{
							if (obstacle.type == "Monster1")
							{
								actualX = col * tileSize;
								actualY = row * tileSize;
								monsterAnim->SetSize(obstacle.w, obstacle.h);
								monsterAnim->Set2DPosition(actualX, actualY);

								Vector2 startPos(actualX, actualY - tileSize * 3);
								Vector2 endPos(actualX, actualY + tileSize * 2);

								auto monster = std::make_shared<Monster>(monsterAnim, startPos, endPos, 150.0f);
								m_monster.push_back(monster);
							}
							else if (obstacle.type == "Monster2")
							{
								actualX = col * tileSize;
								actualY = row * tileSize;
								monsterAnim->SetSize(obstacle.w, obstacle.h);
								monsterAnim->Set2DPosition(actualX, actualY);

								Vector2 startPos(actualX - tileSize * 4, actualY);
								Vector2 endPos(actualX + tileSize * 4, actualY);

								auto monster = std::make_shared<Monster>(monsterAnim, startPos, endPos, 150.0f);
								m_monster.push_back(monster);
							}
							else if (obstacle.type == "Blood")
							{
								auto bloodSprite = std::make_shared<SpriteAnimation>(bloodTexture, 1, 4, 1, 0.2f);
								bloodSprite->SetSize(obstacle.w, obstacle.h);
								bloodSprite->Set2DPosition(actualX, actualY);
								m_bloodList.push_back(bloodSprite);
							}
							else if (obstacle.type == "FireSpike")
							{
								auto fireSpikeSprite = std::make_shared<SpriteAnimation>(fireSpikeTexture, 1, 14, 1, 0.1f);
								fireSpikeSprite->SetSize(obstacle.w, obstacle.h);
								fireSpikeSprite->Set2DPosition(actualX, actualY);
								auto spike = std::make_shared<Spike>(fireSpikeSprite, Vector2(actualX, actualY));
								m_Spike.push_back(spike);

							}
							else if (obstacle.type == "Spike")
							{
								auto spikeSprite = std::make_shared<SpriteAnimation>(trapSpikeTexture, 1, 13, 1, 0.1f);
								spikeSprite->SetSize(obstacle.w, obstacle.h);
								spikeSprite->Set2DPosition(actualX, actualY);
								auto spike = std::make_shared<Spike>(spikeSprite, Vector2(actualX, actualY));
								m_Spike.push_back(spike);

							}
							else if (obstacle.type == "Suriken")
							{
								auto surikenSprite = std::make_shared<SpriteAnimation>(surikenTexture, 1, 8, 1, 0.05f);
								surikenSprite->SetSize(obstacle.w, obstacle.h);
								surikenSprite->Set2DPosition(actualX, actualY);
								auto suriken = std::make_shared<Suriken>(surikenSprite, Vector2(actualX, actualY));
								m_suriken.push_back(suriken);
							}
						}
						
					}
				}
			}
		}
	}
	m_tileMapWidth = numCols * tileSize;
	m_tileMapHeight = numRows * tileSize;
	Camera::GetInstance()->SetLevelDimension(m_tileMapWidth, m_tileMapHeight);

	// Set camera target to player when character is loaded
	if (obj)
	{
		Camera::GetInstance()->SetTarget(obj);
	}
}

void GSPlay::Draw(SDL_Renderer* renderer)
{
	Vector2 camPos = Camera::GetInstance()->GetPosition();

	for (auto tile : m_tileMap)
	{
		Vector3 pos3 = tile->Get2DPosition();
		Vector2 pos(pos3.x, pos3.y);

		tile->Set2DPosition(pos.x - camPos.x, pos.y - camPos.y);
		tile->Draw(renderer);
		tile->Set2DPosition(pos.x,pos.y); 
	}
	
	for (auto tile : m_bloodList)
	{
		Vector2 pos = tile->Get2DPosition();

		//tile->Set2DPosition(pos.x - camPos.x, pos.y - camPos.y);
		tile->Draw(renderer);
		tile->Set2DPosition(pos.x, pos.y);
	}
	for (auto spike : m_Spike)
	{
		auto tile = spike->GetAnimation();
		Vector2 pos = tile->Get2DPosition();
		//tile->Set2DPosition(pos.x - camPos.x, pos.y - camPos.y);
		tile->Draw(renderer);
		tile->Set2DPosition(pos.x, pos.y);
	}

	for (auto spike : m_suriken)
	{
		auto tile = spike->GetAnimation();
		Vector2 pos = tile->Get2DPosition();
		//tile->Set2DPosition(pos.x - camPos.x, pos.y - camPos.y);
		tile->Draw(renderer);
		tile->Set2DPosition(pos.x, pos.y);
	}
	Vector2 pos = lastRoom->Get2DPosition();
	lastRoom->Draw(renderer);
	lastRoom->Set2DPosition(pos.x, pos.y);

	for (auto monster : m_monster)
	{
		auto tile = monster->GetAnimation();
		Vector2 pos = tile->Get2DPosition();
		//tile->Set2DPosition(pos.x - camPos.x, pos.y - camPos.y);
		tile->Draw(renderer);
		tile->Set2DPosition(pos.x, pos.y);
	}
	for (auto anim : m_listAnimation)
	{
		Vector2 pos = anim->Get2DPosition();
		//anim->Set2DPosition(pos.x - camPos.x, pos.y - camPos.y);
		anim->Draw(renderer);
		anim->Set2DPosition(pos.x,pos.y); 
	}

	

	for (auto it : m_listButton)
	{
		it->Draw(renderer);
	}


	//Blood
	auto hearts = m_Blood->GetHeartsToDraw();
	for (auto heart : hearts)
	{
		heart->Draw(renderer);
	}
}