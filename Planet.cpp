#include "Planet.h"
#include "Player.h"
#include "Sprite.h"
#include "Texture.h"
#include "Mesh.h"

#include <SFML/OpenGL.hpp>

#include <iostream>
#include <set>

Planet::Planet() : m_radius(500), m_pPlayer(new Player), m_meshType(MeshType::Polar), m_bWireframe(false), m_bHitTest(true)
{
    if (!m_texture.loadFromFile("../res/craters.png"))
		throw;
	m_texture.setRepeated(true);
	m_texture.setSmooth(true);

	m_xform.MakeIdentity();

	auto& tex = Texture::Get("../res/tree1.png");
	for (int i = 0; i < 500; ++i)
	{
		m_objs.push_back(std::unique_ptr<Sprite>(new Sprite(float(rand() % 30) + 10, 1)));
		
		Vec3 v((float)rand() - RAND_MAX / 2, (float)rand() - RAND_MAX / 2, (float)rand() - RAND_MAX / 2);
		m_objs.back()->SetPos(v.Normalised());
		m_objs.back()->SetAnimation(tex);
	}

	CreateMesh();
}

Planet::~Planet()
{
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

void Planet::Update(float tDelta)
{
	m_pPlayer->Update(tDelta);

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
		{
			float hpw = m_pPlayer->GetWidth() / 2;
			for (auto& p : m_objs)
			{
				float angle = acosf(posPlayer.Dot(p->GetPos()));
				float angleMin = (hpw + p->GetWidth() / 2) / m_radius;
				if (angle < angleMin)
					return false;
			}
		}
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

	m_vTexOffset.x += y * deg / 360;
	m_vTexOffset.y += x * deg / 360;
}

void Planet::Draw(sf::RenderWindow& win) const
{
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
}

void Planet::CreateMesh()
{
	switch (m_meshType)
	{
	case MeshType::Polar: m_pMesh.reset(new Mesh1(m_radius)); break;
	case MeshType::Quad: m_pMesh.reset(new Mesh2(m_radius)); break;
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

	MatrixGL xf;
	xf.GetModelView();
		
	glLoadIdentity();
		
	struct Comp 
	{
		bool operator() (const Sprite* pL, const Sprite* pR) const { return *pL < *pR; };
	};
	std::set<const Sprite*, Comp> set;

	sf::Vector3f vPlayer = GetTopPointLocal();
	m_pPlayer->SetFootPos(xf.MultPoint(vPlayer));
	set.insert(m_pPlayer.get());

	for (auto& p : m_objs)
	{
		sf::Vector3f v = xf.MultPoint(TransformPos(p->GetPos()));
		p->SetFootPos(v);
		set.insert(p.get());
	}

	for (auto p : set)
		p->Draw();

	glPopMatrix();
}