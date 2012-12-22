#pragma once

#include "Animation.h"

#include "SFML/Graphics.hpp"

#include <memory>

class Sprite
{
public:
	Sprite(float width, float alpha = 1);
	~Sprite();

	virtual void Update(float tDelta);

	void Draw() const;

	void SetFootPos(const sf::Vector3f& v) { m_posDraw = v; } // pixel space

	void SetPos(const sf::Vector3f& v) { m_pos = v; }
	const sf::Vector3f& GetPos() const { return m_pos; }
	
	float GetWidth() const { return m_width; }
	float GetHitWidth() const { return m_widthHit; }
	void SetHitWidth(float w) { m_widthHit = w; }

	bool operator <(const Sprite& rhs) const;

	void SetAnimation(std::unique_ptr<Animation> pAnim) { m_pAnim = std::move(pAnim); }
	void SetAnimation(const sf::Texture& texture, int nFrames = 1, float fps = 0);
	void SetAnimation(const char* sPath, int nFrames = 1, float fps = 0);
	const Animation* GetAnimation() const { return m_pAnim.get(); }

private:
	std::unique_ptr<Animation> m_pAnim;
	sf::Vector3f m_pos; // normalised
	sf::Vector3f m_posDraw; 
	float m_width, m_widthHit, m_alpha;
};

