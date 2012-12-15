#pragma once

#include <list>

class Matrix;
struct Vec2;

class Mesh 
{
public:
	Mesh(float radius) : m_radius(radius) {}
	virtual ~Mesh() {}

	virtual void Draw(const Matrix& xf, const Vec2& vTexOffset) const = 0;

protected:
	float m_radius;
};

class Mesh1 : public Mesh
{
public:
	Mesh1(float radius) : Mesh(radius) {}

	virtual void Draw(const Matrix& xf, const Vec2& vTexOffset) const override;
};

