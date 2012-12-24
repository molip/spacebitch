#pragma once

#include "SFML/Graphics.hpp"

class Matrix
{
public:
	void MakeIdentity();
	void MakeRotationX( float angle );
	void MakeRotationY( float angle );
	void MakeRotationZ( float angle );
	void MakeRotationYXZ( float ry, float rx, float rz );
	void MakeScale( float sx, float sy, float sz );
	//void MakeZAxisToVector( const Vector& vec );
	void MakeInverse( const Matrix& src );
//	void MakeRotationVector(float angle, const Vector& axis);

	void MultRight( const Matrix& a );
	void MultLeft( const Matrix& a );

	void MultPoint( sf::Vector3f& dst ) const;
	void MultPoint( sf::Vector3f& dst, const sf::Vector3f& src ) const;
	void MultPointInverse( sf::Vector3f& dst ) const;
	void MultPointInverse( sf::Vector3f& dst, const sf::Vector3f& src ) const;
	
	float* GetData() { return mat; }
	const float* GetData() const { return mat; }
	
//	void GetZAxis( Vector& zVec ) const; 

	void Print() const;	
	void Get4x4( float m[16] ) const;
	void glMultMatrix() const;
	
private:
	static void Mult( Matrix* pDst, const Matrix& a, const Matrix& b );
	float mat[9];

	float Clip( float a );
}; 

class MatrixGL
{
public:
	void GetModelView();
	void Load() const;
	void Mult() const;
	void RemoveTranslation();
	void RemoveRotation();
	sf::Vector3f MatrixGL::MultPoint(const sf::Vector3f& v);
private:
	double m[16];	
};