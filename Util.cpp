#include "Util.h"

float Magnitude(const sf::Vector3f& v)
{
	return sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
}

sf::Vector3f Normalise(const sf::Vector3f& v)
{
	float mag = Magnitude(v);
	return v / mag;
}

// returns cos of angle between vectors
float Dot(const sf::Vector3f& v1, const sf::Vector3f& v2) 
{
	return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;	
}	

float Dot(const sf::Vector2f& v1, const sf::Vector2f& v2) 
{
	return v1.x * v2.x + v1.y * v2.y;	
}	

sf::Vector3f Cross(const sf::Vector3f& v1, const sf::Vector3f& v2) 
{	
	sf::Vector3f result;

	result.x = (v1.y * v2.z) - (v1.z * v2.y);
	result.y = (v1.z * v2.x) - (v1.x * v2.z);
	result.z = (v1.x * v2.y) - (v1.y * v2.x);

	return result; 
}
