#include "Mesh.h"
#include "Matrix.h"
#include "Vector.h"
#include "SpaceBitch.h"

#include <SFML/OpenGL.hpp>
#include <iostream>

void Mesh1::Draw(const Matrix& xf, const Vec2& vTexOffset) const
{
	const int nRows = 100, nSegs = 200; 
	const int nTexRepeat = 4; // How many cells per texture

	float lastT, lastX, lastY[nSegs + 1], lastZ[nSegs + 1];
	float rad = m_radius;	

	auto Colour = [&] (float x, float y, float z, const Vec3& p, const Vec3& col1, Vec3& col2)
	{
		Vec3 v(x, y, z);
		xf.MultPointInverse(v);
		v.Normalise();
		float angle = acosf(v.Dot(p.Normalised()));

		float rad = 0.3f;
		float border = 0.05f;
		float val = (rad - angle) / border;
		float c1 = std::min(val, 1.0f) * 0.5;
		
		if (c1 > 0)
		{
			float c2 = 1 - c1;		
			col2.x = col2.x * c2 + col1.x * c1;
			col2.y = col2.y * c2 + col1.y * c1;
			col2.z = col2.z * c2 + col1.z * c1;

			//col2.x -= col1.x * c1;
			
			//			glColor3f(c2 + c1 * r, c2 + c1 * g, c2 + c1 * b);
		}
	};

	auto Point = [&] (float x, float y, float z, float s,  float t)
	{
		Vec3 col(1, 1, 1);
		
		Colour(x, y, z, Vec3(-1, 4, 0), Vec3(1, 0, 0), col);
		Colour(x, y, z, Vec3(1, 4, 0), Vec3(0, 1, 0), col);
		Colour(x, y, z, Vec3(0, 4, 1.5f), Vec3(0, 0, 1), col);
		glColor3f(col.x, col.y, col.z);

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

