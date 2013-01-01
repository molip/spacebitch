#include "Planet.h"
#include "Player.h"
#include "Sprite.h"
#include "Texture.h"
#include "Mesh.h"
#include "Item.h"

#include <SFML/OpenGL.hpp>

#include <iostream>
#include <set>

namespace 
{
	Vec3 GetRandomPos()
	{
		Vec3 v((float)rand() - RAND_MAX / 2, (float)rand() - RAND_MAX / 2, (float)rand() - RAND_MAX / 2);
		v.Normalise();
		return v;
	}

	float GetAngleBetween(const Vec3& v1, const Vec3& v2) 
	{
		return acosf(v1.Dot(v2));
	}
}

Planet* Planet::s_pInstance;

Planet::Planet() : m_radius(500), m_pPlayer(new Player), m_meshType(MeshType::Polar), m_bWireframe(false), m_bHitTest(true),
	m_nSpriteRotateX(-1), m_nRotateY(0)
{
    s_pInstance = this;
	
	if (!m_texture.loadFromFile("../res/craters.png"))
		throw;
	m_texture.setRepeated(true);
	m_texture.setSmooth(true);

	m_xform.MakeIdentity();

	auto& tex = Texture::Get("../res/tree1.png");
	for (int i = 0; i < 500; ++i)
	{
		float width = float(rand() % 30) + 10;
		m_objs.push_back(std::unique_ptr<Sprite>(new Sprite(width, 1)));
		m_objs.back()->SetHitWidth(width / 2);
		m_objs.back()->SetPos(GetRandomPos());
		m_objs.back()->SetAnimation(tex);
	}

	m_objs.push_back(std::unique_ptr<Sprite>(new Sprite(60, 1)));
	m_objs.back()->SetPos(Vec3(0, 1, -0.2f).Normalised());
	m_objs.back()->SetAnimation("../res/cookery.png");

	for (int i = 0; i < 100; ++i)
	{
		m_items.push_back(std::unique_ptr<Item>(new Item));
		m_items.back()->SetPos(GetRandomPos());
	}

	//m_items.push_back(std::unique_ptr<Item>(new Item));
	//m_items.back()->SetPos(Vec3(0, 1, 0));

	CreateMesh();
}

Planet::~Planet()
{
	s_pInstance = nullptr;
}

// v: normalised
sf::Vector3f Planet::TransformPos(const sf::Vector3f& v) const 
{
	sf::Vector3f v2;
	m_xform.MultPoint(v2, v * m_radius);
	return v2;
}

sf::Vector3f Planet::GetTopPointLocal() const
{
	return sf::Vector3f(0, m_radius,  0);
}

float Planet::GetAngle(float width) const
{
	return width / m_radius;
}

bool Planet::HitTest(const Vec3& p1, float w1, const Vec3& p2, float w2) const
{
	float angle = GetAngleBetween(p1, p2);
	float angleMin = GetAngle((w1 + w2) / 2);
	return angle < angleMin;
} 

bool Planet::HitTest(const Sprite& s, const Vec3& p2, float w2) const
{
	return HitTest(s.GetPos(), s.GetHitWidth(), p2, w2);
}

bool Planet::HitTest(const Sprite& s1, const Sprite& s2) const
{
	return HitTest(s1.GetPos(), s1.GetHitWidth(), s2.GetPos(), s2.GetHitWidth());
}

void Planet::Update(float tDelta)
{
	m_pPlayer->Update(tDelta);

	for (auto& p : m_items)
		p->Update(tDelta);

	int x = 0, y = 0;

	x += sf::Keyboard::isKeyPressed(sf::Keyboard::Left);
	x -= sf::Keyboard::isKeyPressed(sf::Keyboard::Right);
    y += sf::Keyboard::isKeyPressed(sf::Keyboard::Up);
    y -= sf::Keyboard::isKeyPressed(sf::Keyboard::Down);

	m_pPlayer->SetState(y > 0 ? Player::State::WalkUp : x || y ? Player::State::WalkDown : Player::State::Stand);

	float deg = 12 * tDelta;
	
	if (x && y)
		deg /= (float)::sqrt(2);

	float rad = deg * (float)M_PI / 180;

	auto doMove = [&] (const Matrix& xf) -> bool
	{
		Matrix xfNew = m_xform;
		xfNew.MultRight(xf);

		Vec3 posPlayer;
		xfNew.MultPointInverse(posPlayer, Vec3(0, 1, 0));

		if (m_bHitTest)
			for (auto& p : m_objs)
				if (HitTest(*p, posPlayer, m_pPlayer->GetHitWidth()))
					return false;

		m_xform = xfNew;
		return true;
	};

	if (x)
	{
		Matrix xf;
		xf.MakeRotationZ(-x * rad);

		if (!doMove(xf))
			x = 0;
	}
	if (y)
	{
		Matrix xf;
		xf.MakeRotationX(y * rad);

		if (!doMove(xf))
			y = 0;
	}

	Vec3 posPlayer;
	m_xform.MultPointInverse(posPlayer, Vec3(0, 1, 0));
	m_pPlayer->SetPos(posPlayer);

	for (auto it = m_items.begin(); it != m_items.end(); )
		if (HitTest(**it, *m_pPlayer))
			it = m_items.erase(it);
		else
			++it;

	m_vTexOffset.x += y * deg / 360;
	m_vTexOffset.y += x * deg / 360;
}

void Planet::Draw(sf::RenderWindow& win) const
{
	glRotatef(m_nRotateY * 90, 0, 1, 0);
	
	DrawPlanet();
	DrawSprites();
}

void Planet::OnKeyPressed(sf::Keyboard::Key key)
{
	if (key == sf::Keyboard::Tab)
	{
		m_meshType = MeshType((int(m_meshType) + 1) % (int)MeshType::_count);
		CreateMesh();
	}
	else if (key == sf::Keyboard::W)
		m_bWireframe = !m_bWireframe;
	else if (key == sf::Keyboard::H)
		m_bHitTest = !m_bHitTest;
	else if (key == sf::Keyboard::R)
	{
		if (++m_nSpriteRotateX == 1)
			m_nSpriteRotateX = -1;
		std::cout << "Planet::m_nSpriteRotateX: " << m_nSpriteRotateX << std::endl;
	}
	else if (key == sf::Keyboard::Q)
		++m_nRotateY;
	else if (key == sf::Keyboard::E)
		--m_nRotateY;
}

void Planet::CreateMesh()
{
	switch (m_meshType)
	{
	case MeshType::Polar: m_pMesh.reset(new Mesh1(m_radius)); break;
//	case MeshType::Quad: m_pMesh.reset(new Mesh2(m_radius)); break;
	}
}

void Planet::DrawPlanet() const
{
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_CULL_FACE);
	m_texture.bind();

	if (m_bWireframe)
		glPolygonMode(GL_FRONT, GL_LINE);
	
	m_pMesh->Draw(m_xform, m_vTexOffset);
	
	glPolygonMode(GL_FRONT, GL_FILL);

}

void Planet::DrawSprites() const
{
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();

	MatrixGL xfView;
	xfView.GetModelView();
		
	glLoadIdentity();
		
	struct Comp 
	{
		bool operator() (const Sprite* pL, const Sprite* pR) const { return *pL < *pR; };
	};
	std::set<const Sprite*, Comp> set;

	sf::Vector3f vPlayer = GetTopPointLocal();
	m_pPlayer->SetFootPos(xfView.MultPoint(vPlayer));
	set.insert(m_pPlayer.get());

	for (auto& p : m_objs)
	{
		sf::Vector3f v = xfView.MultPoint(TransformPos(p->GetPos()));
		p->SetFootPos(v);
		set.insert(p.get());
	}

	for (auto& p : m_items)
	{
		sf::Vector3f v = xfView.MultPoint(TransformPos(p->GetPos()));
		p->SetFootPos(v);
		set.insert(p.get());
	}

	for (auto p : set)
		p->Draw(m_xform);

	glPopMatrix();
}