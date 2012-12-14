#pragma once

#include "Matrix.h"
#include "Vector.h"

#include "SFML/Graphics.hpp"

#include <memory>

class Sprite;
class Player;
class Mesh;
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

private:
	void DrawPlanet() const;
	void DrawSprites() const;
	void CreateMesh();

	sf::Texture m_texture;
	Matrix m_xform;
	float m_radius;
	bool m_bWireframe, m_bHitTest;

	enum class MeshType { Polar, Quad, _count };

	std::vector<std::unique_ptr<Sprite>> m_objs;
	std::unique_ptr<Player> m_pPlayer;
	std::unique_ptr<Mesh> m_pMesh;
	MeshType m_meshType;

	Vec2 m_vTexOffset;
};

