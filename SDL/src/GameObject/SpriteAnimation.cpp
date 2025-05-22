#include "SpriteAnimation.h"
#include "TextureManager.h"
#include "Camera.h"

SpriteAnimation::SpriteAnimation(std::shared_ptr<TextureManager> texture, int currentAction, int frameCount, int numAction, float frameTime)
	: BaseObject(texture)
{
	m_currentAction = currentAction;
	m_frameCount = frameCount;
	m_numAction = numAction;
	m_frameTime = frameTime;
	m_currentFrame = 0;
	m_currentTicks = 0;
	m_lastUpdate = SDL_GetTicks();
	m_isPlaying = true;
	m_playOnce = false;
	Init();
}

SpriteAnimation::~SpriteAnimation() {}

void SpriteAnimation::Init() {}

void SpriteAnimation::Draw(SDL_Renderer* renderer, SDL_Rect* clip)
{
	if (m_pTexture != nullptr)
	{
		float srcWidth = m_pTexture->GetOriginalWidth() / m_frameCount;
		float srcHeight = m_pTexture->GetOriginalHeight() / m_numAction;

		SDL_Rect srcRect = {
			static_cast<int>(srcWidth * m_currentFrame),
			static_cast<int>(srcHeight * (m_currentAction - 1)),
			static_cast<int>(srcWidth),
			static_cast<int>(srcHeight)
		};

		SDL_Rect dstRect = {
			static_cast<int>(m_position.x - Camera::GetInstance()->GetPosition().x),
			static_cast<int>(m_position.y - Camera::GetInstance()->GetPosition().y),
			m_iWidth,
			m_iHeight
		};

		SDL_RenderCopyEx(Renderer::GetInstance()->GetRenderer(), m_pTexture->GetTextureObj(), &srcRect, &dstRect, m_angle, nullptr, m_flip);
	}
}

void SpriteAnimation::Update(float deltatime)
{
	if (!m_isPlaying || m_frameCount <= 1) return;

	m_currentTicks += deltatime;

	if (m_isRed)
	{
		m_redDuration -= deltatime;
		if (m_redDuration <= 0.0f)
		{
			m_isRed = false;
			m_redDuration = 0.0f;
		}
	}

	if (m_currentTicks >= m_frameTime) {
		m_currentFrame++;
		if (m_currentFrame >= m_frameCount) {
			if (m_playOnce) {
				m_currentFrame = m_frameCount - 1; 
				m_isPlaying = false;
			}
			else {
				m_currentFrame = 0;
			}
		}
		m_currentTicks -= m_frameTime;
	}
}

void SpriteAnimation::Set2DPosition(float x, float y)
{
	m_position = Vector3(x, y, 0.0f);
}

void SpriteAnimation::SetRotation(double angle)
{
	m_angle = angle;
}

void SpriteAnimation::SetFlip(SDL_RendererFlip flip)
{
	m_flip = flip;
}

Vector2 SpriteAnimation::Get2DPosition()
{
	return Vector2(m_position.x, m_position.y);
}

void SpriteAnimation::SetSize(int width, int height)
{
	m_iWidth = width;
	m_iHeight = height;
	m_scale = Vector3((float)m_iWidth, (float)m_iHeight, 0.0f);
}

int SpriteAnimation::GetWidth()
{
	return m_iWidth;
}

int SpriteAnimation::GetHeight()
{
	return m_iHeight;
}

int SpriteAnimation::GetCurrentAction()
{
	return m_currentAction;
}

int SpriteAnimation::GetCurrentFrame()
{
	return m_currentFrame;
}

void SpriteAnimation::SetCurrentAction(int action)
{
	m_currentAction = action;
}

void SpriteAnimation::SetFrameCount(int frame)
{
	m_frameCount = frame;
}

int SpriteAnimation::GetFrameCount()
{
	return m_frameCount;
}

void SpriteAnimation::SetFrameTime(int frame)
{
	m_frameTime = frame;
}

// 🆕 Các hàm mới thêm vào:
void SpriteAnimation::PlayOnce()
{
	m_isPlaying = true;
	m_playOnce = true;
	m_currentFrame = 0;
	m_currentTicks = 0;
}

void SpriteAnimation::Stop()
{
	m_isPlaying = false;
}

void SpriteAnimation::Reset()
{
	m_currentFrame = 0;
	m_currentTicks = 0;
}

bool SpriteAnimation::IsPlaying()
{
	return m_isPlaying;
}
void SpriteAnimation::FlashRed(float duration)
{
	m_isRed = true;
	m_redDuration = duration;
}