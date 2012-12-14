#pragma once

#include "Sprite.h"

class Player : public Sprite
{
public:
	enum class State { Stand, WalkUp, WalkDown };

	Player();
	~Player();

	virtual void Update(float tDelta) override;

	void SetState(State s);

private:
	State m_state;
	const sf::Texture& m_texStand;
	const sf::Texture& m_texWalkUp;
	const sf::Texture& m_texWalkDown;
};

