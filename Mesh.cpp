#include "Mesh.h"
#include "Matrix.h"
#include "Vector.h"

#include <SFML/OpenGL.hpp>
#include <iostream>

void Mesh1::Draw(const Matrix& xf, const Vec2& vTexOffset) const
{
	const int nRows = 50, nSegs = 100; 
	const int nTexRepeat = 2; // How many cells per texture

	float lastT, lastX, lastY[nSegs + 1], lastZ[nSegs + 1];
	float rad = m_radius;	

	auto Point = [&] (float x, float y, float z, float s,  float t)
	{
		Vec3 v(x, y, z);
		xf.MultPointInverse(v);
		glColor3f(v.x < 0, v.y < 0, v.z < 0);

		glNormal3f(x, y, z);
		glTexCoord2f(s, t);
		glVertex3f(x * rad, y * rad, z * rad);
	};

	glMatrixMode(GL_TEXTURE);
	glPushMatrix();
	glLoadIdentity();
	glScalef(nSegs / (float)nTexRepeat, nRows / (float)nTexRepeat, 1);
//	glTranslatef(vTexOffset.x, vTexOffset.y * 2, 0);

	float rowDelta = nTexRepeat * 1.0f / nRows; 
	float segDelta = nTexRepeat * 1.0f / nSegs;

	float rowScroll = ::fmodf(vTexOffset.y, rowDelta) * 2;
	float segScroll = ::fmodf(vTexOffset.x, segDelta);

	bool bTraced = false;

	for( int i=0; i <= nRows; ++i )
	{
		float t = i / (float)nRows;
		float rowAngle = float(M_PI * (t + rowScroll));
		float rowRad = sin(rowAngle);
		float x = -cos(rowAngle);

		glBegin(GL_QUAD_STRIP);
		for( int j=0; j <= nSegs; ++j )
		{
			float s = j / (float)nSegs;
			float segAngle = float(2 * M_PI * (s - segScroll));
			float y = -cos(segAngle) * rowRad;
			float z = sin(segAngle) * rowRad;

			//if (!bTraced)
			//{
			//	Vec3 v(x, y, z);
			//	xf.MultPointInverse(v);
			//	std::cout << v.x << "," << v.y << "," << v.z << std::endl;
			//	bTraced = true;
			//}
			
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
	glColor3f(1, 1, 1);
}

