#include "GSMenu.h"
#include "ResourceManagers.h"
#include "MouseButton.h"
#include "Map/GameData.h"
GSMenu::GSMenu() : GameStateBase(StateType::STATE_MENU),
m_background(nullptr), m_listButton(std::list<std::shared_ptr<MouseButton>>{}), m_textGameName(nullptr)
{
}


GSMenu::~GSMenu()
{
}



void GSMenu::Init()
{
	//auto model = ResourceManagers::GetInstance()->GetModel("Sprite2D.nfg");
	auto texture = ResourceManagers::GetInstance()->GetTexture("background.png");

	// background
	//auto shader = ResourceManagers::GetInstance()->GetShader("TextureShader");
	m_background = std::make_shared<Sprite2D>(texture, SDL_FLIP_NONE);
	m_background->SetSize(SCREEN_WIDTH, SCREEN_HEIDHT);
	m_background->Set2DPosition(0, 0);

	// play button
	texture = ResourceManagers::GetInstance()->GetTexture("btn_Play.png");
	std::shared_ptr<MouseButton> btnPlay = std::make_shared<MouseButton>(texture, SDL_FLIP_NONE);
	
	btnPlay->SetSize(262, 135);
	btnPlay->Set2DPosition((SCREEN_WIDTH - btnPlay->GetWidth())/2, (SCREEN_HEIDHT - btnPlay->GetHeight()) / 3 * 2);
	btnPlay->SetOnClick([]() {
		GameData::GetInstance().isRetry = false;
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

	//Setting game
	texture = ResourceManagers::GetInstance()->GetTexture("btn_Setting.png");
	std::shared_ptr<MouseButton> btnOption = std::make_shared<MouseButton>(texture, SDL_FLIP_NONE);
	btnOption->SetSize(195, 100);
	btnOption->Set2DPosition((SCREEN_WIDTH - btnOption->GetWidth()) / 2 - btnOption->GetWidth() - 50, (SCREEN_HEIDHT / 4) *3 );
	btnOption->SetOnClick([]() {
		GameStateMachine::GetInstance()->ChangeState(StateType::STATE_OPTION);
		});
	m_listButton.push_back(btnOption);

	//CREDIT game
	texture = ResourceManagers::GetInstance()->GetTexture("btn_Help.png");
	btnCredit = std::make_shared<MouseButton>(texture, SDL_FLIP_NONE);
	btnCredit->Set2DPosition((SCREEN_WIDTH - btnCredit->GetWidth()) / 2 + 200, SCREEN_HEIDHT / 4 * 3);
	btnCredit->SetSize(195, 100);
	btnCredit->SetOnClick([]() {
		GameStateMachine::GetInstance()->ChangeState(StateType::STATE_CREDIT);
		});
	m_listButton.push_back(btnCredit);

	// game title
	///Set Font

	m_textColor = { 255,0,0 };
	auto font = ResourceManagers::GetInstance()->GetFont("PressStart2P-Regular.ttf", 24);
	m_textGameName = std::make_shared<Text>("Echo of the Nameless",font,  m_textColor);
	m_textGameName->SetSize(700, 120);
	m_textGameName->Set2DPosition((SCREEN_WIDTH - m_textGameName->GetWidth())/2, SCREEN_HEIDHT / 2 - 300);
	
	/// sound
	

	
}

void GSMenu::Exit()
{
	
}


void GSMenu::Pause()
{
	
	// button close
	
}

void GSMenu::Resume()
{

}


void GSMenu::HandleEvents()
{
}

void GSMenu::HandleKeyEvents(SDL_Event& e)
{
}

void GSMenu::HandleTouchEvents(SDL_Event& e)
{
	for (auto button : m_listButton)
	{
		if (button ->HandleTouchEvent(&e))
		{
			break;
		}
	}
}

void GSMenu::HandleMouseMoveEvents(int x, int y)
{
}

void GSMenu::Update(float deltaTime)
{
	m_background->Update(deltaTime);
	for (auto it : m_listButton)
	{
		it->Update(deltaTime);
	}
}

void GSMenu::Draw(SDL_Renderer* renderer)
{
	m_background->Draw(renderer);
	for (auto it : m_listButton)
	{
		it->Draw(renderer);
	}
	
	m_textGameName->Draw(renderer);
}
