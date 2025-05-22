#include "GSHelp.h"
#include "ResourceManagers.h"
#include "MouseButton.h"
#include "Map/GameData.h"
GSHelp::GSHelp() : GameStateBase(StateType::STATE_MENU),
m_background(nullptr), m_listButton(std::list<std::shared_ptr<MouseButton>>{}), m_textGameName(nullptr)
{
}


GSHelp::~GSHelp()
{
}



void GSHelp::Init()
{
	//auto model = ResourceManagers::GetInstance()->GetModel("Sprite2D.nfg");
	auto texture = ResourceManagers::GetInstance()->GetTexture("help.png");

	// background
	//auto shader = ResourceManagers::GetInstance()->GetShader("TextureShader");
	m_background = std::make_shared<Sprite2D>(texture, SDL_FLIP_NONE);
	m_background->SetSize(SCREEN_WIDTH, SCREEN_HEIDHT);
	m_background->Set2DPosition(0, 0);


	// exit button
	texture = ResourceManagers::GetInstance()->GetTexture("btn_Close.png");
	std::shared_ptr<MouseButton> button = std::make_shared<MouseButton>(texture, SDL_FLIP_NONE);
	//btnClose = std::make_shared<MouseButton>(texture);
	button->SetSize(60, 60);
	button->Set2DPosition(SCREEN_WIDTH - button->GetWidth() - 10, 20);
	button->SetOnClick([this]() {
		GameStateMachine::GetInstance()->ChangeState(StateType::STATE_MENU);
		});
	m_listButton.push_back(button);


	// game title
	///Set Font

	m_textColor = { 255,0,0 };
	auto font = ResourceManagers::GetInstance()->GetFont("PressStart2P-Regular.ttf", 24);
	m_textGameName = std::make_shared<Text>("Help", font, m_textColor);
	m_textGameName->SetSize(180, 100);
	m_textGameName->Set2DPosition((SCREEN_WIDTH - m_textGameName->GetWidth()) / 2, SCREEN_HEIDHT / 2 - 300);

}

void GSHelp::Exit()
{

}


void GSHelp::Pause()
{

	// button close

}

void GSHelp::Resume()
{

}


void GSHelp::HandleEvents()
{
}

void GSHelp::HandleKeyEvents(SDL_Event& e)
{
}

void GSHelp::HandleTouchEvents(SDL_Event& e)
{
	for (auto button : m_listButton)
	{
		if (button->HandleTouchEvent(&e))
		{
			break;
		}
	}
}

void GSHelp::HandleMouseMoveEvents(int x, int y)
{
}

void GSHelp::Update(float deltaTime)
{
	m_background->Update(deltaTime);
	for (auto it : m_listButton)
	{
		it->Update(deltaTime);
	}
}

void GSHelp::Draw(SDL_Renderer* renderer)
{
	m_background->Draw(renderer);
	for (auto it : m_listButton)
	{
		it->Draw(renderer);
	}

	m_textGameName->Draw(renderer);
}
