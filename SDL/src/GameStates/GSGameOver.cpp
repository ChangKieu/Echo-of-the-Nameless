#include "GSGameOver.h"
#include "ResourceManagers.h"
#include "MouseButton.h"
#include "Map/GameData.h"
GSGameOver::GSGameOver() : GameStateBase(StateType::STATE_MENU),
m_background(nullptr), m_listButton(std::list<std::shared_ptr<MouseButton>>{}), m_textGameName(nullptr)
{
}


GSGameOver::~GSGameOver()
{
}



void GSGameOver::Init()
{
	//auto model = ResourceManagers::GetInstance()->GetModel("Sprite2D.nfg");
	auto texture = ResourceManagers::GetInstance()->GetTexture("background.png");

	// background
	//auto shader = ResourceManagers::GetInstance()->GetShader("TextureShader");
	m_background = std::make_shared<Sprite2D>(texture, SDL_FLIP_NONE);
	m_background->SetSize(SCREEN_WIDTH, SCREEN_HEIDHT);
	m_background->Set2DPosition(0, 0);

	// play button
	if (GameData::GetInstance().isRetry)
		texture = ResourceManagers::GetInstance()->GetTexture("btn_Again.png");
	else
		texture = ResourceManagers::GetInstance()->GetTexture("btn_Next.png");
	std::shared_ptr<MouseButton> btnPlay = std::make_shared<MouseButton>(texture, SDL_FLIP_NONE);

	btnPlay->SetSize(262, 135);
	btnPlay->Set2DPosition((SCREEN_WIDTH - btnPlay->GetWidth()) / 2, (SCREEN_HEIDHT - btnPlay->GetHeight()) / 2 );
	btnPlay->SetOnClick([]() {
		GameStateMachine::GetInstance()->ChangeState(StateType::STATE_PLAY);
		});
	m_listButton.push_back(btnPlay);

	// exit button
	texture = ResourceManagers::GetInstance()->GetTexture("btn_Close.png");
	std::shared_ptr<MouseButton> btnClose = std::make_shared<MouseButton>(texture, SDL_FLIP_NONE);
	//btnClose = std::make_shared<MouseButton>(texture);
	btnClose->SetSize(60, 60);
	btnClose->Set2DPosition(SCREEN_WIDTH - btnClose->GetWidth() - 10, 20);
	btnClose->SetOnClick([]() {
		exit(0);
		});
	m_listButton.push_back(btnClose);


	//CREDIT game
	texture = ResourceManagers::GetInstance()->GetTexture("btn_Menu.png");
	std::shared_ptr<MouseButton> btnMenu = std::make_shared<MouseButton>(texture, SDL_FLIP_NONE);

	btnMenu->SetSize(262, 135);
	btnMenu->Set2DPosition((SCREEN_WIDTH - btnPlay->GetWidth()) / 2, (SCREEN_HEIDHT - btnPlay->GetHeight()) / 3 * 2 + 50);
	btnMenu->SetOnClick([]() {
		GameStateMachine::GetInstance()->ChangeState(StateType::STATE_MENU);
		});
	m_listButton.push_back(btnMenu);

	// game title
	///Set Font

	m_textColor = { 255,0,0 };
	auto font = ResourceManagers::GetInstance()->GetFont("PressStart2P-Regular.ttf", 24);
	if (GameData::GetInstance().isRetry)
		m_textGameName = std::make_shared<Text>("You lose!", font, m_textColor);
	else
	{
		m_textGameName = std::make_shared<Text>("You win!", font, m_textColor);
	}
	m_textGameName->SetSize(350, 150);
	m_textGameName->Set2DPosition((SCREEN_WIDTH - m_textGameName->GetWidth()) / 2, SCREEN_HEIDHT / 2 - 300);


}

void GSGameOver::Exit()
{

}


void GSGameOver::Pause()
{

	// button close

}

void GSGameOver::Resume()
{

}


void GSGameOver::HandleEvents()
{
}

void GSGameOver::HandleKeyEvents(SDL_Event& e)
{
}

void GSGameOver::HandleTouchEvents(SDL_Event& e)
{
	for (auto button : m_listButton)
	{
		if (button->HandleTouchEvent(&e))
		{
			break;
		}
	}
}

void GSGameOver::HandleMouseMoveEvents(int x, int y)
{
}

void GSGameOver::Update(float deltaTime)
{
	m_background->Update(deltaTime);
	for (auto it : m_listButton)
	{
		it->Update(deltaTime);
	}
}

void GSGameOver::Draw(SDL_Renderer* renderer)
{
	m_background->Draw(renderer);
	for (auto it : m_listButton)
	{
		it->Draw(renderer);
	}

	m_textGameName->Draw(renderer);
}
