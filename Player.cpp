#include "Player.h"
#include "Texture.h"

#include <SFML/OpenGL.hpp>

Player::Player() : Sprite(20),
	m_texStand(Texture::Get("../res/PlayerStand.png")),
	m_texWalkUp(Texture::Get("../res/PlayerWalkUp.png")),
	m_texWalkDown(Texture::Get("../res/PlayerWalkDown.png"))
{
	SetState(State::Stand);
}

Player::~Player()
{
}

void Player::Update(float tDelta)
{
	__super::Update(tDelta);
}

void Player::SetState(State s)
{
	if (s == m_state && GetAnimation())
		return;

	m_state = s;
	switch (s)
	{
	case State::WalkUp: SetAnimation(m_texWalkUp, 2, 10); break;
	case State::WalkDown: SetAnimation(m_texWalkDown, 2, 10); break;
	case State::Stand: SetAnimation(m_texStand); break;
	}
}

