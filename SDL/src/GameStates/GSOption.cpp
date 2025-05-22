#include "GSOption.h"
#include "ResourceManagers.h"
#include "MouseButton.h"
#include "Map/GameData.h"
GSOption::GSOption() : GameStateBase(StateType::STATE_MENU),
m_background(nullptr), m_listButton(std::list<std::shared_ptr<MouseButton>>{}), m_textGameName(nullptr)
{
}


GSOption::~GSOption()
{
}



void GSOption::Init()
{
	m_isBgmOn = GameData::GetInstance().isBgmOn;
	m_isSfxOn = GameData::GetInstance().isSfxOn;
	//auto model = ResourceManagers::GetInstance()->GetModel("Sprite2D.nfg");
	auto texture = ResourceManagers::GetInstance()->GetTexture("background.png");

	// background
	//auto shader = ResourceManagers::GetInstance()->GetShader("TextureShader");
	m_background = std::make_shared<Sprite2D>(texture, SDL_FLIP_NONE);
	m_background->SetSize(SCREEN_WIDTH, SCREEN_HEIDHT);
	m_background->Set2DPosition(0, 0);

	texture = ResourceManagers::GetInstance()->GetTexture("btn_Close.png");
	std::shared_ptr<MouseButton> button = std::make_shared<MouseButton>(texture, SDL_FLIP_NONE);
	//btnClose = std::make_shared<MouseButton>(texture);
	button->SetSize(60, 60);
	button->Set2DPosition(SCREEN_WIDTH - button->GetWidth() - 10, 20);
	button->SetOnClick([this]() {
		GameStateMachine::GetInstance()->ChangeState(StateType::STATE_MENU);
		});
	m_listButton.push_back(button);
	// play button
	if (m_isBgmOn)
		texture = ResourceManagers::GetInstance()->GetTexture("btn_bgm_on.png");
	else
		texture = ResourceManagers::GetInstance()->GetTexture("btn_bgm_off.png");
	m_btnBgmToggle = std::make_shared<MouseButton>(texture, SDL_FLIP_NONE);
	m_btnBgmToggle->SetSize(120, 60);
	m_btnBgmToggle->Set2DPosition((SCREEN_WIDTH - m_btnBgmToggle->GetWidth()) / 2 + 100, SCREEN_HEIDHT /3);
	m_btnBgmToggle->SetOnClick([this]() {
		std::printf("BGM %s\n", m_isBgmOn ? "off" : "on");
		m_isBgmOn = !m_isBgmOn;
		GameData::GetInstance().isBgmOn = m_isBgmOn;
		if (m_isBgmOn)
		{
			Sound::GetInstance()->ResumeSound();
			m_btnBgmToggle->SetTexture(ResourceManagers::GetInstance()->GetTexture("btn_bgm_on.png"));
		}
		else
		{
			Sound::GetInstance()->PauseSound();
			m_btnBgmToggle->SetTexture(ResourceManagers::GetInstance()->GetTexture("btn_bgm_off.png"));
		}
		});
	m_listButton.push_back(m_btnBgmToggle);
	// SFX Toggle Button
	if (m_isSfxOn)
		texture = ResourceManagers::GetInstance()->GetTexture("btn_bgm_on.png");
	else
		texture = ResourceManagers::GetInstance()->GetTexture("btn_bgm_off.png");
	m_btnSfxToggle = std::make_shared<MouseButton>(texture, SDL_FLIP_NONE);
	m_btnSfxToggle->SetSize(120, 60);
	m_btnSfxToggle->Set2DPosition((SCREEN_WIDTH - m_btnSfxToggle->GetWidth()) / 2 + 100, SCREEN_HEIDHT / 3 * 2);
	m_btnSfxToggle->SetOnClick([this]() {
		std::printf("SFX %s\n", m_isSfxOn ? "off" : "on");
		m_isSfxOn = !m_isSfxOn;
		GameData::GetInstance().isSfxOn = m_isSfxOn;
		if (m_isSfxOn)
		{
			m_btnSfxToggle->SetTexture(ResourceManagers::GetInstance()->GetTexture("btn_bgm_on.png"));
			m_btnSfxToggle->SetSize(120, 60);
		}
		else
		{
			m_btnSfxToggle->SetTexture(ResourceManagers::GetInstance()->GetTexture("btn_bgm_off.png"));
			m_btnSfxToggle->SetSize(120, 60);
		}
		});
	m_listButton.push_back(m_btnSfxToggle);

	m_textColor = { 255,0,0 };
	auto font = ResourceManagers::GetInstance()->GetFont("PressStart2P-Regular.ttf", 24);
	m_textGameName = std::make_shared<Text>("Setting", font, m_textColor);
	m_textGameName->SetSize(300, 100);
	m_textGameName->Set2DPosition((SCREEN_WIDTH - m_textGameName->GetWidth()) / 2, SCREEN_HEIDHT / 2 - 300);

	auto fontTxt = ResourceManagers::GetInstance()->GetFont("PressStart2P-Regular.ttf", 16);
	m_txtBgmLabel = std::make_shared<Text>("Music", fontTxt, m_textColor);
	m_txtBgmLabel->SetSize(100, 40);
	m_txtBgmLabel->Set2DPosition((SCREEN_WIDTH - m_btnBgmToggle->GetWidth()) / 2 - 100, SCREEN_HEIDHT / 3 + 10);

	m_txtSfxLabel = std::make_shared<Text>("SFX", fontTxt, m_textColor);
	m_txtSfxLabel->SetSize(80, 40);
	m_txtSfxLabel->Set2DPosition((SCREEN_WIDTH - m_btnSfxToggle->GetWidth()) / 2 - 100, SCREEN_HEIDHT / 3 * 2 + 10);
}

void GSOption::Exit()
{

}


void GSOption::Pause()
{

	// button close

}

void GSOption::Resume()
{

}


void GSOption::HandleEvents()
{
}

void GSOption::HandleKeyEvents(SDL_Event& e)
{
}

void GSOption::HandleTouchEvents(SDL_Event& e)
{
	for (auto button : m_listButton)
	{
		if (button->HandleTouchEvent(&e))
		{
			break;
		}
	}
}

void GSOption::HandleMouseMoveEvents(int x, int y)
{
}

void GSOption::Update(float deltaTime)
{
	m_background->Update(deltaTime);
	for (auto it : m_listButton)
	{
		it->Update(deltaTime);
	}
}

void GSOption::Draw(SDL_Renderer* renderer)
{
	m_background->Draw(renderer);
	for (auto it : m_listButton)
	{
		it->Draw(renderer);
	}

	m_textGameName->Draw(renderer);
	m_txtBgmLabel->Draw(renderer);
	m_txtSfxLabel->Draw(renderer);

}
