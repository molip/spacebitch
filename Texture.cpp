#include "Sprite.h"

#include <SFML/OpenGL.hpp>

namespace 
{
	std::map<std::string, std::unique_ptr<sf::Texture>> map;
}

namespace Texture
{

const sf::Texture& Get(const char* sPath)
{
	auto it = map.find(sPath);
	if (it != map.end())
		return *it->second.get();

	auto p = new sf::Texture;
	if (!p->loadFromFile(sPath))
		throw;

	map.insert(std::make_pair(sPath, std::unique_ptr<sf::Texture>(p)));
	return *p;
}
}