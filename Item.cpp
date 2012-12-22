#include "Item.h"
#include "Texture.h"

#include <SFML/OpenGL.hpp>

Item::Item() : Sprite(20),
	m_tex(Texture::Get("../res/item1.png"))
{
	SetAnimation(m_tex, 4, 10); 
}

Item::~Item()
{
}

void Item::Update(float tDelta)
{
	__super::Update(tDelta);
}


