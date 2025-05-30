#include "Sprite2D.h"
#include "TextureManager.h"
//#include "Camera.h"
//#include "Renderer.h"

Sprite2D::Sprite2D(std::shared_ptr<TextureManager> texture, SDL_RendererFlip flip)
	:BaseObject(texture), m_iWidth(100), m_iHeight(50)
{
	m_flip = flip;
	Init();
	
}

void Sprite2D::Init()
{
	// Init Camera
}

void Sprite2D::Draw(SDL_Renderer * renderer, SDL_Rect* clip)
{
	// Draw texture.
	if (m_pTexture != nullptr)
	{
		SDL_Rect renderQuad = { m_position.x ,m_position.y, m_iWidth, m_iHeight };
		if (clip != NULL)
		{
			renderQuad.w = clip->w;
			renderQuad.h = clip->h;
		}
		SDL_RenderCopyEx(Renderer::GetInstance()->GetRenderer(), m_pTexture->GetTextureObj(), clip, &renderQuad, m_angle, nullptr, m_flip);
	}
	
}

void Sprite2D::Update(float deltatime)
{
}

void Sprite2D::Set2DPosition(float x, float y)
{
	m_position = Vector3((float)x, (float)y, 0.0f);
}

Vector3 Sprite2D::Get2DPosition()
{
	return Vector3(m_position.x, m_position.y, 0.0f);
}

void Sprite2D::SetSize(int width, int height)
{
	m_iWidth = width;
	m_iHeight = height;
	m_scale = Vector3((float)m_iWidth, (float)m_iHeight, 0.0f);
}

int Sprite2D::GetWidth()
{
	return m_iWidth;
}

int Sprite2D::GetHeight()
{
	return m_iHeight;
}

void Sprite2D::SetRotation(double angle)
{
	m_angle = angle;
}

void Sprite2D::SetFlip(SDL_RendererFlip flip)
{
	m_flip = flip;
}

