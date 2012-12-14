#pragma once

#include "SFML/Graphics.hpp"

#include <memory>

class Animation
{
public:
	Animation(const sf::Texture& texture, int nFrames = 1, float fps = 0);
	~Animation();

	void Update(float tDelta);

	void Draw() const;

	const sf::Texture& GetTexture() const { return m_texture; }

	sf::Vector2u GetSize() const;

private:
	const sf::Texture& m_texture;
	int m_nFrames;
	float m_fps, m_tPlaying;
};

