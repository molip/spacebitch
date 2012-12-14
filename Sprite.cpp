#include "Sprite.h"
#include "Texture.h"

#include <SFML/OpenGL.hpp>

Sprite::Sprite(float width, float alpha) : m_width(width), m_alpha(alpha)
{
}

Sprite::~Sprite()
{
}

void Sprite::SetAnimation(const sf::Texture& texture, int nFrames, float fps)
{
	m_pAnim.reset(new Animation(texture, nFrames, fps));
}

void Sprite::SetAnimation(const char* sPath, int nFrames, float fps)
{
	SetAnimation(Texture::Get(sPath), nFrames, fps);
}

void Sprite::Update(float tDelta)
{
	if (m_pAnim)
		m_pAnim->Update(tDelta);
}

bool Sprite::operator <(const Sprite& rhs) const
{
	return m_posDraw.z < rhs.m_posDraw.z;
}

void Sprite::Draw() const
{
	if (!m_pAnim)
		return;
	
	glPushMatrix();

	auto sz = m_pAnim->GetSize();
	float w = m_width; 
	float h = m_width * sz.y / (float)sz.x;

	glTranslatef(m_posDraw.x, m_posDraw.y, m_posDraw.z);
	glScalef(m_width, m_width * sz.y / (float)sz.x, 1);

	glColor4f(1, 1, 1, m_alpha);

	m_pAnim->Draw();

	glColor4f(1, 1, 1, 1);
	glPopMatrix();
}
