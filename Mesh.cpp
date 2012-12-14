#include "Mesh.h"
#include "Matrix.h"
#include "Vector.h"

#include <SFML/OpenGL.hpp>

void Mesh1::Draw(const Matrix& xf, const Vec2& vTexOffset) const
{
	const int nRows = 60, nCols = 100; 

	float lastT, lastX, lastY[nCols + 1], lastZ[nCols + 1];
	float rad = m_radius;	

	auto Point = [&] (float x, float y, float z, float s,  float t)
	{
		glNormal3f(x, y, z);
		glTexCoord2f(s, t);
		glVertex3f(x * rad, y * rad, z * rad);
	};

	glMatrixMode(GL_TEXTURE);
	glPushMatrix();
	glLoadIdentity();
	glScalef(50, 25, 1);
	glTranslatef(vTexOffset.x, -vTexOffset.y * 2, 0);

	for( int i=0; i <= nRows; ++i )
	{
		float t = i / (float)nRows;
		float rowAngle = float(M_PI * t);
		float rowRad = sin(rowAngle);
		float x = -cos(rowAngle);

		glBegin(GL_QUAD_STRIP);
		for( int j=0; j <= nCols; ++j )
		{
			float s = j / (float)nCols;
			float segAngle = float(2 * M_PI * s);
			float y = -cos(segAngle) * rowRad;
			float z = sin(segAngle) * rowRad;

			if (i)
			{
				Point(lastX, lastY[j], lastZ[j], s, lastT);
				Point(x, y, z, s, t);
			}
			lastY[j] = y, lastZ[j] = z; 
		}
		lastX = x, lastT = t;
		glEnd();
	}

	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
}

Mesh2::Mesh2(float radius) : Mesh(radius) 
{
	std::vector<Vec3> verts;
	for (int x = 0; x < 2; ++x)
		for (int y = 0; y < 2; ++y)
			for (int z = 0; z < 2; ++z)
				verts.push_back(Vec3(radius * (x * 2 - 1), radius * (y * 2 - 1), radius * (z * 2 - 1)));

	//auto init = [&] (Side& s, int a, int b, int c, int d)
	//{
	//	
	//};
}

void Mesh2::Draw(const Matrix& xf, const Vec2& vTexOffset) const
{
	
}


