#pragma once

#include "Sprite.h"

class Item : public Sprite
{
public:
	Item();
	~Item();

	virtual void Update(float tDelta) override;

private:
	const sf::Texture& m_tex;
};

