#pragma once

#include "Matrix.h"
#include "Vector.h"

#include "SFML/Graphics.hpp"

#include <memory>

class Sprite;
class Player;
class Mesh;
class Item;

class Planet
{
public:
	Planet();
	~Planet();

	void Update(float tDelta);

	void Draw(sf::RenderWindow& win) const;

	sf::Vector3f GetTopPointLocal() const;
	sf::Vector3f TransformPos(const sf::Vector3f& v) const;

	const Matrix& GetXForm() const { return m_xform; }

	void OnKeyPressed(sf::Keyboard::Key key);

	static Planet* Instance() { return s_pInstance; }

	int m_nSpriteRotateX;

private:
	void DrawPlanet() const;
	void DrawSprites() const;
	void CreateMesh();

	float GetAngle(float width) const;
	bool HitTest(const Sprite& s1, const Sprite& s2) const;
	bool HitTest(const Sprite& s, const Vec3& p2, float w2) const;
	bool HitTest(const Vec3& p1, float w1, const Vec3& p2, float w2) const;

	sf::Texture m_texture;
	Matrix m_xform;
	int m_nRotateY;
	float m_radius;
	bool m_bWireframe, m_bHitTest;

	enum class MeshType { Polar, _count };

	std::vector<std::unique_ptr<Sprite>> m_objs;
	std::vector<std::unique_ptr<Item>> m_items;
	std::unique_ptr<Player> m_pPlayer;
	std::unique_ptr<Mesh> m_pMesh;
	MeshType m_meshType;

	Vec2 m_vTexOffset;
	static Planet* s_pInstance;
};

