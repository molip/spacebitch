#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>

class Projecter
{
public:
	Projecter()
	{
		glGetDoublev( GL_MODELVIEW_MATRIX, m_model ); 
		glGetDoublev( GL_PROJECTION_MATRIX, m_proj ); 
		glGetIntegerv( GL_VIEWPORT, m_view ); 
	}

	sf::Vector3f Project(const sf::Vector3f& v)
	{
		GLdouble wx, wy, wz;
		gluProject(v.x, v.y, v.z, m_model, m_proj, m_view, &wx, &wy, &wz);
		return sf::Vector3f((float)wx, float(m_view[3] - wy), (float)wz);
	}
private:
	GLdouble m_model[16], m_proj[16];
	GLint m_view[4];
};

float Magnitude(const sf::Vector3f& v);
sf::Vector3f Normalise(const sf::Vector3f& v);
float Dot(const sf::Vector3f& v1, const sf::Vector3f& v2);
float Dot(const sf::Vector2f& v1, const sf::Vector2f& v2);
sf::Vector3f Cross(const sf::Vector3f& v1, const sf::Vector3f& v2);